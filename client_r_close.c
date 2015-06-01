#include "rudi_client.h"

/*Client API function to Close the Remote File */
int
rClose (struct r_file *file) {
        int res = 1;

        if (file == NULL) {
                printf ("Invalid Request\n\n");
                return 1;
        }
        res = r_close (file);
        if (res == 0) {
                printf ("Requested file is closed\n");
                printf ("Operation Successful\n\n");
                sockfd = -1;
                return 0;
        } else if (res == file_not_open) {
                printf ("ERROR: Requested file is not Open\n");
        } else {
                printf ("ERROR: Error in Closing the file\n");
        }
        printf ("Operation Failed\n\n");
        return 1;
}
