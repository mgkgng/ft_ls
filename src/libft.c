#include "libft.h"

void	*ft_memset(void *b, int c, size_t len) {
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

t_list *ft_lstnew(void *content) {
	t_list	*res = ft_calloc(1, sizeof(t_list));;
	if (!res)
		return (NULL);
	res->content = content;
	return (res);
}

void ft_lstadd_back(t_list **alst, t_list *new) {
	if (!*alst) {
		*alst = new;
		return ;
	}

	t_list *begin = *alst;
	while (begin->next)
		begin = begin->next;
	begin->next = new;
}