#include "../include/ft_ls.h"

// static int parse_directory(char *str, t_list **new, int flag)
static int parse_directory(char *str, t_list **new, t_context *c, t_file_context *file_c)
{
    t_file *new_file ;
    struct stat *sb;
    int symlink = 0, error = 0, l_option = has_flag(c->flag_nb, L_OPTION);
    
    if (!str)
        return (MALLOC_ERR);
    sb = check_for_stat(str, c->flag_nb, &symlink);
    if (!sb) {
        free(str);
        return (0);
    }
    if (get_type(*sb) == DIRECTORY && !symlink) { /* if is directory and not symlink*/
            if (l_option) {
                if (!file_c->space) {
                    file_c->space = ft_calloc(sizeof(int), S_MAX);
                    if (!file_c->space)
                        return (MALLOC_ERR);
                }
            }
            file_c->path = str;
            file_c->parent_path = NULL;
            new_file = fill_file_struct(sb, symlink, c, file_c);
            if (!new_file) {
                free(str);
                free(sb);
                return (MALLOC_ERR);
            }
            ft_lstadd_back(new, ft_lstnew(new_file));
    }
    else
        free(sb);
    free(str);
    return (error);
}

// static int recurcive_readir(t_file *file, t_list **new, int flag_nb)
static int recurcive_readir(t_file *file, t_list **lst, t_context *c, t_file_context *file_c)
{
    struct dirent   *my_dir = NULL;
    DIR             *dir = opendir(file->name);
    int             ret = 0;
    
    if (!dir) {
        update_error(&c->error);
        return (0);
    }
    do  {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, c->flag_nb, 1) == 1) { /* if isn't . .. obsiously, check hiden accordate to flag*/
            char *str = join_parent_name(file->name, my_dir->d_name);
            ret = parse_directory(str, lst, c, file_c);
            if (ret != 0) {
                free(str);
                ft_printf_fd(2, "Malloc error\n");
                break;
            }
            // free(str);
        }
    } while (my_dir != NULL);
    closedir(dir);
    return (ret);
}

/** get recursive dir
 * Call recurcive_readir function and check result to update error or sort
 * Ret: t_list *
*/
// static t_list *get_recurcive_dir(t_file *file, int flag_nb, t_int8 *error)
static t_list *get_recurcive_dir(t_file *file, t_context *c, t_file_context *file_c)
{
    t_list  *lst = NULL;

    if (file->type != DIRECTORY) /* i mean not mandatory, directory already slect before push in lst */
        return (NULL);
    int ret = recurcive_readir(file, &lst, c, file_c) == 1;
    if (ret != 0) {
        if (ret == MALLOC_ERR)
            c->special_error = MALLOC_ERR;
        return (NULL);
    }
    sort_lst(&lst, c->flag_nb);
    return (lst);
}

static int recurcive_ls(t_list *dir_lst, t_context *c, t_file_context *file_c)
{
    int err = 0;

    if (has_flag(c->flag_nb, L_OPTION))
        err = ls_l_one_dir(dir_lst->content, c, file_c);
    else {
        err = ls_one_dir(dir_lst->content, c, file_c);
    }
    if (file_c->call_flag == 1) /* ONE TO REMOVE */
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
    file_c.space = NULL;
    file_c.lst_len = ft_lstsize(dir_lst);
    file_c.call_flag = call_flag;

    while(dir_lst) {
        err = recurcive_ls(dir_lst, c, &file_c);
        if (err == MALLOC_ERR)
            break ;
        local_list = get_recurcive_dir(dir_lst->content, c, &file_c);
        err = safe_recurcive(local_list, c, file_c.call_flag);
        if (c->special_error == MALLOC_ERR || err == MALLOC_ERR)
            break ;
        ft_lstclear(&local_list, destroy_file);
        if (file_c.call_flag == 1) /* ONE TO REMOVE */
            ++(file_c.call_flag);
        ++(file_c.idx);
        dir_lst = dir_lst->next;
    }

    if (has_flag(c->flag_nb, L_OPTION) && file_c.space) {
        free(file_c.space);
        file_c.space = NULL;
    }
    ft_lstclear(&local_list, destroy_file);
    return (err);
}