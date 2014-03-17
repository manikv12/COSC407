#include <stdio.h>
#include <stdlib.h>

double f(double x, double y);
double h(double a, double b, int n);
int main()
{
    int n = 100;
    double eps = 0.000001;
    double hx, hy, xa = -1.0, xb = 1.0, ya = -1.0, yb = 1.0;
    while((h(xa, xb, n) >= eps) || (h(ya, yb, n) >= eps))
    {
        hx = h(xa, xb, n);
        hy = h(ya, yb, n);
        double m = f(xa, xb);
        double mx = xa;
        double my = ya;
        double t;
        int i,j;
        for(i = 0;i < n;i++)
        {
            for(j = 0;j < n;j++)
            {
                t = f(xa+(hx*i), ya+(hy*j));
                if(t<m)
                {
                    m = t;
                    mx = xa+(hx*i);
                    my = ya+(hy*j);
                }
            }
            xa = mx-hx;
            xb = mx+hx;
            ya = my-hy;
            yb = my+hy;
        }
    }
    double minx = (xa + xb)/(double)2;
    double miny = (ya + yb)/(double)2;
    printf("xa: %f\nxb: %f\nya: %f\nyb: %f\nGlobal min: x = %f, y = %f\n", xa, xb, ya, yb, minx, miny);
    return 0;
}

double f(double x, double y)
{
    return(x*x*(4-2.1*x*x+(x*x*x*x)/3)+x*y+y*y*(-4+4*y*y));
}

double h(double a, double b, int n)
{
    return (b-a)/(double)n;
}
