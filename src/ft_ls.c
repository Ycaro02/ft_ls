#include "../ft_ls.h"

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
    printf("\n");
}

void reverse_ls(char **tab)
{
    int len = count_char_tab(tab);
    if (len == 0)
        return ;
    while (len >= 0)
    {
        ft_putstr_fd(tab[len], 1);
        if (len != 0 && len != count_char_tab(tab))
            ft_putstr_fd("  ", 1);
        len--;
    }
    printf("\n");
}


void ls_no_args()
{
    char **list = get_all_file_name(".");
    int flag = 0;
    if (list == NULL)
    {
        printf("Error for read current directory\n");
        return ;
    }
    ft_list_dir(list, flag);
    free_all(list);
}

void ft_list_dir(char **tab, int flag_nb)
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
    if (flag_nb == 0)
        classic_ls(save);
    else if (flag_nb == REVERSE_OPTION)
    {
        flag_nb -= REVERSE_OPTION;
        reverse_ls(save);
    }
        
    free_all(save);
}

void ft_ls(char **dir_lst, int flag_nb, int print_dir_name)
{
    int i = 0;
    // int dir_lst_len = count_char_tab(dir_lst);
    dir_lst = parse_dir_lst(dir_lst);
    while (dir_lst && dir_lst[i])
    {
        char **list = get_all_file_name(dir_lst[i]);
        if (list == NULL)
        {
            i++;
            continue ;
        }
        if (print_dir_name != 0)
            printf("%s:\n", dir_lst[i]);
        ft_list_dir(list, flag_nb);
        free_all(list);
        i++;
        printf("\n");
    }
    free_all(dir_lst);
}

void ls_one_dir(char *str, int flag_nb)
{
    char **list = get_all_file_name(str);
    if (list == NULL)
        return ;
    printf("%s:\n", str);
    ft_list_dir(list, flag_nb);
    free_all(list);
    printf("\n");
}