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
        return 1;
    }

    if (!S_ISDIR(statbuf.st_mode)) {
        printf("%s is not a directory\n", path);
        return ;
    }

    printf("%s is a directory\n", path);
    DIR *dir = opendir(path);
    if (!dir) {
        perror("ft_ls");
        return ;
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
