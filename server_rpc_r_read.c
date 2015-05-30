#include "rudi_server.h"

int
read_handler (int conn , char *recvBuff) {

        char len[10] , data[100] , sendBuff[1024] , *buffer;
        int read_status , p_size , ind = 0;
        ssize_t size;
        r_file file;

        memset (data , 0 , sizeof(data));
        memset (len , 0 , sizeof(len));

        /*Start from 4th character since first 3 character specifies the
        *function enum*/
        memcpy (len , &recvBuff[3] , 1);
        ind += 1;

        p_size = atoi (len);
        memcpy (data , &recvBuff[ind] , p_size);
        ind += p_size;
        size = atoi (data);

        memcpy (len , &recvBuff[ind] , 1);
        ind += 1;
        p_size = atoi (len);

        memcpy (&file , &recvBuff[ind] , p_size);

        size = r_read(file , buffer , size);

        if (size == read_failed || size == not_enough_content ||
        size == file_not_open) {

                memset (sendBuff , 0 , sizeof(sendBuff));
                sprintf (data , "%d" , 1);
                strcat (sendBuff , data);

                memset (data , 0 , sizeof(data));
                sprintf (data , "%d" , size);
                strcat (sendBuff , data);
        } else {

                memset (sendBuff , 0 , sizeof(sendBuff));
                sprintf (data , "%d" , 0);
                strcat (sendBuff , data);

                memset (data , 0 , sizeof(data));
                sprintf (data , "%d" , size);
                p_size = strlen (data);
                sprintf (len , "%d" , p_size);
                strcat (sendBuff , len);
                strcat (sendBuff , data);

                strcat (sendBuff , buffer);
        }

out:
        write (conn , sendBuff , sizeof(sendBuff)-1);
        return 0;
}
