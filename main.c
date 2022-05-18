#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

char inputStr[512], POL[512];
int VAL, parametersNum;

typedef struct POL {
    char *str;
    int size;

} polynom;

void *calc(void *);

void freeStruct(polynom *pol);

void loop();

int check();

int countPlus();

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

polynom *splitToStruct();

int main() {
    loop();
}

void loop() {
    double total;
    while (1) {
        total=0;
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
        if (inputStr[index - 1] == ' ' || inputStr[index + 2] == ' ' || inputStr[index + 1] != ' '){
            fprintf(stderr, "bad syntax, check the spaces\n");
            continue;
        }

        strncpy(POL, inputStr, index);
        POL[index] = '\0';
        VAL = atoi(&inputStr[index + 1]);

        if (check() == 1) {
            fprintf(stderr, "bad syntax, check the symbols you used\n");
            continue;
        }

        int lastElem=0;
        while (inputStr[index]!='+'){
            if(inputStr[index]=='x'){
                lastElem=-1;
                break;
            }
            index--;
        }
        if(lastElem==-1)
            lastElem=0;
        else
            lastElem= atoi(&POL[index]);


        double *res;
        polynom *s = splitToStruct();
        pthread_t threads[parametersNum];
        for (int j = 0; j < parametersNum; ++j) {
            pthread_create(&threads[j], NULL, calc, &(s[j]));
        }
        for (int j = 0; j < parametersNum; ++j) {
            pthread_join(threads[j], (void **) &res);
            total += *res;
            free(res);
        }
        if(countPlus()==0&&parametersNum==0)
            total= atoi(POL);
        else if (countPlus() >= parametersNum)
            total += lastElem;

        printf("the result = %lf\n", total);


        freeStruct(s);
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

polynom *splitToStruct() {
    int len = 0, i = 0, j = 0, start = 0;
    polynom *arrayOfStruct = malloc(sizeof(polynom) * parametersNum);
    while (j < parametersNum) {
        if (POL[i] == '+' || POL[i] == '\0') {
            arrayOfStruct[j].str = malloc(sizeof(char) * (len + 1));
            strncpy(arrayOfStruct[j].str, &POL[start], i - start);
            arrayOfStruct[j].str[len] = '\0';
            arrayOfStruct[j].size = (int) strlen(arrayOfStruct[j].str);
            len = 0;
            start = i + 1;
            j++;
        }
        i++;
        len++;
    }

    return arrayOfStruct;
}

void freeStruct(polynom *pol) {
    int j = 0;
    while (j < parametersNum) {
        free(pol[j].str);
        j++;
    }
    free(pol);
}

void *calc(void *args) {
    polynom *eq = (polynom *) args;
    int i = 0, start = 0, j = 0;
    int starInd = -1, powInd = -1;
    int coef = 1, power = 1;
    char number[20], powNum[20];
    while (i < eq->size) {
        if (eq->str[i] == '*')
            starInd = i;
        if (eq->str[i] == '^')
            powInd = i;
        i++;
    }
    i = 0;
    while (i < starInd) {
        if (eq->str[i] >= '0' && eq->str[i] <= '9') {
            number[j] = eq->str[i];
            j++;
        }
        i++;
    }
    number[j] = '\0';
    if (strlen(number) > 0)
        coef = atoi(number);
    j = 0;
    i = powInd;
    while (i < eq->size) {
        if (eq->str[i] >= '0' && eq->str[i] <= '9') {
            powNum[j] = eq->str[i];
            j++;
        }
        i++;
    }
    powNum[j] = '\0';
    if (strlen(powNum) > 0)
        power = atoi(powNum);
    double *res = malloc(sizeof(double));
    *res = mathPow(VAL, power);
    *res *= coef;

    pthread_exit(res);

}