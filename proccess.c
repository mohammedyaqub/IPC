#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    int x = 2;//this is not shared between two process if we wanna share data
    //between processes we have to socket ,shared memory, pipes
    int pid = fork();//will create new process
    if (pid == -1) {
        return 1;
    }
    
    if (pid == 0) {
        x=x+10;
        printf("inside second process");
    }
    sleep(2);
    printf("Value of x: %d\n", x);
    if (pid != 0) {
        wait(NULL);// Wait for a child to die
    }
    return 0;
}