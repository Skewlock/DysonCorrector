#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "libfile.h"

int f_strlen(char *str)
{
    int count;

    count = 0;
    while (str[count])
        count++;
    return (count);
}

file    fi_openRFile(char *path)
{
    file fd;
    fd = _open(path, _O_RDONLY | _O_CREAT, _S_IREAD | _S_IWRITE);
    return (fd);
}

file    fi_openWFile(char *path)
{
    file fd;
    fd = _open(path, _O_WRONLY | _O_CREAT, _S_IREAD | _S_IWRITE);
    return (fd);
}

file    fi_openRWFile(char *path)
{
    file fd;
    fd = _open(path, _O_RDWR | _O_CREAT, _S_IREAD | _S_IWRITE);
    return (fd);
}

void fi_closeFile(file fd)
{
    close(fd);
}

int fi_len(file fd)
{
    int valid;
    int count;
    char buf;

    lseek(fd, 0, SEEK_SET);
    valid = 1;
    while (valid)
    {
        valid = read(fd, &buf, 1);
        count += valid;
    }
    return (count);
}

char    *fi_readFile(file fd)
{
    int size;
    char *res;

    lseek(fd, 0, SEEK_SET);
    size = fi_len(fd);
    res = (char *)malloc((size + 1) * sizeof(char));
    if (!res)
        return (NULL);
    lseek(fd, 0, SEEK_SET);
    read(fd, res, size);
    res[size + 1] = '\0';
    return (res);
}

char    *fi_readLine(file fd)
{
    int size;
    char *res;
    char buf;
    int valid;

    valid = 1;
    size = 0;
    while ((buf != '\n') && valid)
    {
        valid = read(fd, &buf, 1);
        size += valid;
    }
    res = (char *)malloc((size + 1) * sizeof(char));
    if (!res)
        return (NULL);
    lseek(fd, -size, SEEK_CUR);
    read(fd, res, size);
    res[size + 1] = '\0';
    return (res);
}

int fi_moveCursor(file fd, int offset)
{
    int pos;
    pos = lseek(fd, offset, SEEK_CUR);
    return (pos);
}

int fi_getCursor(file fd)
{
    return (lseek(fd, 0, SEEK_CUR));
}

int fi_setCursor(file fd, int offset)
{
    int pos;
    pos = lseek(fd, offset, SEEK_SET);
    return (pos);
}

int fi_writeFile(char *name, char *text)
{
    int fd;
    int len;
    int res;
    fd = _open(name, _O_RDWR | _O_TRUNC);
    len = f_strlen(text);
    res = write(fd, text, len);
    close(fd);
    return (res);
}

int fi_writeLine(file fd, char *text)
{
    int len;
    int res;
    len = f_strlen(text);
    res = write(fd, text, len);
    res += write(fd, "\n", 1);
    return (res);
}

int fi_appendFile(file fd, char *text)
{
    int len;
    int res;
    len = f_strlen(text);
    lseek(fd, 0, SEEK_END);
    res = write(fd, text, len);
    return (res);
}