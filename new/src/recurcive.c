#include "../ft_ls.h"

static int is_point_dir(char *path)
{
    if (path && path[0] == '.')
        return (0);
    return (1);
}

t_list *get_file_name(char *directory_name)
{
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(directory_name);
    while ((my_dir = readdir(dir)) != NULL)
    {
        if (my_dir->d_name[0] != '-' && is_point_dir(my_dir->d_name) == 1)
        {
            char *str = ft_strjoin(directory_name, "/");
            str = ft_strjoin_free(str, my_dir->d_name, 'f');
            if (str && access(str, F_OK | R_OK) == 0 && is_directory(str) == 0)
                ft_lstadd_back(&all, ft_lstnew(ft_strdup(str)));
            if (str)
                free(str);
        }
    }
    closedir(dir);
    return(all);
}



void search_recurcive_dir(t_list *dir_lst, int flag_nb, int first)
{
    t_list * local_list = NULL;
    while(dir_lst)
    {
        ls_one_dir(dir_lst->content, flag_nb);
        // char **local_list = build_local_list(dir_lst[i], first);
        local_list = get_file_name(dir_lst->content);
        search_recurcive_dir(local_list, flag_nb, 1);
        dir_lst = dir_lst->next;
    }
    if (local_list)
        ft_lstclear(&local_list, free);
    (void)first;
    
    // if (dir_lst && first == 0)
    //     ft_ls(dir_lst, flag_nb, 1);
    // return (dir_lst);
}