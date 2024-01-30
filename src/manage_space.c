#include "../include/ft_ls.h"

static int get_len_size(t_file file)
{
    char    *tmp;
    int     max = 0;

    // if (file.type == CHARACTER)
    // {
    //     char *tmp = ft_strjoin_free(ft_itoa(MAJOR(file.rdev)), ", ", 'f');
    //     if (!tmp)
    //         return (MALLOC_ERR);
    //     tmp = ft_strjoin_free(tmp, ft_itoa(MINOR(file.rdev)), 'a');
    //     if (!tmp)
    //         return (MALLOC_ERR);
    //     max = ft_strlen(tmp);
    //     free(tmp);
    //     return (max);
    // }
    if (file.type != CHARACTER)
    {
        tmp = ft_ultoa(file.size);
        if (!tmp)
            return (MALLOC_ERR);
        max = ft_strlen(tmp);
        free(tmp);
    }
    return (max);
}

static int get_user_id_len(t_file file)
{
    char *tmp = ft_ltoa(file.user_id);
    if (!tmp)
        return (MALLOC_ERR);
    int nb = ft_strlen(tmp);
    free(tmp);
    return (nb);
}

static int get_group_id_len(t_file file)
{
    char *tmp = ft_ltoa(file.group_id);
    if (!tmp)
        return (MALLOC_ERR);
    int nb = ft_strlen(tmp);
    free(tmp);
    return (nb);
}

static int get_group_name_len(t_file file)
{
    int len = 0;

    struct group* group = getgrgid(file.group_id);
    if (!group)
        return (get_group_id_len(file));
    len = ft_strlen(group->gr_name);
    return (len);
}

static int get_user_name_len(t_file file)
{
    struct passwd* user = getpwuid(file.user_id);
    if (!user) {
        return (get_user_id_len(file));
    }
    int nb = ft_strlen(user->pw_name);
    return (nb);
}

static int get_len_date_month(t_file file)
{
    char **tmp = get_printable_date(file.last_change);
    if (!tmp)
        return (MALLOC_ERR);
    int nb = ft_strlen(tmp[0]);
    ft_free_tab(tmp);
    return (nb);
}

static int get_len_date_day(t_file file)
{
    char **tmp = get_printable_date(file.last_change);
    if (!tmp)
        return (MALLOC_ERR);
    int nb = ft_strlen(tmp[1]);
    ft_free_tab(tmp);
    return (nb);
}

static int get_len_date_hour(t_file file)
{
    char **tmp = get_printable_date(file.last_change);
    if (!tmp)
        return (MALLOC_ERR);
    int nb = ft_strlen(tmp[2]);
    ft_free_tab(tmp);
    return (nb);
}


static int get_len_nb_link(t_file file)
{
    char *tmp = ft_itoa(file.nb_link);
    if (!tmp)
        return (MALLOC_ERR);
    int nb = ft_strlen(tmp);
    free(tmp);
    return (nb);
}

static int get_nb_space(t_list *lst, int(*get_len_by_info)(t_file))
{
    t_file *file;
    t_list *current = lst;
    int max = -1;

    file = NULL;
    while (current)
    {
        file = current->content;
        int tmp = get_len_by_info(*file);
        if (tmp == MALLOC_ERR)
            return (MALLOC_ERR);
        if (tmp > max)
            max = tmp;
        current = current->next;
    }
    return (max);
}

static int check_malloc_err(int *array)
{
    int i = 0;

    while (i <= S_HOUR)
    {
        if (array[i] == MALLOC_ERR)
        {
            free(array);
            return (MALLOC_ERR);
        }
        i++;
    }
    return (0);
}


int get_minor_size(t_file file)
{
    int ret = 0;
    if (file.type == CHARACTER)
    {
        char *tmp = ft_ultoa(minor(file.rdev));
        ret = ft_strlen(tmp) + 2;
        // printf("for minor tmp %s ret = %d\n", tmp, ret);
        free(tmp);
    }
    return (ret); // + 2 for ', '
}


int get_major_size(t_file file)
{
    int ret = 0;
    if (file.type == CHARACTER)
    {
        char *tmp = ft_ultoa(major(file.rdev));
        ret = ft_strlen(tmp);
        free(tmp);
    }
    return (ret);
}

int get_len_name_quote(t_file file)
{
    // if (file.quote != NORMAL_CHAR)
        // return (1);
    // return (0);
    return (file.quote == NORMAL_CHAR ? 0 : 1);
}


int *get_all_space(t_list *lst, int flag_nb)
{
    int *array = NULL;
    array = ft_calloc(sizeof(int), S_MAX);
    if (!array)
    {
        new_lstclear(&lst, free);
        perror("Malloc");
        exit(1);
    }
    array[S_PERM] = 10;
    int ret = check_lst_acl(lst);
    if (ret == 0)
        array[S_PERM] = 11;
    else if (ret == MALLOC_ERR)
        return (NULL);
    if (has_flag(flag_nb, N_OPTION)) {
        array[S_USER] = get_nb_space(lst, get_user_id_len);
        array[S_GROUP] = get_nb_space(lst, get_group_id_len);
    } 
    else {
        array[S_USER] = get_nb_space(lst, get_user_name_len);
        array[S_GROUP] = get_nb_space(lst, get_group_name_len);
    }
    array[S_SIZE] = get_nb_space(lst, get_len_size);
    array[S_LINK] = get_nb_space(lst, get_len_nb_link);
    array[S_MONTH] = get_nb_space(lst, get_len_date_month);
    array[S_DAY] = get_nb_space(lst, get_len_date_day);
    array[S_HOUR] = get_nb_space(lst, get_len_date_hour);

    array[S_MINOR_SIZE] = get_nb_space(lst, get_minor_size);
    array[S_MAJOR_SIZE] = get_nb_space(lst, get_major_size);
    array[S_NAME_QUOTE] = get_nb_space(lst, get_len_name_quote);
    

    // if (array[S_MINOR_SIZE] + array[S_MAJOR_SIZE] > 0)
        // array[S_SIZE] = array[S_MINOR_SIZE] + array[S_MAJOR_SIZE] + 1; // 2 for ', '
    if (check_malloc_err(array) == MALLOC_ERR)
        return (NULL);
    return (array);
}