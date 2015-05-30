#include "rudi_client.h"

int
r_List (struct r_dentry *list) {

        char sendBuff[1024] , data[100] , recvBuff[1024];
        int read_status , read_bytes , p_size;
        r_dentry *new_entry , *node_ptr;

        if (sockfd == 0) {
                list = NULL;
                memset (sendBuff , 0 , sizeof(sendBuff));
                sprintf (data , "%d" , _r_list);
                strcat (sendBuff , data);

                if (sendBuff == NULL)
                        goto out;
                else
                        write (sockfd , sendBuff , sizeof(sendBuff)-1);

                read_bytes = read(sockfd , recvBuff , sizeof (recvBuff) - 1);

                if (read_bytes > 0) {
                        recvBuff[read_bytes] = 0;

                        /*copying first character of the recvBuff to check
                         * Whether the function call is successful on not*/
                        memcpy (data , &recvBuff , 1);
                        read_status = atoi(data);
                        if (read_status == 1) {
                                goto out;
                        } else {
                                /* Recreating the list from the received Buffer
                                 * */
                                read_bytes = 1;
                                memcpy (data , &recvBuff[read_bytes] , 1);
                                p_size = atoi(data);
                                do {
                                        read_bytes += 1;
                                        new_entry = (r_dentry *)malloc
                                        (sizeof(r_dentry));

                                        if (new_entry == NULL)
                                                goto out;
                                        new_entry->next = NULL;
                                        new_entry->prev = NULL;
                                        memcpy (new_entry , &recvBuff[1] ,
                                        p_size);

                                        if (list == NULL) {
                                                list = new_entry;
                                                node_ptr = list;
                                        } else {
                                                new_entry->prev = node_ptr;
                                                node_ptr->next = new_entry;
                                                node_ptr = new_entry;
                                        }
                                        read_bytes += p_size;
                                        memcpy (data , &recvBuff[read_bytes] ,
                                        1);

                                        p_size = atoi(data);

                                } while (p_size != -1);
                        }
                } else
                        goto out;

                return 0;
        } else {
                return 1;
        }
out:
        return 1;
}
