#include "rudi_server.h"

/*Function to Handle the file open Call
 *
 * INPUT:
 *      int conn :      Socket connection identifier
 *      char *recvBuff :Buffer containing request
 *
 * OUTPUT:
 *      int :   Success/Failure indicator.
 */
int
open_handler (int conn , char *recvBuff) {
        unsigned int mode;
        int ind = 3 , res = 0 , ret = -1 , p_size;

        char sendBuff[1024] , len[5] , data[1000] , filename[500];
        r_file *file = (r_file *) malloc (sizeof (r_file));

        memset (len , 0 , sizeof (len));
        memcpy (len , &recvBuff[ind] , 2);
        ind += 2;
        /*Name of the file to Open */
        memset (filename , 0 , sizeof (filename));
        memcpy (filename , &recvBuff[ind] , atoi (len));
        ind += atoi (len);
        /*Parse the mode of opening the file */
        memset (data , 0 , sizeof (data));
        memcpy (data , &recvBuff[ind] , 1);
        ind += 1;
        mode = atoi (data);
        res = r_open (filename , mode , &file);
        memset (sendBuff , 0 , sizeof (sendBuff));
        memset (data , 0 , sizeof (data));
        memset (len , 0 , sizeof (len));
        /*Marshal based of success or Failure */
        if (res == 0) {
                sprintf (data , "%d" , res);
                strcat (sendBuff , data);
                memcpy (sendBuff + strlen (sendBuff) , file , sizeof (r_file));
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
        ret = 0;
out:
        return ret;
}


/*Function to Handle the file close Call
 * INPUT:
 *      int conn :      Socket connection identifier
 *      char *recvBuff :Buffer containing request
 *
 * OUTPUT:
 *      int :   Success/Failure indicator.
 */
int
close_handler (int conn , char *recvBuff) {
        int ind = 3 , res = 0 , ret = -1;
        char sendBuff[1024] , len[5] , data[1000];
        r_file *file = (r_file *) malloc (sizeof (r_file));

        /*File Structure containing details of file to be Closed */
        memcpy (file , &recvBuff[ind] , sizeof (r_file));
        res = r_close (file);
        memset (sendBuff , 0 , sizeof (sendBuff));
        memset (data , 0 , sizeof (data));
        /*Marshal based on success or failure */
        if (res == 0) {
                sprintf (data , "%d" , res);
                strcat (sendBuff , data);
                ret = -1;
        } else {
                sprintf (data , "%d" , 1);
                strcat (sendBuff , data);
                memset (data , 0 , sizeof (data));
                /*Error code*/
                sprintf (data , "%d" , res);
                strcat (sendBuff , data);
                ret = 0;
        }
        /*Sending data to the Client */
        write (conn , sendBuff , sizeof (sendBuff) - 1);
out:
        return ret;
}

/*Thread Function
 * INPUT:
 *      void *parameter :       Connection Identifier
 */
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
                p_size = read (conn , recvBuff , sizeof (recvBuff) - 1);
                if (p_size != 0) {
                        recvBuff[p_size] = 0;
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
			case _r_write:
				st = write_handler (conn , recvBuff);
				break;
                        case _r_close:
                                st = close_handler (conn , recvBuff);
                                break;
                        }
                }
        }
out:
        return;
}
