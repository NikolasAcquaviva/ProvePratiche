       #include <pthread.h>
       #include <string.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>
       #include <errno.h>
       #include <ctype.h>
       #include <sys/eventfd.h>
       #define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

       #define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

static int bufferVal=-1;
        static int fd;

	struct thread_info {    /* Used as argument to thread_start() */
           pthread_t thread_id;        /* ID returned by pthread_create() */
           int       thread_num;       /* Application-defined thread # */
           char     *argv_string;      /* From command-line argument */
       };

       static void *Producer(void *arg)
       {
	   while(1){
		   if(bufferVal==-1){
		   	bufferVal = rand()%100;
		   	printf("Valore prodotto: %d\n",bufferVal);
		   	write(fd,NULL,1);
		   }
		   sleep((rand()%3)+1);
	   }
           return NULL;
       }

       static void *Consumer(void *arg){
	       while(1){
		       if(bufferVal!=-1){
		       read(fd,NULL,1);
		       printf("Valore letto: %d\n",bufferVal);
		       bufferVal = -1;
		       sleep((rand()%3)+1);
		       }
	       }
	       return NULL;
       }


       int
       main()
       {
           int s, tnum, opt, num_threads;
           struct thread_info *tinfo;
           pthread_attr_t attr;
           int stack_size;
           void *res;
	   fd = eventfd(0,EFD_SEMAPHORE);
	   if(fd == -1) perror("eventfd");

           stack_size = -1;

           num_threads = 2;

           s = pthread_attr_init(&attr);
           if (s != 0)
               handle_error_en(s, "pthread_attr_init");

           if (stack_size > 0) {
               s = pthread_attr_setstacksize(&attr, stack_size);
               if (s != 0)
                   handle_error_en(s, "pthread_attr_setstacksize");
           }

           tinfo = calloc(num_threads, sizeof(struct thread_info));
           if (tinfo == NULL)
               handle_error("calloc");

	   tinfo[0].thread_num = 0;
	   pthread_create(&tinfo[0].thread_id,&attr,&Producer,&tinfo[0]);
	   tinfo[1].thread_num = 1;
	   pthread_create(&tinfo[1].thread_id, &attr, &Consumer, &tinfo[1]);

           pthread_attr_destroy(&attr);
	   pthread_join(tinfo[0].thread_id,&res);
	   free(res);
	   pthread_join(tinfo[1].thread_id,&res);
	   free(res);
           free(tinfo);
	   close(fd);
           exit(EXIT_SUCCESS);
       }
