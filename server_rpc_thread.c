#include "rudi_server.h"

void
*thread_fun (void *parameter) {
        char recvBuff[1024] , len[5] , data[500];
        int p_size , res , conn = 0 , st = 0;

        /*Connection Identifier */
        conn = *(int *)parameter;
        /*Loops till the File is closed */
        while (st == 0) {
                /*Clear the Buffer content */
                memset (recvBuff , 0 , sizeof (recvBuff));
                /*Receive marshalled data sent by Client*/
                p_size = read (conn , recvBuff , sizeof (recvBuff)-1);
                memset (data , 0 , sizeof (data));
                memcpy (data , &recvBuff , 3);
                res = atoi (data);
                /*Interprete received data based on type of call */
                switch (res) {
                case _r_list:
                        st = list_handler (conn);
                        break;
                case _r_open:
                        st = open_handler (conn , recvBuff);
                        break;
                case _r_read:
                        st = read_handler (conn , recvBuff);
                        break;
                case _r_close:
                        st = close_handler (conn , recvBuff);
                        break;
                }
        }
        return;
}

/*Function to Handle the file open Call */
int
open_handler (int conn , char *recvBuff) {
        int ind = 3 , res = 0 , p_size;
        unsigned int mode;
        char sendBuff[1024] , len[5] , data[1000] , filename[500];
        r_file *file = NULL;

        memset (len , 0 , sizeof (len));
        memcpy (len , &recvBuff[ind] , 1);
        ind += 1;
        /*Name of the file to Open */
        memset (filename , 0 , sizeof (filename));
        memcpy (filename , &recvBuff[ind] , atoi (len));
        ind += atoi (len);
        /*Parse the mode of opening the file */
        memset (data , 0 , sizeof (data));
        memcpy (data , &recvBuff[ind] , 1);
        ind += 1;
        mode = atoi (data);
        /*
        memset (len , 0 , sizeof (len));
        memcpy (len , &recvBuff[ind] , 1);
        ind += 1;
         */
        /*Pointer to structure that represents details of newly opened File*/
        memcpy (file , &recvBuff[ind] , sizeof (r_file));
        res = r_open (filename , mode , file);
        memset (sendBuff , 0 , sizeof (sendBuff));
        memset (data , 0 , sizeof (data));
        memset (len , 0 , sizeof (len));
        /*Marshal based of success or Failure */
        if (res == 0) {
                sprintf (data , "%d" , res);
                strcat (sendBuff , data);
                /*p_size = sizeof (file);
                sprintf (len , "%d" , p_size);
                strcat (sendBuff , len);
                char temp_buff[p_size];*/

                memcpy (sendBuff + strlen (sendBuff) , &file , sizeof (r_file));
                //strcat (sendBuff , temp_buff);
        } else {
                sprintf (data , "%d" , 1);
                strcat (sendBuff , data);
                memset (data , 0 , sizeof (data));
                /*Error code */
                sprintf (data , "%d" , res);
                strcat (sendBuff , data);
        }
        /*Sending the Buffer content to the client */
        write (conn , sendBuff , sizeof(sendBuff)-1);
        return 0;
}

/*Function to Handle the file close Call */
int
close_handler (int conn , char *recvBuff) {
        int ind = 3 , res = 0;
        char sendBuff[1024] , len[5] , data[1000];
        r_file *file = NULL;

        /*memset (len , 0 , sizeof (len));
        memcpy (len , &recvBuff[ind] , 1);
        ind += 1;*/
        /*File Structure containing details of file to be Closed */
        memcpy (file , &recvBuff[ind] , sizeof (r_file));
        res = r_close (file);
        memset (sendBuff , 0 , sizeof (sendBuff));
        memset (data , 0 , sizeof (data));
        /*Marshal based on success or failure */
        if (res == 0) {
                sprintf (data , "%d" , res);
                strcat (sendBuff , data);
        } else {
                sprintf (data , "%d" , 1);
                strcat (sendBuff , data);
                memset (data , 0 , sizeof (data));
                /*Error code*/
                sprintf (data , "%d" , res);
                strcat (sendBuff , data);
        }
        /*Sending data to the Client */
        write (conn , sendBuff , sizeof(sendBuff)-1);
        return 0;
}
