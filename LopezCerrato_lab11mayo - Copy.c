#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 60
#define NUMNIETOS 3

int main()
{
        int n;
        int i;
        int j;
        int tub[4][2];
        pid_t hijo;
        char line[MAXLINE];
        pid_t nieto[NUMNIETOS];
        pid_t pidbisnieto;

    hijo = fork();
    if(hijo==0)//H0
    {
            for(j=0; j<4; j++)
        {
            if(pipe(tub[j]) < 0){exit(1);}
        }
            for(i=0;i<NUMNIETOS;i++)
        {
            nieto[i]=fork();
            if(nieto[i]==0)
            {
                if(i==1)//N1
                {
                    close(tub[0][1]);
                    close(tub[1][0]);
                    close(tub[2][0]);
                    close(tub[2][1]);
                    close(tub[3][0]);
                    close(tub[3][1]);
                    pidbisnieto=fork();
                        if(pidbisnieto==0)//B0
                    {
                        read(tub[0][0], line, MAXLINE);
                        printf("%s\n", line);
/*B0*/                  write(tub[1][1],"Recibido", MAXLINE);
                        sleep(4);
                        close(tub[0][0]);
                        close(tub[1][1]);
                        exit(0);
                    }
                        else if(pidbisnieto>0)
                    {
                        close(tub[0][0]);
                        close(tub[1][1]);
                        wait(NULL);
                        exit(0);
                    }
                        else if(pidbisnieto<0)
                    {
                        printf("Error: no se ha creado el hijo");
                        exit(1);
                    }
                }
                if(i==0)//N0
                {
                    close(tub[0][0]);
                    close(tub[1][1]);
                    close(tub[2][0]);
                    close(tub[3][1]);
                    write(tub[0][1],"Iniciar", MAXLINE);
                    read(tub[1][0], line, MAXLINE);
                    printf("%s\n", line);
                    sleep(2);
                    write(tub[2][1],"Iniciar", MAXLINE);
                    read(tub[3][0], line, MAXLINE);
                    printf("%s\n", line);
                    sleep(4);
                    close(tub[0][1]);
                    close(tub[1][0]);
                    close(tub[2][1]);
                    close(tub[3][0]);
                    exit(0);
                }
                if(i==2)//N2
                {
                close(tub[0][0]);
                close(tub[0][1]);
                close(tub[1][0]);
                close(tub[1][1]);
                close(tub[2][1]);
                close(tub[3][0]);
                pidbisnieto=fork();
                    if(pidbisnieto==0)//B1
                    {
                        read(tub[2][0], line, MAXLINE);
                        printf("%s\n", line);
/*B1*/                write(tub[3][1],"Recibido", MAXLINE);
                        sleep(4);
                        close(tub[2][0]);
                        close(tub[3][1]);
                        exit(0);
                    }
                    else if(pidbisnieto>0)
                    {
                        close(tub[2][0]);
                        close(tub[3][1]);
                        wait(NULL);
                        exit(0);
                    }
                    else if(pidbisnieto<0)
                    {
                        printf("Error: no se ha creado el hijo");
                        exit(1);
                    }
                }
            }
            else if(nieto[i]<0)
            {
                printf("Error: no se ha creado el hijo");
                exit(1);
            }
        }
                close(tub[0][0]);
                close(tub[0][1]);
                close(tub[1][0]);
                close(tub[1][1]);
                close(tub[2][0]);
                close(tub[2][1]);
                close(tub[3][0]);
                close(tub[3][1]);
        for(n=0; n<3; n++)
        {
            wait(NULL);
        }
    }

    else if(hijo>0)
    {
        wait(NULL);
    }
    else if(hijo<0)
    {
        printf("Error: no se ha creado el hijo");
        exit(1);
    }

return 0;
}
