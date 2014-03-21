#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(void)
{
    syscall(541);
    return 0;
}
