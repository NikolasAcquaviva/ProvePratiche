#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <wait.h>
int main(int argc, char* argv[]){
	struct stat sb1,sb2;
	stat(argv[1],&sb1);
	stat(argv[2],&sb2);
	off_t size1 = sb1.st_size;
	off_t size2 = sb2.st_size;
	char buffer1[64],buffer2[64],buffer3[64],buffer4[64];
	FILE* f1 = fopen(argv[1],"r");
	FILE* f2 = fopen(argv[2],"r");
	int ch1 = fread(buffer1,1,size1,f1);
	int ch2 = fread(buffer2,1,size2,f2);
	fclose(f1);
	fclose(f2);
	for(int i = 0; i < size1 - size1/2; i++) 
		buffer3[i] = buffer1[i+size1/2];
	for(int i = 0; i < size2 - size2/2; i++)
	        buffer4[i] = buffer2[i+size2/2];
	buffer1[size1/2] = '\0';
	buffer2[size2/2] = '\0';	
	buffer3[size1-size1/2-1] = '\0';
	buffer4[size2-size2/2-1] = '\0';
	pid_t p1 = fork();
	pid_t p2 = fork();
	if(p1==0 && p2!=0){
		if(strcmp(buffer1,buffer2)!=0){//mandare segnale a padre
			printf("segnalata differenza in prima meta\n");
			exit(1);	
		}
		else exit(0);
	}
	else if(p2==0 && p1!=0){
		if(strcmp(buffer3,buffer4)!=0){
			printf("segnalata differenza in seconda meta\n");
			exit(2);
		}
		else exit(0);
	}
	else if(p1 > 0 && p2 > 0){
		int status;
		waitpid(-1,&status,0);
		if(WEXITSTATUS(status) == 1) {
			kill(p2,SIGKILL);
		}
		else if(WEXITSTATUS(status)==2) {
			kill(p1,SIGKILL);
		}
		else{
			wait(NULL);
        		wait(NULL);
       	 		printf("nodifferenze");
		}
	}
	return 0;
}
