#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include "Fonction.c"
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#endif
#define MAX 100
#define MAX_TIME 100
#define MAX_STUDENTS 100

int main()
{
    LancerApp();

    return 0;
}

