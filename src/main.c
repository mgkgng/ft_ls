#include "ft_ls.h"

t_args args;

int main(int ac, char **av) {
    args = parse(ac, av);    
    
    if (args.files)
        ft_ls_files(args.files);
    t_list *curr = args.dirs;
    while (curr) {
        if (args.n_dirs > 1 || args.files) {
            ft_putendl("");
            ft_putstr(curr->content);
            ft_putendl(":");
        }
        ft_ls_dirs(curr->content, NULL);
        curr = curr->next;
    }
    return 0;
}
