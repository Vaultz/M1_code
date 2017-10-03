// TD

// écrire une fonction qui permet de chiffrer avec la méthode de Vigenère
function char* cryptMessage(char[] cryptedMessage, char[] key) {

  char[] decryptedMessage; // allocate as much as memory as in cryptedMessage
  int keyLength = key.length();
  int j = 0;

  // browsing the message
  for(int i = 0 ; i < message.length() ; i++) {
    // reaching the end of the key means starting over to its first character
    if (j <= keyLength) {
      j++;
    }
    else {
        j = 0;
    }

    // stocking each decrypted character
    decryptedMessage[i] = cryptedMessage[i]+key[i];

  }

  return decryptedMessage;

}


// fonction qui permet de défchiffrer
