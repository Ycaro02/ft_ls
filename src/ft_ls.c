#include "../include/ft_ls.h"

static int display_dir_header(t_file file, int lst_len, int call, int index)
{
    (void)lst_len;
    printf("%sCall: %d idx: %d for %s%s\n", CYAN, call, index, file.name, RESET);
    if ((call > 1 || index != 0) || (call >= 1 && lst_len > 1))
    {
        if (index == 0 && call > 1)
            fill_buffer_char('\n');
        if (index != 0)
            fill_buffer("\n\n");        
        multiple_fill_buff(file.name, ":\n", NULL, NULL);
    }
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


int ls_only_file_L(t_list *lst, int flag_nb)
{
    if (fill_l_buffer(lst, flag_nb, 0) == MALLOC_ERR)
        return (MALLOC_ERR);
    if (!has_flag(flag_nb, R_OPTION))
        fill_buffer_char('\n');
    return (0);
}


int ls_l_one_dir(t_file *file, int flag_nb, int lst_len, int *error, int call_flag, int index)
{
    t_list *lst = NULL;

    if (call_flag != 0 && file->type != DIRECTORY)
        return (0);
    lst = get_all_file_struct(file, flag_nb, error);
    if (!lst && *error == MALLOC_ERR) // one of case where int pointer error is mandatory
        return (MALLOC_ERR);
    if (!lst) {
        fill_buffer("total 0\n");
        return (0);
    }
    file->total_size = get_total_size(lst);

    // printf("forL file: %s call %d idx %d\n", file->name, call_flag, index);
    
    if (display_dir_header(*file, lst_len, call_flag, index) == MALLOC_ERR)
        return (MALLOC_ERR);
    
    
    if (has_flag(flag_nb, REVERSE_OPTION))
        if (safe_reverse_lst(&lst, error, flag_nb) == MALLOC_ERR)
            return (MALLOC_ERR);
    if (fill_l_buffer(lst, flag_nb, call_flag) == MALLOC_ERR)
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

static void hard_display_d(t_file *file)
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
}
/*  call_flag:  0 for file display
                1 for dir without file before
                2 for dir with file or another dir before
*/
int ls_one_dir(t_file *file, int flag_nb, int lst_len, int *error, int call_flag, int index)
{
    t_list *lst = NULL;
    int quote = quotes_required(file->name);

    if (has_flag(flag_nb, D_OPTION)) {
        hard_display_d(file);
        return (0);
    }

    (void)lst_len;
    // printf("for file: %s call %d idx %d\n", file->name, call_flag, index);
    if (call_flag >= 1 || index != 0) {
        if (index != 0 || call_flag > 1){
            fill_buffer_char('\n');
            fill_buffer_char('\n');
        }

        if (quote > NOEFFECT_CHAR)
            display_quote(quote);
        fill_buffer(file->name);
        if (quote > NOEFFECT_CHAR)
            display_quote(quote);
    }

    if (call_flag == 0) {
        display_quote(quote);
        fill_buffer(file->name);
    }
    if (call_flag == 0)
        display_quote(quote);

    if (call_flag >= 1 || index != 0)
            fill_buffer(":\n");
        // if (call_flag >= 1)
    
    // if (call_flag == 0 && index == lst_len - 1)
    //     fill_buffer("\n");
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