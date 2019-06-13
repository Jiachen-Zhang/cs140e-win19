#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "demand.h"
#include "../shared-code/simple-boot.h"
#include "support.h"

// read entire file into buffer.  return it, write totat bytes to <size>
unsigned char *read_file(long *size, const char *name) {
    // using `stat()` to get the size
    struct stat fileState;
    stat(name, &fileState);
    // pad the memory so that it is a multiple of 4
    *size = fileState.st_size;
    if (*size % 4 != 0) {
        *size = (*size / 4 + 1) * 4;
    }
    // printf("File %s size = %ld byte, pad to %ld byte\n", name, fileState.st_size, *size);
    // use `read()` to read the entire file into memory
    // char buffer[*size];
    char *buffer = (char *)malloc(*size);
    int fd = open(name, O_RDONLY);
    if (read(fd, buffer, *size) != fileState.st_size) {
        fputs("Reading error", stderr);
        exit(1);
    }
    close(fd);
    // printf("%s", buffer);
    return buffer;
}

#define _SVID_SOURCE
#include <dirent.h>
const char *ttyusb_prefixes[] = {
    "ttyS",     // prefix for WSL
	"ttyUSB",	// linux
	"tty.SLAB_USB", // mac os
	0
};

// open the TTY-usb device:
//	- use <scandir> to find a device with a prefix given by ttyusb_prefixes
//	- returns an open fd to it
// 	- write the absolute path into <pathname> if it wasn't already given.
int open_tty(const char **portname) {
    printf("Running open_tty().\n");
    // get the number under /dev/
    struct dirent **namelist;
    int count = scandir("/dev/", &namelist, 0, alphasort);
    printf("count = %d\n", count);
    // print the coms
    while(count--) {
    //    printf("name = %s, type = %d, recled = %d, ino = %ld\n", namelist[count]->d_name, namelist[count]->d_type, namelist[count]->d_reclen, namelist[count]->d_ino);
    ;}
    free(namelist);

    /* As the WSL cannot find the CP2102, so I have to just pass the right path and open it */
    int fd = open("/dev/ttyS6", O_RDONLY);
	return fd;
}

int main(void) {
	long size = 0;
	const char *name = "test";
	unsigned char* buffer = read_file(&size, name);
    printf("File contains:\n%s", buffer);
    free(buffer);
    printf("\nfile_name: %s, size = %ld byte\n", name, size);
    const char **portname;
    open_tty(portname);
    
	return 0;
}
