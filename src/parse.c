#include "ft_ls.h"

static int get_options(char *flag_str) {
    int res = 0;

    for (int i = 0; flag_str[i]; i++) {
        if (flag_str[i] == 'l')
            res |= FLAG_L;
        else if (flag_str[i] == 'R')
            res |= FLAG_RECUR;
        else if (flag_str[i] == 'r')
            res |= FLAG_REV;
        else if (flag_str[i] == 'a')
            res |= FLAG_A;
        else if (flag_str[i] == 't')
            res |= FLAG_T;
        else {
            ft_putstr_fd("ft_ls: invalid option -- ", 2);
            ft_putstr_fd(flag_str + i, 2);
            ft_putstr_fd("\n", 2);
            exit(EXIT_FAILURE);
        }
    }
    return (res);
}

/** Should admit that it wasn't the best way to run stat two times,
 * once at the parsing and once while running the ls command. **/
t_args parse(int ac, char **av) {
    t_args res = {0};

    int i = 1;
    while (i < ac) {
        if (av[i][0] == '-' && av[i][1] != '\0')
            res.options |= get_options(av[i++] + 1);
        else {
            if (av[i][ft_strlen(av[i]) - 1] == '/')
                av[i][ft_strlen(av[i]) - 1] = '\0';
            t_file *file = ft_calloc(1, sizeof(t_file));
            struct stat statbuf;
            if (stat(av[i], &statbuf) == -1) {
                file->file = av[i++];
                file->stat = statbuf;
                ft_lstadd_back(&res.files, ft_lstnew(file));
                continue ;
            }
            file->file = av[i++];
            file->stat = statbuf;
            ft_lstadd_back((S_ISDIR(statbuf.st_mode)) ? &res.dirs : &res.files, ft_lstnew(file));
        }
    }
    ft_lstsort(&res.files, res.options, compare_files);
    ft_lstsort(&res.dirs, res.options, compare_files);
    if (!res.files && !res.dirs)
        res.dirs = ft_lstnew(".");
    res.n_dirs = ft_lstsize(res.dirs);
    return (res);
}