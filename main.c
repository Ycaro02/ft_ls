#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include "./libft/libft.h"


char	**ft_realloc_str(char **strs, char *str)
{
	char	**new;
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

int count_files(const char *directory_name)
{
    struct dirent *my_dir;
    DIR *dir = opendir(directory_name);
    int nb = 0;
    while ((my_dir = readdir(dir)) != NULL)
        nb++;
    closedir(dir);
    return (nb);
}

char **get_all_file_name(const char *directory_name)
{
    char **all = NULL;
    int nb = count_files(directory_name);
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


void classic_ls(char **tab)
{
    int i = 0;
    while (tab && tab[i])
    {
        ft_putstr_fd(tab[i], 1);
        if (tab[i + 1] != NULL)
            ft_putstr_fd("  ", 1);
        i++;
    }
}
#include <string.h>

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

#include <stdio.h>


int count_char_tab(char **tab)
{
    int i= 0;
    while (tab && tab[i])
        i++;
    return (i);
}

void ft_list_dir(char **tab)
{
    int i = 0;
    char **save = NULL;
    char *tmp = NULL;
    while (tab && tab[i])
    {
        tmp = get_lower_string(tab, save);
        if (tmp != NULL)
            save = ft_realloc_str(save, tmp);
        free(tmp);
        i++;
    }
    classic_ls(save);
    free_all(save);
}

int main (int argc, char** argv)
{
    char *directory;
    if (argc < 2)
        directory = ft_strdup(".");
    else
        directory = ft_strdup(argv[1]);
        // printf("directory = %s\n", directory);
    char **list = get_all_file_name(directory);
    ft_list_dir(list);
    free_all(list);
    free(directory);
    return (0);
}

// void display_without_hidden_file(const char *directory_name)
// {
//         struct dirent *my_dir;
//         DIR *dir = opendir(directory_name);
//         while ((my_dir = readdir(dir)) != NULL)
//         {
//             if (my_dir->d_name[0] != '.')
//                 ft_putstr_fd(my_dir->d_name, 1, "  ");
//         }
//         closedir(dir);
//         write(1, "\n", 1);
// }
