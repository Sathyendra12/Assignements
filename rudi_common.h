#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/*Enumeration for Function Identification */
enum fun_list {_r_list = 101 , _r_open = 102 , _r_read = 103 , _r_close = 104,
_r_write = 105};

/*Enumeration for Error List */
enum err_list {directory_empty = 11 , file_not_found = 12 ,
file_not_open = 13 , not_enough_content = 14 , not_a_file = 15 ,
read_failed = 16 , file_already_open = 17 , file_write_in_progress = 18 ,
write_failed = 19};

/*Export Point Directory content */
struct r_dentry {
        char name[50];
        unsigned long inode_number;
        struct r_dentry *next;
        struct r_dentry *prev;
};

/*Custom Inode-File_Descripter List */
struct r_inode {
        unsigned long inode_number;/*obtained during open() call using stat()*/
	int write_flag;/*signifies write in progress*/
        struct r_fd *fd_list;/*doubly linked list of open file fd's*/
        struct r_inode *next;
        struct r_inode *prev;
};

/*Structure representing a file */
struct r_file {
        unsigned long inode_number;
	int write_flag;
        int fd;
};

/*Open file descriptor */
struct r_fd {
        int fd;
        struct r_fd *next;
        struct r_fd *prev;
};



/*To get the Export point Directory content */
int
r_list (struct r_dentry *list);

/*To open the specified file & make a table entry */
int
r_open (const char *filename , unsigned int mode , struct r_file **file);

/*To read from the file in export point*/
ssize_t
r_read (struct r_file *file , char *buffer , ssize_t size);

/*To write to the file in export point*/
ssize_t
r_write (struct r_file *file , char *buffer , ssize_t size);

/*To close the file */
int
r_close (struct r_file *file);

typedef struct r_dentry r_dentry;
typedef struct r_inode r_inode;
typedef struct r_fd r_fd;
typedef struct r_file r_file;
