#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path to watch>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        close(fd);
        return EXIT_FAILURE;
    }

    struct kevent changes;
    EV_SET(&changes, fd, EVFILT_VNODE,
           EV_ADD | EV_ENABLE | EV_CLEAR,
           NOTE_WRITE | NOTE_EXTEND | NOTE_DELETE | NOTE_ATTRIB | NOTE_LINK |
           NOTE_RENAME | NOTE_REVOKE,
           0, 0);

    for (;;) {
        struct kevent events;
        int nev = kevent(kq, &changes, 1, &events, 1, NULL);

        if (nev == -1) {
            perror("kevent");
            close(kq);
            close(fd);
            return EXIT_FAILURE;
        } else if (nev > 0) {
            if (events.fflags & NOTE_WRITE) {
                printf("Write detected in %s\n", argv[1]);
            }
            if (events.fflags & NOTE_EXTEND) {
                printf("Extend detected in %s\n", argv[1]);
            }
            if (events.fflags & NOTE_DELETE) {
                printf("Delete detected in %s\n", argv[1]);
            }
            if (events.fflags & NOTE_ATTRIB) {
                printf("Attribute change detected in %s\n", argv[1]);
            }
            if (events.fflags & NOTE_LINK) {
                printf("Link count change detected in %s\n", argv[1]);
            }
            if (events.fflags & NOTE_RENAME) {
                printf("Rename detected in %s\n", argv[1]);
            }
            if (events.fflags & NOTE_REVOKE) {
                printf("Revoke detected in %s\n", argv[1]);
            }
        }
    }

    close(kq);
    close(fd);
    return EXIT_SUCCESS;
}