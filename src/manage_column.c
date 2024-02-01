#include "../include/ft_ls.h"

static int get_line_len(int *max_unit_len, int max, int quote_bool)
{
    int nb = 0;
    int i = 0;
    while (i < max) {
        nb += max_unit_len[i];
        nb += (1 + quote_bool);
        i++;
    }
    // nb += 2;
    return (nb);
}

static char **alloc_tab(int nb_line, int *max_unit_len, int max_per_line, int lst_len)
{
    char **tab = ft_calloc(sizeof(char *), nb_line + 2); /* 1 for type, 2 NULL*/
    if (!tab)
        return (NULL);
    int total = get_line_len(max_unit_len, max_per_line, 1);
    for (int i = 0; i < nb_line; i++)
    {
        tab[i] = ft_calloc(sizeof(char), total + 2); /* +2 for two space between word */
        if (!tab[i])
            return (NULL);
    }

    char *type = ft_calloc(sizeof(char), lst_len + 1);
    if (!type)
        return (NULL);
    tab[nb_line] = type;
    tab[nb_line + 1] = NULL;
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


static void add_char(char *dst, char src, int *j)
{
    dst[*j] = src;
    (*j)++;
}

static void insert_space_str(int max_unit_len, char *name, int *j, char *str)
{
    int space = max_unit_len - (int)ft_strlen(name);
    // printf("Insert %d space after %s\n", space, name);
    while (space > 0)
    {
        add_char(str, ' ', j);
        space--;
    }
}

char **manage_column(t_list *lst, int *max_unit_len, int max_per_line, int nb_line, int space_quote)
{
    int current_line = 0, current_colum = 0, i = 0, j = 0;
    char **tab = alloc_tab(nb_line, max_unit_len, max_per_line, get_lst_len(lst));
    if (!tab)
        return (NULL);
    while (lst)
    {
        t_file *file = lst->content;

        if (fill_type_str(tab[nb_line], j, *file) == MALLOC_ERR) {
            ft_free_tab(tab);
            return (NULL);
        }
        i += ft_strcpy(&tab[current_line][i], file->name, ft_strlen(file->name));
        if (current_colum != max_per_line - 1)
            insert_space_str(max_unit_len[current_colum] + 1 + space_quote, file->name, &i, tab[current_line]);
        /* insert 1 space + space quote bool */
        ++current_line;
        if (current_line == nb_line) {
            current_colum++;
            current_line = 0;
        }
        i = get_line_len(max_unit_len, current_colum, space_quote);
        ++j;
        lst = lst->next;
    }
    return (tab);
}







/* DISPLAY */
/* -------------------------------------------------------------------------------------------------------------*/
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
    if (c == CHARACTER)
        return (E_YELLOW);
    return(E_NONE);
}

int fill_buff_stop_char(char *str, char c, int quote_space)
{
    int i = 0;
    int quote = 0;

    if (quote_space == 1) {
        int count = 0;
        while (str && str[count]) {
            ++count;
        if (str[count] == c)
            break;
        }
        str[count] = '\0';

        quote = check_for_quote(str);
        str[count] = ' ';
        display_quote(quote);

    }
    
    /* Display name with coolor */
    // test quote here

    while (str && str[i]) {
        fill_buffer_char(str[i]);
        ++i;
        if (str[i] == c)
            break;
    }

    if (quote_space == 1)
        display_quote(quote);

    return (i);
}

int fill_buffer_color_with_space(char *str, enum e_color color, char c, int flag_nb, int quote_space)
{
    int i = 0;
    if (!str)
        return (-100000);

    if (has_flag(flag_nb , COLOR_OPTION))
        if (color != E_NONE)
            fill_color(color);
    
    i = fill_buff_stop_char(str, c, quote_space);

    if (has_flag(flag_nb , COLOR_OPTION))
        if (color != E_NONE)
            fill_buffer(RESET);

    /* display space */
    while (str[i] && str[i] == c) {
        fill_buffer_char(str[i]);
        i++;
    }
    if (quote_space == 0)
        fill_buffer_char(' ');
    while (str[i] && str[i] == c)
        i++;
    if (str[i] == '\0')
        return (-100000);
    return (i);
}

static void add_color_quote(char **tab, int nb_line, int line, int flag, int quote_space)
{
    char *str = tab[line];
    char *type = tab[nb_line];
    // char *quote = tab[nb_line + 1];
    (void)nb_line;

    int i = 0;
    int color = E_NONE;
    // int quote_val = 0;

    while (str && str[i]) {
        color = get_color_by_index(type, line);
        i += fill_buffer_color_with_space(&str[i], color, ' ', flag, quote_space);
        if (i < 0)
            break;
        line += nb_line;
    }
}

int fill_buffer_with_column(char **tab, int nb_line, t_list **lst, int flag_nb, int quote_space)
{
    for (int i = 0; i < nb_line; i++)
    {
        // if (has_flag(flag_nb , COLOR_OPTION))
        add_color_quote(tab, nb_line, i, flag_nb, quote_space);
        // else
            // fill_buffer(tab[i]);
        if (i != nb_line - 1)
            fill_buffer_char('\n');
    }
    for (int i = 0; i < nb_line + 1; i++)
        free(tab[i]);
    free(tab);
    new_lstclear(lst, free);
    // fill_buffer("\n");
    return (0);
}
/* -------------------------------------------------------------------------------------------------------------*/
