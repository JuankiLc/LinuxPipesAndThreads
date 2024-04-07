#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 50
#define NUMHIJOS 3

int main()
{
        int n, i, j, fdes1[2], fdes2[2];
        pid_t pid[NUMHIJOS];
        char line[MAXLINE];
        pid_t nieto[2];
        if (pipe(fdes1) < 0){exit(1);}

        for(i=0;i<NUMHIJOS;i++)
{
        pid[i] = fork();
        close(fdes1[0]);
        close(fdes1[1]);
        if(pid[i]==0)
        {
        if(i==1)//hijo 2
        {
        close(fdes1[1]);
        n = read(fdes1[0],line,MAXLINE);
        close(fdes1[0]);
        printf("%d", n);
        sleep(3);
        printf("acaba hijo2");
        exit(0);
        }
        else if(i==0)//hijo 1
            {
            if (pipe(fdes2) < 0){exit(1);}
            close(fdes1[0]);
            for(j=0;j<2;j++)
            {
            nieto[j]=fork();
            close(fdes2[0]);
            close(fdes2[1]);
            if(j==1)//nieto 2
            {
            close(fdes2[1]);
            n = read(fdes2[0],line,MAXLINE);
            close(fdes2[0]);
            write(fdes1[1],n,MAXLINE);
            close(fdes1[1]);
            sleep(7);
            printf("acaba nieto2");
            exit(0);
            }
            else if(j==0)//nieto 1
            {
                close(fdes1[1]);
                close(fdes2[0]);
                write(fdes2[1], "Hola proceso H2",MAXLINE);
                close(fdes2[1]);
                sleep(6);
                printf("acaba nieto2");
                exit(0);
            }
            }
            for(j=0;j<2;j++)
{
        wait(NULL);
}
        printf("acaba hijo1");
        exit(0);
}
        else if(i==2)//hijo 3
        {
        close(fdes1[0]);
        close(fdes1[1]);
        nieto[0]=fork();
        if(nieto[0]==0)//nieto 3
        {
            sleep(6);
            exit(0);
        }
        else if(nieto[0]>0)
        {
            sleep(3);
            exit(0);
        }
        else if(nieto[0]<0)
        {
             printf("Error: no se ha creado el hijo");
             printf("acaba hijo3");
             exit(1);
        }
}
        }
        else if(pid[i]<0)
        {
             printf("Error: no se ha creado el hijo");
             exit(1);
        }
}
         for(i=0;i<NUMHIJOS;i++)
{
        wait(NULL);
}
return 0;
}

