#include <stdio.h>
#include "rudi_client.h"

/*Client Main Program that uses the API */
int
main (int argc , char *argv[]) {
        r_file *file = NULL;
        r_dentry *list = NULL;
        char fname[500] , buffer[1024];
        int ch = 0 , st = 0;
        ssize_t size;

        /*Check Input parameter */
        if (argc != 3) {
                printf ("No Connection parameters..!!\nPass IP & Port\n");
                return 0;
        }
        printf ("IP:%s\nPort:%s\n" , argv[1] , argv[2]);
        st = init_rClient (argv[1] , atoi (argv[2]));

        /*On successful Connection */
        if (st == 0) {
                while (ch != 5) {
                        printf ("\n--- MENU ---\n1.Files\n");
                        printf ("2.Open A File\n3.Read File\n");
                        printf ("4.Close File\n5.Exit\nEnter your choice:\t");
                        scanf("%d" , &ch);
                        if (sockfd == -1 && ch != 5) {
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
                                st = rOpen(fname , O_RDONLY , &file);
                                if (st == 0)
                                        printf ("File INODE: %ul - FD: %d\n" ,
                                        file->inode_number , file->fd);
                                break;
                        case 3:
                                printf ("Enter the Read Size:\t");
                                scanf("%d" , &size);
                                rRead (file , buffer , size);
                                break;
                        case 4:
                                st = rClose (file);
                                break;
                        case 5:
                                printf ("Terminating..\n");
                                break;
                        default:
                                printf ("\n-- INVALID CHOICE --\n\n");
                        }
                }
        }
}
