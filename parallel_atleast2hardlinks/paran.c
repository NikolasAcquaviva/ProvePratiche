#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char* argv[]){
	int maxP = strtol(argv[1],NULL,10);
	printf("%d\n", maxP);	
	int childProcesses = 0;
	char* commands[argc];
	for(int i = 0; i < argc; i++) {
		commands[i] = malloc(32*sizeof(char));
	}
	
	int i,j;
       	i = 2; j = 0;
	
	while(i < argc){
		int equal = strcmp(argv[i],"//");
		if(equal != 0){
			strcat(commands[j],argv[i]);
			strcat(commands[j]," ");
			i++;
		}
		else{
			i++;
			j++;
		}
	}
	int count = 0;
	while(count < argc && strcmp(commands[count],"")){
		printf("it %d\n",count);
		char* tmp = malloc(32*sizeof(char));
		strcpy(tmp,commands[count]);
		char* args[32];
		for(int i = 0; i < 32; i++) args[i] = malloc(64*sizeof(char));
		int j = 0;
		char* maincmd = strtok(tmp," ");
		char* bincmd = malloc(sizeof(char)*32);
		strcpy(bincmd,"/bin/");
		strcat(bincmd,maincmd);
		while(maincmd!=NULL){
			strcpy(args[j],maincmd);
			maincmd = strtok(NULL," ");
			j++;
		}
		args[j] = (char*) NULL;
		if(childProcesses < maxP){
			childProcesses++;
			pid_t pid = fork();
			if(pid == 0){
				execv(bincmd,args);
			}
			else if (pid > 0) {
				count++;
			}
		}
		else {wait(NULL); childProcesses--; continue; }
		
	}
        for(int i = 0; i < argc/2; i++) wait(NULL); 
	return 0;
}
