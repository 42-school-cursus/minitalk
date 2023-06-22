/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:11:38 by kjimenez          #+#    #+#             */
/*   Updated: 2023/06/22 20:49:40 by kjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "ft_stdio.h"
#include "ft_stdlib.h"
#include "ft_string.h"

int	send_null(int pid)
{
	static int	i = 0;

	if (i++ != 8)
	{
		kill(pid, SIGUSR2);
		return (0);
	}
	return (1);
}

static int	send_bit(int server_pid, char *str)
{
	static char	*message = 0;
	static int	bits = -1;

	if (str)
		message = ft_strdup(str);
	if (message[++bits / 8])
	{
		if (message[bits / 8] & (0x80 >> (bits % 8)))
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		return (0);
	}
	if (!send_null(server_pid))
		return (0);
	free(message);
	return (1);
}


static void	interrupt_sleep(int sig, siginfo_t *info, void *context)
{
	int	end;

	(void) sig;
	(void) context;

	end = send_bit(info->si_pid, 0);

	if (!end)
		return ;

	ft_printf("Server successfully received message from client\n");
	exit(EXIT_SUCCESS);
}

/*static void	send_message_to_server(int server_pid, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (j < 8)
		{
			if ((str[i] << j) & 0x80)
				kill(server_pid, SIGUSR1);
			else
				kill(server_pid, SIGUSR2);
			j++;
		}
		i++;
	}
	send_termination_to_server(server_pid);
}*/

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

	send_bit(server_pid, message);
	while (1)
		pause();
	return (0);
}
