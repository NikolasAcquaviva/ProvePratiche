#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
	static void
handle_events(int fd)
{
	char *command = malloc(30*sizeof(char));
	char buf[4096]
		__attribute__ ((aligned(__alignof__(struct inotify_event))));
	const struct inotify_event *event;
	ssize_t len;
	char *ptr;
	/* Loop while events can be read from inotify file descriptor. */

	for (;;) {

		/* Read some events. */

		len = read(fd, buf, sizeof buf);
		if (len == -1 && errno != EAGAIN) {
			perror("read");
			exit(EXIT_FAILURE);
		}

		/* If the nonblocking read() found no events to read, then
		   it returns -1 with errno set to EAGAIN. In that case,
		   we exit the loop. */

		if (len <= 0)
			break;

		for (ptr = buf; ptr < buf + len;
				ptr += sizeof(struct inotify_event) + event->len) {

			event = (const struct inotify_event *) ptr;

			strcpy(command,event->name);
			char* tmp = malloc(strlen(command));
			strcpy(tmp,command);
			char *argv[40];
			for(int i = 0; i < 40; i++)
				argv[i] = malloc(40*sizeof(char));
			int i = 0;
			char* list = strtok(tmp, " ");
			while(list!=NULL){
				strcpy(argv[i], list);
				i++;
				list = strtok(NULL," ");
			}
			char* commandPath = malloc(40*sizeof(char));
			strcpy(commandPath,"/bin/");
			strcat(commandPath,argv[0]);
			argv[i]=(char*)NULL;
			char* filename = malloc(30*sizeof(char));
			strcat(filename,"exec/");
			strcat(filename,command);

			int fred = open(filename, O_CREAT | O_RDWR);
			dup2(fred,1);
			close(fred);
			execv(commandPath,argv);
		}
	}
}

	int
main()
{
	char buf;
	int fd, poll_num;
	nfds_t nfds;
	struct pollfd fds[2];

	printf("Press ENTER key to terminate.\n");

	/* Create the file descriptor for accessing the inotify API */

	fd = inotify_init1(IN_NONBLOCK);
	if (fd == -1) {
		perror("inotify_init1");
		exit(EXIT_FAILURE);
	}

	inotify_add_watch(fd,"exec",IN_CREATE);
	/* Prepare for polling */

	nfds = 2;

	/* Console input */

	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;

	/* Inotify input */

	fds[1].fd = fd;
	fds[1].events = POLLIN;

	/* Wait for events and/or terminal input */

	printf("Listening for events.\n");
	while (1) {
		poll_num = poll(fds, nfds, -1);
		if (poll_num == -1) {
			if (errno == EINTR)
				continue;
			perror("poll");
			exit(EXIT_FAILURE);
		}

		if (poll_num > 0) {

			if (fds[0].revents & POLLIN) {

				/* Console input is available. Empty stdin and quit */

				while (read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n')
					continue;
				break;
			}

			if (fds[1].revents & POLLIN) {
				pid_t pid = fork();
				if(pid==0) handle_events(fd);
				else if(pid > 0) waitpid(-1,NULL,0);
			}
		}
	}

	printf("Listening for events stopped.\n");

	/* Close inotify file descriptor */

	close(fd);
	exit(EXIT_SUCCESS);
}
