#include "rudi_server.h"

int
list_handler(int conn) {

        char data[100] , sendBuff[1024] , temp_buff[1024];
        int fun_status , p_size , index = 0;
        r_dentry *ptr;

        if (root_node == NULL)
                return 1;

        memset (sendBuff , 0 , sizeof(sendBuff));
        memset (data , 0 , sizeof(data));
        memset (temp_buff , 0 , sizeof(temp_buff));

        sprintf (data , "%d" , fun_status);
        strcat (sendBuff , data);

        memcpy (sendBuff + 1 , &ptr , sizeof(r_dentry));

        if (fun_status == 0) {
                for (ptr = root_node->next , index = 1 ; ptr != NULL;
                ptr = ptr->next , index++) {

                        memcpy (sendBuff + 1 + sizeof(r_dentry) * index , &ptr ,
                        sizeof(r_dentry));

                }
                memset (data , 0 , sizeof(data));
                sprintf (data , "%d" , -1);
                strcat (sendBuff , data);

                write (conn , sendBuff , sizeof(sendBuff)-1);

                if (sendBuff == NULL)
                        return 1;

                return 0;
        }
}
