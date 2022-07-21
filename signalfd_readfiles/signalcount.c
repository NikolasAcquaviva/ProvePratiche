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
	   pid_t my = getpid();

       	   FILE* f = fopen("/tmp/countpid","w");
	   fprintf(f,"%d\n",my);
	   fclose(f);	   
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

	   int count = 0;
           for (;;) {
               s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
               if (s != sizeof(struct signalfd_siginfo))
                   handle_error("read");

               if (fdsi.ssi_signo == SIGUSR1 || fdsi.ssi_signo == SIGUSR2) {
		   char signal_code[64];
		   if(fdsi.ssi_signo == SIGUSR1){
			  count++;
			  printf("Segnalato segnale SIGUSR1, contatore portato a: %d\n",count); 
		   }			   
		   else {
			   count--;
			   printf("Segnalato segnale SIGUSR2, contatore portato a: %d\n",count);
		   }

               }
           }
       }
