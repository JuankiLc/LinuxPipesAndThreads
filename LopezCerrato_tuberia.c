#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 50
#define NUMHIJOS 2

int main()
{
        int n, i, fdes[2];
        pid_t pid[NUMHIJOS];
        char line[MAXLINE];
        if (pipe(fdes) < 0){exit(1);}

        for(i=0;i<NUMHIJOS;i++)
{
        pid[i] = fork();
        if(pid[i]==0)
        {
        if(i==1)
        {
        close(fdes[1]);
        n = read(fdes[0],line,MAXLINE);
        printf("%s\n", line);
        sleep(5);
        }
        else if(i==0)
        {
        close(fdes[0]);
        write(fdes[1], "Soy tu proceso hermano, te doy la bienvenida", MAXLINE);
        sleep(3);
        }
        exit(0);
        }
        else if(pid[i]<0)
        {
        printf("Error: no se ha creado el hijo");
        }
}
close(fdes[0]);
close(fdes[1]);
         for(i=0;i<NUMHIJOS;i++)
{
        wait(NULL);
}
return 0;
}
