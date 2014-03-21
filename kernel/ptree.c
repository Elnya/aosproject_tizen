#include <asm-generic/errno-base.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

#define QUEUE_SIZE 128

// struct task_struct queue[QUEUE_SIZE];

/*
 *
 1  procedure BFS(Graph,source):
 2      create a queue Q
 3      enqueue source onto Q
 4      mark source
 5      while Q is not empty:
 6          dequeue an item from Q into v
 7          for each edge e incident on v in Graph:
 8              let w be the other end of e
 9              if w is not marked:
 10                 mark w
 11                 enqueue w onto Q
 * */

/*  */

asmlinkage int sys_ptree(void) {
    /*
    * If buf or nr are null, or if the number of entries is less than 1 */
    /*
    if (buf == NULL || nr == NULL || *nr < 1)
        return -EINVAL;
    
     if buf or nr are outside the accessible address space. 
    if (buf == )
        return -EFAULT;
    return
     */

    /*
    struct task_struct *p;
    pr_info("");
    for_each_process(s) {
        pr_info();
    }
    */
    printk("<1>Hellow world!\n");
    return 0;
}

