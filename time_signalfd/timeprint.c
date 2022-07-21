#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char* argv[]){
	if(argc!=2) {
		printf("pass file as argument\n");
		exit(-1);
	}
	struct stat sb;
	stat(argv[1],&sb);
	if(!S_ISREG(sb.st_mode)){
		printf("%s is not a file\n", argv[1]);
		exit(-1);
	}
	FILE* fd = fopen(argv[1],"r");
	int r;
	char *buffer = NULL;
	size_t length = 0;
	int iteration = 0;
	time_t prev = 0;
	time_t curr = 0;
	while(r = getline(&buffer,&length,fd)!=-1){
		char *tmp = malloc(128*sizeof(char));
		struct tm *tm = malloc(sizeof(struct tm));
		tm->tm_isdst = 1;
		tm->tm_mon = 10;
		tm->tm_mday = 1;
		tm->tm_wday=0;
		tm->tm_yday=0;
                tm->tm_year = 2019;
		strcpy(tmp,buffer);
		strtok(tmp," ");
		int count = 0;
		while(count<2){
			tmp = strtok(NULL," ");
			count++;
		}
		strptime(tmp,"%H:%M:%S",tm);
		if(iteration!=0) prev = curr;
		curr = mktime(tm);
		if(iteration!=0) sleep(curr-prev);
		printf("curr: %ld\n",curr);
		printf("%s\n", buffer);
		iteration++;
	}
		
	
	return 0;
}
