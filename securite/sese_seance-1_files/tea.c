#include <stdio.h>
#include <stdint.h>

void TEA_encrypt (uint32_t v[2], uint32_t k[4]) {
  uint32_t v0 = v[0], v1 = v[1];
  uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
  uint32_t delta = 0x9e3779b9, sum = 0;
  int i;

  for (i = 0; i < 32; i++) {
    sum += delta;
    v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
    v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
  }

  v[0] = v0;
  v[1] = v1;
}

void TEA_decrypt (uint32_t v[2], uint32_t k[4]) {
  uint32_t v0 = v[0], v1 = v[1];
  uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
  uint32_t delta = 0x9e3779b9, sum = 0xc6ef3720;
  int i;

  for (i = 0; i < 32; i++) {
    v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
    v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
    sum -= delta;
  }

  v[0] = v0;
  v[1] = v1;
}

int main (int argc, char *argv[])
{
  uint32_t v[2] = {0}, k[4] = {0};

  sscanf(argv[1], "%8x%8x", v, v+1);

  printf("msg: %08x%08x\n\n", v[0], v[1]);

  printf("Encrypt with key 00000000 00000000 00000000 00000000:\n");

  TEA_encrypt(v, k);
  printf("enc: %08x%08x\n\n", v[0], v[1]);

  printf("Decrypt with key 00000000 00000000 00000000 00000000:\n");

  TEA_decrypt(v, k);
  printf("%08x%08x\n", v[0], v[1]);

  return 0;
}
