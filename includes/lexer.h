/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:32:37 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 13:32:38 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdio.h>
# include <libft.h>
# include <stdbool.h>

typedef enum e_token_type
{
	Word,
	Pipe,
	Redirect_In,
	Redirect_Out,
	Append,
	Here_doc,
	End_of_file,
}	t_token_type;

typedef enum e_quote_type
{
	No_quotes,
	Single_quotes,
	Double_quotes
}	t_quote_type;

typedef enum e_expendable
{
	Not_expendable,
	Expendable
}	t_expendable;

typedef enum e_context
{
	Unquoted,
	Quoted,
	Double_quoted,
	Separator,
}	t_context;

typedef enum e_state
{
	space,
	literal,
	pi_pe,
	heredoc,
	redirect_in,
	redirect_out,
	append,
	param_here,
	single_quote,
	double_quote,
	E_OF
}	t_state;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_expendable	expendable;
}	t_token;

typedef struct s_lexer
{
	char			*offset;
	char			*input;
	t_list			*tokens;
	t_state			state;
	t_context		context;
	int				in_heredoc_delim;
}	t_lexer;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redir;
	int				argcount;
	int				redircount;
	struct s_cmd	*next;
}	t_cmd;

int				is_whitespace(char c);
int				is_quote(char c);
int				is_redirect(char c);
int				is_seperator(char c);
int				is_valid_param_start(char *str);
int				is_valid_param_char(char c);
int				contains_parameter(char *value);
void			handle_redirect_in(t_lexer *lexer);
void			handle_redirect_out(t_lexer *lexer);
void			skip_space(t_lexer *lexer);
t_lexer			*init_lexer(const char *input);
void			free_lexer(t_lexer *lexer);
t_token			*create_token(char *value, t_token_type type,
					t_expendable expendable);
void			free_token(void *ptr);
void			append_token(t_lexer *lexer, t_token *token);
t_lexer			*set_state(t_lexer *lexer);
void			set_context(t_lexer *lexer, char c);
void			resolve_tokens(t_lexer *lexer);
t_cmd			*create_pipeline(t_list *list);
char			*quotes(char *str);
t_token_type	get_token_type(t_state state);
t_quote_type	get_quote_type(t_context context);
void sigint_handler(int sig);
void heredoc_sigint_handler(int sig);
void setup_interactive_signals(void);
void setup_child_signals(void);
void setup_heredoc_signals(void);
void setup_parent_signals(void);

#endif