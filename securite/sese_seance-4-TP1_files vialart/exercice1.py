# Principe : on a une clé K de 8 bits et une clé m de 4 bits
# A chaque tour : ajout de la clé avec un xor ; on prend la première moitié du message et on ajoute la clé avec un xor ; on passe le résultat dans la boîte S
# Au deuxième tour, on passe la deuxième moitié de la clé

n = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf]    # from char to hexadecimal
sBox = [9, 0xb, 0xc, 4, 0xa, 1, 0x2, 6, 0xd, 7, 3, 8, 0xf, 0xe, 0, 5]
sXob = [14, 5, 6, 10, 3, 15, 7, 9, 11, 0, 4, 1, 2, 8, 13]   # reversed sBox : order each value of sBox (ASC) and write their index (the index of 0 is 14 ; 1:5 ; etc)

key0 = 0b0101   # syntax = 4 bits variable
key1 = 0b1010   # 0 < 4b > 7
message = 0b1100
# message = random.randint(0, 15)

def makeXor (message, key):
    return sBox[message ^ key]

# print('KEY0 :', key0)
# print('KEY1 :', key1)
print('BEGIN : the old message is', message )
# round 1
r1 = makeXor(message, key0)
print('THEN : the current message is', r1)
# round 2
r2 = makeXor(r1, key1)
print('END : the new message is', r2 )
