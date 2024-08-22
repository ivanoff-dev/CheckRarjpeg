#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char **argv)
{
    char str[100];
    FILE * myfile;
    int j;
    struct cd_file_header{
        char versionMadeBy[2];
        char versionToExtract[2];
        char generalPurposeBitFlag[2];
        char compressionMethod[2];
        char modificationTime[2];
        char modificationDate[2];
        char crc32[4];
        char compressedSize[4];
        char uncompressedSize[4];
        char filenameLength[2];
        char extraFieldLength[2];
        char fileCommentLength[2];
        char diskNumber[2];
        char internalFileAttributes[2];
        char externalFileAttributes[4];
        char localFileHeaderOffset[4];
    };

    struct cd_file_header file_header;

    myfile = fopen(argv[argc-1], "r");

    unsigned char SIGNATURE[4] = {0x50, 0x4b, 0x01, 0x02};

    j = 0;
    while (fread(str, 4, 1, myfile) > 0)
    {
        if (memcmp(str, SIGNATURE, 4) == 0)
        {
            fread(&file_header, 1, sizeof(file_header), myfile);
            printf("----------------------------------------\n");
            printf("Длина имени файла - %d\n", *file_header.filenameLength);
            char file_name[100];
            fread(&file_name, 1, *file_header.filenameLength, myfile);
            printf("Это имя файла #%d - %s\n", j+1, file_name);
            j++;
        }
    }

    if (j == 0)
        printf("Файл не содержит архив\n");

    fclose(myfile);
    
    return 0;
}
