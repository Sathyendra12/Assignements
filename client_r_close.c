#include "rudi_client.h"

/*Client API function to Close the Remote File
 *
 * INPUT:
 *      struct r_file **file1 : File to be closed
 *
 * OUTPUT:
 *      int : Success/Failure indicator
 */
int
rClose (struct r_file **file1) {
        r_file *file = *file1;
        int ret = -1;

        if (file != NULL && file->inode_number != 0) {
                printf ("Request to close file.\nINODE : %lu - FD : %d\n" ,
                        file->inode_number , file->fd);
        } else {
                printf ("Invalid Request\n\n");
                ret = 1;
                goto out;
        }
        ret = r_close (file);
        if (ret == 0) {
                printf ("Requested file is closed\n");
                printf ("Operation Successful\n\n");
                ret = 0;
                goto out;
        } else if (ret == file_not_open) {
                printf ("ERROR: Requested file is not Open\n");
        } else {
                printf ("ERROR: Error in Closing the file\n");
        }
        printf ("Operation Failed\n\n");
        ret = 1;
out:
        if (ret == 0) {
                free (file);
                file = NULL;
                close (sockfd);
                sockfd = -1;
                ret = 1;
        }
        return ret;
}
