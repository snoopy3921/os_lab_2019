#include "revert_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(char *a, char *b)
{
	char temp = *a;
	*a = *b;
	*b = temp;
}

void RevertString(char *str)
{
	// your code here
	int len = strlen(str);
	for(int i = 0; i < len / 2; i++)
	{
		swap(&str[i], &str[len - i - 1]);
	}
}

