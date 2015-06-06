#include "rudi_client.h"

/*Client API function to Open the Remote File 
 *
 * INPUTS:
 *      const char *filename :  Name of the file to be opened
 *      unsigned int mode :     The mode in which the file is to be opened
 *      struct r_file **file :  Structure that will hold opened file details
 * OUTPUT:
 *      int :   Success/Failure indicator
 */
int
rOpen (const char *filename , unsigned int mode , struct r_file **file) {
        int ret = -1;
        if (*file == NULL || (*file)->inode_number == 0l) {
                *file = (r_file *) malloc (sizeof (r_file));
        } else {
                printf ("WARNING: A file is already open.\n");
                printf ("Please close the file, before opening new one.\n");
                ret = 0;
                goto out;
        }
        ret = r_open (filename , mode , file);

        if (ret == 0) {
                printf ("File : %s is opened\n" , filename);
                printf ("Operation Successful\n\n");
                goto out;
        } else if (ret == not_a_file) {
                printf ("ERROR: The requested object : %s is not a file.\n"
                        , filename);
        } else if (ret == file_not_found) {
                printf ("ERROR: The reuested file : %s is not found.\n"
                        , filename);
        } else {
                printf ("ERROR: Could not Open the file.\n");
        }
        printf ("Operation Failed\n\n");
        ret = -1;
out:
        if (*file != NULL && ret != 0)
                free (*file);
        return ret;
}
