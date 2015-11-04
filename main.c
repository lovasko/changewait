#include <sys/types.h>
#include <sys/time.h>
#include <sys/event.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int
main(int argc, char** argv)
{
	int fd;
	int kq;
	int option;
	struct kevent ke;
	unsigned int fflags;

	fflags = 0;

	while ((option = getopt(argc, argv, "adehlmrw")) != -1) {
		switch(option) {
			case 'a': fflags |= NOTE_ATTRIB; break;
			case 'd': fflags |= NOTE_DELETE; break;
			case 'e': fflags |= NOTE_EXTEND; break;
			case 'l': fflags |= NOTE_LINK;   break;
			case 'm': fflags |= NOTE_RENAME; break;
			case 'r': fflags |= NOTE_REVOKE; break;
			case 'w': fflags |= NOTE_WRITE;  break;

			case 'h':
				printf("checkwait [-adelmrw] file\n");
			return EXIT_SUCCESS;

			case '?':
				fprintf(stderr, "ERROR: invalid option '%c'.\n", optopt);	
			return EXIT_FAILURE;

			default: 
				fprintf(stderr, "ERROR: unknown error during option parsing\n");	
			return EXIT_FAILURE;
		}
	}

	if (argc - optind < 1) {
		fprintf(stderr, "ERROR: expected one argument.\n");
		return EXIT_FAILURE;
	}

	if (fflags == 0)
		fflags = NOTE_ATTRIB  | NOTE_DELETE
		        | NOTE_EXTEND | NOTE_LINK
						| NOTE_RENAME | NOTE_REVOKE
						| NOTE_WRITE; 

	errno = 0;
	fd = open(argv[optind], O_RDONLY);
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
	ke.fflags = fflags;

	errno = 0;
	if (kevent(kq, &ke, 1, &ke, 1, NULL) == -1) {
		perror("kevent/add");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

