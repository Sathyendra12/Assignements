#include "rudi_client.h"

int
list_handler() {

        char data[100] , sendBuff[1024] , temp_buff[1024];
        int fun_status , p_size;
        r_dentry list , *ptr;

        fun_status = server_r_list(&list);

        memset (sendBuff , 0 , sizeof(sendBuff));
        memset (data , 0 , sizeof(data));
        memset (temp_buff , 0 , sizeof(temp_buff));

        sprintf (data , "%d" , fun_status);
        strcat (sendBuff , data);

        if (fun_status == 0) {
                for (ptr = &list ; ptr != NULL ; ptr = ptr->next) {
                        p_size = sizeof (ptr);
                        memset (data , 0 , sizeof(data));

                        sprintf (data , "%d" , p_size);

                        memset (temp_buff , 0 , sizeof(temp_buff));

                        memcpy (temp_buff , &ptr , p_size);
                        strcat (sendBuff , data);
                        strcat (sendBuff , temp_buff);
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
