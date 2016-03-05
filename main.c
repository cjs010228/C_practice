#include <stdio.h>
#include <string.h>
#define PARSESTRING "$GNGGA"

int main(void)
{
    FILE *article;
    article = fopen("article.txt","r");
    char *s;
    char buffer[4096]; 
    fread(buffer,4096,1,article);

    s = strtok(buffer,"\n");
    while(s!=NULL)
    {
        if(!strncmp(s,PARSESTRING,strlen(PARSESTRING)))
            printf("%s\n",s);
        s =strtok(NULL,"\n"); 
    }
    

    return 0;

}