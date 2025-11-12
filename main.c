/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokon <mokon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:14:38 by mokon             #+#    #+#             */
/*   Updated: 2025/11/11 17:14:38 by mokon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"

int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;

	(void)argv;
	if (argc != 1)
		return (1);
	if (env == NULL)
	{
		error_exit("Error: wrong env value!\n");
	}
	shell = safe_malloc(sizeof(t_shell));
	value_init(shell);
	create_list_env(&shell->env, env);
	read_input(shell);
	free_env_list(shell->env);
	free(shell);
	return (0);
}
