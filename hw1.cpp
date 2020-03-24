#include <fstream>      // ifstream
#include <string>       // string
#include <cstdio>       // printf(), fprintf(), fgets(), fopen(), fclose(), sscanf()
#include <cstdlib>      // exit(), atoi()
#include <cstring>      // strerror(), strlen(), memset(), strcpy()
#include <cerrno>       // errno
#include <cctype>       // isdigit()
#include <regex.h>      // regexcomp(), regexexec(), regexfree()
#include <dirent.h>     // opendir, readdir()
#include <getopt.h>     // getopt_long()
#include <unistd.h>     // readlink()
#include <arpa/inet.h>  // inet_ntop()
#include <netinet/in.h> // in6_addr

#define BUFFER_SIZE 1024

using namespace std;

bool flag_string;
regex_t regex;

bool is_pid(const char *str) {
    for (size_t i = 0; i < strlen(str); ++i) {
        if (!isdigit(str[i])) { return false; }
    }

    return true;
}

int get_pid_by_inode(const int inode) {
    DIR *dirp_proc, *dirp_fd;
    struct dirent *ent_proc, *ent_fd;
    char name1[BUFFER_SIZE], name2[1279], buf[BUFFER_SIZE];
    int socket_inode;

    if ((dirp_proc = opendir("/proc/")) == nullptr) {
        fprintf(stderr, "opendir /proc/: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((ent_proc = readdir(dirp_proc)) != nullptr) {
        // Check /proc/[pid]/ is DT_DIR and contains numbers only.
        if (ent_proc->d_type == DT_DIR && is_pid(ent_proc->d_name)) {
            snprintf(name1, sizeof(name1), "/proc/%s/fd/", ent_proc->d_name);
            if ((dirp_fd = opendir(name1)) == nullptr) {
                if (errno == EACCES || errno == ENOENT) {
                    // 'Permission denied' or 'No such file or directory'
                    continue;
                }
                fprintf(stderr, "opendir %s: %s\n", name1, strerror(errno));
                exit(EXIT_FAILURE);
            }

            while ((ent_fd = readdir(dirp_fd)) != nullptr) {
                // Check /proc/[pid]/fd/[fd] is link.
                if (ent_fd->d_type == DT_LNK) {
                    snprintf(name2, sizeof(name2), "%s%s", name1, ent_fd->d_name);
                    memset(buf, 0, BUFFER_SIZE);
                    if (readlink(name2, buf, BUFFER_SIZE) == -1) {
                        if (errno == ENOENT) { // No such file or directory
                            continue;
                        }
                        fprintf(stderr, "readlink %s: %s\n", name2, strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    if (strstr(buf, "socket:[")) {
                        sscanf(buf, "socket:[%d]", &socket_inode);

                        if (inode == socket_inode) {
                            closedir(dirp_fd);
                            closedir(dirp_proc);
                            return atoi(ent_proc->d_name);
                        }
                    }
                }
            }

            closedir(dirp_fd);
        }
    }

    closedir(dirp_proc);
    return -1;
}

string get_cmdline_by_pid(const int pid) {
    char pathname[32];
    snprintf(pathname, sizeof(pathname), "/proc/%d/cmdline", pid);

    ifstream ifs(pathname);
    string str, s;
    size_t pos;

    if (ifs.is_open()) {
        getline(ifs, str, '\0');
        if ((pos = str.find_last_of('/')) != string::npos) {
            s =  str.substr(pos + 1);
        }

        while (getline(ifs, str, '\0')) {
            s += ' ' + str;
        }
    }

    ifs.close();
    return s;
}

void list_ipv4_connections(const char *proto) {
    FILE *stream;
    char s[BUFFER_SIZE], pathname[32];

    snprintf(pathname, sizeof(pathname), "/proc/net/%s", proto);
    if ((stream = fopen(pathname, "r")) == nullptr) {
        fprintf(stderr, "fopen %s: %s\n", pathname, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Get first line
    fgets(s, BUFFER_SIZE, stream);

    unsigned int sl, local_address_ip, rem_address_ip, st;
    unsigned int tx_queue, rx_queue, tr, tm, retrnsmt;
    unsigned short local_address_port, rem_address_port;
    int uid, timeout, inode;

    char local_ip[INET_ADDRSTRLEN], remote_ip[INET_ADDRSTRLEN];
    char local_address[22], remote_address[22];
    int pid;

    while (fgets(s, BUFFER_SIZE, stream)) {
        sscanf(s, "%d: %X:%hX %X:%hX %X %X:%X %X:%X %X %d %d %d",
               &sl, &local_address_ip, &local_address_port,
               &rem_address_ip, &rem_address_port, &st,
               &tx_queue, &rx_queue, &tr, &tm, &retrnsmt, &uid, &timeout, &inode);

        //printf("sl(%X), local_address_ip(%s), local_address_port(%d), rem_address_ip(%s), rem_address_port(%d), st(%X), uid(%d), inode(%d)\n\n",
        //        sl, local_ip, local_address_port, remote_ip, rem_address_port, st, uid, inode);

        // Local Address
        inet_ntop(AF_INET, &local_address_ip, local_ip, INET_ADDRSTRLEN);
        snprintf(local_address, sizeof(local_address), "%s:%hu", local_ip, local_address_port);

        // Foreign Address
        inet_ntop(AF_INET, &rem_address_ip, remote_ip, INET_ADDRSTRLEN);
        if (rem_address_port == 0) {
            snprintf(remote_address, sizeof(remote_address), "%s:*", remote_ip);
        } else {
            snprintf(remote_address, sizeof(remote_address), "%s:%hu", remote_ip, rem_address_port);
        }

        // PID
        if ((pid = get_pid_by_inode(inode)) == -1) {
            // Permission denied
            if (!flag_string) {
                printf("%-5s %-23s %-23s -\n", proto, local_address, remote_address);
            }
        }
        else {
            // Program name and arguments
            string cmdline = get_cmdline_by_pid(pid);

            if (!flag_string || regexec(&regex, cmdline.c_str(), 0, nullptr, 0) == 0) {
                printf("%-5s %-23s %-23s %d/%s\n",
                       proto, local_address, remote_address, pid, cmdline.c_str());
            }
        }
    }

    fclose(stream);
}

void list_ipv6_connections(const char *proto) {
    FILE *stream;
    char s[BUFFER_SIZE], pathname[32];

    snprintf(pathname, sizeof(pathname), "/proc/net/%s", proto);
    if ((stream = fopen(pathname, "r")) == nullptr) {
        fprintf(stderr, "fopen %s: %s\n", pathname, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Get first line
    fgets(s, BUFFER_SIZE, stream);

    unsigned int sl, st, tx_queue, rx_queue, tr, tm, retrnsmt;
    unsigned short local_address_port, rem_address_port;
    int uid, timeout, inode;
    char local_address_ip[33], rem_address_ip[33];

    struct in6_addr addr;
    char local_ip[INET6_ADDRSTRLEN], remote_ip[INET6_ADDRSTRLEN];
    char local_address[52], remote_address[48];
    int pid;

    while (fgets(s, BUFFER_SIZE, stream)) {
        sscanf(s, "%d: %[^:]:%hX %[^:]:%hX %X %X:%X %X:%X %X %d %d %d",
               &sl, local_address_ip, &local_address_port,
               rem_address_ip, &rem_address_port, &st,
               &tx_queue, &rx_queue, &tr, &tm, &retrnsmt, &uid, &timeout, &inode);

        // printf("sl(%X), local_address_ip(%s), local_address_port(%hu), rem_address_ip(%s), rem_address_port(%hu), st(%X), uid(%d), inode(%d)\n\n",
        //        sl, local_address_ip, local_address_port, rem_address_ip, rem_address_port, st, uid, inode);
        // continue;

        // Local Address
        sscanf(local_address_ip,
               "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
               &addr.s6_addr[3], &addr.s6_addr[2], &addr.s6_addr[1], &addr.s6_addr[0],
               &addr.s6_addr[7], &addr.s6_addr[6], &addr.s6_addr[5], &addr.s6_addr[4],
               &addr.s6_addr[11], &addr.s6_addr[10], &addr.s6_addr[9], &addr.s6_addr[8],
               &addr.s6_addr[15], &addr.s6_addr[14], &addr.s6_addr[13], &addr.s6_addr[12]);
        inet_ntop(AF_INET6, &addr, local_ip, INET6_ADDRSTRLEN);
        snprintf(local_address, sizeof(local_address), "%s:%hu", local_ip, local_address_port);

        // Foreign Address
        sscanf(rem_address_ip,
               "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
               &addr.s6_addr[3], &addr.s6_addr[2], &addr.s6_addr[1], &addr.s6_addr[0],
               &addr.s6_addr[7], &addr.s6_addr[6], &addr.s6_addr[5], &addr.s6_addr[4],
               &addr.s6_addr[11], &addr.s6_addr[10], &addr.s6_addr[9], &addr.s6_addr[8],
               &addr.s6_addr[15], &addr.s6_addr[14], &addr.s6_addr[13], &addr.s6_addr[12]);
        inet_ntop(AF_INET6, &addr, remote_ip, INET6_ADDRSTRLEN);
        if (rem_address_port == 0) {
            snprintf(remote_address, sizeof(remote_address), "%s:*", remote_ip);
        } else {
            snprintf(remote_address, sizeof(remote_address), "%s:%hu", remote_ip, rem_address_port);
        }

        // PID
        if ((pid = get_pid_by_inode(inode)) == -1) {
            // Permission denied
            if (!flag_string) {
                printf("%-5s %-23s %-23s -\n", proto, local_address, remote_address);
            }
        } else {
            // Program name and arguments
            string cmdline = get_cmdline_by_pid(pid);

            // Output
            if (!flag_string || regexec(&regex, cmdline.c_str(), 0, nullptr, 0) == 0) {
                printf("%-5s %-23s %-23s %d/%s\n",
                       proto, local_address, remote_address, pid, cmdline.c_str());
            }
        }
    }

    fclose(stream);
}

int main(int argc, char *argv[]) {
    int opt;
    const char *optstring = "tu";
    struct option longopts[] = {
            {"tcp", 0, nullptr, 't'},
            {"udp", 0, nullptr, 'u'},
            {nullptr, 0, nullptr, 0}
    };

    bool flag_tcp = false, flag_udp = false, flag_usage = false;

    while ((opt = getopt_long(argc, argv, optstring, longopts, nullptr)) != -1) {
        switch (opt) {
            case 't':
                flag_tcp = true;
                break;
            case 'u':
                flag_udp = true;
                break;
            case ':':
            case '?':
            default:
                flag_usage = true;
        }
    }

    // Process filter-string option
    argc -= optind;
    switch (argc) {
        case 0:
            flag_string = false;
            break;
        case 1:
            argv += optind;
            flag_string = true;
            if (regcomp(&regex, argv[0], 0) != 0) {
                fprintf(stderr, "regcomp: \n");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            flag_usage = true;
    }

    if (flag_usage) {
        fprintf(stderr, "Usage: ./hw1 [-t|--tcp] [-u|--udp] [filter-string]\n");
        exit(EXIT_FAILURE);
    }

    // (flag_tcp && flag_udp) && (!flag_tcp && !flag_udp)
    if (flag_tcp == flag_udp) {
        flag_tcp = true;
        flag_udp = true;
    }
    if (flag_tcp) {
        printf("List of TCP connections:\n");
        printf("%-5s %-23s %-23s %s\n",
                "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");

        list_ipv4_connections("tcp");
        list_ipv6_connections("tcp6");
    }
    if (flag_udp) {
        if (flag_tcp) { printf("\n"); }

        printf("List of UDP connections:\n");
        printf("%5s %-23s %-23s %s\n",
               "Proto", "Local Address", "Foreign Address", "PID/Program name and arguments");

        list_ipv4_connections("udp");
        list_ipv6_connections("udp6");
    }

    regfree(&regex);
    return 0;
}
