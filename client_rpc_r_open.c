#include "rudi_client.h"

/*Open call that marshals the client request to the server
 * & receives the response */
int
r_Open (const char *filename, unsigned int mode, struct r_file *file) {
        char sendBuff[1024] , recvBuff[1024] , len[5] , data[500];
        int p_size , res , ind = 0;

        if (sockfd >= 0) {
                /*Clear the Buffer */
                memset (sendBuff , 0 , sizeof (sendBuff));
                sprintf (data , "%d" , _r_open);
                strcat (sendBuff , data);
                p_size = strlen (filename);
                sprintf (len , "%d" , p_size);
                strcat (sendBuff , len);
                strcat (sendBuff , filename);
                sprintf (data , "%d" , mode);
                strcat (sendBuff , data);
                p_size = sizeof (file);
                sprintf (len , "%d" , p_size);
                strcat (sendBuff , len);
                char temp_buff[p_size];

                memcpy (temp_buff , &file , p_size);
                strcat (sendBuff , temp_buff);
                /*Sending the Buffer content to the server */
                write (sockfd , sendBuff , sizeof(sendBuff)-1);
                /*Receiving server response */
                p_size = read (sockfd , recvBuff , sizeof(recvBuff)-1);
                recvBuff[p_size] = 0;
                /*Unmarshalling the response */
                memset (data , 0 , sizeof (data));
                memcpy (data , &recvBuff , 1);
                if (atoi (data) == 0) {
                        memcpy (file , &recvBuff[1] , p_size);
                        return 0;
                } else {
                        memset (data , 0 , sizeof (data));
                        memcpy (data , &recvBuff[1] , 2);
                        return atoi (data);
                }
        } else {
                /*Socket fd is not set */
                return 1;
        }
}