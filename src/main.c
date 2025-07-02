#include <main.h>
#include <lexer.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>

t_shell g_shell;

const char *token_type_to_str(t_token_type type)
{
    switch (type)
    {
        case Word:         return "Word";
        case Pipe:         return "Pipe";
        case Redirect_In:  return "Redirect_In";
        case Redirect_Out: return "Redirect_Out";
        case Append:       return "Append";
        case Here_doc:     return "Here_doc";
        case End_of_file:  return "EOF";
        default:           return "Unknown";
    }
}

const char *quote_type_to_str(t_quote_type quote)
{
    switch (quote)
    {
        case No_quotes:      return "No_quotes";
        case Single_quotes:  return "Single_quotes";
        case Double_quotes:  return "Double_quotes";
        default:             return "Unknown";
    }
}

const char *expendable_to_str(t_expendable ex)
{
    switch (ex)
    {
        case Expendable:     return "Expendable";
        case Not_expendable: return "Not_expendable";
        default:             return "Unknown";
    }
}


void print_expandable_tokens(t_lexer *lexer)
{
    t_list *cur = lexer->tokens;
    int i = 1;
    while (cur)
    {
        t_token *tok = (t_token *)cur->data;
        if (tok->expendable == Expendable)
            printf("Expandable Token %d: %s\n", i, tok->value);
        cur = cur->next;
        i++;
    }
}

// int main(int ac, char **av, char **envp)
// {
//     char *input = NULL;
//     t_lexer *lexer = NULL;
    
//     init_shell(envp);
//     while (1)
//     {
//         input = readline("minishell> ");
//         if (!input)
//             break;
//         if (*input)
//             add_history(input);

//         lexer = init_lexer(input);
//         if (!lexer)
//         {
//             fprintf(stderr, "Lexer init failed\n");
//             free(input);
//             return 1;
//         }
//         // Print final EOF
//         set_context(lexer, '\0');
//         set_state(lexer);
//         // Reset offset and resolve tokens
//         lexer->offset = lexer->input;
//         resolve_tokens(lexer);

//         // Print resolved tokens
//         printf("\nResolved Tokens:\n");
//         t_list *cur = lexer->tokens;

//         while (cur)
//         {
//             t_token *tok = (t_token *)cur->data;
//             printf("Token: %-15s | Type: %-12s | Expandable: %s\n",
//                 tok->value,
//                 token_type_to_str(tok->type),
//                 expendable_to_str(tok->expendable));
//             cur = cur->next;
//         }
//         start(create_pipeline(lexer->tokens));
//         free_lexer(lexer);
//         free(input);
//     }
//     return 0;
// }

// Modified main
int main(int ac, char **av, char **envp)
{
    char *input = NULL;
    t_lexer *lexer = NULL;
    t_list *cur = NULL;

    init_shell(envp);
    while (1)
    {
        setup_prompt_signals(); // FOR SIGNALS !!
        input = readline("minishell> ");
        if (!input)
        {
            write(1, "\nexit\n", 6);
            break;
        }
        // if (*input)
        //     add_history(input);
        if (*input == '\0')
        {
            free(input);
            continue; // Skip empty input
        }
        add_history(input);
        lexer = init_lexer(input);
        if (!lexer)
        {
            fprintf(stderr, "Lexer init failed\n");
            free(input);
            return 1;
        }
        resolve_tokens(lexer);
        if (check_syntax_errors(lexer))
        {
            free_lexer(lexer);
            free(input);
            continue;
        }
        cur = lexer->tokens;
        g_shell.pipeline = create_pipeline(lexer->tokens);
		free_lexer(lexer);
		free(input);
        start(g_shell.pipeline);
		free_pipeline(g_shell.pipeline);
    }
    return 0;
}