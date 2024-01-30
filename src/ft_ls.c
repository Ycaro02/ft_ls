#include "../include/ft_ls.h"

static int display_dir_header(t_file file, int lst_len)
{
    if (lst_len > 0)
        multiple_fill_buff("\n", file.name, ":\n", NULL);
    char *total_str = ft_ltoa(file.total_size);
    if (!total_str)
        return (MALLOC_ERR);
    fill_buffer("total ");
    fill_buffer(total_str);
    fill_buffer("\n");
    free(total_str);
    return (0);
}

long long get_total_size(t_list *lst)
{
    t_list *current = lst;
    long long total = 0;
    while (current)
    {
        t_file *file = current->content;
        total += (file->nb_block / 2);
        current = current->next;
    }
    return (total);
}

int ls_l_one_dir(t_file *file, int flag_nb, int lst_len, int *error)
{
    t_list *lst = NULL;
    if (file->type != DIRECTORY)
        return (0);
    lst = get_all_file_struct(file, flag_nb, error);
    if (!lst && *error == MALLOC_ERR) // one of case where int pointer error is mandatory
        return (MALLOC_ERR);
    if (!lst) {
        fill_buffer("total 0\n");
        return (0);
    }
    file->total_size = get_total_size(lst);
    if (display_dir_header(*file, lst_len) == MALLOC_ERR)
        return (MALLOC_ERR);
    if (has_flag(flag_nb, REVERSE_OPTION))
        if (safe_reverse_lst(&lst, error, flag_nb) == MALLOC_ERR)
            return (MALLOC_ERR);
    if (fill_l_buffer(lst, flag_nb) == MALLOC_ERR)
        return (MALLOC_ERR);
    return (0);
}

void display_quote(int quote)
{
    if (quote == ADD_SIMPLE_QUOTE_CHAR)
        fill_buffer_char('\'');
    else if (quote == ADD_DOUBLE_QUOTE_CHAR)
        fill_buffer_char('\"');
    else
        fill_buffer_char(' ');
}

int ls_one_dir(t_file *file, int flag_nb, int lst_len, int *error)
{
    t_list *lst = NULL;

    if (has_flag(flag_nb, D_OPTION))
    {
        fill_buffer(BLUE);
        display_quote(file->quote);
        fill_buffer(file->name);
        display_quote(file->quote);
        fill_buffer(RESET);

        fill_buffer_char(' ');
        free(file->parrent);
        free(file->name);
        free(file);
        return (0);
    }
    if (lst_len > 0) {
        /*
            need to detect before call and give it to args
            if not first dir displayed print 
            fill_buffer_char('\n');
        */
        fill_buffer_char('\n');
        display_quote(file->quote);
        fill_buffer(file->name);
        display_quote(file->quote);
        fill_buffer(":\n");
    }
    /* maybe manage space at this call for -l option */
    lst = get_all_file_struct(file, flag_nb, error);
    if (!lst && *error == MALLOC_ERR)
        return (MALLOC_ERR);
    if (!lst)
        return (0);
    if (store_in_buffer(lst, flag_nb) == MALLOC_ERR)
        return (MALLOC_ERR);
    return (0);
}