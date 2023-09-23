#include "../include/ft_ls.h"

static t_file *default_file_struct()
{
    t_file *file;
    struct stat sb;

    file = NULL;
    if (lstat(".", &sb) == -1)
    {
        perror("lstat for current dir");
        return (NULL);
    }
    file = fill_file_struct(sb, ".", "..");
    if (!file || !(file->name))
    {
        perror("Malloc Error ft_ls");
        return (NULL);
    }
    return (file);
}

static int build_file_lst(struct stat sb, char *str, t_list **new, int *found)
{
    t_file *file;
    
    file = fill_file_struct(sb, str, str);
    if (!file || !file->name)
    {
        printf("Malloc error build file lst args\n");
        return (1);
    }
    if (file->type == DIRECTORY)
        ft_lstadd_back(new, ft_lstnew(file));
    else
    {
        *found = 1;
        printf("%s\n", str);
        free(file->name);
        free(file);
    }
    return (0);
}


static int check_args(char *str, t_list **new, int *found, int *error)
{
    struct stat sb;

    if (lstat(str, &sb) == -1)
    {
        *found = 1;
        *error = NA_CMD_LINE_ERR;
        ft_putstr_fd("in check args perror : ft_ls error : ", 2);
        perror(str);
        return (0);
    }
    if (build_file_lst(sb, str, new, found) == 1)
        return (1);
    return (0);
}

t_list *get_dir_args(char **argv, int *error)
{
    int i;
    t_list *new;
    int found;

    found = 0;
    new = NULL;
    i = 0;
    while (argv && argv[i])
    {
        if (argv[i][0] != '-')
        {
            if (check_args(argv[i], &new, &found, error) == 1)
                return (NULL);
        }
        i++;
    }
    if (!new && found == 0)
        ft_lstadd_back(&new, ft_lstnew(default_file_struct()));
    return (new);
}


static int check_for_fill_struct(t_list **all, struct dirent *my_dir, int flag_nb, t_file *file)
{
    char *str;
    struct stat sb;
    t_file *new_file;

    if (my_dir && is_point_dir(my_dir->d_name, flag_nb) == 1)
    {
        str = join_parent_name(file->name, my_dir->d_name);
        if (!str)
            return (1);
        if (lstat(str, &sb) == -1)
        {
            ft_putstr_fd("in check for fill struct perror : ls: cannot open directory : ", 2);
            perror(str);
            free(str);
            return (0);
        }
        new_file = fill_file_struct(sb, my_dir->d_name, file->name);
        if (!new_file)
            return (1);
        ft_lstadd_back(all, ft_lstnew(new_file));
        free(str);
    }
    return (0);
}

t_list* get_all_file_struct(t_file *file, int flag_nb)
{
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(file->name);
    if (!dir)
        return (NULL);
    do 
    {
        my_dir = readdir(dir);
        if (check_for_fill_struct(&all, my_dir, flag_nb, file) == 1)
            return (NULL);
    } while (my_dir != NULL);
    closedir(dir);
    if (!all)
        return (NULL);
    sort_lst(all, flag_nb);
    return(all); 
}