#include <sys/types.h>
#include <sys/time.h>
#include <sys/event.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

int
main(int argc, char** argv)
{
	int kq;
	int fd;
	struct kevent ke;

	if (argc != 2) {
		fprintf(stderr, "ERROR: expected one argument.\n");
		return EXIT_FAILURE;
	}

	errno = 0;
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open");
		return EXIT_FAILURE;
	}

	errno = 0;
	kq = kqueue();
	if (kq == -1) {
		perror("kqueue");
		return EXIT_FAILURE;
	}

	ke.ident = (uintptr_t)fd;
	ke.flags = EV_ADD;
	ke.filter = EVFILT_VNODE;
	ke.fflags = NOTE_WRITE | NOTE_DELETE;

	errno = 0;
	if (kevent(kq, &ke, 1, &ke, 1, NULL) == -1) {
		perror("kevent/add");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

