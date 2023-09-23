#include "../include/ft_ls.h"

static int create_new_file(struct stat sb, t_list **new, char *str, char *parent)
{
    t_file *new_file;
    
    (void)parent;
    new_file = fill_file_struct(sb, str, NULL);
    if (!new_file || !new_file->name)
    {
        printf("Malloc error get recurse dir\n");
        free(new_file->name);
        free(new_file);
        return (1);
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

static int parse_directory(t_file *file, struct dirent* my_dir, t_list **new, char*parent)
{
    char *str;
    struct stat sb;

    str = join_parent_name(file->name, my_dir->d_name);
    if (!str)
    {
        printf("Malloc error join parent name\n");
        return (1);
    }
    if (lstat(str, &sb) == -1)
    {
        ft_putstr_fd("ft_ls: cannot access directory : ", 2);
        perror(str);
        free(str);
        new_lstclear(new, free);
        return (1);
    }
    if (create_new_file(sb, new, str, parent) == 1)
    {
        free(str);
        return (1);
    }
    free(str);
    return (0);
}

static int read_dir(t_file *file, t_list **new, int flag_nb, char* parent)
{
    struct dirent *my_dir;
    DIR *dir;

    dir = opendir(file->name);
    if (!dir)
        return (1);
    do 
    {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb) == 1)
        {
            if (parse_directory(file, my_dir, new, parent) == 1)
            {
                closedir(dir);
                return (1);
            }
        }
    } while (my_dir != NULL);
    closedir(dir);
    return (0);
}

t_list *get_recurcive_dir(t_file *file, int flag_nb, char* parent)
{
    t_list *new;

    new = NULL;
    if (file->type != DIRECTORY || read_dir(file, &new, flag_nb, parent) == 1)
    {
        ft_putstr_fd("ft_ls: cannot open directory : ", 2);
        perror(file->name);
        return (NULL);
    }
    sort_lst(new, flag_nb);
    if (new && flag_nb & REVERSE_OPTION)
    {
        t_list *reverse = NULL;
        reverse_lst(new, &reverse);
        free_node_ptr(&new);
        new = reverse;
    }
    return (new);
}

void search_recurcive_dir(t_list *dir_lst, int flag_nb)
{
    t_list *local_list;

    local_list = NULL;
    int lst_len = get_lst_len(dir_lst);
    while(dir_lst)
    {
        t_file *file = dir_lst->content;
        if (flag_nb & L_OPTION)
            ls_l_one_dir(dir_lst->content, flag_nb, lst_len);
        else
            ls_one_dir(dir_lst->content, flag_nb, lst_len);
        local_list = get_recurcive_dir(file, flag_nb, file->name);
        if (local_list)
        {
            search_recurcive_dir(local_list, flag_nb);
            new_lstclear(&local_list, free);
        }
            new_lstclear(&local_list, free);
        dir_lst = dir_lst->next;
    }
}
