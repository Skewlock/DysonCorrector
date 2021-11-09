#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "libfile/libfile.h"

#define STRING_SIZE 50000
#define TAB_SIZE 2000

typedef struct Input
{
    int type;
    int tab_size;
    union
    {
        char *string;
        int number;
        int *tab;
    } values;
} Input;

enum types {
    STRING = 0,
    INT = 1,
    INTTAB = 2,
};

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

void clean() 
//nettoyage de fichiers résiduels
{
    system("rm -rf res.out truth comparaison truth.out");
}

void execTest(char *name)
//compilation et execution du programme vrai (l'exemple)
{
    file fd;
    char *compile;
    int size;

    printf("\n\ncompiling correct answer.\n\n");
    size = strlen("gcc -Wall -Wextra -Werror -o truth main.c ") + strlen(name) + 1;
    compile = (char *)malloc(size * sizeof(char));
    if (!compile)
        return;
    compile[0] = '\0';
    compile = f_strcat(compile, "gcc -Wall -Wextra -Werror -o truth main.c ");
    compile = f_strcat(compile, name);
    printf("%s", compile);
    system(compile);
    printf("\n\nexecuting it...\n");
    system("./truth > truth.out");
}

void execNext(char **argv, int argc)
//compilation et execution de tous les autres programmes
{
    file fd;
    file fd2;
    char *compiling = "gcc -Wall -Wextra -Werror -o prog main.c ";
    char *compilingRes;
    char *diff = "diff -q truth.out res.out > comparaison";
    char *diffRes;
    char *fc;
    char *fc2;
    //char *temp;
    int size;
    for (int i = 3; i < argc; i++)
    {
        printf("\n--------------------------------\n");
        printf("\nCompiling program n°%d.\n\n", i - 2);
        size = strlen(compiling) + strlen(argv[i]) + 1;
        compilingRes = (char *) malloc(size * sizeof(char));
        compilingRes[0] = '\0';
        compilingRes = f_strcat(compilingRes, compiling);
        compilingRes = f_strcat(compilingRes, argv[i]);
        printf("%s", compilingRes);
        system(compilingRes);
        free(compilingRes);
        printf("\n\nexecuting it...");
        system("./prog > res.out");
        //temp = f_itoa(i - 2);
        size = strlen(compiling) + 1; //strlen(temp) + 1;
        diffRes = (char *) malloc(size * sizeof(char));
        diffRes[0] = '\0';
        diffRes = f_strcat(diffRes, diff);
        //diffRes = f_strcat(diffRes, temp);
        system(diffRes);
        free(diffRes);
        fd = fi_openRFile("comparaison");
        fc = fi_readFile(fd);
        printf("\n\n--------------------------------");
        if (fc[0] != 0)
        {
            fd2 = fi_openRFile("res.out");
            fc2 = fi_readFile(fd2);
            printf("\n\nerror, program output:\n\n");
            printf("%s\n", fc2);
            fi_closeFile(fd2);
            printf("expected output:\n\n");
            fd2 = fi_openRFile("truth.out");
            fc2 = fi_readFile(fd2);
            printf("%s", fc2);
            fi_closeFile(fd2);
        }
        else
        {
            printf("\n\nOk !");
        }
        printf("\n\n");
        fi_closeFile(fd);
        system("rm -rf prog");
    }
}

int main(int argc, char *argv[])
{
    time_t t;
    srand((unsigned) time(&t));
    Input in;
    printf("----------------------------\n");
    // génération d'un input dpendant du type
    switch (atoi(argv[1]))
    {
        case 0:
            printf("Generating input type string...\n\n");
            in.type = STRING;
            in.values.string = genString();
            break;
        case 1:
            printf("Generating input type int...\n\n");
            in.type = INT;
            in.values.number = genInt();
            break;
        case 2:
        printf("Generating input type int tab...\n\n");
            in.type = INTTAB;
            in.values.tab = genIntTab();
            in.tab_size = TAB_SIZE;
            break;
        default:
            return (-1);
    }
    // affichage de l'input
    printf("input: \n\n");
    if (in.type == STRING)
        printf("%s\n", in.values.string);
    else if (in.type == INT)
        printf("%d\n", in.values.number);
    else if (in.type == INTTAB)
    {
        printf("[");
        for (int i = 0; i < in.tab_size; i++)
        {
            printf("%d", in.values.tab[i]);
            if (i < in.tab_size - 1)
            printf(", ");
        }
        printf("]");
    }
    printf("\n\ndone.\n-----------------------------");
    execTest(argv[2]);
    execNext(argv, argc);
    clean();
    return (0);
}