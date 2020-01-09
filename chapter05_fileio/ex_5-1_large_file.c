/* Exercise 5-1
 *
 * If you have access to a 32-bit Linux system, modify the program
 * in Listing 5-3 to use standard file I/O system calls
 * (open() and lseek()) and off_t data type.
 * Compile the program with the _FILE_OFFSET_BITS macro set to 64,
 * and test it to show that a large file can be successfully created.
 */
#define _FILE_OFFSET_BITS 64
#include "tlpi_hdr.h"
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int fd;
    off_t off;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname offset\n", argv[0]);

    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    off = atoll(argv[2]);
    if (lseek(fd, off, SEEK_SET) == -1)
        errExit("lseek");

    if (write(fd, "test", 4) == -1)
        errExit("write");

    exit(EXIT_SUCCESS);
}
