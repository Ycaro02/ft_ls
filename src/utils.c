#include "../ft_ls.h"

static void     free_str_join(char *s1, char *s2, char option)
{
        if (option == 'f' || option == 'a')
                free(s1);
        if (option == 's' || option == 'a')
                free(s2);
}

char    *ft_strjoin_free(char *s1, char *s2, char option)
{
        int             len_new_s;
        char    *new_s;
        int             i;
        int             j;

        len_new_s = ft_strlen(s1) + ft_strlen(s2);
        new_s = malloc((len_new_s + 1) * sizeof(char));
        if (!new_s)
        {
                free_str_join(s1, s2, option);
                return (NULL);
        }
        i = 0;
        while (s1 && s1[i])
        {
                new_s[i] = s1[i];
                i++;
        }
        j = 0;
        while (s2 && s2[j] && i < len_new_s)
                new_s[i++] = s2[j++];
        free_str_join(s1, s2, option);
        new_s[i] = '\0';
        return (new_s);
}

void free_all(char **array)
{
    int i = 0;
    while (array && array[i])
    {
        free(array[i]);
        i++;
    }
    if (array)
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

int      is_directory(const char *path)
{
        struct stat     sb;

        lstat(path, &sb);
        if ((sb.st_mode & S_IFMT) != S_IFDIR)
                return (1);
        // if (access(path, X_OK) != 0)
        //     printf("ft_ls: cannot open directory '%s': Permission denied", path);
        return (0);
}