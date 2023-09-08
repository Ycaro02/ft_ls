#include "../ft_ls.h"

static int is_point_dir(char *path)
{
    if (!path)
        return (1);
    if (path[0] == '.')
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


static char **join_str_tab(char **tab1, char** tab2)
{
    int i = 0;
    int len1 = count_char_tab(tab1);
    int len2 = count_char_tab(tab2);
    if (len1 + len2 == 0)
        return (NULL);
    char **new = malloc(sizeof(char *) * (len1 + len2 + 1));
    while (tab1 && tab1[i])
    {
        new[i] = ft_strdup(tab1[i]);
        i++;
    }
    for (int j = 0; tab2 && tab2[j]; j++)
    {
        new[i] = ft_strdup(tab2[j]);
        i++;
    }
    new[i] = NULL;
    free_all(tab1);
    free_all(tab2);
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
        // if (strcmp(parent, path[i]) != 0)
        // {
            str = ft_strjoin(parent, "/");
            str = ft_strjoin_free(str, path[i], 'f');
            new = ft_realloc_str(new, str);
            free(str);
        // }
        i++;
    }
    free_all(path);
    return (new);
}

static char    **parse_rec_dir_lst(char **dir_lst)
{
    char **new = NULL;
    for (int i = 0; dir_lst && dir_lst[i]; i++)
    {
        if (access(dir_lst[i], F_OK) == 0 && is_directory(dir_lst[i]) == 0)
            new = ft_realloc_str(new, dir_lst[i]);
    }
    free_all(dir_lst);
    return (new);
}

static char **build_local_list(char *str)
{
    char **local_list = NULL;
    local_list = get_all_file_name(str);
    local_list = get_dir_without_point(local_list);
    local_list = add_parent_path(str, local_list);
    local_list = parse_rec_dir_lst(local_list);
    // printf("\nlocal lst = \n");
    // print_tab(local_list);
    return (local_list);
}

char **search_recurcive_dir(char **dir_lst)
{
    for (int i = 0; dir_lst && dir_lst[i]; i++)
    {
        if (is_point_dir(dir_lst[i]) == 0)
        {
            char **local_list = build_local_list(dir_lst[i]);
            if (local_list == NULL)
                return (dir_lst);
            local_list = search_recurcive_dir(local_list);

            dir_lst = join_str_tab(dir_lst, local_list);
            if (dir_lst == NULL)
            {
                printf("error locallist is NULL\n");
                return(NULL);
            }
            // return (new);
            // printf("\nlocal list\n");
            // print_tab(local_list);
        }
    }
    return (dir_lst);
}
