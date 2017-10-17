n = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf]
sBox = [9, 0xb, 0xc, 4, 0xa, 1, 0x2, 6, 0xd, 7, 3, 8, 0xf, 0xe, 0, 5]
sXob = [14, 5, 6, 10, 3, 15, 7, 9, 11, 0, 4, 1, 2, 8, 13]

key1 = 0b0101
key2 = 0b1010
message = 0b1100

def makeXor (message, key):
    return sBox[message ^ key]

print('### PHASE 1 ###')
print('BEGIN : the old message is', bin(message) )
r1 = makeXor(message, key1)
print('THEN : the current message is', bin(r1))
r2 = makeXor(r1, key2)
print('END : the new message is', bin(r2) )


print('### PHASE 2 ###')
# We try to reverse the xor until we reach the original message
key1Found = 0;
key2Found = 0;
tryKey1 = 0b0000;
tryKey2 = 0b0000;

# TODO : make the second loop
print("TRYING to find the second key")
while key2Found == 0 :
    if makeXor(r2, tryKey2) == key2:
        key2Found = 1
        print("FOUND : ", bin(tryKey2))
        print("TRYING to find the first key")
        while key1Found == 0:
            if makeXor(key1, tryKey1) == 0b0101:
                key1Found = 1
                print("<<TEST>> Original key : ", bin(key1), ", tested Key :", bin(tryKey1))
                print("FOUND : ", bin(tryKey1))
            else:
                tryKey1+=1
                print("TRY : with key ", bin(tryKey1))
    else:
        tryKey2+=1
        print("TRY : with key ", bin(tryKey2))
