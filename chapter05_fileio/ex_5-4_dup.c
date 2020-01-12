/* Exercise 5-4
 *
 * Implement dup() and dup2() using fcntl() and, where necessary, close().
 * (You may ignore the fact that dup2() and fcntl() return difference
 * errno values for some error cases.) For dup2(), remember to handle
 * the special case where oldfd equals newfd. In this case, you should
 * check whether oldfd is valid, which can be done by, for example,
 * checking if fcntl(oldfd, F_GETFL) succeeds. If oldfd is not valid,
 * then the function should return -1 with errno set to EBADF.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int
mydup(int oldfd)
{
    return fcntl(oldfd, F_DUPFD, 0);
}

int
mydup2(int oldfd, int newfd)
{
    /* Check the oldfd's validity */
    if (fcntl(oldfd, F_GETFL) == -1) {
        errno = EBADF;
        return -1;
    }

    /* When oldfd equals newfd */
    if (oldfd == newfd)
        return newfd;
    
    /* Check if newfd is open */
    if (fcntl(newfd, F_GETFL) != -1)
        close(newfd);

    return fcntl(oldfd, F_DUPFD, newfd);
}

int
main(void)
{
    int fd1, fd2, fd3, fd4;

    fd1 = open("f1", O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);

    /* Test for mydup() */
    printf("----------------------------------------------------------------\n");
    printf("Testing mydup() function...\n");
    printf("----------------------------------------------------------------\n");

    printf("fd1 is %d.\n", fd1);
    printf("Set fd1's offset to 100:\n");
    printf("fd1's current offset is %ld.\n\n", (long) lseek(fd1, 100, SEEK_SET));

    printf("Try to dup fd1 into fd2:\n");
    fd2 = mydup(fd1);
    printf("fd2 is %d.\n", fd2);
    printf("fd2's current offset is %ld.\n\n", (long) lseek(fd2, 0, SEEK_CUR));

    /* Test for mydup2() */
    printf("----------------------------------------------------------------\n");
    printf("Testing mydup2() function...\n");
    printf("----------------------------------------------------------------\n");
    
    printf("Try to dup fd1 into fd3 with fd 5:\n");
    fd3 = mydup2(fd1, 5);
    printf("fd3 is %d.\n", fd3);
    printf("fd3's current offset is %ld.\n\n", (long) lseek(fd3, 0, SEEK_CUR));
    
    printf("Try to dup an invalid fd 100 with fd 6:\n");
    fd4 = mydup2(100, 6);
    if (fd4 == -1 && errno == EBADF)
        fprintf(stderr, "Bad file descriptor 100.\n\n");

    printf("Try to dup fd1 into fd4 with the fd used by fd2:\n");
    printf("fd2 is %d.\n", fd2);
    fd4 = mydup2(fd1, fd2);
    printf("fd4 is %d now.\n", fd4);
    printf("fd4's current offset is %ld.\n\n", (long) lseek(fd4, 0, SEEK_CUR));
    
    /* Close fds */
    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);

    exit(EXIT_SUCCESS);
}
