/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozamora- <ozamora-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 23:03:32 by ozamora-          #+#    #+#             */
/*   Updated: 2025/03/27 23:29:10 by ozamora-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                              STANDARD INCLUDES                             */
/* ************************************************************************** */

// Standard libraries and system headers required for Minishell functionality.
# include "libft.h"

/* Includes already at libft.h
# include <stdio.h>	// printf, perror
# include <stdlib.h>	// malloc, free, exit
# include <unistd.h>	// write, access, open, read, close, fork, getcwd, chdir
						// unlink, execve, dup, dup2, pipe
						// isatty, ttyname, ttyslot, getenv*/

# include <errno.h>				// errno
# include <linux/limits.h>		// PATH_MAX
# include <readline/history.h>	// add_history
# include <readline/readline.h>	// readline, rl_clear_history
# include <string.h>			// strerror
								// rl_on_new_line, rl_replace_line, rl_redisplay

# include <curses.h>	// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <dirent.h>	// opendir, readdir, closedir
# include <signal.h>	// signal, sigaction, kill
# include <sys/ioctl.h>	// ioctl
# include <sys/stat.h>	// stat, lstat, fstat
# include <sys/wait.h>	// wait, waitpid, wait3, wait4
# include <termios.h>	// tcsetattr, tcgetattr

/* ************************************************************************** */
/*                              MACROS AND DEFINES                            */
/* ************************************************************************** */

// Shell prompt and file descriptor macros
# define PROMPT "\001\033[1;32m\002Minishell > \001\033[0m\002"

# define READ_END 0
# define WRITE_END 1

# define STDIN_FILENO 0
# define STDOUT_FILENO 1
# define STDERR_FILENO 2

// Shell input loop control macros
# define BREAK_LOOP 1
# define CONTINUE_LOOP 2
# define OKAY_LOOP 3

// Error messages
# define ERR_SYNTAX "syntax error\n"
# define ERR_SYNTAX_NL "syntax error near unexpected token `newline'\n"
# define ERR_SYNTAX_PP "syntax error near unexpected token `|'\n"
# define ERR_COMMAND ": command not found\n"
# define ERR_ARGS ": too many arguments\n"
# define ERR_ARG_INT ": numeric argument required\n"
# define ERR_ID ": not a valid identifier\n"
# define ERR_PERMISSION ": permission denied\n"
# define ERR_FILE_DIR ": no such file or directory\n"

// Warning messages
# define WARNING_LIM "warning: here-document delimited by end-of-file\n"

/* ************************************************************************** */
/*                              GLOBAL VARIABLES                              */
/* ************************************************************************** */

// Global signal variable
extern int			g_signal;

/* ************************************************************************** */
/*                              ENUMS AND STRUCTS                             */
/* ************************************************************************** */

// Enum for built-in commands
typedef enum e_builtin
{
	NO_BI,
	BI_ECHO,
	BI_PWD,
	BI_ENV,
	BI_CD,
	BI_EXPORT,
	BI_UNSET,
	BI_EXIT
}					t_builtin;

// Enum for token types
typedef enum e_type
{
	WORD,
	COMMAND,
	CMD_ARG,
	OP_PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_HD,
	REDIR_APP,
	FILE_PATH,
	LIMITER,
	S_QUOTE,
	D_QUOTE
}					t_type;

// Token node struccture in a linked list
typedef struct s_token
{
	char			*content;
	t_type			type;
	int				index;
	bool			had_quotes;
	struct s_token	*next;
}					t_token;

// Command structure
typedef struct s_cmd
{
	int				in_fd;
	int				out_fd;
	int				close_fd;
	char			*cmd_path;
	char			**cmd_args;
	int				cmd_argc;
	char			**env;
	t_builtin		is_bi;
}					t_cmd;

// Input structure
typedef struct s_input
{
	char			*read_line;
	t_token			*token_lst;
	int				pipe_count;
	int				hdoc_count;
	int				hdoc_used;
	pid_t			*pid;
}					t_input;

// Environment variable node struccture in a linked list
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

// Minishell main structure
typedef struct s_shell
{
	t_input			*input;
	t_env			*env;
	int				exit_code;
}					t_shell;

/* ************************************************************************** */
/*                              FUNCTION DECLARATIONS                         */
/* ************************************************************************** */

/* ----------------------------- Core Functions ---------------------------- */
// Core shell functions (shell.c)

t_shell	*create_shell(char **env);
int		handle_readline_input(t_shell *mini_sh, char **readline);
int		loop_shell(t_shell *mini_sh);
int		loop_shell_non_interactive(t_shell *mini_sh);
void	free_shell(t_shell *mini_sh);

// Input handling functions (input.c)

t_input	*init_input(t_shell *mini_sh, char *read_line);
void	init_input_pid_heredoc(t_shell *mini_sh);
void	free_input(t_shell **mini_sh);

/* ----------------------------- Parsing and Tokens ------------------------ */
// Lexer functions (lexer.c)

bool	validate_rline_syntax(char *read_line);
bool	check_syntax_quotes(char *current, int i, int *sq_count, int *dq_count);
bool	validate_tokens_syntax(t_shell *mini_sh);

// Parser functions (parser_tokenize.c)

void	tokenize(t_shell *mini_sh);
void	classify_word_token(t_shell *mini_sh);

// Parser utilities (parser_utils.c)

void	handle_quotes(t_shell *mini_sh, char *current, int i[2], int *type);
void	handle_redir(t_shell *mini_sh, char *current, int i[2], int *type);
void	handle_pipe_space(t_shell *mini_sh, char *current, int i[2], int *type);
void	rm_chr_pos(char *s, int pos);
void	rm_external_quotes(char *s);

// Token list functions (token_list.c)

t_token	*create_token(char *content, t_type token_type);
t_token	*init_tokenlist(t_shell *mini_sh);
void	addback_token(t_shell *mini_sh, char *content, t_type token_type);
void	clear_tokenlist(t_token **token_lst);

// Token utilities (token_utils.c)

t_token	*get_token_type(t_token *node, t_type type);
t_token	*get_token_content(t_token *node, char *content);
char	**get_args(t_token *node);
int		count_token_type(t_token *node, t_type type);

/* ----------------------------- Signals ----------------------------------- */
// Signal handling functions (signal.c)

void	handle_ctrl_c(int signal_sigint);
void	set_signals(void);
void	set_signal_errors(t_shell *mini_sh);
void	just_save_signal(int sign);
void	hdoc_child_ctrl_c(int sign);

/* ----------------------------- Builtins ---------------------------------- */
// Built-in command functions (builtin.c)

int		is_bi(t_token *node);
int		exec_bi(t_shell *mini_sh, t_cmd *cmd, t_builtin bi_cmd);
void	exec_one_bi(t_shell *mini_sh, t_builtin bi_cmd);

// Individual built-in implementations

// (bi_cd.c)

int		bi_cd(t_shell *mini_sh, t_cmd *cmd);
void	handle_cd_error(char *path);

// (bi_echo.c)

int		bi_echo(t_shell *mini_sh, t_cmd *cmd);
bool	is_nflag(char *arg);

// (bi_env.c)

int		bi_env(t_shell *mini_sh, t_cmd *cmd);
void	print_envlist(t_env *start);

// (bi_exit.c)

int		bi_exit(t_shell *mini_sh, t_cmd *cmd);
void	handle_exit_error(char **args, int x);

// (bi_export.c)

int		bi_export(t_shell *mini_sh, t_cmd *cmd);
void	print_export(t_env *start);

// (bi_pwd.c)

int		bi_pwd(t_shell *mini_sh, t_cmd *cmd);

// (bi_unset.c)

int		bi_unset(t_shell *mini_sh, t_cmd *cmd);

/* ----------------------------- Environment ------------------------------ */
// Environment variable functions (env.c)

char	*my_getenv(t_env *start, char *name);
char	**envlist_to_str(t_env *start);
void	init_emergency_env(t_shell *mini_sh);
void	increase_shlvl(t_shell *mini_sh);

// Environment list functions (env_list.c)

t_env	*create_envnode(char *name, char *value);
t_env	*init_envlist(char **env);
void	addback_envnode(t_env **start, t_env *node);
void	clear_envlist(t_env **start);
void	remove_envnode(t_env **env, char *name);

// Environment variable management (env_var.c)

int		is_valid_var_name(const char *argv);
void	register_var(t_shell *mini_sh, const char *argv);
bool	update_var(t_shell *mini_sh, char *name, char *value);
void	add_var(t_shell *mini_sh, char *name, char *value);

/* ----------------------------- Execution ------------------------------- */
// Command execution functions (execution.c)

void	execution(t_shell *mini_sh);
void	execute_cmds(t_shell *mini_sh);
pid_t	exe_in_child(t_shell *mini_sh, t_token *node, t_cmd *cmd);
int		wait_children(t_input *input);
void	safe_execve(t_shell *mini_sh, t_cmd *cmd);

// Command handling functions (cmd.c)

char	*locate_cmd(t_shell *mini_sh, t_token *node);
t_cmd	init_cmd(t_shell *mini_sh, t_token *node, int *pipe1, int *pipe2);
void	init_pipes(int *pipe1, int *pipe2);

// Command cleanup functions (cmd_exit.c)

void	clear_cmd(t_cmd *cmd);
void	cmd_not_found(t_shell *mini_sh, t_cmd *cmd);
void	cmd_exit_and_clean(t_shell *shell, t_cmd *cmd, int exit_code);
void	cmd_close_all_fd(t_cmd *cmd);

/* ----------------------------- Redirections ---------------------------- */
// Redirection handling functions (redirections.c)

int		handle_redirections(t_shell *mini_sh, t_token *node, t_cmd *cmd);
void	open_hdoc_file(t_shell *mini_sh, t_cmd *cmd);
void	open_outfile(t_token *node, t_cmd *cmd);
void	open_infile(t_token *node, t_cmd *cmd);

// Heredoc handling functions (heredoc.c)

int		create_hdoc_file(int id);
void	hdoc_child(t_shell *mini_sh, t_token *lim_node, int id);
void	handle_heredocs(t_shell *mini_sh);
void	hdoc_gnl(t_shell *mini_sh, t_token *lim_node, int id);

// Heredoc cleanup (heredoc2.c)

void	rm_hdoc_files(t_shell *mini_sh);

/* ----------------------------- Expansion ------------------------------- */
// Variable expansion functions (expansion_var.c)

void	mark_variables(char *s, int skip_quote);
void	adjust_mark_variables(char *s);
char	*extract_first_var(char *s);
char	*expand_vars(char *og, t_shell *mini_sh, bool safe_exp);

// Security mark functions for expansion (expansion_security.c)

void	security_mark(char *s);
void	revert_security_mark(char *s);

// Utility functions for expansion (expansion_utils.c)

void	skip_in_quotes(char **s);
void	my_skip(char **s, char c);
size_t	my_strlen_idname(char *s);
char	*my_replace_first(char *og, char *target, char *rep);

/* ----------------------------- Utilities ------------------------------- */
// Exit and cleaning utility functions (utils.c)

void	my_perr(const char *msg, bool should_exit, int exit_code);
void	my_free(void **mem);
void	my_free2d(void ***mem);
void	my_close(int *fd);

// String utilitiy functions(utils2.c)

int		my_strchr_pos(const char *s, char c);
int		my_strnstr_pos(const char *big, const char *little, size_t len);
char	*my_strjoin3(char const *s1, char const *s2, char const *s3);
int		my_atoi_circular(const char *nptr, int min, int max);

// Functions for checking file and directory types (type_check.c)

int		is_file(char *route);
int		is_directory(char *route);

/* ----------------------------- Debugging ------------------------------- */
// Debugging and printing functions (prints.c)

void	print_tokenslist(t_token *token_lst);
void	print_tokenslist_short(t_token *token_lst);
void	print_strings(char **args);

/* ************************************************************************** */

#endif