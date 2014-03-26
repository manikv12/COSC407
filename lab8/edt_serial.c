#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
int main()
{
    int matrix1[5][5] = {{1,1,1,1,1},{1,1,1,1,1},{1,1,0,1,1},{1,1,1,1,1},{1,1,1,1,1}};
    int matrix2[5][5] = {{1,1,1,1,1},{1,0,1,1,1},{1,1,0,1,1},{1,1,1,1,1},{1,1,1,1,1}};
    int matrix3[5][5] = {{1,1,1,1,1},{1,0,1,0,1},{1,1,1,1,1},{1,1,0,1,1},{1,1,1,1,1}};
    int squared_matrix1[5][5];
    int squared_matrix2[5][5];
    int squared_matrix3[5][5];
    int a,b,c,d, N=5;
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            squared_matrix1[a][b] = 1000;
            squared_matrix2[a][b] = 1000;
            squared_matrix3[a][b] = 1000;
        }
    }
    
    printf("\nSerial Implementation\n");
        
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            if(0 == matrix1[a][b])
            {
                for(c=0;c<N;c++)
                {
                    for(d=0;d<N;d++)
                    {
                        int tmp = abs(pow(c-a,2)) + abs(pow(d-b,2));
                        if((squared_matrix1[c][d] > tmp))
                        {
                            squared_matrix1[c][d] = tmp;
                        }
                    }
                }
            }
        }
    }
    printf("%s\nMatrix 1\t%sEDT Matrix\n", RED, GRN);
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            printf("%s%d ", CYN, matrix1[a][b]);
        }
        printf("\t");
        for(b=0;b<N;b++)
        {
            printf("%s%d ", WHT, squared_matrix1[a][b]);
        }
        printf("\n");
    }
    printf("\n");

    

    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            if(0 == matrix2[a][b])
            {
                for(c=0;c<N;c++)
                {
                    for(d=0;d<N;d++)
                    {
                        int tmp = abs(pow(c-a,2)) + abs(pow(d-b,2));
                        if((squared_matrix2[c][d] > tmp))
                        {
                            squared_matrix2[c][d] = tmp;
                        }
                    }
                }
            }
        }
    }
    printf("%s\nMatrix 2\t%sEDT Matrix\n", RED, GRN);
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            printf("%s%d ", CYN, matrix2[a][b]);
        }
        printf("\t");
        for(b=0;b<N;b++)
        {
            printf("%s%d ", WHT, squared_matrix2[a][b]);
        }
        printf("\n");
    }
    printf("\n");


    
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            if(0 == matrix3[a][b])
            {
                for(c=0;c<N;c++)
                {
                    for(d=0;d<N;d++)
                    {
                        int tmp = abs(pow(c-a,2)) + abs(pow(d-b,2));
                        if((squared_matrix3[c][d] > tmp))
                        {
                            squared_matrix3[c][d] = tmp;
                        }
                    }
                }
            }
        }
    }
    printf("%s\nMatrix 3\t%sEDT Matrix\n", RED, GRN);
    for(a=0;a<N;a++)
    {
        for(b=0;b<N;b++)
        {
            printf("%s%d ", CYN, matrix3[a][b]);
        }
        printf("\t");
        for(b=0;b<N;b++)
        {
            printf("%s%d ", WHT, squared_matrix3[a][b]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}
