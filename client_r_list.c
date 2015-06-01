#include "rudi_client.h"

int
rList (r_dentry *list) {

        int fun_status , index;
        r_dentry *ptr;

        fun_status = r_List (list);

        if (fun_status == 1) {
                printf ("\n Export Point is Empty\n");
                return 1;
        } else {
                printf ("\tSl.No \tFileName");
                for (ptr = list , index = 0 ; ptr != NULL ; ptr = ptr->next) {
                        pintf ("\t%d) \t%s ", index++ , ptr->name);
                }
                return 0;
        }
}
