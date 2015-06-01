#include "rudi_client.h"

ssize_t
rRead (struct r_file *file, char *buffer, ssize_t size) {
        return r_read (file , buffer , size);
}
