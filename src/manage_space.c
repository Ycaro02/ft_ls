#include "../include/ft_ls.h"

static int get_len_size(t_file file)
{
    char    *tmp;
    int     max;

    tmp = ft_itoa((int)file.size);
    max = ft_strlen(tmp);
    free(tmp);
    return (max);
}

static int get_user_id_len(t_file file)
{
    char *tmp = ft_ltoa(file.user_id);
    int nb = ft_strlen(tmp);
    return (nb);
}

static int get_group_id_len(t_file file)
{
    char *tmp = ft_ltoa(file.group_id);
    int nb = ft_strlen(tmp);
    return (nb);
}

static int get_group_name_len(t_file file)
{
    struct group* group = getgrgid(file.group_id);
    if (!group)
    {
        perror("getgrgid");
        return (-1);
    }
    int nb = ft_strlen(group->gr_name);
    return (nb);
}

static int get_user_name_len(t_file file)
{
    struct passwd* user = getpwuid(file.user_id);
    if (!user)
    {
        perror("getpwuid");
        return (-1);
    }
    int nb = ft_strlen(user->pw_name);
    return (nb);
}

static int get_len_date_month(t_file file)
{
    char **tmp = get_printable_date(&file.last_change);
    int nb = ft_strlen(tmp[0]);
    ft_free_tab(tmp);
    return (nb);
}

static int get_len_date_day(t_file file)
{
    char **tmp = get_printable_date(&file.last_change);
    int nb = ft_strlen(tmp[1]);
    ft_free_tab(tmp);
    return (nb);
}

static int get_len_date_hour(t_file file)
{
    char **tmp = get_printable_date(&file.last_change);
    int nb = ft_strlen(tmp[2]);
    ft_free_tab(tmp);
    return (nb);
}


static int get_len_nb_link(t_file file)
{
    char *tmp = ft_itoa(file.nb_link);
    int nb = ft_strlen(tmp);
    free(tmp);
    return (nb);
}

int get_nb_space(t_list *lst, int(*get_len_info)(t_file))
{
    t_file *file;
    t_list *current = lst;
    int max = -1;

    file = NULL;
    while (current)
    {
        file = current->content;
        int tmp = get_len_info(*file);
        if (tmp > max)
            max = tmp;
        current = current->next;
    }
    return (max);
}

int *get_all_space(t_list *lst, int flag_nb)
{
    int *array;

    array = malloc(sizeof(int )* 7);
    if (!array)
    {
        new_lstclear(&lst, free);
        perror("Malloc");
        exit(1);
    }
    if (flag_nb & N_OPTION)
    {
        array[S_USER] = get_nb_space(lst, get_user_id_len);
        array[S_GROUP] = get_nb_space(lst, get_group_id_len);
    }
    else
    {
        array[S_USER] = get_nb_space(lst, get_user_name_len);
        array[S_GROUP] = get_nb_space(lst, get_group_name_len);
    }
    array[S_SIZE] = get_nb_space(lst, get_len_size);
    array[S_LINK] = get_nb_space(lst, get_len_nb_link);
    array[S_MONTH] = get_nb_space(lst, get_len_date_month);
    array[S_DAY] = get_nb_space(lst, get_len_date_day);
    array[S_HOUR] = get_nb_space(lst, get_len_date_hour);
    return (array);
}