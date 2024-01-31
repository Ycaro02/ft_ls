#include "../include/ft_ls.h"

static t_file *default_file_struct(int flag)
{
    t_file *file = NULL;
    int symlink = 0;

    struct stat *sb = check_for_stat(".", flag, &symlink);

    if (!sb)
        return (NULL);

    file = fill_file_struct(sb, ".", "..", symlink);
    if (!file || !(file->name)) {
        perror("Malloc");
        return (NULL);
    }
    return (file);
}

static int build_file_lst(struct stat sb, char *str, t_list **new, int *found, int flag_nb, int symlink)
{
    t_file *file = NULL;
    
    file = fill_file_struct(&sb, str, str, symlink);
    if (!file) {
        perror("Malloc");
        return (MALLOC_ERR);
    }
    // if (file->type == DIRECTORY)
    if (get_type(sb) == DIRECTORY)
        ft_lstadd_back(new, ft_lstnew(file));
    else {
        *found = 1;
        if (has_flag(flag_nb, L_OPTION)) {
            int array[S_MAX] = {0}; /* Todo malloc here */
            fill_buffer_l_option(*file, array, flag_nb);
        }
        else {
            int is_exec = 1;
            char *perm = get_perm(file->perm);
            if (perm) {
                for (int i = 0; perm && perm[i]; ++i)
                    fill_buffer_perm(perm[i], &is_exec, 0);
                write_file_name(*file, 0, flag_nb, is_exec);
                free(perm);
            }
        }
        if (file->parrent)
            free(file->parrent);
        free(file->name);
        free(file);
    }
    return (0);
}

static int check_args(char *str, t_list **new, int *found, int *error, int flag_nb)
{
    int symlink = 0;
    struct stat *sb = check_for_stat(str, flag_nb, &symlink);

    if (!sb) {
        *found = 1;
        *error = NA_CMD_LINE_ERR;
        display_error_phrase(str);
        perror("'");
        return (0);
    }

    if (build_file_lst(*sb, str, new, found, flag_nb, symlink) == MALLOC_ERR) {
        free(sb);
        return (MALLOC_ERR);
    }

    free(sb);
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
        ft_lstadd_back(&new, ft_lstnew(default_file_struct(flag_nb)));
    return (new);
}


static int check_for_fill_struct(t_list **all, struct dirent *my_dir, t_file *file, int *error, int flag)
{
    char *full_path;
    struct stat *sb;
    t_file *new_file;
    int symlink = 0;

    full_path = join_parent_name(file->name, my_dir->d_name);
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