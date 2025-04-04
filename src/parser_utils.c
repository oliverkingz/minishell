/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozamora- <ozamora-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 00:34:59 by ozamora-          #+#    #+#             */
/*   Updated: 2025/03/23 00:21:43 by ozamora-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quotes(t_shell *mini_sh, char *current, int i[2], int *type)
{
	(void)mini_sh;
	if (current[i[1]] == '\'' && *type != D_QUOTE)
	{
		if (*type == WORD)
			*type = S_QUOTE;
		else if (*type == S_QUOTE)
			*type = WORD;
	}
	else if (current[i[1]] == '\"' && *type != S_QUOTE)
	{
		if (*type == WORD)
			*type = D_QUOTE;
		else if (*type == D_QUOTE)
			*type = WORD;
	}
}

void	handle_redir(t_shell *mini_sh, char *current, int i[2], int *type)
{
	if ((*type == WORD) && (current[i[1]] == '>' || current[i[1]] == '<'))
	{
		if (i[1] > i[0])
			addback_token(mini_sh, ft_substr(current, i[0], i[1] - i[0]), WORD);
		if (current[i[1]] == '>' && current[i[1] + 1] == '>')
		{
			addback_token(mini_sh, ft_strdup(">>"), REDIR_APP);
			i[0] = i[1] + 2;
			i[1]++;
		}
		else if (current[i[1]] == '<' && current[i[1] + 1] == '<')
		{
			addback_token(mini_sh, ft_strdup("<<"), REDIR_HD);
			i[0] = i[1] + 2;
			i[1]++;
		}
		else if (current[i[1]] == '>')
		{
			addback_token(mini_sh, ft_strdup(">"), REDIR_OUT);
			i[0] = i[1] + 1;
		}
		else if (current[i[1]] == '<')
			(addback_token(mini_sh, ft_strdup("<"), REDIR_IN), i[0] = i[1] + 1);
	}
}

void	handle_pipe_space(t_shell *mini_sh, char *current, int i[2], int *type)
{
	if (current[i[1]] == '|' && *type == WORD)
	{
		if (i[1] > i[0])
			addback_token(mini_sh, ft_substr(current, i[0], i[1] - i[0]), WORD);
		addback_token(mini_sh, ft_strdup("|"), OP_PIPE);
		i[0] = i[1] + 1;
	}
	else if (*type == WORD && ft_isspace(current[i[1]]))
	{
		if (i[1] > i[0])
			addback_token(mini_sh, ft_substr(current, i[0], i[1] - i[0]), WORD);
		i[0] = i[1] + 1;
	}
}

void	rm_chr_pos(char *s, int pos)
{
	int		len;

	if (!s)
		return ;
	len = ft_strlen(s);
	if (pos >= len)
		return ;
	ft_memmove(&s[pos], &s[pos + 1], ft_strlen(&s[pos]));
}

void	rm_external_quotes(char *s)
{
	int		i;
	char	quote;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			quote = s[i];
			rm_chr_pos(s, i);
			while (s[i] && s[i] != quote)
				i++;
			rm_chr_pos(s, i);
			i--;
		}
		i++;
	}
}
