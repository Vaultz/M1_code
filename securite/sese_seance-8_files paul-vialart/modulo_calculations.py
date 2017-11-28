## The Code below is from : https://gist.github.com/ofaurax/6103869014c246f962ab30a513fb5b49
def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    g, y, x = egcd(b%a,a)
    return (g, x - (b//a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('No modular inverse')
    return x%m
## The Code above is from : https://gist.github.com/ofaurax/6103869014c246f962ab30a513fb5b49


def squareAndMultiply(b, e, m):
    res = 1
    b = b % m
    while e != 0:
        if e & 1 ==1:
            res = (res*b) % m
        e = e>>1
        b = (b*b) % m
    return res
