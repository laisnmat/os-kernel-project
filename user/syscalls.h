#ifndef SYSCALLS_H
#define SYSCALLS_H

void exit(int retval);
void return_exit(int retval);
void cons_write(const char *str, unsigned long len);
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

#endif // SYSCALLS_H
