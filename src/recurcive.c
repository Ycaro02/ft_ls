#include "../include/ft_ls.h"

static int create_new_file(struct stat sb, t_list **new, char *str, int symlink)
{
    t_file *new_file;
    
    new_file = fill_file_struct(&sb, str, NULL, symlink);
    if (!new_file || !new_file->name) {
        ft_printf_fd(2, "Malloc error\n");
        free(new_file->name);
        free(new_file);
        return (MALLOC_ERR);
    }
    if (new_file->type == DIRECTORY)
        ft_lstadd_back(new, ft_lstnew(new_file));
    else {
        free(new_file->name);
        free(new_file);
    }
    return (0);
}

static int parse_directory(t_file *file, struct dirent* my_dir, t_list **new, int flag)
{
    char *str;
    struct stat *sb;
    int symlink = 0;

    str = join_parent_name(file->name, my_dir->d_name);
    if (!str)
        return (print_error("Malloc error\n", NULL, 1, 1));
    sb = check_for_stat(str, flag, &symlink);
    if (!sb)
        return (0);
    if (create_new_file(*sb, new, str, symlink) == MALLOC_ERR) {
        free(str);
        return (MALLOC_ERR);
    }
    free(sb);
    free(str);
    return (0);
}

static int read_dir(t_file *file, t_list **new, int flag_nb)
{
    struct dirent *my_dir;
    DIR *dir;
    int ret;

    ret = 0;
    dir = opendir(file->name);
    if (!dir)
        return (print_error("Opendir error\n", NULL, 1, 1));
    do  {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb, 1) == 1) {
            ret = parse_directory(file, my_dir, new, flag_nb);
            if (ret != 0)
                break;
        }
    } while (my_dir != NULL);
    closedir(dir);
    return (ret);
}

t_list *get_recurcive_dir(t_file *file, int flag_nb, int *error)
{
    t_list *new = NULL;
    if (file->type != DIRECTORY)
        return (NULL);
    int ret = read_dir(file, &new, flag_nb) == 1;
    if (ret != 0) {
        if (ret == MALLOC_ERR)
            *error = MALLOC_ERR;
        else {
            ft_putstr_fd("ft_ls: cannot open directory : ", 2);
            perror(file->name);
            update_error(error);
        }
        return (NULL);
    }
    sort_lst(new, flag_nb);
    if (new && has_flag(flag_nb, REVERSE_OPTION))
        safe_reverse_lst(&new, error, flag_nb);
    return (new);
}

static int recurcive_ls(t_list *dir_lst, int flag_nb ,int* error, int lst_len, int idx, int *call_flag)
{
    int err;
    
    err = 0;
    if (has_flag(flag_nb, L_OPTION))
        err = ls_l_one_dir(dir_lst->content, flag_nb, lst_len, error, *call_flag, idx);
    else {
        err = ls_one_dir(dir_lst->content, flag_nb, lst_len, error, *call_flag, idx);
    }
    if (*call_flag == 1)
        *call_flag = 2;
    if (err == MALLOC_ERR)
        return (err);
    return (err);
} 

int safe_recurcive(t_list *local_list, int flag_nb, int* error, int call_flag)
{
    int err;

    err = 0;
    if (local_list)
        err = search_recurcive_dir(local_list, flag_nb, error, call_flag);
    if (err == MALLOC_ERR)
        return (err);
    return (err);
}

int search_recurcive_dir(t_list *dir_lst, int flag_nb, int *error, int call_flag)
{
    t_list *local_list;
    int err = 0, index = 0, lst_len = get_lst_len(dir_lst);

    local_list = NULL;
    while(dir_lst)
    {
        /* nned to take call flag, 0 for first time and increment it */
        err = recurcive_ls(dir_lst, flag_nb, error, lst_len, index, &call_flag);
        if (err == MALLOC_ERR)
            break ;
        local_list = get_recurcive_dir(dir_lst->content, flag_nb, error);
        err = safe_recurcive(local_list, flag_nb, error, call_flag);
        if (*error == MALLOC_ERR || err == MALLOC_ERR)
            break ;
        new_lstclear(&local_list, free);
        ++index;
        dir_lst = dir_lst->next;
    }
    new_lstclear(&local_list, free);
    return (err);
}
