#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include <time.h>

#include "libft.h"

#define FLAG_L 1 << 0
#define FLAG_RECUR 1 << 1
#define FLAG_REV 1 << 2
#define FLAG_A 1 << 3
#define FLAG_T 1 << 4

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define FILE(x, y) ((t_file *)(x->content))->y

typedef struct s_args {
    int options;
    t_list *files;
} t_args;

typedef struct s_file {
    char *file;
    char *path;
    struct stat stat;
} t_file;

typedef struct s_dir {
    char *path;
    int max_len_size;
    int max_len_links;
    int total;
    t_list *files;
    t_list *subdirs;
} t_dir;

extern t_args args;

t_args parse(int ac, char **av);
void ft_ls(char *path, char *file);
void print_long_format(struct stat statbuf, int max_len_links, int max_len_size);
void terminate(t_dir *dir);
void print_ls_err(char *path);