/* Exercise 5-2
 *
 * Writing a program that opens an existing file for writing with
 * the O_APPEND flag, and then seeks to the beginning of the file
 * before writing some data.
 * Where does the data appear in the file? Why?
 *
 * According to the notes section in the man page of lseek(),
 * If the O_APPEND file status flag is set on the open file description,
 * then a write() always moves the file offset to then end of the file,
 * regardless of the use of lseek().
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    int fd;
    ssize_t numWritten;
    off_t off;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s filename", argv[1]);
        exit(EXIT_FAILURE);
    }

    /* Open the file */
    fd = open(argv[1], O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        fprintf(stderr, "Faild to open the file \"%s\".\n"
                "errno: %d\n", argv[1], errno);
        exit(EXIT_FAILURE);
    }

    /* Seek to the beginning before write */
    off = lseek(fd, 0, SEEK_SET);
    if (off == -1) {
        fprintf(stderr, "Faild to seek to the start.\n");
        exit(EXIT_FAILURE);
    }
    printf("After seeking to the beginning, the offset is %lld.\n",
            (long long) off);

    /* Start to write content */
    numWritten = write(fd, argv[2], strlen(argv[2]));
    if (numWritten == -1) {
        fprintf(stderr, "Faild to write.\n");
        exit(EXIT_FAILURE);
    }
    off = lseek(fd, 0, SEEK_CUR);       // get the current offset
    if (off == -1) {
        fprintf(stderr, "Faild to get the current offset.\n");
        exit(EXIT_FAILURE);
    }
    printf("%lld bytes has been written before the current offset %lld.\n",
            (long long) numWritten,
            (long long) off);
    
    exit(EXIT_SUCCESS);
}
