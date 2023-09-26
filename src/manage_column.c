#include "../include/ft_ls.h"

static long long get_total_len(t_list *list)
{
    long long total;
    t_file *file;
    
    total = 0;
    while (list)
    {
        file = list->content;
        total += ft_strlen(file->name);
        total += 2;
        list = list->next;
    }
    total -= 2;
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

int ft_strcpy(char* dst, char *src, int len)
{
    int i = 0;
    if (!src || len < 1)
        return (0);
    while (src[i])
    {
        dst[i] = src[i];
        i++;
    }
    return (len);
}    

static int get_raw_len(int *max_unit_len, int max)
{
    int nb = 0;
    int i = 0;
    while (i < max)
    {
        nb += max_unit_len[i];
        nb += 2;
        i++;
    }
    return (nb);
}

static char **alloc_tab(int nb_raw, int *max_unit_len, int max_per_raw, int lst_len)
{
    char **tab = ft_calloc(sizeof(char *), nb_raw + 2);
    if (!tab)
        return (NULL);
    int total = get_raw_len(max_unit_len, max_per_raw);
    for (int k = 0; k < nb_raw; k++)
    {
        tab[k] = ft_calloc(sizeof(char), total + 2);
        if (!tab[k])
            return (NULL);
    }

    char *type = ft_calloc(sizeof(char), lst_len + 1);
    if (!type)
        return (NULL);
    tab[nb_raw] = type;
    tab[nb_raw + 1] = NULL;
    return (tab);
}

static int fill_type_str(char *str, int k, t_file file)
{
    int ret = -1;
    str[k] = file.type;
    if (str[k] == REGULAR)
        ret = check_file_perm(file.perm, 1);
    if (ret == MALLOC_ERR)
        return (MALLOC_ERR);
    else if (ret == 0)
        str[k] = EXEC;
    return (0);
}

static char **manage_column(t_list *lst, int *max_unit_len, int max_per_raw, int nb_raw)
{
    int current_raw = 0, current_colum = 0, i = 0, j = 0;
    char **tab = alloc_tab(nb_raw, max_unit_len, max_per_raw, get_lst_len(lst));
    if (!tab)
        return (NULL);
    while (lst)
    {
        t_file *file = lst->content;
        if (fill_type_str(tab[nb_raw], j, *file) == MALLOC_ERR)
        {
            ft_free_tab(tab);
            return (NULL);
        }
        i += ft_strcpy(&tab[current_raw][i], file->name, ft_strlen(file->name));
        if (current_colum != max_per_raw - 1)
            insert_space_str(max_unit_len[current_colum] + 2, file->name, &i, tab[current_raw]);
        current_raw++;
        if (current_raw == nb_raw)
        {
            current_colum++;
            current_raw = 0;
        }
        i = get_raw_len(max_unit_len, current_colum);
        j++;
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
        if (i == nb_raw )
        {
            tab[column] = max;
            max = 0;
            i = 0;
            column++;
        }
        lst = lst->next;
    }
    if (i < nb_raw && column < nb_column)
        tab[column] = max;
    return (tab);
}

// get the max column len for each test
static int get_max_len_by_index(int start, int test ,int max, int* tab)
{
    int nb = 0;
    int i = start;
    while (i < (start + test))
    {
        if (i < max)
        {
            if (nb < tab[i])
                nb = tab[i];
        }
        i++;
    }
    return (nb);
}

static int brut_test(int i, int test, int tab_len, int *all_len, int *local_space)
{
    int ret = -1;
    int column = 0;
    // printf("in brut test i = %d test = %d ", i ,test);
    while (i < tab_len - test)
    {
        if (ret == -1)
        {
            local_space[column] = get_max_len_by_index(i, test, tab_len, all_len);
            ret = local_space[column];
            ret += 2;
            column++;
        }
        if (i + test <= tab_len)
        {
            local_space[column] = get_max_len_by_index(i + test, test, tab_len, all_len);
            ret += local_space[column];
            ret += 2;
            column++;
        }
        else
            break ;
        i += test;
    }
    ret -= 2; // remove last space
    // printf("ret = %d\n", ret);
    return (ret);
}

static int test_all(int test, int* all_len, int tab_len, int stdout_w)
{
    int *local_space = ft_calloc(sizeof(int), tab_len);
    int ret = -1;
    int i = 0;
    while (i < test)
    {
        ret = brut_test(i, test, tab_len, all_len, local_space);
        if (ret > stdout_w)
            break ;
        i++;
    }
    if (ret > stdout_w)
        ret = -1;
    else
        ret = 0;
    free(local_space);
    return (ret);
}

static int* get_all_len(t_list *lst, int len)
{
    int i;
    int *all_len = NULL;
    
    i = 0;
    all_len = ft_calloc(sizeof(int), len);
    if (!all_len)
        return (NULL);
    while (lst)
    {
        t_file *file = lst->content;
        all_len[i] = ft_strlen(file->name);
        i++;
        lst = lst->next;
    }
    return (all_len);
}

static int get_nb_raw(int stdout_w, t_list *lst)
{
    int len = get_lst_len(lst);
    int *all_len = get_all_len(lst, len);
    if (!all_len)
        return (MALLOC_ERR);
    int test = 1; // test value for nb_raw, //brute force it
    int ret = -1;
    while (ret != 0)
    {
        ret = test_all(test, all_len, len, stdout_w);
        test++;
    }
    free(all_len);
    return (test - 1);
}

/* Protection after max_per_raw affectation, not sure is mandatory
        if (max_per_raw <= 0)
            max_per_raw = 1;
*/

char **check_manage_colum(t_list *lst, int *err, int *value, int lst_len)
{
    char    **tab = NULL;
    int     *tab_max_unit = NULL;
    int     stdout_width = get_stdout_width();
    int     nb_raw = 0, max_per_raw = 1;
    
    nb_raw = get_nb_raw(stdout_width, lst);
    if (nb_raw == MALLOC_ERR)
    {
        *err = MALLOC_ERR;
        return (NULL);
    }
    *value = nb_raw;
    max_per_raw  = (int)(lst_len / nb_raw) + (lst_len % nb_raw != 0); // add 1 if (lst_len % nb_raw != 0)
    if (get_total_len(lst) > (long long)stdout_width)
    {
        tab_max_unit = get_max_by_column(lst, max_per_raw, nb_raw);
        if (!tab_max_unit)
        {
            *err = MALLOC_ERR;
            return (NULL);
        }
        tab = manage_column(lst, tab_max_unit, max_per_raw, nb_raw);
        if (!tab)
            *err = MALLOC_ERR;
        free(tab_max_unit);
    }
    return (tab);
}

static enum e_color get_color_by_index(char *type, int index)
{
    char c = '-';
    if (index < (int)ft_strlen(type))
        c = type[index];
    if (c == DIRECTORY)
        return (E_BLUE);
    if (c == SYMLINK)
        return (E_CYAN);
    if (c == EXEC)
        return (E_GREEN);
    return(E_NONE);
}

int fill_buffer_stop(char *str, char c)
{
    int i = 0;
    while (str && str[i])
    {
        fill_buffer_char(str[i]);
        i++;
        if (str[i] == c)
            break;
    }
    return (i);
}

int fill_buffer_color_with_space(char *str, enum e_color color, char c)
{
    int i = 0;
    if (!str)
        return (-100000);
    if (color != E_NONE)
        fill_color(color);
    i = fill_buffer_stop(str, c);
    if (color != E_NONE)
        fill_buffer(RESET);
    while (str[i] && str[i] == c)
    {
        fill_buffer_char(str[i]);
        i++;
    }
    while (str[i] && str[i] == c)
        i++;
    if (str[i] == '\0')
        return (-100000);
    return (i);
}

static void add_color(char *str, int nb_raw ,char *type, int raw)
{
    int i = 0;
    int color = E_NONE;
    int nb = raw;
    while (str && str[i])
    {
        if (nb < (int)ft_strlen(type))
            color = get_color_by_index(type, nb);
        i += fill_buffer_color_with_space(&str[i], color, ' ');
        if (i < 0)
            break;
        nb += nb_raw;
    }
}

int fill_buffer_with_column(char **tab, int nb_raw, t_list **lst)
{
    for (int i = 0; i < nb_raw; i++)
    {
        add_color(tab[i], nb_raw, tab[nb_raw], i);
        if (i != nb_raw - 1)
            fill_buffer_char('\n');
    }
    for (int i = 0; i < nb_raw + 1; i++)
        free(tab[i]);
    free(tab);
    fill_buffer("\n");
    new_lstclear(lst, free);
    return (0);
}