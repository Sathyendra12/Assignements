#include "rudi_client.h"

/*Client API function to Open the Remote File */
int
rOpen (const char *filename , unsigned int mode , struct r_file *file) {
        int res = r_open (filename , mode , file);

        if (res == 0) {
                printf ("File : %s is opened.\n" , filename);
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
        return 1;
}
