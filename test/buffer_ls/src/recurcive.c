#include "../ft_ls.h"

static int last_char_is_slash(char *str)
{
    if (!str)
        return (-1);
    int i = 0;
    while (str[i])
        i++;
    if (i == 1 && str[0] == '/')
        return (0);
    if (str[i - 1] == '/')
        return (0);
    return (1);
}

static int check_recurcive_dir(char *dir_name)
{
    int nb = -1;

    if (access(dir_name, F_OK) != 0)
        printf("\nft_ls: cannot access '%s': No such file or directory\n\n", dir_name);
    else if (is_directory(dir_name) == 0 && access(dir_name, R_OK) != 0)
        printf("ft_ls: cannot open directory '%s': Permission denied\n\n", dir_name);
    else if (is_directory(dir_name) == 0 && access(dir_name, R_OK) == 0)
        nb = 0;
    return (nb);
}

void sort_by_name(t_list *lst)
{
    if (!lst)
        return ;
    t_list *head = lst;
    t_list *min = NULL;
    while (lst)
    {
        if (!min)
            min = lst;
        else if (lower_strcmp(lst->content, min->content) < 0)
            min = lst;
        lst = lst->next;
    }
    char *tmp = head->content;
    head->content = min->content;
    min->content = tmp;
    sort_by_name(head->next);
}

static t_list *get_recurcive_file_name(char *directory_name, int flag_nb)
{
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(directory_name);
    do 
    {
        if (access(directory_name, F_OK) != 0)
        {
            printf("\nft_ls: (in get recurce file name) cannot access '%s': No such file or directory\n", directory_name);
            break;
        }
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb) == 1)
        {
            char *str = NULL;
            if (last_char_is_slash(directory_name) == 0)
                str = ft_strjoin(directory_name, my_dir->d_name);
            else
            {
                str = ft_strjoin(directory_name, "/");
                str = ft_strjoin_free(str, my_dir->d_name, 'f');
            }
            if (str && check_recurcive_dir(str) == 0 && is_directory(str) == 0)
                ft_lstadd_back(&all, ft_lstnew(ft_strdup(str)));
            if (str)
                free(str);
        }
    } while (my_dir != NULL);
    closedir(dir);
    sort_by_name(all);
    if (all && flag_nb & REVERSE_OPTION)
    {
        t_list *new = NULL;
        reverse_lst(all, &new);
        ft_lstclear(&all, free);
        return (new);
    }
    return(all);
}


void search_recurcive_dir(t_list *dir_lst, int flag_nb)
{
    t_list *local_list = NULL;
    while(dir_lst)
    {
        ls_one_dir(dir_lst->content, flag_nb);
        local_list = get_recurcive_file_name(dir_lst->content, flag_nb);
        search_recurcive_dir(local_list, flag_nb);
        if (local_list)
            ft_lstclear(&local_list, free);
        dir_lst = dir_lst->next;
    }
}