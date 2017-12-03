import random
import time
from modulo_calculations import modinv
from modulo_calculations import squareAndMultiply

## EXERCICE 1
print("## EXERCICE 1 ##\n")
# 1
p = 144738715101199389767052114455328981273229675184791080279089345672793360848686815824416894584140398541055687817754722483383388507122400205907442124869996192870232676180860525783432392729164778102001027364877878817605838724338878429866767189504532646865454617478066040472343670930769290331954248490412775249419
q = 145615517516874587916460678265369586427835863259372891860982778368112895293256265327758448333295592533388721593532509641574891337918480588559005180025786502031485877525667013412530158629477345931951192039056728676392556690268773091679557172463659569930616766573364069267876800014231071108023785957603882606427


# 2. Module de chiffrement
N = p*q
print(
    "- Nombre premier p = ", p,
    "\n\n- Nombre premier q = ",q,
    "\n\n- Module de chiffrement N = p*q = ", N)

#3. Calcul de φ(n), de e et de d
phiN = (p-1)*(q-1)
e = 65537
d = modinv(e, phiN)
print (
    "\n\n- φ(n) = ", phiN,
    "\n\n- Exposant de chiffrement (clé publique) e = ", e,
    "\n\n- Modulo inverse (clé privée) d = ", d)

#4. Generate the message
message = random.getrandbits(random.randint(1, 2048))
print("\n\n- Clear message = ", message)

#5. Cypher the message
cyphered = squareAndMultiply(message, d, N)
print("\n\n- Cyphered Message = ", cyphered)

#6. Decypher the message
decyphered = squareAndMultiply(cyphered, e, N)
print("\n\n- Decyphered Message = ", decyphered)
print("\n\n- Are the original message and the decyphered one the same ?... ", message==decyphered)


## EXERCICE 2
print("## EXERCICE 2 ##\n")
timer = time.clock()
nbCalculations = 100;
for i in range(1,nbCalculations):
    squareAndMultiply(message, d, N)
print("- It took ",time.clock()-timer, " to calculate ", nbCalculations, " signatures")
