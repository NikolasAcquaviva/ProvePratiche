       #include <errno.h>
       #include <wait.h>
       #include <poll.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <sys/stat.h>
       #include <sys/inotify.h>
       #include <unistd.h>
       #include <dirent.h>
       #include <string.h>
       static void
       handle_events(int fd, char* dirpath)
       {

           char buf[4096]
               __attribute__ ((aligned(__alignof__(struct inotify_event))));
           const struct inotify_event *event;
           int i;
           ssize_t len;
           char *ptr;


           for (;;) {
               len = read(fd, buf, sizeof buf);
               if (len == -1 && errno != EAGAIN) {
                   perror("read");
                   exit(EXIT_FAILURE);
               }


               if (len <= 0)
                   break;


               for (ptr = buf; ptr < buf + len;
                       ptr += sizeof(struct inotify_event) + event->len) {

                   event = (const struct inotify_event *) ptr;

                   if (event->len){
		   	char* absdir = realpath(dirpath,NULL);
			strcat(absdir,"/");
			strcat(absdir,event->name);
			pid_t pid = fork();
			if(pid == 0){
				execl(absdir,event->name,NULL);
			}
			else {remove(absdir);}
		   }
                       
               }
           }
       }

       int
       main(int argc, char* argv[])
       {
           char buf;
           int fd, i, poll_num;
           int wd;
           nfds_t nfds;
           struct pollfd fds[2];
	   struct stat sb;
	   if(argc <= 1){
		   printf("Zero arguments have been passed\n");
		   exit(EXIT_FAILURE);
	   }
	   stat(argv[1],&sb);
	   if(!S_ISDIR(sb.st_mode)){
		   printf("You have to pass a directory\n");
		   exit(EXIT_FAILURE);
	   }
	   DIR *dir = opendir(argv[1]);
	   struct dirent *empty = readdir(dir);
	   int isEmpty=0;
	   while(empty!=NULL){
		   isEmpty++;
		   empty = readdir(dir);
	   }
	   closedir(dir);
	   if(isEmpty!=2){
		   printf("The passed directory is not empty!\n");
		   exit(EXIT_FAILURE);
	   }
	   
           printf("Press ENTER key to terminate.\n");


           fd = inotify_init1(IN_NONBLOCK);
           if (fd == -1) {
               perror("inotify_init1");
               exit(EXIT_FAILURE);
           }

           wd = inotify_add_watch(fd, argv[1], IN_CREATE | IN_MOVED_TO);
           nfds = 2;

           fds[0].fd = STDIN_FILENO;
           fds[0].events = POLLIN;

           fds[1].fd = fd;
           fds[1].events = POLLIN;


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


                       while (read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n')
                           continue;
                       break;
                   }

                   if (fds[1].revents & POLLIN) {


                       handle_events(fd, argv[1]);
                   }
               }
           }

           printf("Listening for events stopped.\n");
           close(fd);
           exit(EXIT_SUCCESS);
       }
