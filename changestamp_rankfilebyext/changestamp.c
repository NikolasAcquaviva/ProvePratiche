#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

void age(char* file){
	struct stat sb;
	stat(file,&sb);
	time_t lastA = sb.st_atime;
	lastA-=864000;
	struct tm *local = localtime(&lastA);
	char newdate[64];
	strftime(newdate,64,"%Y-%m-\%d %H:%M:\%S",local);
        pid_t x = fork();
	if(x==0){
		execl("/bin/touch","touch","-d",newdate,file,NULL);
	}
	else wait(NULL);	
}
int main(int argc, char*argv[]){
	if(argc==1){
		DIR *d = opendir(".");
		struct dirent *tmp;
		while((tmp=readdir(d))!=NULL){
			age(tmp->d_name);
		}
		closedir(d);
	}
	else{
		for(int i = 1; i < argc; i++) age(argv[1]);
	}
	return 0;
}
