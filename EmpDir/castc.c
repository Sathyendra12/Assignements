#include <stdio.h>
#include <math.h>
int
main () {
        int p_size = 500;
        char ch_rep[102];
        sprintf (ch_rep,"%d",p_size);
        printf (" %s \n ATOI:\t%d\n",ch_rep,atoi(ch_rep));
        return 0;
}