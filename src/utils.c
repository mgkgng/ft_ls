#include "ft_ls.h"

void print_ls_err(char *path) {
    ft_putstr_fd("ft_ls: ", 2);
    ft_putstr_fd(path, 2);
    ft_putstr_fd(": ", 2);
    perror(0);
}

void terminate(t_dir *dir) {
    t_list *tmp = dir->files;
    while (tmp) {
        t_list *next = tmp->next;
        free(FILE(tmp, file));
        free(FILE(tmp, path));
        free(tmp->content);
        free(tmp);
        tmp = next;
    }
    tmp = dir->subdirs;
    while (tmp) {
        t_list *next = tmp->next;
        free(tmp->content);
        free(tmp);
        tmp = next;
    }
    free(dir);
}