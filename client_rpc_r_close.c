#include "rudi_client.h"

/*TO close the open file on the remote Server*/
int
rClose (struct r_file *file) {
        char sendBuff[1024] , recvBuff[1024] , len[5] , data[500];
        int p_size , res;

        /*Check, if Connection is established */
        if (sockfd >= 0) {
                /*Is the argument set */
                if (file == NULL) {
                        return -1;
                }
                /*Clean the writing Buffer */
                memset (sendBuff , 0 , sizeof (sendBuff));
                sprintf (data , "%d" , _r_close);
                strcat (sendBuff , data);
                /*p_size = sizeof (file);
                sprintf (len , "%d" , p_size);
                char temp_buff[p_size];*/

                memcpy (sendBuff + strlen (sendBuff) , &file , sizeof (r_file));
                /*strcat (sendBuff , len);
                strcat (sendBuff , sendBuff);*/
                /*Sending the Buffer content to the server */
                write (sockfd , sendBuff , sizeof(sendBuff)-1);
                /*Receiving server response */
                p_size = read (sockfd , recvBuff , sizeof(recvBuff)-1);
                recvBuff[p_size] = 0;
                /*Unmarshalling the response */
                memset (data , 0 , sizeof (data));
                memcpy (data , &recvBuff , 1);
                if (atoi (data) == 0) {
                        return 0;
                } else {
                        memset (data , 0 , sizeof (data));
                        memcpy (data , &recvBuff[1] , 2);
                        return atoi (data);
                }
        } else {
                return 1;
        }
}


int
main () {
        r_file *file;
        int n= rClose (file);
        printf ("RES : %d",n);

}

