#ifndef DYSON_H
#define DYSON_H

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

int	get_size(int n);
char	*f_itoa(int n);
char *f_strcat(char *dest, char *src);


#endif