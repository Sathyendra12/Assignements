#include "rudi_client.h"

int
rList (r_dentry *list) {

        int fun_status , index;
        r_dentry *ptr;

        fun_status = r_list (list);
        if (fun_status == 1) {
                printf ("\n Export Point is Empty\n");
                return 1;
        } else {
                printf("__________________________________________________\n");
                printf ("\tSl.No \t FileName\n");
                printf("==================================================\n");
                for (ptr = root , index = 1 ; ptr != NULL ; ptr = ptr->next)
                        printf ("\t%d)  %s\n", index++ , ptr->name);
                return 0;
        }
}
