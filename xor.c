#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void xorEncrypt(char *message, char *key) {
    int keyLen = strlen(key);
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] ^= key[i % keyLen];
    }
}

void encryptFile(const char *filename, const char *message, const char *key) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
	size_t len = strlen(message); // save length 
    xorEncrypt((char *)message, key);
    fwrite(message, 1, len, file); //uses saved length 

    fclose(file);
}

void decryptFile(const char *filename, const char *key) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    if (!buffer) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';

    xorEncrypt(buffer, key);

	fwrite(buffer, 1, fileSize, stdout); // Output decrypted data to stdout

    free(buffer);
    fclose(file);
}

int main() {
    char choice[20];
    printf("Enter 'encrypt' or 'decrypt': ");
    scanf("%19s", choice);
	while (getchar() != '\n'); // Clear the input buffer
    if (strcmp(choice, "encrypt") == 0) {
        char message[1000], filename[50], key[50];
        printf("Enter the message to encrypt: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0';
        printf("Enter the filename to save encrypted data: ");
        scanf("%49s", filename);
        printf("Enter the encryption key: ");
        scanf("%49s", key);

        encryptFile(filename, message, key);
    } else if (strcmp(choice, "decrypt") == 0) {
        char filename[50], key[50];
        printf("Enter the filename to decrypt: ");
        scanf("%49s", filename);
        printf("Enter the decryption key: ");
        scanf("%49s", key);

        decryptFile(filename, key);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}