/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 18:23:09 by kjimenez          #+#    #+#             */
/*   Updated: 2023/06/23 01:05:42 by kjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <ft_string.h>
#include <ft_stdio.h>
#include <stdlib.h>
#include <ft_math.h>
#include <ft_stdlib.h>

//proteger malloc
//relink makefile

static void	append_char(char **str, char c)
{
	size_t	str_len;

	if (!*str)
	{
		*str = malloc(2);
		(*str)[0] = '\0';
	}
	else
	{
		str_len = ft_strlen(*str);
		*str = ft_realloc((void **) str, str_len + 1, str_len + 2);
	}
	ft_strcat(*str, &c);
}

static void	print_message(char **str)
{
	ft_printf("Message received from client : %s\n", *str);
	free(*str);
	*str = NULL;
}

static void	handle_binary(int sig, siginfo_t *info, void *context)
{
	static char	c = (char) 255;
	static int	bits;
	static char	*a_str;

	(void) context;
	if (sig == SIGUSR1)
		c |= 0x80 >> bits;
	else if (sig == SIGUSR2)
		c ^= 0x80 >> bits;
	bits++;
	if (bits == 8)
	{
		if (c == '\0')
			print_message(&a_str);
		else
		{
			append_char(&a_str, c);
		}
		bits = 0;
		c = (char) 255;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	binary1_sig;
	struct sigaction	binary0_sig;

	binary1_sig.sa_handler = (void *) handle_binary;
	binary1_sig.sa_flags = SA_SIGINFO;
	sigemptyset(&binary1_sig.sa_mask);
	sigaction(SIGUSR1, &binary1_sig, NULL);
	binary0_sig.sa_handler = (void *) handle_binary;
	binary0_sig.sa_flags = SA_SIGINFO;
	sigemptyset(&binary0_sig.sa_mask);
	sigaction(SIGUSR2, &binary0_sig, NULL);
	ft_printf("Server PID is : %d\n", getpid());
	while (1)
		pause();
}
