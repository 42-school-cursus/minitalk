/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 18:23:09 by kjimenez          #+#    #+#             */
/*   Updated: 2023/06/20 18:26:16 by kjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <ft_string.h>
#include <stdlib.h>
#include <ft_math.h>

char	*g_binary_str;

static int	ft_substr_len(char const *s, unsigned int start, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len && s[start + i])
		i++;
	return (i);
}

static char	*substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	malloc_len;
	char	*sub_str;

	if (!s || start >= strlen(s))
		return (strdup(""));
	malloc_len = ft_substr_len(s, start, len);
	sub_str = (char *) malloc((malloc_len + 1) * sizeof(char));
	if (!sub_str)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		sub_str[i] = s[start + i];
		i++;
	}
	sub_str[i] = '\0';
	return (sub_str);
}

void	*ft_realloc(void *ptr, size_t newsize)
{
	char	*newptr;
	size_t	cursize;

	if (ptr == 0)
		return (malloc(newsize));
	cursize = sizeof(ptr);
	if (newsize <= cursize)
		return (ptr);
	newptr = malloc(newsize);
	memcpy(ptr, newptr, cursize);
	free(ptr);
	return (newptr);
}

static char	*btoa(char *str)
{
	char				*a_str;
	unsigned long		substr_start;
	char				*octect;
	int					i;
	int					dec;

	a_str = malloc((strlen(str) / 8) * sizeof(char));
	a_str[0] = '\0';
	substr_start = 0;
	while (substr_start < strlen(str))
	{
		octect = substr(str, substr_start, 8);
		dec = 0;
		i = 0;
		while (i < 8)
		{
			if (octect[i] == '1')
				dec += 1 * ft_pow(2, 8 - 1 - i);
			i++;
		}
		strcat(a_str, (char *) &dec);
		substr_start += 8;
		free(octect);
	}
	return (a_str);
}

static void	handle_binary_1(int sig, siginfo_t *info, void *context)
{
	(void) sig;
	(void) context;
	if (g_binary_str == NULL)
	{
		g_binary_str = malloc(2 * sizeof(char));
		g_binary_str[0] = '\0';
	}
	else
	{
		g_binary_str = realloc(g_binary_str,
				strlen(g_binary_str) + 2 * sizeof(char));
	}
	strcat(g_binary_str, "1\0");
	kill(info->si_pid, SIGUSR1);
}

static void	handle_binary_0(int sig, siginfo_t *info, void *context)
{
	char	*a_str;

	(void) sig;
	(void) context;
	if (g_binary_str == NULL)
	{
		g_binary_str = malloc(2 * sizeof(char));
		g_binary_str[0] = '\0';
	}
	else
		g_binary_str = realloc(g_binary_str,
				strlen(g_binary_str) + 2 * sizeof(char));
	strcat(g_binary_str, "0\0");
	if (strlen(g_binary_str) >= 8
		&& !strcmp(g_binary_str + strlen(g_binary_str) - 8, "00000000")
		&& strlen(g_binary_str) % 8 == 0)
	{
		a_str = btoa(g_binary_str);
		printf("Message received from client: %s\n", a_str);
		free(a_str);
		free(g_binary_str);
		g_binary_str = NULL;
		kill(info->si_pid, SIGUSR2);
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	binary1_sig;
	struct sigaction	binary0_sig;

	binary1_sig.sa_handler = (void *) handle_binary_1;
	binary1_sig.sa_flags = SA_SIGINFO;
	sigemptyset(&binary1_sig.sa_mask);
	sigaction(SIGUSR1, &binary1_sig, NULL);
	binary0_sig.sa_handler = (void *) handle_binary_0;
	binary0_sig.sa_flags = SA_SIGINFO;
	sigemptyset(&binary0_sig.sa_mask);
	sigaction(SIGUSR2, &binary0_sig, NULL);
	printf("Server PID is : %d\n", getpid());
	while (1)
		;
}
