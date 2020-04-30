#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sched.h>
#include <string.h>

typedef struct
{
	char name[32];
	int ready;
	int execu;
}	proc;

void USE_CPU(pid_t pid, int x){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET((x), &mask);
	return;
}

void unit_time(){
	volatile unsigned long i;
	for(i=0;i<1000000UL;i++);
	return;
}

pid_t my_fork(int execu){
	int status;
	pid_t PID = fork();
	long int start_time;
	long int end_time;
	switch(PID){
		case -1:
			perror("fork()");
			exit(-1);
		case 0:		// child process
			start_time = syscall(346);
			for (int i = 0; i < execu; i++)
				unit_time();
			end_time = syscall(346);
			syscall(345, getpid(), start_time, end_time);
			exit(0);

		default:	// parent process
			wait(&status);
			break;
	}
	return PID;
}

int compare_FIFO(const void *p1, const void *p2){
	proc *e1 = (proc *)p1;
	proc *e2 = (proc *)p2;
	if (e1->ready > e2->ready)
		return 1;
	else
		return -1;
}

void FIFO(proc *p, int N){
	qsort(p, N, sizeof(proc), compare_FIFO);
	int index = 0;
	while (index < N){
		while (p[index].ready == 0 && index < N){
			pid_t pid = my_fork(p[index].execu);
			printf("%s %d\n", p[index].name, pid);
			fflush(stdout);
			index++;
			for (int i = index; i < N; i++){
				if (p[i].ready > p[index-1].execu)
					p[i].ready -= p[index-1].execu;
				else
					p[i].ready = 0;
			}
		}
		unit_time();
		for (int i = 0; i < N; i++)
			p[i].ready--;
	}
	return;
}

void RR_scheduler(){
	USE_CPU(getpid(), 0);
	int time = 0;
	while(1){
		// work
		time++;
		unit_time();
	}
	return;
}

void RR(){
	// time quantum: 500*unit_time()
	return;
}

int compare_SJF(const void *p1, const void *p2){
	proc *e1 = (proc *)p1;
	proc *e2 = (proc *)p2;
	if (e1->ready > e2->ready)
		return 1;
	else if (e1->ready < e2->ready)
		return -1;
	else{
		if (e1->execu > e2->execu)
			return 1;
		else
			return -1;
	}
}

void SJF(proc *p, int N, int count){
	while (count < N){
		qsort(p, N, sizeof(proc), compare_SJF);
		if (p[0].ready > 0){
			int T = p[0].ready;
			for (int i = 0; i < N; i++){
				p[i].ready -= T;
				for (int t = 0; t < T; t++)
					unit_time();
			}
		}
		if (p[0].ready == 0){
			fflush(stdout);
			pid_t pid = my_fork(p[0].execu);
			printf("%s %d\n", p[0].name, pid);
			fflush(stdout);
			p[0].ready = 300000;
			count++;
			for (int i = 1; i < N; i++){
				if (p[i].ready > p[0].execu)
					p[i].ready -= p[0].execu;
				else
					p[i].ready = 0;
			}
		}
	}
	return;
}
void PSJF(){
	return;
}
int main(){
	char S[10] = {};
	int N = 0;
	scanf("%s", S);
	scanf("%d", &N);
	proc p[N];
	for (int n = 0; n < N; n++)
		scanf("%s\t%d %d", p[n].name, &p[n].ready, &p[n].execu);
	if (strcmp(S, "FIFO") == 0)
		FIFO(p, N);
	else if (strcmp(S, "RR") == 0)
		RR();
	else if (strcmp(S, "SJF") == 0)
		SJF(p, N, 0);
	else if (strcmp(S, "PSJF") == 0)
		PSJF();
	
	return 0;
}