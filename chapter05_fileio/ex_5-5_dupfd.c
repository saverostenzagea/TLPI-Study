/* Exercise 5-5
 *
 * Write a program to verify that dupliated file descriptors
 * share a file offset value and open file status flags.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int fd, dp1, dp2, dp3;

    fd = open("a.txt", O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        fprintf(stderr, "%s: failed to open.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("%s: a.txt opened.\n\n", argv[0]);

    printf("Try to duplicate fd with the function dup():\n");
    dp1 = dup(fd);
    if (dp1 == -1) {
        fprintf(stderr, "%s: failed to duplicate fd with dup().\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("dp1 with fd %d was made by dup().\n\n", dp1);

    printf("Try to duplicate fd with the function dup2():\n");
    dp2 = dup2(fd, 11);
    if (dp2 == -1) {
        fprintf(stderr, "%s: failed to duplicate fd with dup2().\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("dp2 with fd %d was made by dup().\n\n", dp2);

    printf("Try to duplicate fd with the function fcntl().\n");
    dp3 = fcntl(fd, F_DUPFD, 3);
    if (dp3 == -1) {
        fprintf(stderr, "%s: failed to duplicate fd with fcntl().\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("dp3 with fd %d was made by fcntl().\n\n", dp3);

    printf("Try to seek the file to the offset 200:\n");
    if (lseek(fd, 200, SEEK_CUR) == -1) {
        fprintf(stderr, "%s: failed to seek.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("fd was seeked to the offset 200.\n");
    printf("dp1's offset: %ld\n", (long) lseek(dp1, 0, SEEK_CUR));
    printf("dp2's offset: %ld\n", (long) lseek(dp2, 0, SEEK_CUR));
    printf("dp3's offset: %ld\n\n", (long) lseek(dp3, 0, SEEK_CUR));

    int flags;
    flags = fcntl(dp1, F_GETFL) & O_ACCMODE;
    if (flags == O_RDONLY)
        printf("dp1 has the open file status flag read only.\n");

    flags = fcntl(dp2, F_GETFL) & O_ACCMODE;
    if (flags == O_RDONLY)
        printf("dp2 has the open file status flag read only.\n");

    flags = fcntl(dp3, F_GETFL) & O_ACCMODE;
    if (flags == O_RDONLY)
        printf("dp3 has the open file status flag read only.\n");

    exit(EXIT_SUCCESS);
}
