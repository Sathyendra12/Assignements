#include "rudi_client.h"

/* TO close the open file on the remote Server
 *
 * INPUT:
 *      struct r_file *file :   File identifier of the file to be closed
 *
 * OUTPUT:
 *      int :   Success/Failure indicator
 */
int
r_close (struct r_file *file) {
        char sendBuff[1024] , recvBuff[1024] , len[5] , data[500];
        int p_size , ret = -1;

        /*Check, if Connection is established */
        if (sockfd >= 0) {
                /*Is the argument set */
                if (file == NULL) {
                        ret = -1;
                        goto out;
                }
                /*Clean the writing Buffer */
                memset (sendBuff , 0 , sizeof (sendBuff));
                sprintf (data , "%d" , _r_close);
                strcat (sendBuff , data);
                memcpy (sendBuff + strlen (sendBuff) , file , sizeof (r_file));
                /*Sending the Buffer content to the server */
                write (sockfd , sendBuff , sizeof(sendBuff) - 1);
                /*Receiving server response */
                p_size = read (sockfd , recvBuff , sizeof(recvBuff) - 1);
                recvBuff[p_size] = 0;
                /*Unmarshalling the response */
                memset (data , 0 , sizeof (data));
                memcpy (data , &recvBuff , 1);
                if (atoi (data) == 0) {
                        ret = 0;
                        goto out;
                } else {
                        memset (data , 0 , sizeof (data));
                        memcpy (data , &recvBuff[1] , 2);
                        ret = atoi (data);
                        goto out;
                }
        } else {
                ret = -1;
                goto out;
        }
out:
        return ret;
}
