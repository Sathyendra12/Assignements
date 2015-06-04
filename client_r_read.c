#include "rudi_client.h"

ssize_t
rRead (r_file *file, char *buffer, ssize_t size) {

        int sta = size;

        if (sta > 0) {
                if (file != NULL) {
                        int siz = r_read (file , buffer , size);

                        sta = atoi(buffer);
                        if (siz == -1) {
                                if (sta == 16) {
                                        printf ("ERROR: File Not Open\n");
                                        printf ("\tRead failed\n");
                                } else if (sta == 14)
                                        printf ("ERROR: Not Enough Content\n");
                                else
                                        printf ("ERROR: Read Failed");
                        } else if (siz < size) {
                                printf ("File only Contain  : %d bytes\n",
                                siz);
                                printf ("File Content :\n\n%s\n\n" , buffer);
                        } else
                                printf ("File Content :\n\n%s\n\n", buffer);

                } else
                        printf ("ERROR: File Not Open\n");
        } else
                printf ("ERROR: Please Enter a Valid Size\n");
        return 0;
}
