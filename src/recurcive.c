#include "../ft_ls.h"

static int is_point_dir(char *path)
{
    if (path && path[0] == '.')
        return (0);
    return (1);
}

static char **get_dir_without_point(char **dir_lst)
{
    int i = 0;
    char **new = NULL;
    while (dir_lst && dir_lst[i])
    {
        if (dir_lst[i][0] != '-' && is_point_dir(dir_lst[i]) == 1)
            new = ft_realloc_str(new, dir_lst[i]);
        i++;
    }
    free_all(dir_lst);
    return (new);
}

static char **add_parent_path(char *parent, char** path)
{
    int len = count_char_tab(path);
    if (len == 0)
        return (path);
    char **new = NULL;
    int i = 0;
    while (path && path[i])
    {
        char *str = NULL;
		if (strcmp(parent, "/") != 0)
        	str = ft_strjoin(parent, "/");
        else
			str = ft_strdup("/");
		str = ft_strjoin_free(str, path[i], 'f');
        new = ft_realloc_str(new, str);
        free(str);
        i++;
    }
    free_all(path);
    return (new);
}

static char    **parse_rec_dir_lst(char **dir_lst, int first)
{
    char **new = NULL;
    for (int i = 0; dir_lst && dir_lst[i]; i++)
    {
        if (access(dir_lst[i], F_OK | R_OK) == 0 && is_directory(dir_lst[i]) == 0)
            new = ft_realloc_str(new, dir_lst[i]);
    }
    (void)first;
    free_all(dir_lst);
    return (new);
}

static char **build_local_list(char *str, int first)
{
    char **local_list = NULL;
    local_list = get_all_file_name(str);
    local_list = get_dir_without_point(local_list);
    local_list = add_parent_path(str, local_list);
    local_list = parse_rec_dir_lst(local_list, first);
//    ft_putstr_fd("\nlocal lst for str ", 2);
//    ft_putstr_fd(str, 2);
//    ft_putstr_fd("\n\n", 2);
//    print_tab(local_list);
//    ft_putstr_fd("\n\n", 2);
    return (local_list);
}


void search_recurcive_dir(char **dir_lst, int flag_nb, int first)
{
    int len = count_char_tab(dir_lst);
    for (int i = 0; dir_lst && dir_lst[i] && i < len; i++)
    {
        ls_one_dir(dir_lst[i], flag_nb);
        char **local_list = build_local_list(dir_lst[i], first);
        search_recurcive_dir(local_list, flag_nb, 1);
        free_all(local_list);
    }
    (void)first;
    // if (dir_lst && first == 0)
    //     ft_ls(dir_lst, flag_nb, 1);
    // return (dir_lst);
}