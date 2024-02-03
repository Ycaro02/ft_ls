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
    struct stat *sb;
    int symlink = 0, error = 0;
    char *str = join_parent_name(file->name, my_dir->d_name);
    
    if (!str)
        return (MALLOC_ERR);
    sb = check_for_stat(str, flag, &symlink);
    if (!sb)
        return (0);
    if (create_new_file(*sb, new, str, symlink) == MALLOC_ERR)
        error = (MALLOC_ERR);
    free(sb);
    free(str);
    return (error);
}

static int read_dir(t_file *file, t_list **new, int flag_nb)
{
    struct dirent   *my_dir = NULL;
    DIR             *dir = opendir(file->name);
    int             ret = 0;
    
    if (!dir)
        return (1);
    do  {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb, 1) == 1) {
            ret = parse_directory(file, my_dir, new, flag_nb);
            if (ret != 0) {
                ft_printf_fd(2, "Malloc error\n");
                break;
            }
        }
    } while (my_dir != NULL);
    closedir(dir);
    return (ret);
}

static t_list *get_recurcive_dir(t_file *file, int flag_nb, t_int8 *error)
{
    t_list  *new = NULL;

    if (file->type != DIRECTORY)
        return (NULL);
    int ret = read_dir(file, &new, flag_nb) == 1;
    if (ret != 0) {
        if (ret == MALLOC_ERR)
            *error = MALLOC_ERR;
        else
            update_error(error);
        return (NULL);
    }
    sort_lst(&new, flag_nb);
    return (new);
}

static int recurcive_ls(t_list *dir_lst, t_context *c, t_file_context *file_c)
{
    int err = 0;

    if (has_flag(c->flag_nb, L_OPTION))
        err = ls_l_one_dir(dir_lst->content, c, file_c);
    else {
        err = ls_one_dir(dir_lst->content, c, file_c);
    }
    if (file_c->call_flag == 1)
        file_c->call_flag = 2;
    if (err == MALLOC_ERR)
        return (err);
    return (err);
} 

static int safe_recurcive(t_list *local_list, t_context *c, int call_flag)
{
    int err = 0;

    if (local_list)
        err = search_recurcive_dir(local_list, c, call_flag);
    if (err == MALLOC_ERR)
        return (err);
    return (err);
}

int search_recurcive_dir(t_list *dir_lst, t_context *c, int call_flag)
{
    t_list *local_list = NULL;
    int     err = 0;
    t_file_context file_c;
    file_c.idx = 0;
    file_c.lst_len = ft_lstsize(dir_lst);
    file_c.call_flag = call_flag;

    while(dir_lst) {
        err = recurcive_ls(dir_lst, c, &file_c);
        if (err == MALLOC_ERR)
            break ;
        local_list = get_recurcive_dir(dir_lst->content, c->flag_nb, &c->error);
        err = safe_recurcive(local_list, c, file_c.call_flag);
        if (file_c.call_flag == 1)
            ++(file_c.call_flag);
        if (c->error == MALLOC_ERR || err == MALLOC_ERR)
            break ;
        file_lstclear(&local_list, free);
        ++(file_c.idx);
        dir_lst = dir_lst->next;
    }
    file_lstclear(&local_list, free);
    return (err);
}