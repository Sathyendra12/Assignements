/*This function is used to print the file content on the Clinet side
 * that are read from the server
 *
 * input parameters:
 *                      struct r_file *file         :this will be the file
 * pointer which points to the file to be read
 *
 *                      char *buffer                :this character pointer
 * points to the file contents that are read from the server or the error
 * enum tha are read from the server
 *
 *                      ssize_t size                :the size of the file
 * content to be read from the server
 *
 * output parameters:
 *                      ssize_t         :Success(0)/Failure(-1) indicator
 * */

#include "rudi_client.h"

ssize_t
rRead (struct r_file *file, char *buffer, ssize_t size) {

        int status = 0 , ret = -1 , siz = 0 , flage = 0;
        unsigned long temp_size = 0 , bytes_read = 0 , temp_size1 = 0;
        unsigned long counter = 0;

        temp_size = size;
        if (temp_size > 0) {
                if (file != NULL) {

			if (file->write_flag == 1) {
				printf ("ERROR: File is Open in Write mode\n");
				goto out;
			}

                        /*if size is greater then 1024, then we have to
                        * repeatedly call the r_read function as server can
                        * send only 1024 bytes per request*/
                        if (temp_size > 1020) {
                                temp_size1 = size;
                                flage = 1;
loop:
                                if (temp_size > 1020) {
                                        size = 1020;
                                        temp_size -= 1020;
                                } else {
                                        size = temp_size;
                                        temp_size = 0;
                                }
                        }

                        siz = r_read (file , buffer , size);

                        if (siz == -1) {
                                /*checking the enum to print the error*/
                                /*if error occures the buffer will only
                                 * contain error enum*/
                                status = atoi(buffer);
                                if (status == file_not_open) {
                                        printf ("ERROR: File Not Open\n");
                                        printf ("\tRead failed\n");
                                } else if (status == not_enough_content) {
                                        if (flage == 1)
                                                goto message;
                                        printf ("ERROR: Not Enough Content\n");

                                } else
                                        printf ("ERROR: Read Failed");
                                ret = -1;
                                goto out;
                        } else if (siz < size) {
                                if (flage != 1) {

                                        printf ("File Contains only:");
                                        printf("%d bytes\n" , siz);
                                        printf ("File Content :\n\n%s\n\n",
                                        buffer);

                                        ret = 0;
                                        goto out;
                                } else if (flage == 1) {
                                        printf ("%s" , buffer);
                                        counter += siz;

                                        ret = 0;
                                        goto message;
                                }
                        } else {
                                if (flage == 1) {
                                        printf ("%s" , buffer);
                                        counter += siz;
                                        if (counter == temp_size1) {
                                                ret = 0;
                                                goto message;
                                        } else {
                                                goto loop;
                                        }
                                } else {
                                        printf ("File Content :\n\n%s\n\n"
								, buffer);
                                        ret = 0;
                                        goto out;
                                }
                        }
                } else
                        printf ("ERROR: File Not Open\n\n");
        } else
                printf ("ERROR: Please Enter a Valid Size\n\n");

message:
        if (flage == 1) {
                if (counter == temp_size1)
                        printf ("Total Contents read = %lu" , counter);
                else
                        printf ("File only Contain  : %lu bytes\n" , counter);
                flage = 0;
        }

out:
        memset(buffer , 0 , sizeof(buffer));
        return ret;
}
