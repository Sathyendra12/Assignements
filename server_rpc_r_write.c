/* This function unmarshal the write request from the client and perform the 
 * requested task and marshal the response and send it to the client
 * 
 * input parameters:
 *                      int conn                 :socket connection identifier
 * 
 *                      char *recvBuff                :this contains the client
 * request
 * 
 * 
 * output parameters:
 *                      int         :Success(0)/Failure(-1) indicator
 * */

#include "rudi_server.h"

int
write_handler (int conn , char *recvBuff) {

        char len[10] , data[100] , sendBuff[1024] , buffer[1024];
        int read_status = 0 , p_size = 0 , ind = 0 , ret = -1;
        ssize_t size;
        r_file *file = (r_file *) malloc (sizeof (r_file));

        memset (data , 0 , sizeof(data));
        memset (len , 0 , sizeof(len));
        /*Start from 4th character since first 3 character specifies the
        *function enum*/
        memcpy (len , &recvBuff[3] , 1);
        ind += 4;
        p_size = atoi (len);
        memcpy (data , &recvBuff[ind] , p_size);
        ind += p_size;
        size = atoi (data);
	memcpy (buffer , &recvBuff[ind] , size);
        ind += size;

        memcpy (file , &recvBuff[ind] , sizeof(r_file));

        /* request to read the files */
        size = r_write1 (file , buffer , size);

        if (size == -1 || size == -2) {
                /*storing the error enums into the buffer*/
                memset (sendBuff , 0 , sizeof(sendBuff));

                memset (data , 0 , sizeof(data));
                sprintf (data , "%d" , 1);
                strcat (sendBuff , data);
                memset (data , 0 , sizeof(data));

                if (size == -1)
                        sprintf (data , "%d" , write_failed);
                else
                        sprintf (data , "%d" , not_enough_content);

                strcat (sendBuff , data);
                ret = 0;
                goto out;
        } else {

                /*storing the file contents into the buffer*/
                memset (sendBuff , 0 , sizeof(sendBuff));
                sprintf (data , "%d" , 0);
                strcat (sendBuff , data);

                memset (data , 0 , sizeof(data));
                sprintf (data , "%d" , size);
                p_size = strlen (data);
                sprintf (len , "%d" , p_size);
                strcat (sendBuff , len);
                strcat (sendBuff , data);

                ret = 0;
                goto out;
        }

out:
        free(file);
        write (conn , sendBuff , sizeof(sendBuff)-1);
        return ret;
}
