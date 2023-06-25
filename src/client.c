/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:11:38 by kjimenez          #+#    #+#             */
/*   Updated: 2023/06/25 19:41:01 by kjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "ft_stdio.h"
#include "ft_stdlib.h"
#include "ft_string.h"

int	send_null_terminator(int pid)
{
	static int	i;

	if (i == 8)
		return (1);
	kill(pid, SIGUSR2);
	i++;
	return (0);
}

char	*str_to_binary(char *str)
{
	char	*binary_str;
	int		i;
	int		j;

	i = 0;
	binary_str = malloc((ft_strlen(str) * 8) + 1);
	if (!binary_str)
		return (NULL);
	binary_str[ft_strlen(str) * 8] = '\0';
	while (str[i])
	{
		j = 7;
		while (j >= 0)
		{
			if (str[i] & (1 << j))
				binary_str[(i * 8) + (7 - j)] = '1';
			else
				binary_str[(i * 8) + (7 - j)] = '0';
			j--;
		}
		i++;
	}
	return (binary_str);
}

static int	send_next_bit(int server_pid, char *str)
{
	static char		*binary_str;
	static size_t	binary_str_len;
	static size_t	bits = -1;

	if (!binary_str)
	{
		binary_str = str_to_binary(str);
		binary_str_len = ft_strlen(binary_str);
	}
	bits++;
	if (bits < binary_str_len)
	{
		if (binary_str[bits] == '1')
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		return (0);
	}
	if (!send_null_terminator(server_pid))
		return (0);
	free(binary_str);
	return (1);
}

static void	interrupt_sleep(int sig, siginfo_t *info, void *context)
{
	int	end;

	(void) sig;
	(void) context;
	end = send_next_bit(info->si_pid, 0);
	if (!end)
		return ;
	ft_printf("Server successfully received message from client\n");
	exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	int					server_pid;
	char				*message;
	struct sigaction	allow_signals_sig;

	if (argc < 3)
	{
		ft_printf("Invalid syntax. Please use %s <server PID> <message>\n",
			argv[0]);
		return (0);
	}
	server_pid = ft_atoi(argv[1]);
	message = argv[2];
	allow_signals_sig.sa_handler = (void *) interrupt_sleep;
	allow_signals_sig.sa_flags = SA_SIGINFO;
	sigemptyset(&allow_signals_sig.sa_mask);
	sigaction(SIGUSR1, &allow_signals_sig, NULL);
	send_next_bit(server_pid, message);
	while (1)
		pause();
}
