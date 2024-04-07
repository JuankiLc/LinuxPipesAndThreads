#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 60
#define NUMNIETOS 2
#define NUMBIS 2
#define NUMTUBS 6
#define NUMPROCESOS 3

int main()
{
    int n, c;
    int i;
    int j;
    int b;
    int bucle=0;
    int cien=100;
    int tub[6][2];
    int fdes[2];
    int fich;
    pid_t hijo;
    char line[MAXLINE];
    pid_t nieto[NUMNIETOS];
    pid_t bisnieto[NUMBIS];
    pid_t pidbisnieto;
    pid_t procesos[NUMPROCESOS];

    hijo = fork();
    if(hijo==0)//Modulo
    {
        for(j=0; j<6; j++)
        {
            if(pipe(tub[j]) < 0)
            {
                exit(1);
            }
        }
        for(i=0; i<NUMNIETOS; i++)
        {
            nieto[i]=fork();
            if(nieto[i]==0)
            {
                if(i==1)//Conten2
                {
                    close(tub[0][1]);
                    close(tub[1][0]);
                    close(tub[2][1]);
                    close(tub[3][0]);
                    close(tub[4][1]);
                    close(tub[5][0]);

                    pidbisnieto=fork();
                    if(pidbisnieto==0)//Sopor
                    {
                        for(n=0; n<NUMPROCESOS; n++)
                        {
                            procesos[n] = fork();
                            if(procesos[n]==0)//Procesos
                            {
                                if(n=0)
                                {
                                    close(tub[2][0]);
                                    close(tub[3][1]);
                                    close(tub[4][0]);
                                    close(tub[5][1]);
                                    read(tub[n*2][0],line,MAXLINE);
                                    write(tub[n*2+1][1], "200", 1);
                                    sleep(1);
                                    write(1, "Proceso in\n", 20);
                                    sleep(1);
                                    write(1, "Proceso out\n", 20);
                                    close(tub[0][0]);
                                    close(tub[1][1]);
                                    exit(0);
                                }
                                if(n=1)
                                {
                                    close(tub[0][0]);
                                    close(tub[1][1]);
                                    close(tub[4][0]);
                                    close(tub[5][1]);
                                    read(tub[n*2][0],line,MAXLINE);
                                    write(tub[n*2+1][1], "200", 1);
                                    sleep(1);
                                    write(1, "Proceso in\n", 20);
                                    sleep(1);
                                    write(1, "Proceso out\n", 20);
                                    close(tub[2][0]);
                                    close(tub[3][1]);
                                    exit(0);
                                }
                                if(n=2)
                                {
                                    close(tub[0][0]);
                                    close(tub[1][1]);
                                    close(tub[2][0]);
                                    close(tub[3][1]);
                                    read(tub[n*2][0],line,MAXLINE);
                                    write(tub[n*2+1][1], "200", 1);
                                    sleep(1);
                                    write(1, "Proceso in\n", 20);
                                    sleep(1);
                                    write(1, "Proceso out\n", 20);
                                    close(tub[4][0]);
                                    close(tub[5][1]);
                                    exit(0);
                                }
                            }
                            else if(procesos[n]<0)
                            {
                                printf("Error: no se ha creado el proceso");
                                exit(1);
                            }
                        }


                    }
                    else if(pidbisnieto>0)
                    {
                        close(tub[0][0]);
                        close(tub[1][1]);
                        close(tub[2][0]);
                        close(tub[3][1]);
                        close(tub[4][0]);
                        close(tub[5][1]);
                        wait(NULL);
                        exit(0);
                    }
                    else if(pidbisnieto<0)
                    {
                        printf("Error: no se ha creado el hijo");
                        exit(1);
                    }
                }
                if(i==0)//Conten1
                {
                    close(tub[0][0]);
                    close(tub[1][1]);
                    close(tub[2][0]);
                    close(tub[3][1]);
                    close(tub[4][0]);
                    close(tub[5][1]);
                    if (pipe(fdes) < 0)
                        exit(1);
                    for(b=0; b<NUMBIS; b++)
                    {
                        bisnieto[b]=fork();
                        if(bisnieto[b]==0)
                        {
                            if(b==1) //Controla
                            {
                                close(fdes[1]);
                                close(tub[0][0]);
                                close(tub[1][1]);
                                close(tub[2][0]);
                                close(tub[3][1]);
                                close(tub[4][0]);
                                close(tub[5][1]);

                                read(fdes[0], line, 20);

                                for(c=0; c<4; c++)
                                {
                                for(n=0; n<NUMPROCESOS; n++)
                                {
                                    write(tub[n*2][1], &cien, 4);
                                    sleep(3);
                                }
                                for(n=0; n<NUMPROCESOS; n++)
                                {
                                    read(tub[n*2+1][0], line, 1);
                                }
                                sleep(2);
                                }
                                sleep(2);

                                read(fdes[0], line, 20);
                                close(fdes[0]);
                                close(tub[0][1]);
                                close(tub[1][0]);
                                close(tub[2][1]);
                                close(tub[3][0]);
                                close(tub[4][1]);
                                close(tub[5][0]);
                                exit(0);
                            }
                            if(b==0)//Actuador
                            {
                                close(fdes[0]);
                                for(c=0; c<NUMTUBS; c++)
                                {
                                    close(tub[c][1]);
                                }
                                for(c=0; c<NUMTUBS; c++)
                                {
                                    close(tub[c][0]);
                                }
                                sleep(2);
                                fich = open("/temp/aviso.txt", O_RDONLY);
                                write(fdes[1], "inicio", 20);
                                sleep(5);
                                write(fdes[1], "final", 20);
                                close(fdes[1]);
                                exit(0);
                            }
                        }
                        else if(bisnieto[b]<0)
                        {
                            printf("Error: no se ha creado el hijo");
                            exit(1);
                        }
                    }
                    close(tub[0][1]);
                    close(tub[1][0]);
                    close(tub[2][1]);
                    close(tub[3][0]);
                    close(tub[4][1]);
                    close(tub[5][0]);
                    close(fdes[0]);
                    close(fdes[1]);
                    exit(0);
                }
            }
            else if(nieto[i]<0)
            {
                printf("Error: no se ha creado el hijo");
                exit(1);
            }
        }
        for(c=0; c<NUMTUBS; c++)
        {
            close(tub[c][1]);
        }
        for(c=0; c<NUMTUBS; c++)
        {
            close(tub[c][0]);
        }
        for(n=0; n<NUMNIETOS; n++)
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
        printf("Error: no se ha creado el modulo");
        exit(1);
    }

    return 0;
}
