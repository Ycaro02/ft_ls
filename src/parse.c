#include "../ft_ls.h"

int count_files(const char *directory_name)
{
    struct dirent *my_dir;
    DIR *dir = opendir(directory_name);
    int nb = 0;
	if (access(directory_name, F_OK) != 0)
    {
        printf("\nft_ls: cannot access '%s': No such file or directory\n", directory_name);
		return (-1);
    }
    while ((my_dir = readdir(dir)) != NULL)
        nb++;
    closedir(dir);
    return (nb);
}

char **get_all_file_name(const char *directory_name)
{
    char **all = NULL;
    int nb = count_files(directory_name);
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
    if (new == NULL)
        printf("nice c null\n\n");
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
                if (strcmp(lower, tab[i]) >= 0)
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