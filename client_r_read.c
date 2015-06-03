#include "rudi_client.h"

ssize_t
rRead (r_file *file, char *buffer, ssize_t size) {
        int st;

        if (file != NULL) {
                int siz = r_read (file , buffer , size);

                st = atoi (buffer);
                if (siz == 1 && size != 1) {
                        if (st == 16)
                                printf ("ERROR: File Not Open\nRead failed\n");
                        else if (st == 14)
                                printf ("ERROR: Not Enough Content\n");
                } else if (siz < size) {
                        printf ("File only Contains : %d bytes\n" , siz);
                        printf ("File Content :\n%s\n\n" , buffer);
                } else
                        printf ("File Content :\n%s\n\n" , buffer);
        } else
                printf ("ERROR: File Not Open\n");
        return 0;
}
