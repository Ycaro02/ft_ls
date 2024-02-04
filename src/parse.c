#include "../include/ft_ls.h"

static t_file   *default_file_struct(int flag, t_file_context *file_c)
{
    t_file          *file = NULL;
    int             symlink = 0;
    struct stat     *sb = check_for_stat(".", flag, &symlink);

    if (!sb)
        return (NULL);
    (void)file_c;
    // file = fill_file_struct(sb, ".", "..", symlink, file_c);
    file = fill_file_struct(sb, ".", "..", symlink);
    if (!file || !(file->name)) {
        ft_printf_fd(2, "Malloc error default file struct\n");
        return (NULL);
    }
    free(sb);
    return (file);
}


/** check_args
 *  Check path, push dir or simple list in consequence, same for found and c->error update
 * Args:    - argv from main
 *          - simple_lst: ptr on t_list ptr declare in ft_ls, push no directory file here
 *          - found: pointer on bool, update to 1 if file found
 *          - c: ptr on t_context up
 *              - update c->error to NA_CMD_LINE_ERR if can't access file
 *              - update c->space in fill_file
 * Ret: MALLOC_ERR for malloc error otherwise 0
*/
static int  check_args
(char *path, t_list **new, t_list **simple_lst, t_int8 *found, t_context *c)
{
    int         symlink = 0;
    t_file      *file = NULL;
    struct stat *sb = check_for_stat(path, c->flag_nb, &symlink);

    if (!sb) {
        *found = 1; /* signal we found another file but can't  be acces*/
        c->error = NA_CMD_LINE_ERR;
        ft_printf_fd(2, "ft_ls cannot access '%s", path);
        perror("'");
        return (0);
    }
    // file = fill_file_struct(sb, path, path, symlink, c);
    file = fill_file_struct(sb, path, path, symlink);
    if (!file)
        return (MALLOC_ERR);
    if (get_type(*sb) == DIRECTORY)
        ft_lstadd_back(new, ft_lstnew(file));
    else {
        *found = 1; /* signal we found another file not directory */
        ft_lstadd_back(simple_lst, ft_lstnew(file));
    }
    free(sb);
    return (0);
}


/** parse_cmd_args
 *  Parse commande line argument call in:cv ft_ls
 * Args:    - argv from main
 *          - simple_list: ptr on t_list ptr declare in ft_ls, push no directory file here
 *          - args_found: pointer on bool, argument found before can be uptate in check args
 *          - c: ptr on t_context 
 * Update value: fill c->error with MALLOC_ERR for distingue no dir to malloc error
 * Ret: ptr on list contain t_file, or NULL if no directory founs
*/
t_list  *parse_cmd_args(char **argv, t_list **simple_file, t_int8 *args_found, t_context *c, t_file_context *file_c)
{
    /*  ISSUE file_c can't reprendre simple_file + dir lst*/
    /* Potential fast solution file_c just represent dir_lst and we recompute for simple after buiding lst 
    ?? maybe find another solution, split this shit taking 6 args for example but how 
        - 2 list are mandatory
        - argv is the data to parse
        - args_found can be removed but realy a pain i mean
        - c and file_c are mandatory and can't be merge
            - c  is global app context
            - file_c is file_list context
        Probably not ugly to do a parse structure taking all of this
        char**data
        t_list **dir
        t_list **simple_file
        t_context* context
        t_file_context *dir_context
        t_file_context *simple_file_context
        and just give args_found to second args
         */
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
        ft_lstadd_back(&new, ft_lstnew(default_file_struct(c->flag_nb, file_c)));
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
        update_error(error);    /* Error update here if syscall stat fail */
        return (0);
    }
    free(full_path);
    new_file = fill_file_struct(sb, my_dir->d_name, file->name, symlink);
    // new_file = fill_file_struct(sb, my_dir->d_name, file->name, symlink, c);
    if (!new_file) {
        free(sb);
        return (MALLOC_ERR);
    }
    ft_lstadd_back(all, ft_lstnew(new_file));
    free(sb);
    return (0);
}

t_list* get_all_file_struct(t_file *file, int flag_nb, t_int8 *error)
{
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(file->name);
    
    if (!dir) {
        update_error(error); /* try to set error to 1 */
        return (NULL);
    }
    do  {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb, 0) == 1) {
            if (check_for_fill_struct(&all, my_dir, file, error, flag_nb) == MALLOC_ERR) { /*call to fill_file_struct is inside*/
                *error = MALLOC_ERR;    /* Set ptr to malloc error and return NULL */
                return (NULL);
            }
        }
    } while (my_dir != NULL);
    closedir(dir);
    if (!all)
        return (NULL);
    sort_lst(&all, flag_nb);
    return(all);    /* Default return, if NULL just can't open file or empty file */
}