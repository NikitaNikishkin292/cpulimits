#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

double workTime;
int pid;
int periodFlag = 1;

int main(int argc, char** argv) {
	if(argc <= 2) {
		printf ("Too few arguments.\n");
		return 1;
	}
	int t_lim = 10;
	workTime = 10 * atoi(argv[1]) / 100;
	pid = fork();
	if(pid == -1) {
		printf("Fork() error.\n");
		return 1;
	}
	char** arg_list = argv + 2;
	if(pid == 0) {
		if(execv(argv[2], arg_list) == -1) {
			printf("Error exec\n");
			return 1;
		}
	} 
	else {
		while(1) {
			if(periodFlag == 1) {
				usleep(workTime);
				if(kill(pid, SIGSTOP) == -1) {
					printf("Error. kill()\n");
					return 1;
				}
				periodFlag = 0;
			} 
			else {
				usleep(10 - workTime);
				if(kill(pid, SIGCONT) == -1) {
					printf ("Error. kill()\n");
					return 1;
				}
				periodFlag = 1;
			}
		}
	}
	wait(pid);
	return 0;
}
