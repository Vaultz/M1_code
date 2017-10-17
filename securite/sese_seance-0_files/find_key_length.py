with open('secret') as f:
    c = f.read() # c is the ciphertext

Q = 27 # Q is used to hash letter, 27 is the first odd number > #alphabet
h = 0  # h will be used as a circular hash

n = 3  # length of the strings that we look for repititions of

seen = {}  # dictionary k => p, remembers that the hash for the substring from
           # p to p+n is k

previous = {} # dictionary p1 => p2, store that a previous occurence of the
              # substring of length n starting at p1 started at p2


## first, compute the hash for the first n char:
for i in range(0, n):
    h = h * Q + ord(c[i]) # ord gives the ascii code (a unique number
                          # representing the letter)

# then, iterate over the string:
for i in range(n, len(c)):
    if h in seen:                     # if we already saw the substring then
        previous[i - n] = seen[h] - n # store where the previous occurence was

    seen[h] = i # remember that we saw the substring with hash h at position i

    h = h * Q + ord(c[i]) - (Q ** n) * ord(c[i - n]) # removes c[i - n] from
                                                     # the hash and adds c[i]

print('Search results for substring of length ' + str(n) + ':')
                                                     
visited = {}
for i in previous.keys():
    visited[i] = False
diff = []
for i in previous.keys():
    if visited[i]: pass
    print(c[i:i+n] + ' seen at : ' + str(i), end='')
    while i in previous and not visited[i]:
        visited[i] = True
        diff.append(i - previous[i])
        i = previous[i]
        print(', ' + str(i), end='')
    print('.')

print('Spaces between these positions: ' + str(diff))

from functools import reduce
from fractions import gcd

k_len = reduce(gcd, diff)

print('Probable key length is ' + str(k_len) + '.')
