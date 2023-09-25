#include "../include/ft_ls.h"

static long long get_all_len(t_list *list, int len)
{
    long long total;

    total = 0;
    while (list)
    {
        total += len;
        list = list->next;
    }
    return (total);
} 

static int get_name_len(t_file file)
{
    int len = ft_strlen(file.name);
    return (len);
}


static void add_char(char *dst, char src, int *j)
{
    dst[*j] = src;
    (*j)++;
}

static void insert_space_str(int max_unit_len, char *name, int *j, char *str)
{
    int space = max_unit_len - (int)ft_strlen(name);
    while (space >= 0)
    {
        add_char(str, ' ', j);
        space--;
    }
}

static char **manage_column(t_list *lst, int max_unit_len, int max_per_raw, int nb_raw)
{
    int current_colum = 0;
    int i = 0;
    int j = 0;
    char **tab = ft_calloc(sizeof(char *), nb_raw + 1);
    if (!tab)
        return (NULL);
    for (int i = 0; i < nb_raw; i++)
    {
        tab[i] = ft_calloc(sizeof(char), (max_unit_len * (max_per_raw + 3)));
        if (!tab[i])
            return (NULL);
    }
    while (lst)
    {
        t_file *file = lst->content;
        j += ft_strlcpy(&tab[i][j], file->name, ft_strlen(file->name) + 1);
        insert_space_str(max_unit_len, file->name, &j, tab[i]);
        i++;
        if (i == nb_raw)
        {
            current_colum++;
            i = 0;
        }
        j = current_colum * max_unit_len;
        lst = lst->next;
    }
    return (tab);
}


static int *get_max_by_column(t_list *lst, int nb_column, int nb_raw)
{
    int *tab = ft_calloc(sizeof(int), nb_column);
    int column = 0;
    int i = 0;
    int max = 0;
    while (lst)
    {
        t_file *file = lst->content;
        int tmp = get_name_len(*file);
        if (tmp > max)
            max = tmp;
        i++;
        if (i == nb_raw)
        {
            tab[column] = max;
            max = 0;
            i = 0;
            column++;
        }
        lst = lst->next;
    }
    if (i < nb_raw)
        tab[column] = max;
    for (int k =0; k< nb_column; k++)
        printf("max de %d = %d\n", k, tab[k]);
    return (tab);
}

char **check_manage_colum(t_list *lst, int *err, int *value, int lst_len)
{
    char        **tab = NULL;
    int         nb_raw = 0 ;
    int         max_unit_len = get_nb_space(lst, get_name_len) + 2;
    int         stdout_width = get_stdout_width();
    int         max_per_raw = (int)(stdout_width / (max_unit_len));
    if (max_per_raw == 0)
        max_per_raw = 1;
    nb_raw = (int)(lst_len / max_per_raw);
    if (lst_len % max_per_raw != 0)
        nb_raw++;
    if (nb_raw <= 0)
        nb_raw = 1;
    *value = nb_raw;

    int *tab_max_unit = get_max_by_column(lst, max_per_raw, nb_raw);
    if (!tab_max_unit)
    {
        *err = MALLOC_ERR;
        return (NULL);
    }
    free(tab_max_unit);



    if (get_all_len(lst, max_unit_len) > (long long)stdout_width)
    {
        tab = manage_column(lst, max_unit_len, max_per_raw, nb_raw);
        if (!tab)
            *err = MALLOC_ERR;
    }
    return (tab);
}

int fill_buffer_with_column(char **tab, int nb_raw, t_list **lst)
{
    for (int i = 0; i < nb_raw; i++)
    {
        fill_buffer(tab[i]);
        if (i != nb_raw - 1)
            fill_buffer_char('\n');
    }
    for (int i = 0; i < nb_raw; i++)
        free(tab[i]);
    free(tab);
    fill_buffer("\n");
    new_lstclear(lst, free);
    return (0);
}