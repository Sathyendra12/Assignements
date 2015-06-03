#include <pthread.h>
#include "rudi_common.h"

/*Export Point for searving files */
static char *exp_point = "./EmpDir/";

/*Pointer to Open File Table */
r_inode *op_tab;

/*Root Node of List of File */
r_dentry *root_node;

/*Thread Function */
void *thread_fun (void *parameter);

