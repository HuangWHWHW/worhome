#include <stdio.h>

char getch()
{
	system("stty raw -echo");
        char ch = getchar();
        system("stty -raw echo");
	
	return ch;
}
