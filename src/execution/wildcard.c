// #include <main.h>

// int	check_wildcard(char *exp, char *file)
// {
// 	if (*exp == '\0' && *file == '\0')
// 		return (1);
// 	if (*exp == '*' && *(exp + 1) == '\0')
// 		return (1);
// 	if (*exp == '\0' || *file == '\0')
// 		return (0);
// 	if (*exp == *file)
// 		return (check_wildcard(exp + 1, file + 1));
// 	if (*exp == '*')
// 		return (check_wildcard(exp + 1, file) || check_wildcard(exp, file + 1));
// 	return (0);
// }

// int get_wildcard_files(t_list **files, char *exp)
// {
// 	int count;
// 	DIR *dir;
// 	struct dirent *dirent;

// 	dir = opendir(".");
// 	count = 0;
// 	while (1)
// 	{
// 		dirent = readdir(dir);
// 		if (!dirent)
// 			break ;
// 		if (!_strcmp(dirent->d_name, "..") || !_strcmp(dirent->d_name, "."))
// 			continue ;
// 		if (check_wildcard(exp, dirent->d_name))
// 		{
// 			add_node(files, dirent->d_name);
// 			count++;
// 		}
// 	}
// 	if (!count)
// 	{
// 		add_node(files, exp);
// 		count = 1;
// 	}
// 	return (count);
// }

// char **make_args(t_list *files, int size)
// {
// 	char **new;
// 	t_list *curr;

// 	new = malloc(sizeof(char *) * size);
// 	curr = files;
// 	while (curr)
// 	{
// 		*new = (char *)curr->data;
// 		new++;
// 		curr = curr->next;
// 	}
// 	new -= size;
// 	return (new);
// }

// t_member *expand_wildcard(t_member *args)
// {
// 	char **arr;
// 	int count;
// 	int size;
// 	t_list *files;

// 	files = NULL;
// 	count = 0;
// 	size = 0;
// 	arr = (char **)args->members;
// 	while (count < args->size)
// 	{
// 		if (!_strchr(arr[count], '*'))
// 		{
// 			add_node(&files, arr[count]);
// 			size++;
// 		}
// 		else
// 			size += get_wildcard_files(&files, arr[count]);
// 		count++;
// 	}
// 	free(args->members);
// 	args->members = (void **)make_args(files, size);
// 	args->size = size;
// 	args->type = ARGS;
// 	return (args);
// }
