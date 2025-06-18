#ifndef SYSCALLS_H
#define SYSCALLS_H

void exit(int retval);
void return_exit(int retval);
void cons_write(const char *str, unsigned long len);
void cons_echo(int on);
int cons_read(char *string, unsigned long length);
int start(int (*pt_func)(void *), unsigned long ssize, int prio, const char *name, void *arg);
int waitpid(int pid, int *retvalp);
int kill(int pid);
int chprio(int pid, int newprio);
int getprio(int pid);
int getpid(void);
int pcreate(int count);
int pdelete(int fid);
int pcount(int fid, int *count);
int psend(int fid, int msg);
int preceive(int fid, int *msg);
int preset(int fid);
void wait_clock(unsigned long clock);
void clock_settings(unsigned long *quartz, unsigned long *ticks);
unsigned long current_clock();
#endif // SYSCALLS_H
