#include "../ft_ls.h"

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
	char *tmp = NULL;
    t_list *current = lst;
    
    while (current)
	{
        if (current->next && lower_strcmp(current->content, current->next->content) >= 0)
        {
            tmp = current->next->content;
            current->next->content = current->content;
            current->content = tmp;
            current = lst;
            continue ;
        }
        current = current->next;
	}
}

t_list *get_recurcive_file_name(char *directory_name)
{
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(directory_name);
    while ((my_dir = readdir(dir)) != NULL)
    {
        if (is_point_dir(my_dir->d_name) == 1)
        {
            char *str = NULL;
            if (strcmp("/", directory_name) == 0)
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
    }
    sort_by_name(all);
    closedir(dir);
    return(all);
}

void search_recurcive_dir(t_list *dir_lst, int flag_nb)
{
    t_list * local_list = NULL;
    while(dir_lst)
    {
        ls_one_dir(dir_lst->content, flag_nb);
        local_list = get_recurcive_file_name(dir_lst->content);
        search_recurcive_dir(local_list, flag_nb);
        if (local_list)
            ft_lstclear(&local_list, free);
        dir_lst = dir_lst->next;
    }
}