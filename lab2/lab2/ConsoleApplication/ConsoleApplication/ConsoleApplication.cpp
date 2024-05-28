#include <stdio.h>
#include <iostream>
#include <conio.h> 

#pragma warning(disable : 4996)

#define BUFFER_SIZE 16 // Размер буфера чтения/записи

int main() {
    const char* filename = "test.mylab2"; 

    // Чтение файла

    FILE* fileRead = fopen(filename, "r");

    if (!fileRead) {
        printf("Could not access file %s\n", filename);
        return 1;
    }

    char content[BUFFER_SIZE];

    size_t charsCount = fread(content, 1, BUFFER_SIZE, fileRead);

    printf("Read %zu chars:\n", charsCount, filename);

    if (charsCount == BUFFER_SIZE) {
        printf("%s \n", content);
    }
    else {
        if (feof(fileRead)) {
            printf("Unexpected end of file\n");
            printf("Chars read: %zu\n", charsCount);
            for (size_t i = 0; i < charsCount; i++) {
                printf("%c", content[i]);
            }
            printf("\n");
        }
        else if (ferror(fileRead)) {
            printf("Error occured while reading file\n");
        }
    }

    fclose(fileRead);

    const char* phrase = "non timebo malaa";

    // Запись в файл

    FILE* fileWrite = fopen(filename, "w");

    if (!fileWrite) {
        printf("Could not access file %s\n", filename);
        return 1;
    }

    size_t bytesWritten = fwrite(phrase, 1, BUFFER_SIZE, fileWrite);

    fclose(fileWrite);

    system("pause");
    return 0;
}
