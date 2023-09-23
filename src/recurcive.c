#include "../include/ft_ls.h"

static int create_new_file(struct stat sb, t_list **new, char *str)
{
    t_file *new_file;
    
    new_file = fill_file_struct(sb, str, NULL);
    if (!new_file || !new_file->name)
    {
        ft_printf_fd(2, "Malloc error\n");
        free(new_file->name);
        free(new_file);
        return (MALLOC_ERR);
    }
    if (new_file->type == DIRECTORY)
        ft_lstadd_back(new, ft_lstnew(new_file));
    else
    {
        free(new_file->name);
        free(new_file);
    }
    return (0);
}

static int parse_directory(t_file *file, struct dirent* my_dir, t_list **new)
{
    char *str;
    struct stat sb;

    str = join_parent_name(file->name, my_dir->d_name);
    if (!str)
        return (print_error("Malloc error\n", NULL, 1, 1));
    if (lstat(str, &sb) == -1)
    {
        print_error(str, "ft_ls: cannot access directory : ", 1, 0);
        free(str);
        return (1);
    }
    if (create_new_file(sb, new, str) == MALLOC_ERR)
    {
        free(str);
        return (MALLOC_ERR);
    }
    free(str);
    return (0);
}

static int read_dir(t_file *file, t_list **new, int flag_nb)
{
    struct dirent *my_dir;
    DIR *dir;

    dir = opendir(file->name);
    if (!dir)
        return (print_error("Opendir error\n", NULL, 1, 1));
    do 
    {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb) == 1)
        {
            int ret = parse_directory(file, my_dir, new);
            if (ret != 0)
            {
                closedir(dir);
                return (ret);
            }
        }
    } while (my_dir != NULL);
    closedir(dir);
    return (0);
}

t_list *get_recurcive_dir(t_file *file, int flag_nb, int *error)
{
    t_list *new = NULL;
    if (file->type != DIRECTORY)
        return (NULL);
    int ret = read_dir(file, &new, flag_nb) == 1;
    if (ret != 0)
    {
        if (ret == MALLOC_ERR)
            *error = MALLOC_ERR;
        else
        {
            ft_putstr_fd("ft_ls: cannot open directory : ", 2);
            perror(file->name);
            update_error(error);
        }
        return (NULL);
    }
    sort_lst(new, flag_nb);
    if (new && flag_nb & REVERSE_OPTION)
    {
        t_list *reverse = NULL;
        reverse_lst(new, &reverse);
        if (!reverse)
        {
            *error = MALLOC_ERR;
            return (NULL);
        }
        free_node_ptr(&new);
        new = reverse;
    }
    return (new);
}

int search_recurcive_dir(t_list *dir_lst, int flag_nb, int *error)
{
    t_list *local_list;
    int err = 0;

    local_list = NULL;
    int lst_len = get_lst_len(dir_lst);
    while(dir_lst)
    {
        t_file *file = dir_lst->content;
        if (flag_nb & L_OPTION)
            ls_l_one_dir(dir_lst->content, flag_nb, lst_len);
        else
            ls_one_dir(dir_lst->content, flag_nb, lst_len);
        local_list = get_recurcive_dir(file, flag_nb, error);
        if (*error == MALLOC_ERR)
            break ;
        if (local_list)
            err = search_recurcive_dir(local_list, flag_nb, error);
        if (err == MALLOC_ERR)
            break ;
        new_lstclear(&local_list, free);
        dir_lst = dir_lst->next;
    }
    if (*error == MALLOC_ERR || err == MALLOC_ERR)
        new_lstclear(&local_list, free);
    return (err);
}
