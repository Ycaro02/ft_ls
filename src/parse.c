#include "../include/ft_ls.h"

static t_file   *default_file_struct(int flag)
{
    t_file          *file = NULL;
    int             symlink = 0;
    struct stat     *sb = check_for_stat(".", flag, &symlink);

    if (!sb)
        return (NULL);
    file = fill_file_struct(sb, ".", "..", symlink);
    if (!file || !(file->name)) {
        ft_printf_fd(2, "Malloc error default file struct\n");
        return (NULL);
    }
    free(sb);
    return (file);
}

static int  check_args
(char *str, t_list **new, t_list **simple_file, t_int8 *found, t_context *c)
{
    int         symlink = 0;
    struct stat *sb = check_for_stat(str, c->flag_nb, &symlink);
    t_file      *file = NULL;

    if (!sb) {
        *found = 1;
        c->error = NA_CMD_LINE_ERR;
        display_error_phrase(str);
        perror("'");
        return (0);
    }
    file = fill_file_struct(sb, str, str, symlink);
    if (!file)
        return (MALLOC_ERR);
    if (get_type(*sb) == DIRECTORY)
        ft_lstadd_back(new, ft_lstnew(file));
    else {
        *found = 1;
        ft_lstadd_back(simple_file, ft_lstnew(file));
    }
    free(sb);
    return (0);
}

t_list  *get_dir_args(char **argv, t_list **simple_file, t_int8 *args_found, t_context *c)
{
    int     i = 0;
    t_list  *new = NULL;

    while (argv && argv[i]) {
        if (argv[i][0] != '-') {
            if (check_args(argv[i], &new,  simple_file, args_found, c) == MALLOC_ERR) {
                c->error = MALLOC_ERR;
                return (NULL);
            }
        }
        i++;
    }
    if (!new && *args_found == 0) /* default search if nothing found */
        ft_lstadd_back(&new, ft_lstnew(default_file_struct(c->flag_nb)));
    return (new);
}

static int  check_for_fill_struct(t_list **all, struct dirent *my_dir, t_file *file, t_int8 *error, int flag)
{
    struct stat     *sb;
    t_file          *new_file;
    int             symlink = 0;
    char            *full_path = join_parent_name(file->name, my_dir->d_name);
    
    if (!full_path)
        return (MALLOC_ERR);
    sb = check_for_stat(full_path, flag, &symlink);
    if (!sb) {
        update_error(error);
        return (0);
    }
    free(full_path);

    new_file = fill_file_struct(sb, my_dir->d_name, file->name, symlink);
    if (!new_file)
        return (MALLOC_ERR);
    ft_lstadd_back(all, ft_lstnew(new_file));
    free(sb);
    return (0);
}

t_list* get_all_file_struct(t_file *file, int flag_nb, t_int8 *error)
{
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(file->name);
    
    if (!dir)
        return (NULL);
    do  {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb, 0) == 1) {
            if (check_for_fill_struct(&all, my_dir, file, error, flag_nb) == MALLOC_ERR) {
                *error = MALLOC_ERR;
                return (NULL);
            }
        }
    } while (my_dir != NULL);
    closedir(dir);
    if (!all)
        return (NULL);
    sort_lst(all, flag_nb);
    return(all); 
}