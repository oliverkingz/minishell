/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozamora- <ozamora-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:01:51 by ozamora-          #+#    #+#             */
/*   Updated: 2025/03/15 00:37:34 by ozamora-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_echo(t_shell *mini_sh)
{
	char	**args;
	bool	n_flag;
	int		i;

	args = get_cmd_args(mini_sh, "echo");
	i = 1;
	n_flag = false;
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
		(n_flag = true, i++);
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (n_flag == false)
		ft_putchar_fd('\n', STDOUT_FILENO);
	my_free2d((void ***)&args);
}
