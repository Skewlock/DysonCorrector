#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "libfile.h"
#include "dyson.h"
#include <stdlib.h>

int	get_size(int n)
{
	int		size;
	long int	nb;
	size = 0;
	nb = n;
	if (nb < 0)
	{
		size++;
		nb = -nb;
	}
	while (nb >= 10)
	{
		nb /= 10;
		size++;
	}
	return (size + 1);
}


char	*f_itoa(int n)
{
	int	size;
	int	i;
	char	*res;

	size = get_size(n);
	i = 0;
	res = malloc((size + 1) * sizeof(char));
	if (!res)
		return (NULL);
	if (n < 0)
		n = -n;
	while (i < size)
	{
		res[size - i - 1] = ((n % 10) + 48);
		n /= 10;
		i++;
	}
	res[i] = '\0';
	return (res);
}


char *f_strcat(char *dest, char *src)
{
    int destsize;
    int srcsize;
    destsize = strlen(dest);
    srcsize = strlen(src);
    for (int i = 0; i < srcsize; i++)
    {
        dest[destsize + i] = src[i];
    }
    dest[destsize + srcsize] = '\0';
    return (dest);
}

char    *genString()
{
    char *str;
    str = malloc((STRING_SIZE + 1) * sizeof(char));
    for (int i = 0; i < STRING_SIZE; i++)
        str[i] = (rand() % 95) + 32;
    str[STRING_SIZE + 1] = '\0';
    return str;
}

int genInt()
{
    int i;
    i = rand();
    return i;
}

int *genIntTab()
{
    int *tab;
    tab = malloc(TAB_SIZE * sizeof(int));
    for (int i = 0; i < TAB_SIZE; i++)
    {
        tab[i] = rand();
    }
    return tab;
}