/* Exercise 4-2
 *
 * Write a program like *cp* that, when used to copy a regular file
 * that contains holes (sequences of null bytes), also create
 * corresponding holes in the target file.
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int
main(int argc, char *argv[])
{
    int     inputFd, outputFd, openFlags;
    mode_t  filePerms;
    ssize_t numRead;
    char    buf[BUF_SIZE];

    /* Check if the arguments are complete */

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s old-file new-file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Open the input file and the output file */

    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        fprintf(stderr, "%s: Unable to open the file - %s\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1) {
        fprintf(stderr, "%s: Unable to open the file - %s\n", argv[0], argv[2]);
        exit(EXIT_FAILURE);
    }

    /* Transfer data from the input file to the output file */

    int         i = 0;
    off_t       holeSize = 0;
    ssize_t     dataSize = 0;
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        for (i = 0; i < numRead; i++) {
            if (buf[i] == '\0') {
                if (dataSize > 0) {
                    write(outputFd, &buf[i - dataSize], dataSize);
                    dataSize = 0;
                }
                holeSize++;
            } else {
                if (holeSize > 0) {
                    lseek(outputFd, holeSize, SEEK_CUR);
                    holeSize = 0;
                }
                dataSize++;
            }
        }
        if (dataSize > 0) {
            write(outputFd, &buf[i - dataSize], dataSize);
            dataSize = 0;
        }
    }

    /* Close the files */
    close(outputFd);
    close(inputFd);

    exit(EXIT_SUCCESS);
}
