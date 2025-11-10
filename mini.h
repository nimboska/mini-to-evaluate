/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochmurzy <ochmurzy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:38:42 by ochmurzy          #+#    #+#             */
/*   Updated: 2025/10/30 17:07:40 by ochmurzy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_H
# define MINI_H

# define _XOPEN_SOURCE 700
# define ECHOCTL 0001000

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# ifndef MINI_PATH_MAX
#  define MINI_PATH_MAX 4096
# endif

typedef enum e_token_type
{
	TOKEN_WORD,				// word token
	TOKEN_PIPE,				// '|'
	TOKEN_REDIRECT_IN,		// '<'
	TOKEN_REDIRECT_OUT,		// '>'
	TOKEN_REDIRECT_APPEND,	// '>>'
	TOKEN_HEREDOC
}		t_token_type;

typedef struct s_token // reprezentacja jednego tokena
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}		t_token;

typedef struct s_outredir
{
	char	*path;		// ścieżka pliku docelowego
	int		append;		// 0 => '>' (truncate), 1 => '>>' (append)
}		t_outredir;

typedef struct s_heredoc
{
	char	*delim;		// delimiter (np. "EOF")
	char	*tmp_path;	// ścieżka tymczasowego pliku z treścią heredoca
}		t_heredoc;

typedef struct s_cmd
{
	char			**argv;		// argv[0] = program (np. "ls")
	int				argc;		// liczba argumentów (bez NULL)
	char			*infile;	// ścieżka pliku po '<' (ostatnie < wygrywa)
	int				in_fd;		// FD do odczytu , -1 gdy brak
	int				last_in_type; //0 = none, 1 = infile, 2 = heredoc
	int				last_heredoc_idx;

	t_outredir		*outs;// dynamiczna tablica wyjść (> i >>) w kolejności
	int				outs_len; // ile elementów w 'outs'
	int				out_fd;// FD do zapisu, -1 gdy brak
				// UWAGA: przy wykonaniu zwykle liczy się OSTATNI element outs
	t_heredoc		*heredocs; // dynamiczna tablica heredoców
	int				heredoc_cnt;// liczba heredoców
	struct s_cmd	*next;	// kolejna komenda w pipeline
							// A | B | C-> lista jednokierunkowa
}		t_cmd;

typedef struct s_execctx
{
	int		n;			//liczba poleceń w linii
	int		*pipes;		//tablica deskryptorów potoków (najczęściej n-1 potoków)
	pid_t	*pidv;		//tablica PIDs procesów-dzieci (rozmiar n)
	pid_t	last_pid;	//PID ostatniego zforkowanego dziecka
}		t_execctx;

typedef struct s_env // zmienne srodowiskowe
{
	char			*key;
	char			*value;
	struct s_env	*next;
}		t_env;

typedef struct s_shell // stan calego shella
{
	char	*path;		// sciezka
	int		last_status;// kod wyjścia poprzedniego polecenia
	t_env	*env;		// lista zmiennych srodowiskowych
	int		count_cmds; // ilosc komend bedzie potrzebna do zwalniania pamieci
	t_cmd	*cmds;		// lista komend (po parserze)
	char	*line;		// linia wejsciowa
}		t_shell;

//****MAIN****
int		main(int argc, char **argv, char **env);

//****Input****
int		read_input(t_shell *shell);

//****Init****
void	set_path(t_shell *shell);
void	value_init(t_shell *shell);
void	init_cmd_defaults(t_cmd *cmd);

//****Environment_things****
void	*find_last(void *stack, size_t offset);
int		split_env(t_env **stack, char **str);
void	create_list_env(t_env **stack, char **env);
char	*get_var_value(char *name, t_shell *shell);
int		env_len(t_env *e);
t_env	*find_env(t_env *env, const char *key);
void	update_env_val(t_env **env, const char *key, const char *new_val);
t_env	*add_new_env(t_env **env, const char *key, const char *val);

//****Input tokenization****
t_token	*lexer(char *input, t_shell *shell);
void	append_to_list(t_token **head, t_token *new_token);
t_token	*append_operator(char *input, int *i);
void	helper_redir(char *input, int i, t_token *new_token);
char	*expand_var(char *input, int *i, t_shell *shell);
t_token	*append_word(char *input, int *i, t_shell *shell);
t_token	*handle_quote(char *input, int *i, t_shell *shell);

//****Command_Struct****
t_cmd	*command_init(t_shell **shell, t_cmd **head);
void	handle_redirects(t_cmd *command, t_token *tokens);
t_cmd	*handle_pipe(t_cmd *node, t_token *tokens, t_shell *shell);
void	add_cmd_argv(t_cmd *command, const char *arg);
t_cmd	*adding_command(t_token *tokens, t_shell *shell);

//*Heredock*
int		read_stdin(const t_heredoc *hd);
int		read_to_file(t_heredoc *new_hrdc);
void	add_to_file(t_heredoc *new_hrdc, int fd);
void	add_heredoc(t_cmd *command, t_token *delim);
int		handle_status(int status, char *path);

//****Signals****
void	signals_things(void);
void	sig_handler(int signal);

//***Execution***
int		is_builtin(const char *name);
int		run_single_builtin(t_shell *shell);
int		call_builtin(t_shell *sh, char **argv);
int		apply_in_redir(t_cmd *cmd);
int		apply_out_redir(t_cmd *cmd);
int		run_pipeline_or_external(t_shell *shell);
void	child(t_shell *sh, t_execctx *x, int i);
int		create_pipes(t_execctx *x);
void	close_all_pipes(t_execctx *x);
int		pipe_read_fd(t_execctx *x, int idx);
int		pipe_write_fd(t_execctx *x, int idx);
void	wire_child_pipes(t_execctx *x, int i);
int		has_slash(const char *s);
char	*join_path(const char *dir, const char *bin);
t_cmd	*child_cmd(t_cmd *head, int i);

/* Builtins */
int		ft_cd(t_shell *sh, char **argv);
int		ft_pwd(t_shell *sh, char **argv);
int		ft_echo(t_shell *sh, char **argv);
int		ft_env(t_shell *sh, char **argv);
int		ft_exit(t_shell *sh, char **argv);
int		ft_unset(t_shell *sh, char **argv);
int		handle_arg(t_shell *sh, const char *arg);
int		print_sorted_export(t_env *env);
int		ft_export(t_shell *sh, char **argv);

//****Utils****
void	*safe_malloc(size_t bytes);
char	*join_free(char **str1, char *str2);
char	*join_char(char **str1, char c);
int		is_operator(char c);

//****Cleaning functions****
void	cmds_free(t_shell *shell);
void	free_hrdc(t_cmd *command);
void	free_split(char **tab);
void	free_token_list(t_token *head);
void	free_env_list(t_env *env);

//****Errors****
void	error_exit(const char *error);
void	print_cmd_struct(const t_cmd *cmd);
void	apply_redirs(t_cmd *cmd);

#endif