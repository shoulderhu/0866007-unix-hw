from capstone import *
from pwn import *

r = remote("aup.zoolab.org", 2530)

for i in range(10):
    recv = r.recvuntil("Your answer: ").decode()
    print(recv)
    recv = recv.split("\n")[-2].split(" ")[1]

    CODE = binascii.unhexlify(recv)
    md = Cs(CS_ARCH_X86, CS_MODE_64)
    ans = ""

    for (address, size, mnemonic, op_str) in md.disasm_lite(CODE, 0x0):
        if op_str == "":
            ans += mnemonic + "\n"
        else:
            ans += mnemonic + " " + op_str + "\n"

    ans = binascii.hexlify(ans.encode()).decode()
    r.sendline(ans)

print(r.recvall().decode())