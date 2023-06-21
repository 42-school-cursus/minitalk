/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 18:23:09 by kjimenez          #+#    #+#             */
/*   Updated: 2023/06/21 17:07:15 by kjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <ft_string.h>
#include <ft_stdio.h>
#include <stdlib.h>
#include <ft_math.h>

char	*g_binary_str;

//proteger malloc

static char	*btoa(char *str)
{
	char				*a_str;
	unsigned long		substr_start;
	char				*octect;
	int					i;
	int					dec;

	a_str = malloc((ft_strlen(str) / 8) * sizeof(char));
	a_str[0] = '\0';
	substr_start = 0;
	while (substr_start < ft_strlen(str))
	{
		octect = ft_substr(str, substr_start, 8);
		dec = 0;
		i = 0;
		while (i < 8)
		{
			if (octect[i] == '1')
				dec += 1 * ft_pow(2, 8 - 1 - i);
			i++;
		}
		ft_strcat(a_str, (char *) &dec);
		substr_start += 8;
		free(octect);
	}
	return (a_str);
}

void	*ft_realloc(void *ptr, size_t old_size,
		size_t new_size)
{
	void	*new_ptr;

	if (new_size == old_size)
		return (ptr);
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, old_size);
	free(ptr);
	ptr = new_ptr;
	return (ptr);
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
		size_t len1 = ft_strlen(g_binary_str) + 1 * sizeof(char);
		size_t len2 = ft_strlen(g_binary_str) + 2 * sizeof(char);
		g_binary_str = ft_realloc((void *) g_binary_str, len1, len2);
	}
	ft_strcat(g_binary_str, "1");
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
	{
		size_t len1 = ft_strlen(g_binary_str) + 1 * sizeof(char);
		size_t len2 = ft_strlen(g_binary_str) + 2 * sizeof(char);
		g_binary_str = ft_realloc((void *) g_binary_str, len1, len2);
	}
	ft_strcat(g_binary_str, "0");
	if (ft_strlen(g_binary_str) >= 8
		&& !ft_strcmp(g_binary_str + ft_strlen(g_binary_str) - 8, "00000000")
		&& ft_strlen(g_binary_str) % 8 == 0)
	{
		a_str = btoa(g_binary_str);
		ft_printf("Message received from client: %s\n", a_str);
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
	ft_printf("Server PID is : %d\n", getpid());
	while (1)
		;
}
