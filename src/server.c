/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 18:23:09 by kjimenez          #+#    #+#             */
/*   Updated: 2023/07/12 22:07:44 by kjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <ft_string.h>
#include <ft_stdio.h>
#include <stdlib.h>
#include <ft_math.h>
#include <ft_stdlib.h>

static void	append_char(char **str, char c)
{
	size_t	str_len;
	char	terminated_c[2];

	terminated_c[0] = c;
	terminated_c[1] = '\0';
	if (!*str)
	{
		*str = malloc(2);
		if (!*str)
			return ;
		(*str)[0] = '\0';
	}
	else
	{
		str_len = ft_strlen(*str);
		*str = ft_realloc((void **) str, str_len + 1, str_len + 2);
	}
	ft_strcat(*str, terminated_c);
}

static void	print_message(char **str)
{
	ft_printf("Message received from client: %s\n", *str);
	free(*str);
	*str = NULL;
}

static char	binary_to_char(char char_binary[8])
{
	int	i;
	int	dec_char;

	i = 0;
	dec_char = 0;
	while (i < 8)
	{
		if (char_binary[i] == '1')
			dec_char += 1 * ft_pow(2, 7 - i);
		i++;
	}
	return (dec_char);
}

static void	handle_binary(int sig, siginfo_t *info, void *context)
{
	static int	bits;
	static char	*a_str;
	static char	char_binary[8];
	char		c;

	(void) context;
	if (sig == SIGUSR1)
		char_binary[bits] = '1';
	else if (sig == SIGUSR2)
		char_binary[bits] = '0';
	bits++;
	if (bits == 8)
	{
		c = binary_to_char(char_binary);
		if (c == '\0')
			print_message(&a_str);
		else
			append_char(&a_str, c);
		char_binary[0] = '\0';
		bits = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	handle_binary_sig;

	handle_binary_sig.sa_handler = (void *) handle_binary;
	handle_binary_sig.sa_flags = SA_SIGINFO;
	sigemptyset(&handle_binary_sig.sa_mask);
	sigaction(SIGUSR1, &handle_binary_sig, NULL);
	sigaction(SIGUSR2, &handle_binary_sig, NULL);
	ft_printf("Server PID is : %d\n", getpid());
	while (1)
		pause();
}
