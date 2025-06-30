#include <main.h>

void free_arr(void **arr, int size)
{
	int i;

	i = -1;
	while (++i < size)
		free(*(arr + i));
	free(arr);
}

void free_pipeline(t_cmd *pipeline)
{
	t_cmd *freee;
	t_cmd *curr;

	while (curr)
	{
		freee = curr;
		curr = curr->next;
		freee->args->value;
		freee->redir->file.value;
		freee->redir->file;	
	}
}