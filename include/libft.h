#pragma once

#include <stdlib.h>
#include <unistd.h>

typedef struct s_list {
    void			*content;
    struct s_list	*next;
}	t_list;

t_list *ft_lstnew(void *content);
void ft_lstadd_back(t_list **alst, t_list *new_list);
void ft_puterr(char *s);
void ft_putendl(char *s);
void ft_putstr(char *s);
void ft_puttab(char *s);
char *ft_strdup(const char *s);
void ft_lstsort(t_list **alst);
char *ft_strjoin(char const *s1, char const *s2);