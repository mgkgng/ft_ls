#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define FLAG_L 1 << 0
#define FLAG_RECUR 1 << 1
#define FLAG_REV 1 << 2
#define FLAG_A 1 << 3
#define FLAG_T 1 << 4


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
void ft_lstsort(t_list **alst, int flags, int (*cmp)(void *, void *, int));
char *ft_strjoin(char const *s1, char const *s2);
void ft_putnbr(int n);
size_t ft_strlen(const char *s);
char *ft_lltoa(long long n);
void ft_strcpy(char *dst, const char *src);
int ft_strcmp(const char *s1, const char *s2);
void ft_putchar(char c);
int ft_numlen(long long n);