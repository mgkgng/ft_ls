#include "ft_ls.h"

t_ls args;

void print_ls_err(char *path) {
    ft_puterr("ft_ls: ");
    ft_puterr(path);
    ft_puterr(": ");
    perror(0);
}

void ft_ls(char *path) {
    struct stat statbuf;

    if (lstat(path, &statbuf) == -1) {
        print_ls_err(path);
        return ;
    }

    if (!S_ISDIR(statbuf.st_mode)) {
        printf("%s is not a directory\n", path);
        // Should get information
        return ;
    }

    DIR *dir = opendir(path);
    if (!dir) {
        perror("ft_ls");
        return ;
    }
    struct dirent *entry;
    t_list *files = NULL;
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.' && !(args.options & FLAG_A))
            continue ;
        ft_lstadd_back(&files, ft_lstnew(ft_strdup(entry->d_name)));
    }
    closedir(dir);
    ft_lstsort(&files);
    while (files) {
        ft_putendl(files->content);
        files = files->next;
    }

    // if recursive mode, call ft_ls on this directory
    // if (args.options & FLAG_RECUR) {
        // ft_ls(path, args);
    // }
}



int main(int ac, char **av) {
    args = parse(ac, av);
    
    t_list *begin = args.files;
    while (begin) {
        ft_ls(begin->content);
        begin = begin->next;
    }
    return 0;
}
