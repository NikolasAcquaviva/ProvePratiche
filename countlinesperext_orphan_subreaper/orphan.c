#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <wait.h>
int main(){
	pid_t father = getpid();
	printf("pid of father %d\n",father);
	prctl(PR_SET_CHILD_SUBREAPER,1);
	prctl(PR_SET_NAME,"nonno");
	pid_t child =fork();
	if(child==0){
		prctl(PR_SET_NAME,"figlio");
		pid_t nipote = fork();
		if(nipote==0){
			prctl(PR_SET_NAME,"nipote");
			sleep(10);
			printf("pid of nipote %d\n", getpid());
			exit(69);
		}
		else {
			printf("pid of child %d\n",getpid());
			sleep(5);
			exit(20);
		}
	}
	else {
		int status;
		waitpid(-1,&status,0);
		printf("exit status: %d\n", WEXITSTATUS(status));
		int status2;
		waitpid(-1,&status2,0);
		printf("exit status: %d\n", WEXITSTATUS(status2));
	}
	return 0;
}
