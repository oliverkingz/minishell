/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozamora- <ozamora-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:27:41 by ozamora-          #+#    #+#             */
/*   Updated: 2025/03/15 16:45:53 by ozamora-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*create_shell(char **env)
{
	t_shell	*mini_sh;

	mini_sh = (t_shell *)ft_calloc(1, sizeof(t_shell));
	if (!mini_sh)
		return (my_perr("create mini_sh", false, errno), NULL);
	mini_sh->env = init_envlist(env);
	mini_sh->exit_code = 0;
	mini_sh->input = NULL;
	return (mini_sh);
}

int	process_readline_toinput(t_shell *mini_sh, char **readline)
{
	if (!readline || !*readline)
		return (BREAK_LOOP);
	if (ft_strncmp(*readline, "", -1) == 0)
		return (CONTINUE_LOOP);
	else
		add_history(*readline);
	if (!validate_rline_syntax(*readline))
	{
		(free(*readline), *readline = NULL);
		return (CONTINUE_LOOP);
	}
	mini_sh->input = init_input(mini_sh, *readline);
	if (!mini_sh->input)
	{
		(free(*readline), *readline = NULL);
		return (CONTINUE_LOOP);
	}
	return (OKAY_LOOP);
}

void	execution(t_shell *mini_sh)
{
	t_token		*node;
	t_builtin	is_bi;

	print_tokenslist_short(mini_sh->input->token_lst);
	node = mini_sh->input->token_lst;
	is_bi = check_if_bi(node);
	if (is_bi != BI_NO)
		mini_sh->exit_code = exec_bi(mini_sh, node, is_bi);
	//execute_cmds(mini_sh);
}

int	loop_shell(t_shell *mini_sh)
{
	char	*read_line;
	int		result;

	set_signals();
	while (1)
	{
		read_line = readline(PROMPT);
		set_signal_errors(mini_sh);
		result = process_readline_toinput(mini_sh, &read_line);
		if (result == BREAK_LOOP)
			break ;
		else if (result == CONTINUE_LOOP)
			continue ;
		execution(mini_sh);
		(free(read_line), free_input(&mini_sh));
	}
	return (mini_sh->exit_code);
}

void	free_shell(t_shell *mini_sh)
{
	if (!mini_sh)
		return ;
	if (mini_sh->input)
		free_input(&mini_sh);
	clear_envlist(&(mini_sh->env));
	free(mini_sh);
}
