#include <stdio.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char* argv[]){
	FILE* f = fopen("/tmp/countpid","r");
	char pid_s[10];
	char cmd[32];
	fgets(pid_s,10,f);
	fclose(f);
	pid_t signalcount = atoi(pid_s);
	kill(signalcount,SIGUSR1);
	char **args = calloc(10,16*sizeof(char));	
	for ( int i = 0; i < 10; i++) args[i] = malloc(16*sizeof(char));
	strcpy(cmd,"/bin/");
	strcat(cmd,argv[1]);
	int i = 0;
	while(i < argc-1) {strcpy(args[i],argv[i+1]); i++;}
	args[i] = (char*)NULL;
	pid_t child = fork();
	if(child==0) execv(cmd,args);
	else {
	        wait(NULL);
		kill(signalcount,SIGUSR2);
	}
	return 0;
}
