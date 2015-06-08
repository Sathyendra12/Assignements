# Assignements

Implement a rudimentary Network File System called "rudiFS"

![alt text](https://github.com/Sathyendra12/Assignements/blob/master/img/rudi_FS_microdesign_update1.jpg "")

- #####RudiServer:
The Server serves out read-only files from single directory which we call an export point. The Server is a multi-thread program that serves multiple clients at a time.
The Server should cater to the following requests from the clients via custom rpc library (described below)

    - ***List*** all the files in that directory: return rpc value: buffer that contains lists of files in the export point. It should be the list of struct r_dentry objects.
Remember the file name are POSIX complaint i.e they can have spaces and other allowed special characters in the file name. 

```
            struct r_dentry {
                    char *name;
                    unsigned long inode_number;
                    struct r_dentry *next;
                    struct r_dentry *prev;
            };
```

   - ***open*** a file:
    - input rpc parameter: Filename
    - input rpc parameter: mode : CREATE  or READ. The CREATE mode will create a empty file. If its READ mode then the file should exists or we error out.
    - return rpc value: return value from the actual open() system call.i.e File descriptor if successful or the error code. return rpc value: On success object of struct r_file 
    - Once the file is opened the Server should remember the file descriptor in a linked list for that file called fd_list. Multiple clients can open the same file and obtain different fd's. All these fd's should be in the fd_list. The file itself will be represented in memory my an inode structure.

```
                struct r_inode {
                        unsigned long inode_number; /* obtained during the first 
                                                     * open() call using stat()
                                                     * system */
                        struct r_fd *fd_list;  /* Doubly Linked list of open fd's
                                                * for the file */
                        struct r_inode *next;
                        struct r_inode *prev;
                }

                struct r_fd {
                        int fd;
                        struct fd *next;
                        struct fd *prev;
                }

                struct r_file {
                        unsigned long inode_number;       
                        int fd;
                }  
```
The inode themselves should be maintained in a doubly linked list.
- ***Read*** a file:
 - input rpc paramter : inode number
 - input rpc parameter: file descriptor

    Both the above represented in
```
         struct r_file {
                 unsigned long inode_number;       
                  int fd;
         }
```
  - input rpc parameter: size of data to be read.
  - return rpc values:
    - return value from the actual read() system call i.e number of bytes read or error code.
    - Read data in the buffer.

    Before doing the actual system call you need to check if the input fd is present      inode's fd list.
  
- ***Close*** file:
    - input rpc paramter : inode number
    - input rpc parameter: file descriptor
    - Both the above represented in 
```
        struct r_file {
                 unsigned long inode_number;       
                  int fd;
         }
```

Clean the fd from the fd_list. If the fd_list is empty the inode should be destroyed  from the inode list.Great care should be taken while modifying the fd list or inode list as there will be multiple threads accessing the same list!


#####The RudiClient API library:
The client API Libray will be a static link which will provide the following API:

***int init_rudiClient (char *host_name, unsigned int port);***
initiates the Client API library by setting the global configurable
     - server hostname
     - server port.

***int rudiOpen (const char *filename, unisgned int mode, struct r_file *file);*** 
Open the file with the specifed mode CREATE or READ. the modes will be represented in a enum. If the call is successful struct r_file *file will point to struct r_file object.

```
        struct r_file {
                 unsigned long inode_number;       
                  int fd;
        }
```
    Return system errors as specified open() system call as mentioned man pages.

***ssize_t rudiRead ( struct r_file *file, char *buffer, ssize_t size);***
Read the specified number of bytes (ssize_t size) into the buffer. return values same as normal read() system call. please refer the man pages of read().

***int rudiList (struct r_dentry *list);***
Get the list of files in the export point from the server into the doubly linked list of

```
struct r_dentry.
         struct r_dentry {
                    char *name;
                    unsigned long inode_number;
                    struct r_dentry *next;
                    struct r_dentry *prev;
         }
```

***int rudiClose (struct r_file *file);***
Closes the file. return value sames a close () system call.

#####Custom RPC mechasim
should be build which helps in marking the above remote procedure code. The machasim should deal with the marshaling and de-marshaling of in-memory data structures into streamable data buffers.
for example the sending the doubly linked list of dentries over the network from server to client, when the list call is made.

___Coding Guild-lines (The Ten commandments)___ :
1. All the structures/functions/#includes/#defines etc which are common to client and server should be in rudi_common.h

2. The Client API should be exposed only via rudi_client.h and all the other functions should be either static or not exposed to the consumer of the API.

3. The code should be compiled using Makefiles only.

4. To test the Client API's a test program should be written.

5. Code should be share only and only via github.com. so create a github account.

6. All the .c and .h files should pass the **checkpatch.pl** test. Checkpatch.pl checks    for coding standards like indentation, extra spaces etc.
```   
$> ./checkpatch.pl -f <.c or .h file_path>
```
There should be no warning or errors when the code is inspected by    
    checkpatch.pl. checkpatch.pl requires **perl**.

7. Code should be well commented and documented. Also the code should be well indented. Each indentation is 8 spaces. Codelite is a good IDE for this purpose.

8. Should have appropriate  variable names, i, j, k, temp, a, b are strictly not allowed.

9. Code should be modular in nature with no memory leaks and core dumps. i,e should be tested well with all scenarios. Automated tests are welcomed.

10. The Code should be submitted incrementally. i.e in incremental patches and not in huge commits.



