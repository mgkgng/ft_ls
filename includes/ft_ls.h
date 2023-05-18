#pragma once

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "libft.h"

#define FLAG_L 1 << 0
#define FLAG_RECUR 1 << 1
#define FLAG_RVRS 1 << 2
#define FLAG_A 1 << 3
#define FLAG_T 1 << 4

typedef struct s_ls {
    int options;
    t_list *files;
} t_ls;

t_ls parse(int ac, char **av);