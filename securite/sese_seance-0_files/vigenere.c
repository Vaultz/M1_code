#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *
vigenere_enc (char *msg, int m_len, char *key, int k_len)
{
  int i, k = 0;
  char c;
  char *enc = malloc(m_len * sizeof(char) + 1);
  for (i = 0; i < m_len; i++)
    enc[i] = (msg[i] + key[k++ % k_len] - 2*'A') % 26 + 'A';
  enc[i] = '\0';
  return enc;
}

char *
vigenere_dec (char *enc, int c_len, char *key, int k_len)
{
  int i, k = 0;
  char c;
  char *msg = malloc(c_len * sizeof(char) + 1);
  for (i = 0; i < c_len; i++)
    msg[i] = (enc[i] - key[k++ % k_len] + 26) % 26 + 'A';
  msg[i] = '\0';
  return msg;
}

char *
vigenere_key (char *msg, char *enc, int len)
{
  int i;
  char c;
  char *key = malloc(len * sizeof(char) + 1);
  for (i = 0; i < len; i++)
    key[i] = (enc[i] - msg[i] + 26) % 26 + 'A';
  key[i] = '\0';
  return key;
}

int
main (int argc, char *argv[])
{
  char *s = NULL, *in = NULL;
  int k_len, c, i;
  char *(*vig)(char *, int, char *, int);

  if (argc != 3) {
    fprintf(stderr, "Usage: %s (enc|dec) KEY\n", argv[0]);
    return 1;
  }

  k_len = strlen(argv[2]);
  in = malloc(k_len * sizeof(*in) + 1);

  if (0 == strcmp("enc", argv[1]))
    vig = vigenere_enc;
  else if (0 == strcmp("dec", argv[1]))
    vig = vigenere_dec;

  i = 0;
  while ((c = getchar()) != EOF) {
    if (isspace(c)) continue;
    in[i++] = c;
    if (i == k_len) {
      s = vig(in, i, argv[2], k_len);
      printf("%s", s);
      free(s);
      i = 0;
    }
  }
  if (i > 0) {
    s = vig(in, i, argv[2], k_len);
    printf("%s", s);
    free(s);
  }
  printf("\n");

  free(in);

  return 0;
}
