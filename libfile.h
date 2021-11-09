#ifndef LIBFILE_H
#define LIBFILE_H

typedef int file;

file    fi_openRFile(char *path);
file    fi_openWFile(char *path);
file    fi_openRWFile(char *path);
int fi_len(file fd);
char *fi_readFile(file fd);
char *fi_readLine(file fd);
int fi_moveCursor(file fd, int offset);
int fi_getCursor(file fd);
int fi_setCursor(file fd, int offset);
int fi_writeFile(char *name, char *text);
int fi_writeLine(file fd, char *text);
int fi_appendFile(file fd, char *text);
void    fi_closeFile(file fd);

#endif