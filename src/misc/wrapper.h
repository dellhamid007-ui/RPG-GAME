#ifndef WRAPPER_H
#define WRAPPER_H


#include <stdbool.h>

#define MAX_PATH_LEN 260

bool getDocumentsPath(char outPath[MAX_PATH_LEN]);
bool createDirectory(const char *path);


#endif