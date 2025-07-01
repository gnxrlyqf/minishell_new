#ifndef MAIN_H
# define MAIN_H

# include <wait.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <lexer.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_err
{
	ERR_NONE,
	EMPTY_PROMPT,
	INV_TOKEN,
	SYSCALL_FAIL,
	PERM_DENIED,
	CMD_ENOENT,
	AMBIG_REDIR,
	IS_DIR,
	MAX_HEREDOC
} t_err;

typedef struct s_err
{
	t_err code;
	void *data;
} t_error;

typedef struct s_env
{
	struct s_env *next;
	char *key;
	char *value;
} t_env;

typedef struct s_shell
{
	t_cmd *pipeline;
	t_env *env;
	int status;
} t_shell;

typedef void (*error_handler)(char *data);
typedef int (*builtin)(char **args);

extern t_shell g_shell;
	
t_list	*add_node(t_list **head, void *value);
void	free_list(t_list **head);
int		list_len(t_list *list);
int		fill_var(char *str, t_list **list);
char	*make_str(t_list *list);
char	*quotes_expand(char *str);
int		my_open(char *file, int flags);
char	*mkfilename(char *path);
char	*do_heredoc(char *eof, int expand);
void	redir(t_redir *redir, int size);
char	*mkpath(char *path, char *cmd);
char	*check_cwd(char *cmd, t_env *env);
char	*check_path(char *path);
char	*which(char *cmd, t_env *env);
int		cmd(t_cmd *cmd);
void	cmd_pipe(t_cmd *cmd);
char	**extract_args(t_token *tokens, int size);
t_env	*init_env(char **envp);
char	*mkvar(t_env *env);
char	**mkenvp(t_env *env, int *envsize);
t_env	*add_node_env(t_env **head, char *name, char *value);
char	*get_env_val(t_env *env, char *key);
t_env	*get_env(t_env *env, char *key);
int		env_len(t_env *env);
t_env	*update_env(t_env **env, char *key, char *value);
void	free_node(t_env **env, t_env *node);
int		env(char **args);
int		_isalpha(int c);
int		_isalnum(int c);
int		validate_id(char *name);
int		export_add(char **args);
int		export(char **args);
int		check_builtins(t_cmd *cmd);
int		echo(char **args);
int		unset(char **args);
int		__exit(char **args);
int		pwd(char **args);
int		_chdir(char *dir);
int		cd(char **args);
char	*max_str(char *a, char *b);
int		skip(char *str, int i, char c, int rev);
int		wc(char *str, char c);
int		is_empty(char *str);
char	*_strrstr_skip(char *str, char *sub);
char	*_strstr_skip(char *str, char *sub);
char	*_strtok(char *str, char *delims);
char	*_strchr(char *str, char c);
char	*_strdup(char *src);
int		_strlen(char *str);
int		_strncmp(char *s1, char *s2, unsigned int n);
char	*_strndup(char *str, char *set);
int		_strcmp(char *s1, char *s2);
void	throw_err(t_err code, char *data);
void	unexpected_token(char *data);
void	syscall_fail(char *data);
void	ambig_redir(char *data);
void	empty_prompt(char *data);
void	cmd_enoent(char *data);
void	perm_denied(char *data);
void	is_dir(char *data);
int		start(t_cmd *pipeline);
void	init_shell(char **envp);
char    *_itoa(int n);
int		expand_status(t_list **list);
void	free_arr(char **arr, int size);
void	free_pipeline(t_cmd *pipeline);
void	free_env(t_env *env);

# endif
