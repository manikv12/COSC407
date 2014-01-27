#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void countsort();
int main(int argc, char *argv[])
{
    int a[] = {5,7,3,8,9,23,28734,37,3,48,51,1,9,54};
    countsort(a, 14);
    return 0;
}

void countsort(int a[], int n) {
    int i, j, count;
    int∗ temp = malloc(n∗sizeof(int));
    for (i = 0; i < n; i++) {
    count = 0;
    for (j = 0; j < n; j++)
        if (a[j] < a[i]) count++;
        else if (a[j] == a[i] && j < i) count++;
        temp[count] = a[i];
    }
    memcpy(a, temp, n∗sizeof(int));
    free(temp);
    for(i = 0; i < n; i++)
        print("%d, ",a[i]);
}
