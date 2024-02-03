#include "../include/ft_ls.h"

/**
 * Hardcode display for -d need to refact (use manage Column)
*/
static int hard_display_d(t_file *file)
{
    if (file->type == DIRECTORY)
        fill_buffer(BLUE);
    display_quote(file->quote);
    fill_buffer(file->name);
    display_quote(file->quote);
    if (file->type == DIRECTORY)
        fill_buffer(RESET);
    fill_buffer_char(' ');
    return (0);
}

/** Display dir header
 * args: file, lst_len, call, index, l_flag
 *      file: t_file obj to display
 *      lst_len: file's lst_len
 *      call: call deep, 0 for only file, 1 for first call, 2 for next
 *      index: index of file in lst
 *      l_flag: bool flag for l option 0 for no, 1 for l, 2 for l + r (just display total) 
*/
static int display_dir_header(t_file file, t_file_context *file_c, int l_flag)
{
    int quote = quotes_required(file.name);

    // printf("%sCall: [%d] idx: [%d] l_flag: [%d] for |%s|%s\n", CYAN, file_c->call_flag, file_c->index, l_flag, file.name, RESET);
    if ((file_c->call_flag > 1 || file_c->idx != 0) || (file_c->call_flag >= 1 && file_c->lst_len > 1))
    {
        if (file_c->idx == 0 && file_c->call_flag > 1 && l_flag != 1) // && L + R_OPTION
            fill_buffer("\n\n");
        else if (file_c->idx == 0 && file_c->call_flag > 1)
            fill_buffer_char('\n');
            
        if (file_c->idx != 0)
            fill_buffer("\n\n");

        if (quote > NOEFFECT_CHAR)
            display_quote(quote);
        fill_buffer(file.name);
        if (quote > NOEFFECT_CHAR)
            display_quote(quote);

        fill_buffer(":\n");
    }
    if (l_flag >= 1) {
        char *total_str = ft_ltoa(file.total_size);
        if (!total_str)
            return (MALLOC_ERR);
        multiple_fill_buff("total ", total_str, "\n", NULL);
        free(total_str);
    }
    return (0);
}

static long long get_total_size(t_list *lst)
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
    return (0);
}


int ls_l_one_dir(t_file *file, t_context *c, t_file_context *file_c)
{
    t_list *lst = NULL;
    int r_flag = has_flag(c->flag_nb, R_OPTION); /* bool r_flag enable */

    if (file_c->call_flag != 0 && file->type != DIRECTORY)
        return (0);
    lst = get_all_file_struct(file, c->flag_nb, &c->error);
    if (!lst && c->error == MALLOC_ERR) /* One case where int pointer error is mandatory */
        return (MALLOC_ERR);
    else if (!lst) {
        multiple_fill_buff("\nft_ls: cannot open directory '"\
            , file->name, "': Permission denied", NULL);
        if (!has_flag(c->flag_nb, R_OPTION))
            return (NA_CMD_LINE_ERR); /* CMD LINE ERROR */
        update_error(&c->error);
        return (c->error); /* classic denie error */
    }
    file->total_size = get_total_size(lst);

    // printf("forL file: %s call %d idx %d\n", file->name, call_flag, index);
    if (display_dir_header(*file, file_c, 1 + r_flag) == MALLOC_ERR)
        return (MALLOC_ERR);
    
    // if (has_flag(c->flag_nb, REVERSE_OPTION))
    //     safe_reverse_lst(&lst, &c->error, c->flag_nb);
    if (fill_l_buffer(lst, c->flag_nb, file_c->call_flag) == MALLOC_ERR)
        return (MALLOC_ERR);
    return (0);
}


int ls_one_dir(t_file *file, t_context *c, t_file_context *file_c)
{
    t_list *lst = NULL;

    if (has_flag(c->flag_nb, D_OPTION))
        return (hard_display_d(file));

    // printf("%sCallC: %d idx: %d for %s%s\n", CYAN, call_flag, index, file->name, RESET);
    /* really ugly need to apply mange column here but still working */
    if (file_c->call_flag == 0) {
        int quote = quotes_required(file->name);
        display_quote(quote);
        fill_buffer(file->name);
        display_quote(quote);
        return (0);
    }

    lst = get_all_file_struct(file, c->flag_nb, &c->error);
    if (!lst && c->error == MALLOC_ERR)
        return (MALLOC_ERR);
    else if (!lst) {
        multiple_fill_buff("\nft_ls: cannot open directory '", file->name, "': Permission denied", NULL);
        if (!has_flag(c->flag_nb, R_OPTION))
            return (NA_CMD_LINE_ERR); /* CMD LINE ERROR */
        update_error(&c->error);
        return (c->error); /* classic denie error need to check before ? call update error instead */
    }

    if (display_dir_header(*file, file_c, 0) == MALLOC_ERR)
        return (MALLOC_ERR);

    // if (has_flag(c->flag_nb, REVERSE_OPTION))
    //     safe_reverse_lst(&lst, NULL, c->flag_nb);

    if (store_in_buffer(lst, c->flag_nb) == MALLOC_ERR)
        return (MALLOC_ERR);
    return (0);
}

/**
 * Basic display quote
*/
void display_quote(int quote)
{
    if (quote == ADD_SIMPLE_QUOTE_CHAR)
        fill_buffer_char('\'');
    else if (quote == ADD_DOUBLE_QUOTE_CHAR)
        fill_buffer_char('\"');
    else
        fill_buffer_char(' ');
}