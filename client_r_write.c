/*This function is used to write to the file content on the Clinet side
 * to the server
 *
 * input parameters:
 *                      struct r_file *file         :this will be the file
 * pointer which points to the file to be written
 *
 *                      char *buffer                :this character pointer
 * points to the file contents that are to be written to server or the error
 * enum tha are read from the server
 *
 *                      ssize_t size                :the size of the file
 * content to write to the server
 *
 * output parameters:
 *                      ssize_t         :Success(0)/Failure(-1) indicator
 * */

#include "rudi_client.h"

ssize_t
rWrite (struct r_file *file, char *o_buffer, ssize_t size) {

        int status = 0 , ret = -1 , siz = 0 , o_siz = 0 , flag = 0;
        unsigned long temp_size = 0 , bytes_read = 0 , temp_size1 = 0;
        unsigned long counter = 0;
	int start1 = 0 ,start2, copy_ptr;
	char buffer[1024];

	if (file->write_flag == 0) {
		printf ("ERROR: File is Open in Read mode\n");
		goto out;
	}

        temp_size = size;
        if (temp_size > 0) {
                if (file != NULL) {
                        if (temp_size > 1000) {
                                temp_size1 = size;
                                flag = 1;
loop:
                                if (temp_size > 1000) {
                                        size = 1000;
                                        temp_size -= 1000;
                                } else {
                                        size = temp_size;
                                        temp_size = 0;
                                }
                        }
			for (start2 = 0, start1 = siz; start1 < size && start2 <= 1000; start1++ , start2++) {
				buffer[start2] =  o_buffer[start1];
			}
			if (start2 == 1001)
				buffer[1001]= '\0';
			o_siz = siz;
                        siz = r_write (file , buffer , size);

                        if (siz == -1) {
                                status = atoi(buffer);
                                if (status == file_not_open) {
                                        printf ("ERROR: File Not Open\n");
                                        printf ("\nWrite failed\n");
                                } else if (status == not_enough_content) {
                                        if (flag == 1)
                                                goto message;
                                        printf ("ERROR: Not Enough Content\n");

                                } else
                                        printf ("ERROR: Write Failed\n");
                                ret = -1;
                                goto out;
                        } else {
				siz += o_siz;
                                if (flag == 1) {
                                        printf ("%s" , buffer);
                                        counter += siz;
                                        if (counter == temp_size1) {
                                                ret = 0;
                                                goto message;
                                        } else {
                                                goto loop;
                                        }
                                } else {
                                        printf ("Content Written to file:\n%s\n" , buffer);
                                        ret = 0;
                                        goto out;
                                }
                        }
                } else
                        printf ("ERROR: File Not Open\n\n");
        } else
                printf ("ERROR: Please Enter a Valid Size\n\n");

message:
        if (flag == 1) {
                if (counter == temp_size1)
                        printf ("Total Contents written = %lu" , counter);
                else
                        printf ("Data only Contain  : %lu bytes\n" , counter);
                flag = 0;
        }

out:
        memset(buffer , 0 , sizeof(buffer));
        return ret;
}
