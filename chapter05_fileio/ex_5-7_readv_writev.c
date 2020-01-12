/* Exercise 5-7
 *
 * Implement readv() and writev() using read(), write(),
 * and suitable functions from the malloc package.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>

ssize_t
myreadv(int fd, struct iovec *iov, int iovcnt)
{
    int i;
    ssize_t numRead = 0, totalRead = 0;

    for (i = 0; i < iovcnt; i++)
    {
        iov[i].iov_base = malloc(iov[i].iov_len);
        numRead = read(fd, iov[i].iov_base, iov[i].iov_len);
        if (numRead == -1)
            return numRead;
        totalRead += numRead;
    }

    return totalRead;
}

ssize_t
mywritev(int fd, const struct iovec *iov, int iovcnt)
{
    int i;
    ssize_t numWritten = 0, totalWritten = 0;

    for (i = 0; i < iovcnt; i++)
    {
        numWritten = write(fd, iov[i].iov_base, iov[i].iov_len);
        if (numWritten == -1)
            return numWritten;
        totalWritten += numWritten;
    }

    return totalWritten;
}

#define IOVCNT 3

int
main(int argc, char *argv[])
{
    int fd1, fd2;
    struct iovec iov[IOVCNT];
    ssize_t numRequired = 0, numRead, numWritten;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s file1 file2.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd1 = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);
    if (fd1 == -1) {
        fprintf(stderr, "%s: failed to open %s.\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }
    
    iov[0].iov_len = sizeof(struct stat);
    iov[1].iov_len = sizeof(int);
    iov[2].iov_len = 100;
    for (int i = 0; i < IOVCNT; i++)
        numRequired += iov[i].iov_len; 
    numRead = myreadv(fd1, iov, IOVCNT);
    if (numRead == -1) {
        fprintf(stderr, "%s: failed on myreadv.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("The bytes required: %ld\n", (long) numRequired);
    printf("The bytes read: %ld\n", (long) numRead);

    fd2 = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd2 == -1) {
        fprintf(stderr, "%s: faild to open %s.\n", argv[0], argv[2]);
        exit(EXIT_FAILURE);
    }

    numWritten = mywritev(fd2, iov, IOVCNT);
    if (numWritten == -1) {
        fprintf(stderr, "%s: faild on mywritev.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("The bytes written: %ld\n", (long) numWritten);

    close(fd1);
    close(fd2);

    exit(EXIT_SUCCESS);
}
