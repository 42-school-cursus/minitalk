/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kjimenez <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 18:23:09 by kjimenez          #+#    #+#             */
/*   Updated: 2023/06/23 00:51:21 by kjimenez         ###   ########.fr       */
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

char	*ft_straddc_first(char c)
{
	char	*add;

	add = (char *)malloc(sizeof(char) * 2);
	if (!add)
		return (NULL);
	add[0] = c;
	add[1] = '\0';
	return (add);
}

char	*ft_straddc(char *str, char c)
{
	char	*add;
	int		i;

	if (!c)
		return (NULL);
	if (!str)
		return (ft_straddc_first(c));
	add = (char *)malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!add)
	{
		free(str);
		return (NULL);
	}
	i = -1;
	while (str[++i])
		add[i] = str[i];
	free(str);
	add[i++] = c;
	add[i] = '\0';
	return (add);
}

static void	print_message(char **message)
{
	ft_printf("Message received from client : %s\n", *message);
	free(*message);
	*message = NULL;
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
			if (a_str == NULL)
			{
				a_str = malloc(2);
				a_str[0] = '\0';
			}
			else
				a_str = ft_realloc((void **) &a_str, ft_strlen(a_str) + 1, ft_strlen(a_str) + 2);
			ft_strcat(a_str, &c);
			ft_printf("New version\n");
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
