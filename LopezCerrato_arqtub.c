#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 50
#define NUMHIJOS 2

int main()
{
        int n, i, j, fdes1[2], fdes2[2];
        pid_t pid[NUMHIJOS];
        char line[MAXLINE];
        pid_t nieto;
        pid_t bisnieto;
        if (pipe(fdes1) < 0){exit(1);}
        if (pipe(fdes2) < 0){exit(1);}

        for(i=0;i<NUMHIJOS;i++)
{
    pid[i] = fork();
    if(pid[i]==0)
    {
        if(i==1)//h1
        {
            close(fdes1[1]);
            close(fdes2[0]);
            nieto=fork();
            if(nieto==0)//n1
            {
                bisnieto=fork();
                if(bisnieto==0)
                {
 /*B1*/             read(fdes1[0], line, MAXLINE);
                    printf("%d\n", line);
                    write(fdes2[1],"Bien, y ¿tu?", MAXLINE);
                    read(fdes1[0], line, MAXLINE);
                    printf("%d\n", line);
                    write(fdes2[1], "Adios", MAXLINE);
                    close(fdes1[1]);
                    close(fdes2[0]);
                }
                else if(bisnieto>0)
        {
            close(fdes2[1]);
            close(fdes1[0]);
            wait(NULL);
            printf("Acaba N1\n");
            exit(0);
        }
        else if(bisnieto<0)
        {
             printf("Error: no se ha creado el bisnieto");
             exit(1);
        }
            }
        else if(nieto>0)
        {
            close(fdes2[1]);
            close(fdes1[0]);
            wait(NULL);
            printf("Acaba H1\n");
            exit(0);
        }
        else if(nieto<0)
        {
             printf("Error: no se ha creado el nieto");
             exit(1);
        }
}
        else if(i==0)//h0
        {
            close(fdes2[1]);
            close(fdes1[0]);
            nieto=fork();
            if(nieto==0)//n1
            {
/*N0*/          write(fdes1[1],"Hola, como estas?", MAXLINE);
                read(fdes2[0], line, MAXLINE);
                printf("%d\n", line);
                write(fdes1[1], "Tambien bien, Adios", MAXLINE);
                read(fdes2[0], line, MAXLINE);
                printf("%d\n", line);
                close(fdes2[1]);
                close(fdes1[0]);
            }
            else if(nieto>0)
        {
            close(fdes1[1]);
            close(fdes2[0]);
            wait(NULL);
            printf("Acaba H0\n");
            exit(0);
        }
        else if(nieto<0)
        {
             printf("Error: no se ha creado el nieto");
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
        close(fdes1[1]);
        close(fdes2[0]);
        close(fdes1[0]);
        close(fdes2[1]);
        wait(NULL);
}
return 0;
}

