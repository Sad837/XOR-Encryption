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


// Clears any leftover characters up to and including the next newline.
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
// Prompts for a key using fgets so empty input is readable as a zero-length
// string instead of causing scanf to hang, and re-prompts until non-empty.
void getKey(const char* prompt, char* key, size_t keySize) {
    while (1) {
        printf("%s", prompt);
        fgets(key, (int)keySize, stdin);
        key[strcspn(key, "\n")] = '\0';
        if (strlen(key) > 0) {
            break;
        }
        printf("Key cannot be empty. Please try again.\n");
    }
}
int main() {
    char choice[20];

    while (1) {
        printf("Enter 'encrypt' or 'decrypt': ");
        scanf("%19s", choice);
        clearInputBuffer();

        if (strcmp(choice, "encrypt") == 0) {
            char message[1000], filename[50], key[50];

            printf("Enter the message to encrypt: ");
            fgets(message, sizeof(message), stdin);
            if (strchr(message, '\n') == NULL) {
                clearInputBuffer();
            }
            message[strcspn(message, "\n")] = '\0';

            printf("Enter the filename to save encrypted data: ");
            scanf("%49s", filename);
            clearInputBuffer();

            getKey("Enter the encryption key: ", key, sizeof(key));

            encryptFile(filename, message, key);
            break;
        }
        else if (strcmp(choice, "decrypt") == 0) {
            char filename[50], key[50];

            printf("Enter the filename to decrypt: ");
            scanf("%49s", filename);
            clearInputBuffer();

            getKey("Enter the decryption key: ", key, sizeof(key));

            decryptFile(filename, key);
            break;
        }
        else {
            printf("Invalid choice. Please enter 'encrypt' or 'decrypt'.\n");
        }
    }

    return 0;
}


