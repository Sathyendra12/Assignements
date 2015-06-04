#include "rudi_client.h"

ssize_t
r_read (r_file *file , char *buffer , ssize_t size) {

        char sendBuff[1024] , len[10] , data[100] , recvBuff[1024];
        int read_status , p_size , ind = 0;
        ssize_t read_size;

        if (sockfd >= 0) {
                /* Marshaling the data to send to the server */
                memset (sendBuff , 0 , sizeof(sendBuff));
                sprintf (data , "%d" , _r_read);
                strcat (sendBuff , data);

                memset (data , 0 , sizeof(data));
                sprintf (data , "%d" , size);
                p_size = strlen (data);
                sprintf (len , "%d" , p_size);
                strcat (sendBuff , len);
                strcat (sendBuff , data);
                memcpy (sendBuff + strlen (sendBuff) , file , sizeof (r_file));

                if (sendBuff == NULL)
                        return -1;
                else{
                        write (sockfd , sendBuff , sizeof(sendBuff)-1);
                }

                read_size = read(sockfd , recvBuff , sizeof (recvBuff) - 1);
                if (read_size == -1) {
                        return -1;
                } else {

                        recvBuff[read_size] = 0;

                        /*copying first character of the recvBuff to check
                         * Whether the function call is successful on not*/
                        memset (data , 0 , sizeof(data));
                        memcpy (data , recvBuff , 1);
                        read_status = atoi(data);
                        if (read_status != 0) {

                                memset (buffer , 0 , sizeof(buffer));
                                memcpy (buffer , &recvBuff[1] , 2);
                                return -1;

                        } else {

                                /* Un-Marshaling the data recevied from
                                 * the server */
                                memset (data , 0 , sizeof(data));
                                memcpy (len , &recvBuff[1] , 1);
                                ind = atoi(len);
                                memcpy (data , &recvBuff[2] , ind);
                                read_size = atoi(data);

                                ind += 2;
                                memset (buffer , 0 , sizeof(buffer));
                                memcpy (buffer , &recvBuff[ind] , read_size);
                                buffer[read_size] = 0;
                                return read_size;
                        }
                }

        }
}
