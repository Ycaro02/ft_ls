#include "../ft_ls.h"


t_list *get_recurcive_dir(t_file *file, int flag_nb)
{
    t_list *new = NULL;
    struct dirent *my_dir;
    if (file->type != 'd')
        return (NULL);
    DIR *dir = opendir(file->name);
    if (!dir)
        return (NULL);
    do 
    {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb) == 1)
        {
            char *str = join_parent_name(file->name, my_dir->d_name);
            if (!str)
            {
                printf("Malloc error join parent name\n");
                continue ;
            }
            struct stat sb;
            if (lstat(str, &sb) == -1)
            {
                perror(str);
                new_lstclear(&new, free);
                return (NULL);
            }
            t_file *new_file = fill_file_struct(sb, str);
            if (!new_file || !new_file->name)
            {
                printf("Malloc error get recurse dir\n");
                free(new_file->name);
                free(new_file);
                return (NULL);
            }
            if (new_file->type == 'd')
                ft_lstadd_back(&new, ft_lstnew(new_file));
            else
            {
                free(new_file->name);
                free(new_file);
            }
            if (str)
                free(str);
        }
    } while (my_dir != NULL);
    closedir(dir);
    sort_lst(new, flag_nb);
    if (new && flag_nb & REVERSE_OPTION)
    {
        t_list *reverse = NULL;
        reverse_lst(new, &reverse);
        free_node_ptr(&new);
        new = reverse;
    }
    return (new);
}

void search_recurcive_dir(t_list *dir_lst, int flag_nb)
{
    t_list *local_list = NULL;
    while(dir_lst)
    {
        t_file *file = dir_lst->content;
        if (flag_nb & L_OPTION)
            ls_l_one_dir(dir_lst->content, flag_nb);
        else
            ls_one_dir(dir_lst->content, flag_nb);
        local_list = get_recurcive_dir(file, flag_nb);
        if (local_list)
        {
            search_recurcive_dir(local_list, flag_nb);
            new_lstclear(&local_list, free);
        }
        dir_lst = dir_lst->next;
    }
}