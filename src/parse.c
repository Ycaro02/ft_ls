#include "../include/ft_ls.h"

static t_file   *default_file_struct(t_context *c, t_file_context *file_c)
{
    t_file          *file = NULL;
    int             symlink = 0, l_option = has_flag(c->flag_nb, L_OPTION);;
    char            *point_dir = ".";
    struct stat     *sb = check_for_stat(point_dir, c->flag_nb, &symlink);

    (void)l_option;
    if (!sb)
        return (NULL);
    file_c->path = point_dir;
    file_c->parent_path = NULL;

    file = fill_file_struct(sb, symlink, c, file_c);
    if (!file) {
        ft_printf_fd(2, "Malloc error default file struct\n");
        return (NULL);
    }
    return (file);
}


// static int try_opendir(char *path, t_context *c)
// {
//     DIR *dir = opendir(path);
//     if (!dir) {
//         ft_printf_fd(2, "ft_ls cannot access '%s", path);
//         perror("'");
//         c->error = 2;
//         closedir(dir);
//         return (FALSE);
//     }
//     closedir(dir);
//     return (TRUE);
// }

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
static int  check_args(char *path, t_args *arg, t_int8 *found)
{
    int             symlink = 0;
    t_file          *file = NULL;
    struct stat     *sb = check_for_stat(path, arg->c.flag_nb, &symlink);
    t_int8          is_directory = 0;
    if (!sb) {
        *found = 1; /* signal we found another file but can't  be acces*/
        arg->c.error = NA_CMD_LINE_ERR;
        ft_printf_fd(2, "ft_ls cannot access '%s", path);
        perror("'");
        return (0);
    }
    is_directory = get_type(*sb) == DIRECTORY; /* get dir type for add in lst */

    arg->file_c.path = path;
    arg->file_c.parent_path = path; /* NULL MAYBE */
    file = fill_file_struct(sb, symlink, &arg->c, &arg->file_c);
    if (!file)
        return (MALLOC_ERR);
    if (is_directory){
        // int open = try_opendir(file->name, &arg->c);
        // if (open == FALSE) {
        //     *found = 1; /* signal we found another file but can't  be acces*/
        //     free(arg->file_c.space);
        //     destroy_file(file);
        //     return (0);
        // }
        ft_lstadd_back(&arg->dir_lst, ft_lstnew(file));
    }
    else {
        *found = 1; /* signal we found another file not directory */
        ft_lstadd_back(&arg->simple_file, ft_lstnew(file));
    }
    // free(sb);
    return (0);
}

/** parse_cmd_args
 *  Parse commande line argument call in:cv ft_ls
 * Args:    - argv from main
 *          - arg: t_args structure ptr containing
 *              - simple_list: ptr on t_list ptr declare in ft_ls, push no directory file here
 *              - dir_ls: ptr on t_list ptr declare in ft_ls, push directory here
 *              - c: ptr on t_context 
 *              - file_c: ptr on t_file_context for simple_list 
 * Ret: replace args_found: found value 0 for nothing, 1 for yes or MALLOC_ERR
*/
t_int8 parse_cmd_args(char **argv, t_args *arg)
{
    t_int8      file_found = 0; /*default ret value update if args found or malloc error */
    int         i = 0;


    if (has_flag(arg->c.flag_nb, L_OPTION)) {
        arg->file_c.space = ft_calloc(sizeof(int), S_MAX);
        if (!arg->file_c.space)
            return (MALLOC_ERR);
    }
    while (argv && argv[i]) {
        if (argv[i][0] != '-') {
            if (check_args(argv[i], arg, &file_found) == MALLOC_ERR)
                return (MALLOC_ERR);
        }
        ++i;
    }
    if (!arg->dir_lst && file_found == 0) /* default search if nothing found */
        ft_lstadd_back(&arg->dir_lst, ft_lstnew(default_file_struct(&arg->c, &arg->file_c)));
    
    return (file_found);
}


// static int  check_for_fill_struct(t_list **all, struct dirent *my_dir, t_file *file, t_int8 *error, int flag)
static int  check_for_fill_struct(t_list **all, struct dirent *my_dir, t_file *file, t_context *c, t_file_context *file_c)
{
    struct stat     *sb;
    t_file          *new_file;
    int             symlink = 0;
    char            *full_path = join_parent_name(file->name, my_dir->d_name);
    
    if (!full_path)
        return (MALLOC_ERR);
    sb = check_for_stat(full_path, c->flag_nb, &symlink);
    if (!sb) {
        update_error(&c->error);    /* Error update here if syscall stat fail */
        return (0);
    }
    free(full_path);

    file_c->path = my_dir->d_name;
    file_c->parent_path = file->name;
    new_file = fill_file_struct(sb, symlink, c, file_c);

    if (!new_file)
        return (MALLOC_ERR);
    ft_lstadd_back(all, ft_lstnew(new_file));
    return (0);
}

t_list* get_all_file_struct(t_file *file, t_context *c, t_file_context *file_c)
{
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(file->name);
    /* New call version check fir fill here */
    
    if (!dir) {
        // printf("TO failled on file->name %s\n", file->name);
        update_error(&c->error); /* try to set error to 1 */
        return (NULL);
    }
    do  {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, c->flag_nb, 0) == 1) {
            if (check_for_fill_struct(&all, my_dir, file, c, file_c) == MALLOC_ERR) {
                c->error = MALLOC_ERR;    /* Set ptr to malloc error and return NULL */
                return (NULL);
            }
        }
    } while (my_dir != NULL);
    closedir(dir);
    if (!all)
        return (NULL);
    sort_lst(&all, c->flag_nb);
    return(all);    /* Default return, if NULL just can't open file or empty file */
}