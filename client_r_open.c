#include "rudi_client.h"

/*Client API function to Open the Remote File */
int
rOpen (const char *filename , unsigned int mode , struct r_file **file) {
        if (*file == NULL || (*file)->inode_number == 0l) {
                *file = (r_file *) malloc (sizeof (r_file));
        } else {
                printf ("WARNING: A file is already open.\n");
                printf ("Please close the file, before opening new one.\n");
                return 0;
        }
        int res = r_open (filename , mode , file);

        if (res == 0) {
                printf ("File : %s is opened\n" , filename);
                printf ("Operation Successful\n\n");
                return 0;
        } else if (res == not_a_file) {
                printf ("ERROR: The requested object : %s is not a file.\n"
                        , filename);
        } else if (res == file_not_found) {
                printf ("ERROR: The reuested file : %s is not found.\n"
                        , filename);
        } else {
                printf ("ERROR: Could not Open the file.\n");
        }
        printf ("Operation Failed\n\n");
        if (*file != NULL)
                free (*file);
        return 1;
}
