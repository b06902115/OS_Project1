// syscall  345

#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void sys_my_osProject1_print(int pid, long start_time, long end_time){
	static const long base = 1e9;
	printk(KERN_INFO "[Project1] %d %ld.%9ld %ld.%9ld\n", pid, start_time/base, start_time%base, end_time/base, end_time%base);
}
