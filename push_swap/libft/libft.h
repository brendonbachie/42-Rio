/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgomes-b <bgomes-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:13:32 by bgomes-b          #+#    #+#             */
/*   Updated: 2026/01/29 22:01:46 by bgomes-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <limits.h>

typedef struct s_list
{
	struct s_list	*next;
	void			*content;
}	t_list;

long	ft_atol(const char *nptr);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putendl_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
void	*ft_memchr(const void *s, int c, size_t n);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(const char *s, int c);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strtrim(const char *s1, const char *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strrchr(const char *s, int c);
char	*ft_strdup(char *s, char *s2);
char	*ft_strjoin(char **s1, char const *s2);
size_t	ft_strlen(const char *s);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
int		ft_isalnum(int c);
int		ft_atoi(const char *nptr);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_tolower(int c);
int		ft_toupper(int c);
int		ft_isprint(int c);
int		ft_isdigit(int c);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(const char *s1, const char *s2);

#endif
