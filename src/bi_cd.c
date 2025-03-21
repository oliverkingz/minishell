/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozamora- <ozamora-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 23:10:58 by ozamora-          #+#    #+#             */
/*   Updated: 2025/03/21 17:35:57 by ozamora-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(char *route)
{
	struct stat	info;

	stat(route, &info);
	return (S_ISDIR(info.st_mode));
}

char	*cd_set_path(t_shell *mini_sh, t_cmd *cmd)
{
	char	*path;

	if (cmd->cmd_argc == 1)
	{
		path = my_getenv(mini_sh->env, "HOME");
		if (!path)
			return (ft_puterr("cd: HOME not set\n"), NULL);
	}
	else if (ft_strncmp(cmd->cmd_args[1], "-", -1) == 0)
	{
		path = my_getenv(mini_sh->env, "OLDPWD");
		if (!path)
			return (ft_puterr("cd: OLDPWD not set\n"), NULL);
		printf("%s\n", path);
	}
	else if (cmd->cmd_argc > 2)
		return (ft_puterr("cd: too many argumets\n"), NULL);
	else
		path = cmd->cmd_args[1];
	return (path);
}

void	expand_tilde(t_shell *mini_sh, char **og_path)
{
	char	*var;
	char	*temp;
	char	*home;
	char	*path;

	temp = *og_path;
	var = ft_strdup("~");
	home = ft_strdup(my_getenv(mini_sh->env, "HOME"));
	path = my_replace_first(temp, var, home);
	my_free((void **)&temp);
	my_free((void **)&var);
	my_free((void **)&home);
	*og_path = path;
	my_free((void **)&temp);
}

int	change_update_dir(t_shell *mini_sh, char *path)
{
	int		exit_code;
	char	cwd[PATH_MAX];
	char	*oldpwd;
	char	*pwd;

	oldpwd = NULL;
	pwd = NULL;
	exit_code = EXIT_SUCCESS;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		oldpwd = ft_strjoin("OLDPWD=", cwd);
	else
		(perror("cd: getcwd"), exit_code = EXIT_FAILURE);
	if (chdir(path) < 0)
		(perror("cd"), exit_code = EXIT_FAILURE);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		register_var(mini_sh, oldpwd);
		pwd = ft_strjoin("PWD=", cwd);
		register_var(mini_sh, pwd);
	}
	else
		(perror("cd: getcwd"), exit_code = EXIT_FAILURE);
	return (my_free((void **)&oldpwd), my_free((void **)&pwd), exit_code);
}

int	bi_cd(t_shell *mini_sh, t_cmd *cmd)
{
	int		exit_code;
	char	*path;

	exit_code = EXIT_SUCCESS;
	path = ft_strdup(cd_set_path(mini_sh, cmd));
	if (!path)
		return (EXIT_FAILURE);
	if (path[0] == '~')
		expand_tilde(mini_sh, &path);
	exit_code = change_update_dir(mini_sh, path);
	return (my_free((void **)&path), exit_code);
}
