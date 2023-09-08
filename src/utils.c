#include "../ft_ls.h"

void free_all(char **array)
{
    int i = 0;
    while (array && array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

int already_use(char *str, char** used)
{
    int i = 0;
    while (used && used[i])
    {
        if (strcmp(str, used[i]) == 0)
            return (1);
        i++;
    }
    return (0);
}

void print_tab(char **tab)
{
    int i = 0;
    while (tab && tab[i])
    {
        ft_putstr_fd(tab[i], 1);
        ft_putstr_fd("\n", 1);
        i++;
    }
}

int count_char_tab(char **tab)
{
    int i= 0;
    while (tab && tab[i])
        i++;
    return (i);
}

char	**ft_realloc_str(char **strs, char *str)
{
	char	**new;;
	int		i;

	i = 0;
	while (strs && strs[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
		return (strs);
	i = 0;
	while (strs && strs[i])
	{
		new[i] = strs[i];
		i++;
	}
	new[i] = ft_strdup(str);
	new[i + 1] = NULL;
	if (strs)
		free(strs);
	return (new);
}