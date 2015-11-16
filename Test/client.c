#include <stdio.h>
#include "../rudi_client.h"

/*Client Main Program that uses the API */
int
main (int argc , char *argv[]) {
        r_file *file = NULL;
        r_dentry *list = NULL;
        char fname[500] , buffer[1024] , cch , *wr_buffer = NULL;
        int ch = 0 , st = 0;
	unsigned int mode;
        ssize_t size;

        /*Check Input parameter */
        if (argc != 3) {
                printf ("No Connection parameters..!!\nPass IP & Port\n");
                printf ("Example: ./Client 127.0.0.1 5000\n\n");
                return 0;
        }
        printf ("IP:%s\nPort:%s\n" , argv[1] , argv[2]);
        st = init_rClient (argv[1] , atoi (argv[2]));

        /*On successful Connection */
        if (st == 0) {
                while (1) {
menu:                  printf ("\n--- MENU ---\n1.Files\n");
                        printf ("2.Open A File\n3.Read File\n4.Write to File\n");
                        printf ("5.Close File\nEnter your choice:\t");
                        scanf(" %c" , &cch);
                        int inv = 0;

                        while (getchar() != '\n')
                                inv = 1;
                        if ((cch - '0') <= 0 || (cch - '0') > 5 || inv == 1) {
                                printf ("\n--Invalid Choice--\n\n");
                                goto menu;
                        } else {
                                ch = cch - '0';
                        }
                        if (sockfd == -1) {
                                st = init_rClient (argv[1] , atoi (argv[2]));
                                /*On Failure */
                                if (st != 0)
                                        break;
                        }
                        switch (ch) {
                        case 1:
                                st = rList (list);
                                break;
                        case 2:
                                printf ("Enter the File name:\t");
                                scanf ("%s" , &fname);
				printf ("Enter the Mode:\n1.Read\t2.Write\nChoice:\t");
                                scanf ("%d" , &ch);
				if (ch==1)
					mode = 0;
				else
					mode = 1;
                                st = rOpen(fname , mode , &file);
                                if (st == 0)
                                        printf ("File INODE: %lu - FD: %d\n" ,
                                        file->inode_number , file->fd);
                                break;
                        case 3:
                                printf ("Enter the Read Size:\t");
                                scanf("%d" , &size);
                                rRead (file , buffer , size);
                                break;
                        case 4:
				printf ("Enter the Write Size:\t");
                                scanf("%d" , &size);
				wr_buffer = (char *) malloc (sizeof(char) * size);
				printf ("Enter the content to write:\t");
				scanf ("%[^\t]" , wr_buffer);
				buffer[strlen(wr_buffer)] = '\0';
				size = strlen (wr_buffer);
				rWrite (file , wr_buffer , size);
				break;
			case 5:
                                st = rClose (&file);
                                break;
                        default:
                                printf ("\n-- INVALID CHOICE --\n\n");
                        }
                        if (st == 1 || file == NULL) {
                                char cont;

input:                          printf ("Do you want to continue..?(y/n)\t");
                                scanf (" %c" , &cont);
                                if (cont == 'n')
                                        break;
                                else if (cont != 'y') {
                                        printf ("\n--Invalid Choice--\n\n");
                                        goto input;
                                }
                        }
                }
        }
}
