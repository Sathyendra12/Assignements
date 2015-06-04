#include "rudi_client.h"

int
r_list (struct r_dentry *list) {

        char sendBuff[1024] , data[100] , recvBuff[1024];
        int read_status , read_bytes , index = 0;
        r_dentry *new_entry , *node_ptr;

        if (sockfd >= 0) {
                root = NULL;
                memset (sendBuff , 0 , sizeof(sendBuff));
                sprintf (data , "%d" , _r_list);
                strcat (sendBuff , data);

                if (sendBuff == NULL) {
                        goto out;
                } else
                        write (sockfd , sendBuff , sizeof(sendBuff)-1);

                read_bytes = read(sockfd , recvBuff , sizeof (recvBuff));

                if (read_bytes > 0) {

                        recvBuff[read_bytes] = 0;

                        /*copying first character of the recvBuff to check
                         * Whether the function call is successful on not*/
                        memset (data , 0 , sizeof(data));
                        memcpy (data , recvBuff , 1);
                        read_status = atoi(data);
                        memset (data , 0 , sizeof(data));
                        if (read_status == 1) {
                                /* Extracting the error enum to send response
                                 * to client */
                                memcpy (data , recvBuff + 1 , 2);
                                return atoi(data);
                        } else {
                                /*Extracting the next two character of the
                                 * recvBuff as it contains the number of files
                                 * on the Export Point
                                 * */
                                memset (data , 0 , sizeof(data));
                                memcpy (data , recvBuff + 1 , 2);
                                read_status = atoi(data);
                                /* Recreating the linked list from the
                                 * received Buffer */
                                while (index < read_status) {

                                        memset(data , 0 , sizeof(data));

                                        new_entry = (r_dentry *)malloc
                                        (sizeof(r_dentry));

                                        new_entry->next = NULL;
                                        new_entry->prev = NULL;
                                        /*here number 3 is used to leave the
                                        first 3 chacters as they are not part of
                                         * linked list.
                                         * */
                                        memcpy(new_entry,
                                        recvBuff + 3  +
                                        (sizeof(r_dentry) * index),
                                        sizeof(r_dentry));

                                        if (root == NULL) {
                                                root = new_entry;
                                                node_ptr = root;
                                        } else {
                                                new_entry->prev = node_ptr;
                                                node_ptr->next = new_entry;
                                                node_ptr = new_entry;
                                        }
                                        index += 1;

                                }

                                list = root;

                        }
                } else if (read_bytes == -1) {
                        goto out;
                }

                return 0;
        } else {
                return 1;
        }
out:
        return 1;
}
