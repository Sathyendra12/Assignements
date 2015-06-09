/* This function marshal the read request from the clinet send it to the server
 * and receives the response from the server and unmarshal it and then send it
 * to the client
 *
 * input parameters:
 *                      struct r_file *file         :this will be the file
 * pointer which points to the file to be read
 *
 *                      char *buffer                :this character pointer
 * points to the file contents that are read from the client or the error enum
 * tha areread from the server
 *
 *                      ssize_t size                :the size of the file
 * content to be read from the server
 *
 * output parameters:
 *                      ssize_t         :Success(0)/Failure(-1) indicator
 * */

#include "rudi_client.h"

ssize_t
r_read (r_file *file , char *buffer , ssize_t size) {

        char sendBuff[1024] , len[10] , data[100] , recvBuff[1024];
        int read_status = 0 , p_size = 0 , ind = 0 , ret = -1;
        ssize_t read_size;

        if (sockfd >= 0) {
                /* Marshaling the request to send to the server */
                memset (sendBuff , 0 , sizeof(sendBuff));
                sprintf (data , "%d" , _r_read);
                strcat (sendBuff , data);

                memset (data , 0 , sizeof(data));
                sprintf (data , "%d" , size);
                p_size = strlen (data);
                memset (len , 0 , sizeof(len));
                sprintf (len , "%d" , p_size);
                strcat (sendBuff , len);
                strcat (sendBuff , data);
                memcpy (sendBuff + strlen (sendBuff) , file , sizeof (r_file));

                if (sendBuff == NULL) {
                        ret = -1;
                        goto out;
                } else {
                        write (sockfd , sendBuff , sizeof(sendBuff)-1);
                }

                read_size = read(sockfd , recvBuff , sizeof (recvBuff) - 1);
                if (read_size == -1) {
                        ret = -1;
                        goto out;
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
                                ret = -1;
                                goto out;

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
                                ret = read_size;
                                goto out;
                        }
                }

        }
out:
        return ret;
}
