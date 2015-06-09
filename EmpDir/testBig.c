#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "rudi_client.h"

int
r_Open (const char *filename, unsigned int mode, struct r_file *file) {
        char sendBuff[1024],rec[100],rec2[100],ch_rep[100];
        int p_size;
        
        //if (sockfd >= 0) {
                memset (sendBuff,0,sizeof(sendBuff));
                //p_size = strlen ((char)_r_open);
                //sprintf (ch_rep,"%d",);
                //strcat (sendBuff , ch_rep);
                sprintf (ch_rep,"%d",_r_open);
                printf ("Enum %d\n",_r_open);
                strcat (sendBuff , ch_rep);
                p_size = strlen (filename);
                printf ("LEN : %d\n",p_size);
                sprintf (ch_rep,"%d",p_size);
                strcat (sendBuff , ch_rep);
                strcat (sendBuff , filename);
                //p_size = sizeof (mode);
                //sprintf (ch_rep,"%d",p_size);
                //strcat (sendBuff , ch_rep);
                sprintf (ch_rep,"%d",mode);
                strcat (sendBuff , ch_rep);
                p_size = sizeof (file);
                sprintf (ch_rep,"%d",p_size);
                char temp_buff[p_size];
                memcpy (temp_buff , &file , p_size);
                strcat (sendBuff , ch_rep);
                strcat (sendBuff , temp_buff);
                printf ("Content:\n%s\n",sendBuff);
                //memcpy (ch_rep , &sendBuff , 1);
                //printf ("Got : %s\n",ch_rep);
                memcpy (rec , &sendBuff , 3);
                printf ("En : %s\n",rec);
                memset (rec,0,sizeof(rec));
                memcpy (rec , &sendBuff[3] , 1);
                memset (ch_rep,0,sizeof(ch_rep));
                int ind = atoi(rec);
                memcpy (ch_rep , &sendBuff[4] , ind);
                memset (rec,0,sizeof(rec));
                ind+=4;
                memcpy (rec , &sendBuff[ind] , 1);
                ind+=2;
                memset (ch_rep,0,sizeof(ch_rep));
                memcpy (file , &sendBuff[ind] , 8);
                printf ("fd; %d \n",file->fd);
                printf ("F_N_LEN : %s\n",rec);
                return 0;
        /*} else {
                printf ("\nERROR: Not connected to Server\n");
                return 1;
        }*/
}

int
main () {
        char *filename = "abcd.txt";
        struct r_file *file;
        file->inode_number = 102;
        file->fd = 2;
        int n = r_Open (filename, O_RDONLY , file);
        printf ("Res: %d",n);
}