/* Exercise 5-3
 *
 * Write a program that takes up to three command-line arguments:
 *  $ atomic_append filename num-bytes [x]
 * 
 * This program should open the specified filename
 * (creating it if necessary) and append num-bytes to the file
 * by using write() a byte at a time. By default, the program
 * should open the file with the O_APPEND flag, but if a third
 * command-line argument (x) is supplied, then the O_APPEND flag
 * should be omitted, and instead the program should perform
 * an lseek(fd, 0, SEEK_END) call before each write().
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    int fd, append_flag, isseek;
    long num_bytes;

    /* Check arguments and options */
    if ((argc !=3 && argc !=4) || (strcmp(argv[1], "--help") == 0)
            || (argc == 4 && argv[3][0] != 'x')) {
        fprintf(stderr, "Usage: %s filename num-bytes [x]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Set O_APPEND lag and determin whether to use lseek() */
    if (argc == 3) {
        append_flag = O_APPEND;
        isseek = 0;
    } else {
        append_flag = 0;
        isseek = 1;
    }
    num_bytes = atol(argv[2]);
    
    /* Open the file */
    fd = open(argv[1], O_CREAT | O_WRONLY | append_flag, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        fprintf(stderr, "%s: faild to open the file %s.\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }

    /* Write a byte at a time */
    long i;
    for (i = 0; i < num_bytes; i++)
    {
        if (isseek) 
            lseek(fd, 0, SEEK_END);
        write(fd, "a", 1);
    }

    exit(EXIT_SUCCESS);
}
