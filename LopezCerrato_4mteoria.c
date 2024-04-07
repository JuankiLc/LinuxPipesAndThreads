#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include “visual_GPIO_leds.h”
#define MAXLINE 60
#define NUMHIJOS 2
#define NUMLEDS 8

int main()
{
    int n, i, j, t, c, r;
    int tub[8][2];
    pid_t pid[NUMHIJOS], led[NUMLEDS];
    char line[MAXLINE];
    int estados[NUMLEDS];
    pid_t pidnieto;
    pid_t pidbisnieto;
    for(j=0; j<8; j++)
    {
        if(pipe(tub[j]) < 0)
        {
            exit(1);
        }
    }

    for(i=0; i<NUMHIJOS; i++)
    {
        pid[i] = fork();
        if(pid[i]==0)
        {
            if(i==0)//H0
            {
                for(c=0; c<NUMLEDS; c++)
                {
                    close(tub[c][1]);
                }
                for(r=0; r<NUMLEDS; r++)
                {
                    read(tub[r][0], &estados[r], 4);
                    visualizador(estados[r]);
                }

                for(c=0; c<NUMLEDS; c++)
                {
                    close(tub[c][0]);
                }
                exit(0);
            }
            else if(i==1)//H1
            {
                for(c=0; c<NUMLEDS; c++)
                {
                    close(tub[c][0]);
                }
                pidnieto=fork();
                if(pidnieto==0)//N0
                {
                    for(n=0; n<NUMLEDS; n++)
                    {
                        led[n] = fork();
                        if(led[n]==0)//LEDS
                        {
                            if(n%2==0)//PARES
                            {
                                for(t=0; t<NUMLEDS; t+=2)
                                {
                                    write(tub[n][1], "1", 1);
                                    sleep(1);
                                    write(tub[n][1], "0", 1);
                                    sleep(1);
                                }
                                sleep(1);
                            }
                            else if(n%2==1)//IMPARES
                        {
                            for(t=1; t<NUMLEDS; t+=2)
                                {
                                    write(tub[n][1], "0", 1);
                                    sleep(1);
                                    write(tub[n][1], "1", 1);
                                    sleep(1);
                                }
                                sleep(1);
                            }
                            exit(0);
                        }
                        else if(led[n]<0)
                        {
                            printf("Error: no se ha creado el led");
                            exit(1);
                        }
                    }
                    for(c=0; c<NUMLEDS; c++)
                    {
                        close(tub[c][1]);
                    }
                    exit(0);
                }
                else if(pidnieto>0)
                {
                    for(c=0; c<NUMLEDS; c++)
                    {
                        close(tub[c][1]);
                    }
                    wait(NULL);
                    exit(0);
                }
                else if(pidnieto<0)
                {
                    printf("Error: no se ha creado el hijo");
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
    for(i=0; i<NUMHIJOS; i++)
    {
        for(c=0; c<NUMLEDS; c++)
        {
            close(tub[c][0]);
        }
        for(c=0; c<NUMLEDS; c++)
        {
            close(tub[c][1]);
        }
        wait(NULL);
    }
    return 0;
}
