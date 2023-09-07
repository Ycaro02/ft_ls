#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include "./libft/libft.h"
#include <stdio.h>
#include <string.h>


#define L_FLAG_CHAR 'l'
#define R_FLAG_CHAR 'R'
#define REVERSE_FLAG_CHAR 'r'
#define A_FLAG_CHAR 'a'
#define T_FLAG_CHAR 't'

enum e_flag  {
    UNKNOW=-1,
    L_OPTION=1,
    R_OPTION=2,
    REVERSE_OPTION=4,
    A_OPTION=8,
    T_OPTION=10,
};

int count_char_tab(char **tab)
{
    int i= 0;
    while (tab && tab[i])
        i++;
    return (i);
}

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

void reverse_ls(char **tab)
{
    int len = count_char_tab(tab);
    while (len >= 0)
    {
        ft_putstr_fd(tab[len], 1);
        if (len != 0 && len != count_char_tab(tab))
            ft_putstr_fd("  ", 1);
        len--;
    }
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


int get_flag(enum e_flag *flag)
{
    int i = 0;
    int nb = 0;
    while (flag && flag[i] != UNKNOW)
    {
        nb += flag[i];
        i++;
    }   
    return (nb);
}

void ft_list_dir(char **tab, enum e_flag *flag)
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
    int flag_nb = get_flag(flag);
    if (flag_nb == 0)
        classic_ls(save);
    else if (flag_nb == REVERSE_OPTION)
        reverse_ls(save);
    free_all(save);
}


void fill_used_flag(enum e_flag *tab, enum e_flag flag)
{
    int i = 0;
    while (tab && tab[i] != UNKNOW)
        i++;
    tab[i] =  flag;
}

int already_add(enum e_flag *tab, enum e_flag to_check)
{
    int i = 0;
    while (i < 6)
    {
        if (tab[i] == to_check)
            return (1);
        i++;
    }
    return (0);
}

int check_flag(char c, enum e_flag *used)
{
    if (c == L_FLAG_CHAR && already_add(used, L_OPTION) == 1)
        return (1);
    else if (c == R_FLAG_CHAR && already_add(used, R_OPTION) == 1)
        return (1);
    else if (c == REVERSE_FLAG_CHAR && already_add(used, REVERSE_OPTION) == 1)
        return (1);
    else if (c == A_FLAG_CHAR && already_add(used, A_OPTION) == 1)
        return (1);
    else if (c == T_FLAG_CHAR && already_add(used, T_OPTION) == 1)
        return (1);
    return (0);
}

int add_flag(char c, enum e_flag *used)
{
    int tmp = check_flag(c, used);
    if (tmp == 1)
        return (0);
    if (c == L_FLAG_CHAR && already_add(used, L_OPTION) == 0)
    {
        fill_used_flag(used, L_OPTION);
        return (L_OPTION);
    }
    else if (c == R_FLAG_CHAR && already_add(used, R_OPTION) == 0)
    {
        fill_used_flag(used, R_OPTION);
        return (R_OPTION);
    }
    else if (c == REVERSE_FLAG_CHAR && already_add(used, REVERSE_OPTION) == 0)
    {
        fill_used_flag(used, REVERSE_OPTION);
        return (REVERSE_OPTION);
    }
    else if (c == A_FLAG_CHAR && already_add(used, A_OPTION) == 0)
    {
        fill_used_flag(used, A_OPTION);
        return (A_OPTION);
    }
    else if (c == T_FLAG_CHAR && already_add(used, T_OPTION) == 0)
    {
        fill_used_flag(used, T_OPTION);
        return (T_OPTION);
    }
    else
        printf("\nError end parsing flag, invalid flag found %c\n", c);
    for (int i = 0; i < 6; i++)
        printf("used de [%d] = %d\n", i, used[i]);
    return -1;
}

enum e_flag *parse_flag(char **argv, enum e_flag *used)
{
    int i = 0;
    while (i < 6)
    {
        used[i] = UNKNOW;
        i++;
    }
    i = 0;
    while (argv && argv[i])
    {
        if (argv[i][0] == '-')
        {
            int j = 1;
            while (argv[i] && argv[i][j])
            {
                int check = add_flag(argv[i][j], used);
                if (check == -1)
                    return (NULL);
                j++;
            }
        }
        i++;
    }
    return(used);
}

int main (int argc, char** argv, char **envp)
{
    char *directory;
    (void)envp;
    if (argc < 2)
        directory = ft_strdup(".");
    else
        directory = ft_strdup(argv[1]);
    char **list = get_all_file_name(directory);
    enum e_flag *used = malloc(sizeof(int) * 6);
    if (!used)
    {
        printf("Error malloc failed\n");
        return (1);
    }
    void *save_ptr = used;
    used = parse_flag(argv, used);
    if (used == NULL)
    {
        printf("parse flag return -1");
        free_all(list);
        free(directory);
        free(save_ptr);
        return (1);
    }
    ft_list_dir(list, save_ptr);
    free_all(list);
    free(directory);
    free(save_ptr);
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
