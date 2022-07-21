#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <poll.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
static void handle_events(int fd, int argc, char* argv[]){
	char *filename = malloc(30*sizeof(char));
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
			char *introMsg = malloc(256*sizeof(char));
			strcpy(filename,event->name);
			strcpy(introMsg,"Hai aggiunto il file: ");
			strcat(introMsg,filename);
			char *afterMv = malloc(256*sizeof(char));
			strcpy(afterMv,argv[1]);
			strcat(afterMv,"/");
			strcat(afterMv,filename);
			//check if executable
			struct stat sb;
			if (stat(afterMv, &sb) == 0 && sb.st_mode & S_IXUSR){
				FILE *dest = fopen(argv[2],"a");
				int red = fileno(dest);
				dup2(red,1);
				close(red);
				pid_t p = fork();
				if(p==0) execl(afterMv,afterMv,(char*)NULL);
				//here the execution terminates
				else if(p > 0) {
					waitpid(-1,NULL,0);
					remove(afterMv);
					exit(0);
				}
			}
			else{
				FILE *from, *to;
				char buffer[256];
				from = fopen(afterMv, "r");
				to = fopen(argv[2], "a");
				if(from == NULL) perror("error from");
				if(to == NULL) perror("error to");
				fprintf(to,"%s\n",introMsg);
				while(fgets(buffer,sizeof(buffer),from))
					fprintf(to,"%s",buffer);
				fprintf(to,"\n");
				fclose(from);
				fclose(to);
				remove(afterMv);
				exit(0);
			}
		}
	}
}

int main(int argc, char* argv[]){
	if(argc < 3){
		printf("USAGE: %s directory file\n",argv[0]);
		exit(-1);
	}
	else{
		struct stat path_stat;
		stat(argv[1], &path_stat);
		if(S_ISREG(path_stat.st_mode)){
			printf("first argument has to be a directory\n");
			exit(-1);
		}

		struct stat file_stat;
		stat(argv[2], &file_stat);
		if(!S_ISREG(file_stat.st_mode)){
			printf("second argument has to be a file\n");
			exit(-1);
		}
	}

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

	inotify_add_watch(fd,"box",IN_MOVED_TO);
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
				if(pid==0) handle_events(fd,argc,argv);
				else if(pid > 0) waitpid(-1,NULL,0);
			}
		}
	}

	printf("Listening for events stopped.\n");

	/* Close inotify file descriptor */

	close(fd);
	exit(EXIT_SUCCESS);
}
