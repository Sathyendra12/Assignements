#include "rudi_client.h"

/* Open call that marshals the client request to the server &
 * receives the response
 *
 * INPUTS:
 *      const char *filename :  Name of the file to be opened
 *      unsigned int mode :     File open mode
 *      struct r_file **file :  Structure that will hold details of the file
 *                              that is opend by the call.
 *
 * OUTPUT:
 *      int :   Success/Failure indicator.
 */
int
r_open (const char *filename, unsigned int mode, struct r_file **file) {
        char sendBuff[1024] , recvBuff[1024] , len[5] , data[500] , len_cat[5];
        int p_size , ret = -1 , ind = 0;

        if (sockfd >= 0) {
                /*Clear the Buffer */
                memset (sendBuff , 0 , sizeof (sendBuff));
                sprintf (data , "%d" , _r_open);
                strcat (sendBuff , data);
                p_size = strlen (filename);
                memset (len_cat , 0 , sizeof (len_cat));
                memset (len , 0 , sizeof (len));
                sprintf (len_cat , "%d" , 0);
                sprintf (len , "%d" , p_size);
                if (strlen (len) == 1) {
                        strcat (len_cat , len);
                        strcpy (len , len_cat);
                }
                strcat (sendBuff , len);
                strcat (sendBuff , filename);
		memset (data , 0 , sizeof (data));
                sprintf (data , "%d" , mode);

                strcat (sendBuff , data);
                memcpy (sendBuff + strlen (sendBuff) , *file , p_size);
                /*Sending the Buffer content to the server */
                write (sockfd , sendBuff , sizeof(sendBuff) - 1);
                /*Receiving server response */
                p_size = read (sockfd , recvBuff , sizeof(recvBuff) - 1);
                recvBuff[p_size] = 0;
                /*Unmarshalling the response */
                memset (data , 0 , sizeof (data));
                memcpy (data , &recvBuff , 1);
                if (atoi (data) == 0) {
                        memcpy (*file , &recvBuff[1] , sizeof (r_file));
                        ret = 0;
                        goto out;
                } else {
                        memset (data , 0 , sizeof (data));
                        memcpy (data , &recvBuff[1] , 2);
                        ret = atoi (data);
                        goto out;
                }
        } else {
                /*Socket fd is not set */
                ret = -1;
                goto out;
        }
out:
        return ret;
}
