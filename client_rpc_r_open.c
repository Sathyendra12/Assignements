#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "rudi_client.h"

int
r_Open (const char *filename, unsigned int mode, struct r_file *file) {
        char sendBuff[1024] , recvBuff[1024] , len[5] , data[500];
        int p_size;

        if (sockfd >= 0) {
                memset (sendBuff , 0 , sizeof (sendBuff));
                sprintf (data , "%d" , _r_open);
                strcat (sendBuff , data);
                p_size = strlen (filename);
                sprintf (len , "%d" , p_size);
                strcat (sendBuff , len);
                strcat (sendBuff , filename);
                sprintf (data , "%d" , mode);
                strcat (sendBuff , data);
                p_size = sizeof (file);
                sprintf (len , "%d" , p_size);
                char temp_buff[p_size];

                memcpy (temp_buff , &file , p_size);
                strcat (sendBuff , len);
                strcat (sendBuff , temp_buff);
                write (sockfd , sendBuff , sizeof(sendBuff)-1);
                p_size = read (sockfd , recvBuff , sizeof(recvBuff)-1);
                recvBuff[p_size] = 0;
                memcpy (file , &recvBuff , p_size);
                return 0;
        } else {
                printf ("\nERROR: Not connected to Server\n");
                return 1;
        }
}
/*
int
main () {
        char *filename = "abcd.txt";
        struct r_file *file;
        file->inode_number = 102;
        file->fd = 2;
        int n = r_Open (filename, O_RDONLY , file);
        printf ("Res: %d",n);
}*/
