#include "../ft_ls.h"

t_list *get_dir_args(char **argv)
{
    int i = 0;
    t_list *new = NULL;
    while (argv && argv[i])
    {
        if (argv[i][0] != '-')
        {
            struct stat sb;
            if (lstat(argv[i], &sb) == -1)
            {
                perror(argv[i]);
                new_lstclear(&new, free);
                return (NULL);
            }
            t_file *file = fill_file_struct(sb, argv[i]);
            if (!file || !file->name)
            {
                printf("Malloc error get fir args\n");
                return (NULL);
            }
            if (file->type == DIRECTORY)
                ft_lstadd_back(&new, ft_lstnew(file));
            else
            {
                printf("%s\n", argv[i]);
                free(file->name);
                free(file);
            }
        }
        i++;
    }
    return (new);
}

t_list* get_all_file_struct(t_file *file, int flag_nb)
{
    (void)flag_nb;
    if (file->type != 'd')
        return NULL;
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(file->name);
    if (!dir)
        return (NULL);
    do 
    {
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name, flag_nb) == 1)
        {
            char *str = join_parent_name(file->name, my_dir->d_name);
            struct stat sb;
            if (!str || lstat(str, &sb) == -1)
            {
                printf("Malloc error || ");
                printf("ls stat faillure for %s\n", str);
                continue ;
            }
            t_file *file = fill_file_struct(sb, my_dir->d_name);
            ft_lstadd_back(&all, ft_lstnew(file));
            if (str)
                free(str);
        }
    } while (my_dir != NULL);
    closedir(dir);
    if (!all)
        return (NULL);
    sort_lst(all, flag_nb);
    return(all); 
}