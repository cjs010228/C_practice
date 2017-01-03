#include <stdio.h> 

int main(int argc, char* argv[]) 
{
    long int size;
    unsigned char ch; 
    int count = 0; 
    unsigned char checkSum = 0;

    if(argc != 2) 
    { 
        puts("Command: read <filename>"); 
        return 1; 
    } 

    FILE *file = fopen(argv[1], "rb");
    if(!file) 
    { 
        puts("Failed to read"); 
        return 1; 
    } 

    /* Count the file size */
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    

    /* Print bin file in Hex type */
    while(!feof(file)) 
    { 
        fread(&ch, sizeof(char), 1, file);

        checkSum ^= ch;
        printf("%02x ", ch);
        count++; 
        if(count > 16) {  
            putchar('\n');
            count = 0; 
        } 
    } 
    putchar('\n');

    printf("The file size is %d\n",size);
    printf("checksum = %x\n",checkSum);

    fclose(file);

    return 0; 
} 

