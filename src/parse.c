#include "ft_ls.h"

static int get_options(char *flag_str) {
    int res = 0;

    for (int i = 0; flag_str[i]; i++) {
        if (flag_str[i] == 'l')
            res |= FLAG_L;
        else if (flag_str[i] == 'R')
            res |= FLAG_RECUR;
        else if (flag_str[i] == 'r')
            res |= FLAG_RVRS;
        else if (flag_str[i] == 'a')
            res |= FLAG_A;
        else if (flag_str[i] == 't')
            res |= FLAG_T;
        else {
            perror("ft_ls: invalid option -- ");
            exit(EXIT_FAILURE);
        }
    }
    return (res);
}

t_ls parse(int ac, char **av) {
    t_ls res = {0};

    int i = 1;
    while (i < ac && av[i][0] == '-' && av[i][1] != '\0')
        res.options |= get_options(av[i++] + 1);
    while (i < ac)
        ft_lstadd_back(&res.files, ft_lstnew(av[i++]));
    if (!res.files)
        res.files = ft_lstnew(".");
    return (res);
}