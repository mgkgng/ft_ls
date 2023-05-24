#include "ft_ls.h"

t_args args;

int main(int ac, char **av) {
    args = parse(ac, av);    
    
    if (args.files)
        ft_ls_files(args.files);
    t_list *curr = args.dirs;
    while (curr) {
        t_list *next = curr->next;
        if (args.n_dirs > 1 || args.files) {
            ft_putendl("");
            ft_putstr(FILE(curr, file));
            ft_putendl(":");
        }
        ft_ls_dirs(FILE(curr, file), NULL);
        free(curr->content);
        free(curr);
        curr = next;
    }
    return 0;
}
