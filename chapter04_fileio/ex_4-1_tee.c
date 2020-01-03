/* Exercise 4-1
 *
 * The *tee* command reads its standard input until end-of-file, writing a copy
 * of the input to standard output and to the file named in its command-line
 * argument.
 * Implement *tee* using I/O system calls. By default, *tee* overwrites any
 * existing file with the given name.
 * Implement the *-a* command-line option (tee -a file), which causes *tee* to
 * append text to the end of a file if it already exists. (Refer to Appendix B
 * for a description of the getopt() function, which can be used to parse
 * command-line options.)
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define printable(ch) (isprint((unsigned char) ch) ? ch : '#')

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

static void             /* Print 'usage' message and exit */
usageError(char *progName, char *msg, int opt)
{
    if (msg != NULL && opt != 0)
        fprintf(stderr, "%s (-%c)\n", msg, printable(opt));
    fprintf(stderr, "Usage: %s [-a] [file]\n", progName);
    exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
    int append_mode = 0;
    int opt;
    char *pstr = NULL;
    int fd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    /* Use getopt() to parse the option */

    while ((opt = getopt(argc, argv, ":a:")) != -1) {
        printf("opt = %3d (%c); optind = %d", opt, printable(opt), optind);
        if (opt == '?' || opt == ':')
            printf("; optopt = %3d (%c)", optopt, printable(optopt));
        printf("\n");

        switch (opt) {
            case 'a':   pstr = optarg;
                        append_mode = 1;
                        break;
            case ':':   usageError(argv[0], "Missing argument", optopt);
            case '?':   usageError(argv[0], "Unrecognized option", optopt);
            default:    fprintf(stderr, "Unexpected case in switch()");
                        exit(EXIT_FAILURE);
        }
    }

    /* If there's no option and its argument,
     * but an non-option argument. */

    if (pstr == NULL && argv[optind] != NULL)
        pstr = argv[optind];

    /* Open the file specified */

    if (pstr != NULL) {
        openFlags = O_CREAT | O_WRONLY | (append_mode ? O_APPEND : O_TRUNC);
        filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
        fd = open(pstr, openFlags, filePerms);
        if (fd == -1) {
            fprintf(stderr, "Unable to open the file: %s.\n", pstr);
            exit(EXIT_FAILURE);
        }
    }

    /* Read from the standard input,
     * and then write into the standard output and the file */

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, numRead) != numRead) {
            fprintf(stderr, "Unable to write whole buffer into stdout.\n");
            exit(EXIT_FAILURE);
        }
        if (pstr != NULL) {
            if (write(fd, buf, numRead) != numRead) {
                fprintf(stderr, "Unable to write whole buffer into "
                        "the file: %s.\n", pstr);
                exit(EXIT_FAILURE);
            }
        }
    }

    /* Clean up fd */

    if (pstr != NULL) {
        if (close(fd) == -1) {
            fprintf(stderr, "Unable to close the file: %s.\n", pstr);
        }
    }

    exit(EXIT_SUCCESS);
}
