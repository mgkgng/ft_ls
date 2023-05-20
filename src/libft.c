#include "libft.h"

size_t ft_strlen(const char *s) {
	size_t res = 0;
	while (s[res])
		res++;
	return (res);
}

void ft_putstr(char *s) {
	write(1, s, ft_strlen(s));
}

void ft_puttab(char *s) {
	write(1, s, ft_strlen(s));
	write(1, "\t", 1);
}

void ft_puterr(char *s) {
	write(2, s, ft_strlen(s));
}

void ft_putendl(char *s) {
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
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

char *ft_strjoin(char const *s1, char const *s2) {
	char *res = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	if (!res)
		return (NULL);
	int i = 0;
	while (*s1)
		res[i++] = *s1++;
	while (*s2)
		res[i++] = *s2++;
	return (res);
}

char *ft_strdup(const char *s) {
	char *res = ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!res)
		return (NULL);
	for (int i = 0; s[i]; i++)
		res[i] = s[i];
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