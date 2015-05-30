#include "rudi_client.h"

ssize_t
r_read (r_file *file , char *buffer , ssize_t size) {

        char sendBuff[1024] , len[10] , data[100] , recvBuff[1024];
        int read_status , p_size , ind = 0;
        ssize_t read_size;

        if (sockfd == 0) {
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

                memset (data , 0 , sizeof(data));
                p_size = sizeof (file);
                sprintf (len , "%d" , p_size);

                char temp_buff[p_size];

                memcpy (temp_buff , &file , p_size);
                strcat (sendBuff , len);
                strcat (sendBuff , temp_buff);

                if (sendBuff == NULL)
                        return -1;
                else
                        write (sockfd , sendBuff , sizeof(sendBuff)-1);

                read_size = read(sockfd , recvBuff , sizeof (recvBuff) - 1);
                if (read_size == -1) {
                        return -1;
                } else {

                        recvBuff[read_size] = 0;

                        /*copying first character of the recvBuff to check
                         * Whether the function call is successful on not*/
                        memcpy (data , &recvBuff , 1);
                        read_status = atoi(data);

                        if (read_status == 1) {

                                memset (data , 0 , sizeof(data));
                                memcpy (data , &recvBuff[1] , 2);
                                return atoi(data);

                        } else {

                                /* Un-Marshaling the data recevied from
                                 * the server */
                                memset (data , 0 , sizeof(data));
                                memcpy (len , &recvBuff[1] , 1);
                                ind = atoi(len);
                                memcpy (data , &recvBuff[2] , ind);
                                read_size = atoi(data);

                                ind += 2;
                                memset (data , 0 , sizeof(data));
                                memcpy (buffer , &recvBuff[ind] , read_size);

                                return read_size;
                        }
                }

        }
}
