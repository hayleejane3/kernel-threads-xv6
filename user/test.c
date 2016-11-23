#include "types.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
int global = 1;
lock_t lock;

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   ppid = getpid();
   lock_init(&lock);

   int arg = 35;
   sleep(20);
  lock_acquire(&lock);
  sleep(10);
  lock_release(&lock);

   int thread_pid1 = thread_create(worker, &arg);
   printf(1, "Created thread 1. PID : %d\n\n", thread_pid1);

   sleep(60);
  lock_acquire(&lock);

   int thread_pid2 = thread_create(worker, &arg);
   printf(1, "Created thread 2. PID : %d\n\n", thread_pid2);

   int thread_pid3 = thread_create(worker, &arg);
   printf(1, "Created thread 3. PID : %d\n\n", thread_pid3);

   sleep(80);
   lock_release(&lock);

   int thread_pid4 = thread_create(worker, &arg);
   printf(1, "Created thread 4. PID : %d\n\n", thread_pid4);

   int thread_pid5 = thread_create(worker, &arg);
   printf(1, "Created thread 5. PID : %d\n", thread_pid5);

   assert(thread_pid1 > 0);
   assert(thread_pid2 > 0);
   assert(thread_pid3 > 0);
   assert(thread_pid4 > 0);
   assert(thread_pid5 > 0);

   int join_pid = thread_join();
   //assert(join_pid == thread_pid);
   printf(1, "Joined : %d\n", join_pid);
   join_pid = thread_join();
   printf(1, "Joined : %d\n", join_pid);
   join_pid = thread_join();
   printf(1, "Joined : %d\n", join_pid);
   join_pid = thread_join();
   printf(1, "Joined : %d\n", join_pid);
   join_pid = thread_join();
   printf(1, "Joined : %d\n\n", join_pid);

   printf(1, "global : %d\n", global);
   assert(global == 6);

   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   lock_acquire(&lock);
   int arg = *(int*)arg_ptr;
   assert(arg == 35);
   //assert(global == 1);
   global++;
   lock_release(&lock);
   exit();
}
