#include "ft_ls.h"

t_args args;

int main(int ac, char **av) {
    args = parse(ac, av);    
    
    t_list *begin = args.files;
    while (begin) {
        ft_ls(begin->content, NULL);
        begin = begin->next;
    }
    return 0;
}
