/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 18:23:09 by kjimenez          #+#    #+#             */
/*   Updated: 2023/06/21 18:28:35 by kjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <ft_string.h>
#include <ft_stdio.h>
#include <stdlib.h>
#include <ft_math.h>
#include <ft_stdlib.h>

//proteger malloc

static char	*btoa(char *str)
{
	char				*a_str;
	unsigned long		substr_start;
	char				*octect;
	int					i;
	int					dec;

	a_str = malloc((ft_strlen(str) / 8) * sizeof(char));
	if (!a_str)
		return (NULL);
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

static void	print_and_flush_str(pid_t pid, char **binary_str)
{
	char		*a_str;

	if (ft_strlen(*binary_str) < 8
		|| ft_strcmp(*binary_str + ft_strlen(*binary_str) - 8, "00000000")
		|| ft_strlen(*binary_str) % 8 == 1)
		return ;

	a_str = btoa(*binary_str);
	ft_printf("%s", a_str);
	free(a_str);
	free(*binary_str);
	*binary_str = NULL;
	kill(pid, SIGUSR2);
}

static void	append_binary(int sig, siginfo_t *info, void *context)
{
	static char	*binary_str;

	(void) sig;
	(void) context;
	if (binary_str == NULL)
	{
		binary_str = malloc(2 * sizeof(char));
		binary_str[0] = '\0';
	}
	else
	{
		size_t len1 = ft_strlen(binary_str) + 1 * sizeof(char);
		size_t len2 = ft_strlen(binary_str) + 2 * sizeof(char);
		ft_printf("Old size: %d New size : %d\n", len1, len2);
		binary_str = ft_realloc((void *) binary_str, len1, len2);
	}
	if (sig == SIGUSR1)
		ft_strcat(binary_str, "1");
	else
		ft_strcat(binary_str, "0");
	print_and_flush_str(info->si_pid, &binary_str);
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	binary1_sig;
	struct sigaction	binary0_sig;

	binary1_sig.sa_handler = (void *) append_binary;
	binary1_sig.sa_flags = SA_SIGINFO;
	sigemptyset(&binary1_sig.sa_mask);
	sigaction(SIGUSR1, &binary1_sig, NULL);
	binary0_sig.sa_handler = (void *) append_binary;
	binary0_sig.sa_flags = SA_SIGINFO;
	sigemptyset(&binary0_sig.sa_mask);
	sigaction(SIGUSR2, &binary0_sig, NULL);
	ft_printf("Server PID is : %d\n", getpid());
	while (1)
		;
}
