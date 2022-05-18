//
// Created by Laith on 13/05/2022.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

char inputStr[512], POL[512];
int VAL, parametersNum;

char **splitToArray();

double calc(char *);

double solvePol(char *[]);

int check();

int countPlus();

void loop();

double mathPow(int x, int y) {
    int i = 1;
    double res = 0;
    if (y == 1)
        return x;
    if (y == 0)
        return 1;
    while (i < y) {
        res = x * x;
        i++;
    }
    return res;
}

int main() {
    loop();
}

void loop() {
    double total;
    while (1) {
        total = 0;
        parametersNum = 0;
        printf("please enter an \"POLYNOM, VAL\" or \"done\" to exit\n");
        fgets(inputStr, 512, stdin);
        if (inputStr[strlen(inputStr) - 1] == '\n')
            inputStr[strlen(inputStr) - 1] = '\0';
        if (strcmp(inputStr, "done") == 0)
            break;

        int i = 0, index = -1;
        while (i < strlen(inputStr)) {
            if (inputStr[i] == ',')
                index = i;
            if (inputStr[i] == 'x')
                parametersNum++;
            i++;
        }
        if (inputStr[index - 1] == ' ' || inputStr[index + 2] == ' ' || inputStr[index + 1] != ' ')
            fprintf(stderr, "bad syntax, check the spaces\n");
        else {
            strncpy(POL, inputStr, index);
            POL[index] = '\0';
            VAL = atoi(&inputStr[index + 1]);

            char **array = splitToArray();


            double temp = solvePol(array);
            total = temp;


            if (check() == 1) {
                fprintf(stderr, "bad syntax, check the symbols you used\n");
                continue;
            }
            int lastElem = 0;
            while (inputStr[index] != '+') {
                if (inputStr[index] == 'x') {
                    lastElem = -1;
                    break;
                }
                index--;
            }
            if (lastElem == -1)
                lastElem = 0;
            else
                lastElem = atoi(&POL[index]);

            if (countPlus() == 0 && parametersNum == 0)
                total = atoi(POL);
            else if (countPlus() >= parametersNum)
                total += lastElem;

            for (int j = 0; j < parametersNum; ++j) {
                free(array[j]);
            }
            free(array);

            printf("the result = %lf\n", total);

        }
    }
}

int check() {
    int i = 0;
    while (i < strlen(POL)) {
        if ((POL[i] < '0' || POL[i] > '9') && (POL[i] != 'x' && POL[i] != '+' && POL[i] != '^' && POL[i] != '*'))
            return 1;
        i++;
    }
    return 0;
}

int countPlus() {
    int i = 0, count = 0;
    while (i < strlen(POL)) {
        if (POL[i] == '+')
            count++;
        i++;
    }
    return count;
}

double solvePol(char *arrayOfString[]) {
    double *ptr;
    double res = 0;
    key_t key;
    int shm_id;
    key = ftok("/tmp", 'y');
    if ((shm_id = shmget(key, 10 * sizeof(double), IPC_CREAT | IPC_EXCL | 0600)) == -1) {
        perror("failed shmget");
        exit(EXIT_FAILURE);
    }
    if ((ptr = (double *) shmat(shm_id, NULL, 0)) < 0) {
        perror("failed shmat");
        exit(EXIT_FAILURE);
    }

    pid_t child;
    int i = 0;
    while (i < parametersNum) {

        if ((child = fork()) == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if (child == 0) {
            ptr[i] = calc(arrayOfString[i]);
            exit(EXIT_SUCCESS);
        }
        wait(NULL);
        i++;
    }
    i = 0;
    while (i < parametersNum)
        res += ptr[i++];

    shmdt(ptr);
    shmctl(shm_id, IPC_RMID, NULL);

    return res;

}

char **splitToArray() {
    int j = 0, i = 0, start = 0, len = 0;
    char **arrayOfString = malloc(sizeof(char) * parametersNum);
    while (j < parametersNum) {
        if (POL[i] == '+' || POL[i] == '\0') {
            arrayOfString[j] = malloc(sizeof(char) * (len + 1));
            strncpy(arrayOfString[j], &POL[start], i - start);
            len = 0;
            start = i + 1;
            j++;
        }
        i++;
    }
    return arrayOfString;
}

double calc(char *str) {
    int i = 0, start = 0, j = 0;
    int starInd = -1, powInd = -1;
    int coef = 1, power = 1;
    char number[20], powNum[20];
    while (i < strlen(str)) {
        if (str[i] == '*')
            starInd = i;
        if (str[i] == '^')
            powInd = i;
        i++;
    }
    i = 0;
    while (i < starInd) {
        if (str[i] >= '0' && str[i] <= '9') {
            number[j] = str[i];
            j++;
        }
        i++;
    }
    number[j] = '\0';
    if (strlen(number) > 0)
        coef = atoi(number);
    j = 0;
    i = powInd;
    while (i < strlen(str)) {
        if (str[i] >= '0' && str[i] <= '9') {
            powNum[j] = str[i];
            j++;
        }
        i++;
    }
    powNum[j] = '\0';
    if (strlen(powNum) > 0)
        power = atoi(powNum);
    double res;
    res = mathPow(VAL, power);
    res *= coef;


    return res;

}