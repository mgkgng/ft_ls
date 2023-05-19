#include "libft.h"

size_t ft_strlen(const char *s) {
	size_t res = 0;
	while (s[res])
		res++;
	return (res);
}

void ft_puterr(char *s) {
	write(2, s, ft_strlen(s));
}

void *ft_memset(void *b, int c, size_t len) {
	char *p = b;
	while (len-- > 0)
		*p++ = c;
	return (b);
}

void ft_bzero(void *s, size_t n) {
	ft_memset(s, 0, n);
}

void *ft_calloc(size_t count, size_t size) {
	void *res = malloc(count * size);
	if (!res)
		return (NULL);
	ft_bzero(res, count * size);
	return (res);
}

int ft_strcmp(const char *s1, const char *s2) {
	while (*s1 && *s2 && *s1 == *s2) {
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

t_list *ft_lstnew(void *content) {
	t_list	*res = ft_calloc(1, sizeof(t_list));;
	if (!res)
		return (NULL);
	res->content = content;
	return (res);
}

void ft_lstadd_back(t_list **alst, t_list *new_list) {
	if (!*alst) {
		*alst = new_list;
		return ;
	}

	t_list *begin = *alst;
	while (begin->next)
		begin = begin->next;
	begin->next = new_list;
}

void ft_lstsort(t_list **alst) {
	t_list *begin = *alst;
	t_list *tmp = NULL;
	while (begin) {
		tmp = begin->next;
		while (tmp) {
			if (ft_strcmp(begin->content, tmp->content) > 0) {
				void *content = begin->content;
				begin->content = tmp->content;
				tmp->content = content;
			}
			tmp = tmp->next;
		}
		begin = begin->next;
	}
}