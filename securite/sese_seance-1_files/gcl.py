def make_gcl (a, b, m):
    def gcl_next (x):
        return (a * x + b) % m
    return gcl_next

def test_gcl (gcl, seed, i):
    n = seed
    while i > 0:
        i = i - 1
        print(n)
        n = gcl(n)

# Exemples vu en cours :
# >>> bad_prng = make_gcl(25,16,256)
# >>> test_gcl(bad_prng, 125, 30)
# >>> test_gcl(bad_prng, 50, 30)
# >>> test_gcl(bad_prng, 10, 30)
