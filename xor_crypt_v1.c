#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 256

// XOR the input buffer with the key, writing the result into output.
// output must be at least len bytes.
void xor_buffer(const char *input, size_t len, const char *key, size_t keylen, char *output) {
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ key[i % keylen];
    }
}

void do_encrypt(void) {
    char filename[MAX_LEN];
    char message[MAX_LEN];
    char key[MAX_LEN];
    char encrypted[MAX_LEN];

    printf("Enter filename to save encrypted message: ");
    scanf("%s", filename);

    printf("Enter plaintext message: ");
    scanf(" %[^\n]", message);

    printf("Enter key: ");
    scanf("%s", key);

    size_t msg_len = strlen(message);
    size_t key_len = strlen(key);

    xor_buffer(message, msg_len, key, key_len, encrypted);

    FILE *fp = fopen(filename, "wb");
    fwrite(encrypted, 1, msg_len, fp);
    fclose(fp);

    printf("Message encrypted and saved to %s\n", filename);
}

void do_decrypt(void) {
    char filename[MAX_LEN];
    char key[MAX_LEN];
    char buffer[MAX_LEN];
    char decrypted[MAX_LEN];

    printf("Enter filename to decrypt: ");
    scanf("%s", filename);

    printf("Enter key: ");
    scanf("%s", key);

    FILE *fp = fopen(filename, "rb");
    size_t bytes_read = fread(buffer, 1, MAX_LEN, fp);

    size_t key_len = strlen(key);
    xor_buffer(buffer, bytes_read, key, key_len, decrypted);

    fclose(fp);

    printf("Decrypted message: ");
    fwrite(decrypted, 1, bytes_read, stdout);
    printf("\n");
}

int main(void) {
    char choice[MAX_LEN];

    printf("Encrypt or Decrypt? (e/d): ");
    scanf("%s", choice);

    if (choice[0] == 'e') {
        do_encrypt();
    } else {
        do_decrypt();
    }

    return 0;
}
