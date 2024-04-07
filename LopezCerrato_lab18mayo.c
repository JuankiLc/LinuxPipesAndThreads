#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "visual_GPIO_leds.h"
#define NUMHIJOS 2
#define NUMLEDS 8

int main()
{
    int n, v, i, j, t, c, r, l;
    int tub[8][2];
    int estado;
    pid_t pid[NUMHIJOS], led[NUMLEDS];
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
                for(r=0; r<5; r++)
                {
                    for(l=0; l<NUMLEDS; l++)
                    {
                        read(tub[l][0], &estados[l], 4);
                    }
                    Visualizador((int*) estados, NUMLEDS);
                    sleep(1);
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
                            estado = n%2;
                            for(v=0; v<5; v++)
                            {
                                write(tub[n][1], &estado, 4);
                                sleep(1);
                                if(estado==1)
                                    estado=0;
                                else
                                    estado=1;
                            }
                            close(tub[n][1]);
                            sleep(1);
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

