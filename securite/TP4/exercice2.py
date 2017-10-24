n = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf]
sBox = [9, 0xb, 0xc, 4, 0xa, 1, 0x2, 6, 0xd, 7, 3, 8, 0xf, 0xe, 0, 5]
sXob = [14, 5, 6, 10, 3, 15, 7, 9, 11, 0, 4, 1, 2, 8, 13]

key1 = 0b0101
key2 = 0b1010
message = 0b1111

def makeXor (message, key):
    return sBox[message ^ key]

print('### PHASE 1 ###')
print('BEGIN : the old message is', bin(message) )
r1 = makeXor(message, key1)
print('THEN : the current message is', bin(r1))
r2 = makeXor(r1, key2)
print('END : the new message is', bin(r2), "\n" )


print('### PHASE 2 ###')
firstKeyFound = 0
secondKeyFound = 0
firstKey = 0b0
secondKey = 0b0
tryMessage = 0b0
tempMessage = 0b0
attempts = 0

while firstKeyFound == 0:
    # we reverse-xor (let's call that rox) our temporary message with a first key
    tempMessage = makeXor(r2, firstKey)
    print("FIRST KEY : ", bin(firstKey))
    # then we rox it with a second key while we haven't found the original message
    while (secondKeyFound == 0):
        print("SECOND KEY ", bin(secondKey))
        # wring message : roxing again
        if makeXor(tempMessage, secondKey) != message:
            print("SECOND KEY FAILED : ", bin(secondKey))
            tryMessage = makeXor(tempMessage, secondKey)
            secondKey+=1
            attempts+=1
        # when the proper message is found, ending the loop
        else:
            print(attempts, "attempts")
            print("MESSAGE FOUND : ", bin(makeXor(tempMessage, secondKey)), "\n")
            firstKeyFound = secondKeyFound = 1

    # increasing the first key, then retry...
    firstKey+=1
