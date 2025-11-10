/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:16:41 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/09/08 14:28:10 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

void	sig_handler(int signal)
{
	(void)signal;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	signals_things(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sigemptyset(&sig_int.sa_mask);
	sig_int.sa_flags = SA_RESTART;
	sig_int.sa_handler = sig_handler;
	sigaction(SIGINT, &sig_int, NULL);
	sigemptyset(&sig_quit.sa_mask);
	sig_quit.sa_flags = 0;
	sig_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sig_quit, NULL);
}
