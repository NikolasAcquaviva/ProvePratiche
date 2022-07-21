#include <sys/signalfd.h>
#include <fcntl.h>
#include <string.h>       
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <time.h>
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

       int
       main(int argc, char *argv[])
       {
           sigset_t mask;
           int sfd;
           struct signalfd_siginfo fdsi;
           ssize_t s;

           sigemptyset(&mask);
           sigaddset(&mask, SIGUSR1);
           sigaddset(&mask, SIGUSR2);

           /* Block signals so that they aren't handled
              according to their default dispositions */

           if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
               handle_error("sigprocmask");

           sfd = signalfd(-1, &mask, 0);
           if (sfd == -1)
               handle_error("signalfd");

           for (;;) {
               s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
               if (s != sizeof(struct signalfd_siginfo))
                   handle_error("read");

               if (fdsi.ssi_signo == SIGUSR1 || fdsi.ssi_signo == SIGUSR2) {
		   char signal_code[64];
		   if(fdsi.ssi_signo == SIGUSR1) 
			   strcpy(signal_code,"SIGUSR1 ");
		   else strcpy(signal_code,"SIGUSR2 ");
                   pid_t sender = fdsi.ssi_pid;
		   time_t t = time(NULL);
    		   struct tm *tm = localtime(&t);
    		   char s[64];
    		   strftime(s, sizeof(s), "%c", tm);
		   char buffer[128];
		   strcpy(buffer,"");
		   strcpy(buffer,signal_code);
		   strcat(buffer,s);
		   strcat(buffer,"\n");
		   char filename[64];
		   sprintf(filename,"%d",sender);
		   int fd = open(filename,O_RDWR);
		   close(fd);
		   int index;
		   for(int i = 0; i < 128; i++){
			   if(buffer[i]=='\0') {index = i; break;}
		   }
		   if(fd == -1){
			   int fdn = open(filename, O_RDWR|O_CREAT,S_IRUSR | S_IWUSR);
			   strtok(buffer,"\0");
			   int w = write(fdn,buffer,index);
			   close(fdn);
		   }
		   else{
			   int fdn = open(filename, O_RDWR | O_APPEND);
			   write(fdn,buffer,index);
			   close(fdn);
		   }
               } else {
                   printf("Read unexpected signal\n");
               }
           }
       }
