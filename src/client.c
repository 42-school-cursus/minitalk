/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:11:38 by kjimenez          #+#    #+#             */
/*   Updated: 2023/06/20 18:16:46 by kjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "ft_stdio.h"
#include "ft_stdlib.h"

static void	interrupt_sleep(int sig)
{
	(void) sig;
}

static void	notify_message_received(int sig)
{
	(void) sig;
	ft_printf("Server successfully received message from client\n");
	exit(0);
}

static void	send_termination_to_server(int server_pid)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		kill(server_pid, SIGUSR2);
		usleep(100);
		i++;
	}
}

static void	send_message_to_server(int server_pid, char *str)
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
			usleep(100);
			j++;
		}
		i++;
	}
	send_termination_to_server(server_pid);
}

int	main(int argc, char *argv[])
{
	int					server_pid;
	char				*message;
	struct sigaction	allow_signals_sig;
	struct sigaction	notify_message_received_sig;

	if (argc < 3)
	{
		ft_printf("Invalid syntax. Please use %s <server PID> <message>\n",
			argv[0]);
		return (0);
	}
	server_pid = ft_atoi(argv[1]);
	message = argv[2];
	allow_signals_sig.sa_handler = interrupt_sleep;
	allow_signals_sig.sa_flags = 0;
	sigemptyset(&allow_signals_sig.sa_mask);
	sigaction(SIGUSR1, &allow_signals_sig, NULL);
	notify_message_received_sig.sa_handler = notify_message_received;
	notify_message_received_sig.sa_flags = 0;
	sigemptyset(&notify_message_received_sig.sa_mask);
	sigaction(SIGUSR2, &notify_message_received_sig, NULL);
	send_message_to_server(server_pid, message);
	while (1)
		;
	return (0);
}
