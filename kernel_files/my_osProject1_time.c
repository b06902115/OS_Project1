// syscall 346

#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage long sys_my_osProject1_time(void){
	static const long base = 1e9;
	struct timespec t;
	getnstimeofday(&t);
	return t.tv_sec * base + t.tv_nsec;
}
