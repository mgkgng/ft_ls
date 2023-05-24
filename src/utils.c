#include "ft_ls.h"

int compare_files(void *a, void *b, int flags) {
    t_file *file_a = (t_file *) a;
    t_file *file_b = (t_file *) b;

    if (flags & FLAG_T) {
        int res = (flags & FLAG_REV) ? file_a->stat.st_mtime - file_b->stat.st_mtime : file_b->stat.st_mtime - file_a->stat.st_mtime;
        if (res != 0)
            return (res);
    }
    return (flags & FLAG_REV) ? ft_strcmp(file_b->file, file_a->file) : ft_strcmp(file_a->file, file_b->file);
}

int compare_files_at_parsing(void *a, void *b, int flags) {
    return (flags & FLAG_REV) ? ft_strcmp(b, a) : ft_strcmp(a, b);
}

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
        free(FILE(tmp, file));
        free(tmp->content);
        free(tmp);
        tmp = next;
    }
    free(dir);
}