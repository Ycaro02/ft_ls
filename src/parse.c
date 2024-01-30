#include "../include/ft_ls.h"

static t_file *default_file_struct()
{
    t_file *file;
    struct stat sb;

    file = NULL;
    if (lstat(".", &sb) == -1) {
        perror("ft_ls lstat");
        return (NULL);
    }
    file = fill_file_struct(sb, ".", "..");
    if (!file || !(file->name)) {
        perror("Malloc");
        return (NULL);
    }
    return (file);
}

static int build_file_lst(struct stat sb, char *str, t_list **new, int *found, int flag_nb)
{
    t_file *file;
    
    file = fill_file_struct(sb, str, str);
    if (!file) {
        perror("Malloc");
        return (MALLOC_ERR);
    }
    if (file->type == DIRECTORY)
        ft_lstadd_back(new, ft_lstnew(file));
    else {
        *found = 1;
        // if (flag_nb & L_OPTION)
        if (has_flag(flag_nb, L_OPTION)) {
            int array[S_HOUR + 1] = {0};
            fill_buffer_l_option(*file, array, flag_nb);
        }
        else
            ft_printf_fd(1, "%s\n", str);
        if (file->parrent)
            free(file->parrent);
        free(file->name);
        free(file);
    }
    return (0);
}


static int check_args(char *str, t_list **new, int *found, int *error, int flag_nb)
{
    struct stat sb;

    if (lstat(str, &sb) == -1) {
        *found = 1;
        *error = NA_CMD_LINE_ERR;
        ft_putstr_fd("in check args perror : ft_ls error : ", 2);
        perror(str);
        return (0);
    }
    if (build_file_lst(sb, str, new, found, flag_nb) == MALLOC_ERR)
        return (MALLOC_ERR);
    return (0);
}

t_list *get_dir_args(char **argv, int *error, int flag_nb)
{
    int i = 0;
    t_list *new = NULL;
    int found = 0;
    while (argv && argv[i]) {
        if (argv[i][0] != '-') {
            if (check_args(argv[i], &new, &found, error, flag_nb) == MALLOC_ERR) {
                *error = MALLOC_ERR;
                return (NULL);
            }
        }
        i++;
    }
    if (!new && found == 0)
        ft_lstadd_back(&new, ft_lstnew(default_file_struct()));
    return (new);
}


static int check_for_fill_struct(t_list **all, struct dirent *my_dir, t_file *file, int *error)
{
    char *full_path;
    struct stat sb;
    t_file *new_file;

    full_path = join_parent_name(file->name, my_dir->d_name);
    if (!full_path)
        return (MALLOC_ERR);
    if (lstat(full_path, &sb) == -1) {
        print_error(full_path, "ft_ls: cannot open directory : ", NO_ACCESS_ERR, 0);
        free(full_path);
        update_error(error);
        return (0);
    }
    free(full_path);
    new_file = fill_file_struct(sb, my_dir->d_name, file->name);
    if (!new_file)
        return (MALLOC_ERR);
    ft_lstadd_back(all, ft_lstnew(new_file));
    return (0);
}

t_list* get_all_file_struct(t_file *file, int flag_nb, int *error)
{
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(file->name);
    if (!dir)
        return (NULL);
    do  {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb, 0) == 1) {
            if (check_for_fill_struct(&all, my_dir, file, error) == MALLOC_ERR) {
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