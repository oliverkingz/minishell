/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozamora- <ozamora-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:27:44 by ozamora-          #+#    #+#             */
/*   Updated: 2025/03/14 23:58:36 by ozamora-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	my_perr(const char *msg, bool should_exit, int exit_status)
{
	perror(msg);
	if (should_exit)
		exit(exit_status);
}

void	my_free(void **mem)
{
	if (*mem)
	{
		free(*mem);
		*mem = NULL;
	}
}

void	my_free2d(void ***mem)
{
	int	i;

	if (!mem || !*mem)
		return;
	i = 0;
	while ((*mem)[i])
	{
		if ((*mem)[i] != NULL)
		{
			free((*mem)[i]);
			(*mem)[i] = NULL;
		}
		i++;
	}
	free(*mem);
	*mem = NULL;
}

size_t	my_strlen_word(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && (ft_isalnum(s[i]) || ft_strchr("_", s[i])))
		i++;
	return (i);
}
