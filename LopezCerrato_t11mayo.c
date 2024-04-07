#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXLINE 50
#define MAXLINEREAD 20
#define NUMNIETOS 2
#define NUMBISNIETOS 5

int main()
{
    int n, tubSt[2], tubP0[2], i, j, fich;
    int tub[3][2];
    pid_t hijo;
    char line[MAXLINE];
    char lineread[MAXLINEREAD];
    pid_t nieto[NUMNIETOS];
    pid_t pidbisnieto[NUMBISNIETOS];
    int diez=10;
    char veinte="20";
    int k=0;
    int r=0;

    hijo = fork();
    if(hijo==0)//Plataforma
    {
        if (pipe(tubSt) < 0)
        {
            exit(1);
        }
        for(i=0; i<NUMNIETOS; i++)
        {
            nieto[i]=fork();
            if(nieto[i]==0)
            {
                if(i==1)//Soporte
                {
                    close(tubSt[1]);
                    if (pipe(tubP0) < 0)
                    {
                        exit(1);
                    }
                    for(n=0; n<3; n++)
                    {
                        if(pipe(tub[n]) < 0)
                        {
                            exit(1);
                        }
                    }

                    for(j=0; j<NUMNIETOS; j++)
                    {
                        pidbisnieto[j]=fork();
                        if(pidbisnieto[j]==0)
                        {
                            if(i==4)//P3
                            {
                                close(tubSt[0]);
                                close(tubP0[0]);
                                close(tubP0[1]);
                                close(tub[0][0]);
                                close(tub[0][1]);
                                close(tub[1][0]);
                                close(tub[1][1]);
                                close(tub[2][1]);
                                close(tub[3][0]);

                                read(tub[2][0], line, MAXLINE);
                                printf("Recogido relevo");
                                write(tub[3][1],"Relevo", MAXLINE);

                                if(r==5)
                                {
                                    close(tub[2][0]);
                                    close(tub[3][1]);
                                    exit(0);
                                }




                            }
                            if(i==3)//P2
                            {
                                close(tubSt[0]);
                                close(tubP0[0]);
                                close(tubP0[1]);
                                close(tub[0][0]);
                                close(tub[0][1]);
                                close(tub[1][1]);
                                close(tub[2][0]);
                                close(tub[3][0]);
                                close(tub[3][1]);

                                read(tub[1][0], line, MAXLINE);
                                printf("Recogido relevo");
                                write(tub[2][1],"Relevo", MAXLINE);

                                if(r==5)
                                {
                                    close(tub[1][0]);
                                    close(tub[2][1]);
                                    exit(0);
                                }



                            }
                            if(i==2)//P1
                            {
                                close(tubSt[0]);
                                close(tubP0[0]);
                                close(tubP0[1]);
                                close(tub[0][1]);
                                close(tub[1][0]);
                                close(tub[2][0]);
                                close(tub[2][1]);
                                close(tub[3][0]);
                                close(tub[3][1]);

                                read(tub[0][0], line, MAXLINE);
                                printf("Recogido relevo");
                                write(tub[1][1],"Relevo", MAXLINE);

                                if(r==5)
                                {
                                    close(tub[0][0]);
                                    close(tub[1][1]);
                                    exit(0);
                                }



                            }
                            if(i==1)//P0
                            {
                                close(tubSt[0]);
                                close(tubP0[1]);
                                close(tub[0][0]);
                                close(tub[1][0]);
                                close(tub[1][1]);
                                close(tub[2][0]);
                                close(tub[2][1]);
                                close(tub[3][1]);

                                if(k=0)
                                {
                                    read(tubP0[0], line, 1);
                                    k++;
                                }
                                printf("Recogido relevo");
                                write(tub[0][1],"Relevo", MAXLINE);

                                r++;
                                if(r==5)
                                {
                                    close(tub[0][1]);
                                    close(tub[3][0]);
                                    close(tubP0[0]);
                                    exit(0);
                                }

                            }
                            if(i==0)//sincro
                            {
                                close(tubP0[0]);
                                close(tub[0][0]);
                                close(tub[0][1]);
                                close(tub[1][0]);
                                close(tub[1][1]);
                                close(tub[2][0]);
                                close(tub[2][1]);
                                close(tub[3][0]);
                                close(tub[3][1]);

                                read(tubSt[0], line, MAXLINE);
                                write(tubP0[1], veinte,1);
                                sleep(4);

                                close(tubP0[1]);
                                close(tubSt[0]);
                                exit(0);
                            }
                        }
                    }

                }
                if(i==0)//Inicia
                {
                    close(tubSt[0]);
                    sleep(3);
                    write(tubSt[1],diez,MAXLINE);
                    sleep(7);
                    close(tubSt[1]);
                    exit(0);
                }
            }
            else if(nieto[i]<0)
            {
                printf("Error: no se ha creado el nieto");
                exit(1);
            }
        }
        close(tubSt[0]);
        close(tubSt[1]);
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
        printf("Error: no se ha creado el hijo");
        exit(1);
    }

    return 0;
}



