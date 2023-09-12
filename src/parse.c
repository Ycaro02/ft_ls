#include "../ft_ls.h"

static int check_for_dir(char *dir_name)
{
    int nb = -1;

    if (access(dir_name, F_OK) != 0)
        printf("\nft_ls: cannot access '%s': No such file or directory\n", dir_name);
    else if (is_directory(dir_name) == 1)
        printf("\n%s\n", dir_name);
    else if (is_directory(dir_name) == 0 && access(dir_name, R_OK) != 0)
        printf("ft_ls: cannot open directory '%s': Permission denied\n", dir_name);
    else if (is_directory(dir_name) == 0 && access(dir_name, R_OK) == 0)
        nb = 0;
    return (nb);
}

t_list *get_dir_no_hiden(char **argv)
{
    int i = 0;
    t_list *new = NULL;
    while (argv && argv[i])
    {
        if (argv[i][0] != '-' && check_for_dir(argv[i]) == 0)
            ft_lstadd_back(&new, ft_lstnew(ft_strdup(argv[i])));
        i++;
    }
    return (new);
}

t_list *get_all_file_name(const char *dir_name, int hiden_file)
{
    t_list *all = NULL;
    struct dirent *my_dir;
    DIR *dir = opendir(dir_name);
    do 
    {
        if (access(dir_name, F_OK) != 0)
        {
            printf("\nft_ls: (get all file name) cannot access '%s': No such file or directory\n", dir_name);
            break;
        }
        my_dir = readdir(dir);
        if (my_dir && is_point_dir(my_dir->d_name) == hiden_file)
                ft_lstadd_back(&all, ft_lstnew(ft_strdup(my_dir->d_name)));
    } while (my_dir != NULL);
    sort_by_name(all);
    closedir(dir);
    return(all);
}