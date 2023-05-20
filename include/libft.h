#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct s_list {
    void			*content;
    struct s_list	*next;
}	t_list;

t_list *ft_lstnew(void *content);
void ft_lstadd_back(t_list **alst, t_list *new_list);
void ft_putstr_fd(char *s, int fd);
void ft_putendl(char *s);
void ft_putstr(char *s);
char *ft_strdup(const char *s);
void ft_lstsort(t_list **alst, bool reverse);
char *ft_strjoin(char const *s1, char const *s2);
void ft_putnbr(int n);
size_t ft_strlen(const char *s);