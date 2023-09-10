#include "../ft_ls.h"

char    **parse_dir_lst(char **dir_lst)
{
    char **new = NULL;
    for (int i = 0; dir_lst && dir_lst[i]; i++)
    {
        if (access(dir_lst[i], F_OK) != 0)
            printf("\nft_ls: cannot access '%s': No such file or directory\n", dir_lst[i]);
        else if (is_directory(dir_lst[i]) == 1)
            printf("\n%s\n", dir_lst[i]);
        else if (access(dir_lst[i], R_OK) == 0)
            new = ft_realloc_str(new, dir_lst[i]);
    }
    free_all(dir_lst);
    return (new);
}

static int count_file(const char *directory_name)
{
    struct dirent *my_dir;
    DIR *dir = opendir(directory_name);
    int nb = 0;
    if (access(directory_name, R_OK | F_OK) == 0 && is_directory(directory_name) == 0)
    {
        while ((my_dir = readdir(dir)) != NULL)
            nb++;
    }
    else 
    {
        printf("ft_ls: cannot open directory '%s': Permission denied\n", directory_name);
        // printf("reject in count _file access F/X_OK and isdir is checked for %s\n", directory_name);
        return (-1);
    }
    closedir(dir);
    return (nb);
}

char **get_all_file_name(const char *directory_name)
{
    char **all = NULL;
    int nb = count_file(directory_name);
	if (nb == -1)
		return (NULL);
    all = malloc(sizeof(char *) * (nb + 1));
    struct dirent *my_dir;
    DIR *dir = opendir(directory_name);
    int i = 0;
    while ((my_dir = readdir(dir)) != NULL)
    {
        all[i] = ft_strdup(my_dir->d_name);
        i++;
    }
    all[i] = NULL;
    closedir(dir);
    return(all);
}

char **get_dir(char **argv)
{
    int i = 0;
    char **new = NULL;
    while (argv && argv[i])
    {
        if (argv[i][0] != '-')
            new = ft_realloc_str(new, argv[i]);
        i++;
    }
    return (new);
}

char *get_lower_string(char **tab, char **used)
{
	int i = 0;
	char *lower = NULL;
    char *save = NULL;

    while (tab && tab[i])
	{
        if (tab[i][0] != '.')
        {
            if (already_use(tab[i], used) == 0)
            {
                if (lower == NULL)
                    lower = tab[i];
                if (my_strcmp(lower, tab[i]) >= 0)
                {
                    lower = tab[i];
                    if (save)
                        free(save);
                    save = ft_strdup(tab[i]);
                }
            }
        }
        i++;
	}
    return (save);
}