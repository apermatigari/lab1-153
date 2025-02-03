#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_waitpid(void)
{
  // declaring variables
  // pid - stores process ID of child processes
  // options - stores flags
  // status - store exit status

  uint64 status; 
  int pid, options;


  // I believe the return must correspond to: int waitpid(int *status, int pid, int options);

    //gets the first argument which is the child exit status
    argaddr(0, &status);
    
    // takes second argument which makes waitpid() wait for the child pid
    argint(1, &pid);
    // gets third argument defines the behavior of each waitpid() system call
    argint(2, &options);
   
    // if option is 1, return 0, meaning no waiting occurs, returns immediately
    if (options == 1) {
        printf("OPTIONS=1\n");
        return 0;
    // if 3, return -1, meaning invalid, returns an error
    } 
    else if (options == 3) {
        return -1;
    }


    // 0 and 2 are not checked explicitly because waitpid() will handle it
    // options == 0 is default behavior, no need for a check
    // 2 is valid, so it gets forwarded to waitpid()
    // only filters 1 and 3 because they require an immediate action, the return
   
    return waitpid(status, pid, options);
}
