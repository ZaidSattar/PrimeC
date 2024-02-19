#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

bool primeCheck(int num) { // this function will return a boolean true or false depending on if the specific number in the range is prime or not
    if (num <= 1)
        return false;

    if (num <= 3)
        return true;

    if (num % 2 == 0)
        return false;

    if (num % 3 == 0)
        return false;

    for (int i = 5; i * i <= num; i += 6) {

        if (num % i == 0)
            return false;

        if (num % (i + 2) == 0)
            return false;
    }

    return true;
}

void serialFunction(int min, int max) { // this function will be used to do the serial calculations (when the argument starts with 0)

    int parentPid = getpid();
    int totalRange = max - min;
    int range = totalRange / 4;

    for (int i = 0; i < 4; i++) {
        int start = min + i * range;
        int finish;

        if (i == 3) {
            finish = max;
        }
        else {

            finish = start + range - 1;
            if (i < 3) {
                finish++;
            }
        }

        int count = 0;
        int sum = 0;

        for (int j = start; j <= finish; j++) {
            if (primeCheck(j)) { //checks if it returns true
                count++;
                sum += j;
            }
        }

        printf("pid: %d, ppid: %d - Count and sum of prime numbers between %d and %d are %d and %d\n", parentPid, parentPid, start, finish, count, sum);
    }
}

void parallelMethod(int start, int end, int parentPid) { // this will be used in the parralel process only
    int count = 0;
    int sum = 0;

    for (int i = start; i <= end; i++) {
        if (primeCheck(i)) {
            count++;
            sum += i;
        }
    }

    printf("pid: %d, ppid: %d - Count and sum of prime numbers between %d and %d are %d and %d\n", getpid(), parentPid, start, end, count, sum);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <mode> <min> <max>\n", argv[0]);
        return 1;
    }

    int mode = atoi(argv[1]);
    int min = atoi(argv[2]);
    int max = atoi(argv[3]);
    int range = (max - min + 1) / 4;

    printf("Process id: %d\n", getpid());

    if (mode == 0) { // if the argument starts with 0 do the serial function
        serialFunction(min, max);

    }
    else { // else if the argument starts with any other number do the parallel execution

        for (int i = 0; i < 4; i++) {

            pid_t pid = fork();
            if (pid == 0) {
                int start = min + i * range;
                int end = (i == 3) ? max : start + range;
                parallelMethod(start, end, getppid());
                exit(0);

            }
            else if (pid > 0) {
                continue;

            }
            else {
                perror("fork");
                return 1;
            }
        }

        for (int i = 0; i < 4; i++) {
            wait(NULL);
        }
    }

    return 0;
}

