#include "rudi_server.h"
#include <fcntl.h>
ssize_t
r_read1 (r_file *file , char *buffer , ssize_t size) {
        int fd = file->fd;
        ssize_t byts_read;

                byts_read = read (fd, buffer, size);
                if (byts_read == -1) {
                        return read_failed;
                } else if (byts_read == 0) {
                        return not_enough_content;
                } else  {
                        return byts_read;
                }
}
