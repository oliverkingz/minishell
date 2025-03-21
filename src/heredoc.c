/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raperez- <raperez-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:03:26 by raperez-          #+#    #+#             */
/*   Updated: 2025/03/21 20:49:58 by raperez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_hdoc_file(int id)
{
	char	*num;
	char	*file;
	int		fd;

	num = ft_itoa(id);
	file = ft_strjoin("hdoc_minish", num);
	fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
		perror(file);
	my_free((void **)&num);
	my_free((void **)&file);
	return (fd);
}

void	hdoc_child(t_shell *mini_sh, char *limiter, int id)
{
	int		fd;
	char	*line;
	pid_t	pid;

	pid = fork();
	if (pid != 0)
		return ;
	rl_catch_signals = 0;
	signal(SIGINT, hdoc_child_ctrl_c);
	fd = create_hdoc_file(id);
	while (g_signal != SIGINT)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd(ERR_LIM, 2);
			break ;
		}
		else if (ft_strncmp(limiter, line, -1) == 0)
			break ;
		else
			ft_putendl_fd(line, fd);
		my_free((void **)&line);
	}
	(free_shell(mini_sh), my_close(&fd), exit(0));
}

void	handle_heredocs(t_shell *mini_sh)
{
	int		id;
	t_token	*node;

	id = 0;
	node = mini_sh->input->token_lst;
	signal(SIGINT, hdoc_parent_ctrl_c);
	while (node && g_signal != SIGINT)
	{
		if (node->type == REDIR_HD)
		{
			hdoc_child(mini_sh, node->next->content, id++);
			wait(NULL);
		}
		node = node->next;
	}
	signal(SIGINT, handle_ctrl_c);
}

void	rm_hdoc_files(t_shell *mini_sh)
{
	char	*file;
	char	*num;
	char	*err;
	int		i;

	i = 0;
	while (i < mini_sh->input->hdoc_count)
	{
		num = ft_itoa(i);
		file = ft_strjoin("hdoc_minish", num);
		if (unlink(file) == -1 && g_signal != SIGINT)
		{
			err = ft_strjoin("Error deleting: ", file);
			perror(err);
			my_free((void **)&err);
			mini_sh->exit_code = 1;
		}
		my_free((void **)&num);
		my_free((void **)&file);
		i++;
	}
}
