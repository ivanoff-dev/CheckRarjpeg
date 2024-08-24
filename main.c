#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char **argv)
{
    char *buffer;
    FILE * myfile;
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
    if (myfile == 0)
    {
        printf("Не удалось открыть входной файл\n");
        exit(0);
    }
    // Сигнатура центральной директории
    unsigned char SIGN_FIRST_BYTE = 0x50;
    unsigned char *pSIGN_FIRST_BYTE;
    pSIGN_FIRST_BYTE = &SIGN_FIRST_BYTE;
    unsigned char SIGN_SECOND_BYTE = 0x4b;
    unsigned char *pSIGN_SECOND_BYTE;
    pSIGN_SECOND_BYTE = &SIGN_SECOND_BYTE;
    unsigned char SIGN_THIRD_BYTE = 0x01;
    unsigned char *pSIGN_THIRD_BYTE;
    pSIGN_THIRD_BYTE = &SIGN_THIRD_BYTE;
    unsigned char SIGN_FOURTH_BYTE = 0x02;
    unsigned char *pSIGN_FOURTH_BYTE;
    pSIGN_FOURTH_BYTE = &SIGN_FOURTH_BYTE;
    // Часть сигнатуры EOCD
    unsigned char ARCH_SIGN_THIRD_BYTE = 0x05;
    unsigned char *pARCH_SIGN_THIRD_BYTE;
    pARCH_SIGN_THIRD_BYTE = &ARCH_SIGN_THIRD_BYTE;
    unsigned char ARCH_SIGN_FOURTH_BYTE = 0x06;
    unsigned char *pARCH_SIGN_FOURTH_BYTE;
    pARCH_SIGN_FOURTH_BYTE = &ARCH_SIGN_FOURTH_BYTE;
    

    int i = 0, j = 0;
    while (fread(buffer, 1, 1, myfile) != 0)
    {
        if (memcmp(buffer, pSIGN_FIRST_BYTE, 1) == 0)
        {
            fread(buffer, 1, 1, myfile);
            if (memcmp(buffer, pSIGN_SECOND_BYTE, 1) == 0)
            {
                fread(buffer, 1, 1, myfile);
                if (memcmp(buffer, pSIGN_THIRD_BYTE, 1) == 0)
                {
                    fread(buffer, 1, 1, myfile);
                    if (memcmp(buffer, pSIGN_FOURTH_BYTE, 1) == 0)
                    {
                        fread(&file_header, sizeof(file_header), 1, myfile);

                        char file_name[] = "";
                        fread(&file_name, *file_header.filenameLength, 1, myfile);
                        printf("#%d - %s\n", j+1, file_name);

                        char extra_field[] = "";
                        fread(&extra_field, *file_header.extraFieldLength, 1, myfile);

                        char file_comment[] = "";
                        fread(&file_comment, *file_header.fileCommentLength, 1, myfile);
                        j++;
                    }
                }
                else if (memcmp(buffer, pARCH_SIGN_THIRD_BYTE, 1) == 0)
                {
                    fread(buffer, 1, 1, myfile);
                    if (memcmp(buffer, pARCH_SIGN_FOURTH_BYTE, 1) == 0)
                    {
                        i++;
                    }
                }
            }
        }
    }

    if (j == 0 && i ==0)
    {
        printf("Файл не содержит архив\n");
    }
    else if (i > 0 && j == 0)
    {
        printf("Файл содержит пустой архив\n");
    }
    fclose(myfile);
    
    return 0;
}
