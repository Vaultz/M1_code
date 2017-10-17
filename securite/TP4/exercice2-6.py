import random

sBox = [9, 0xb, 0xc, 4, 0xa, 1, 0x2, 6, 0xd, 7, 3, 8, 0xf, 0xe, 0, 5]
n = random.randint(0, 99)
key = 0b1010
pigList = []

def makeXor (message, key):
    return sBox[message ^ key]

print("Generating ",n, "pairs in the [clearMessage, encrMessage] list")
for i in range (0, n):
    clearMessage = random.randint(0, 15)
    encrMessage = makeXor(clearMessage, key)
    pigList.append([clearMessage, encrMessage])

print(pigList)
