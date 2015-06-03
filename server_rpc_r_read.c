#include "rudi_server.h"

int
read_handler (int conn , char *recvBuff) {

        char len[10] , data[100] , sendBuff[1024] , *buffer;
        int read_status , p_size , ind = 0;
        ssize_t size;
        r_file *file = (r_file *) malloc (sizeof (r_file));

        memset (data , 0 , sizeof(data));
        memset (len , 0 , sizeof(len));
        /*Start from 4th character since first 3 character specifies the
        *function enum*/
        memcpy (len , &recvBuff[3] , 1);
        ind += 4;
        p_size = atoi (len);
        memcpy (data , &recvBuff[ind] , p_size);
        ind += p_size;
        size = atoi (data);

        memcpy (file , &recvBuff[ind] , sizeof(r_file));

        size = r_read1 (file , buffer , size);
        if (size == 16 || size == 14 ||
        size == 13) {
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
