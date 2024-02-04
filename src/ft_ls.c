#include "../include/ft_ls.h"

/** display_fcontext_flag
 * Basic debug function to display file_context
*/
void display_fcontext_flag(t_file_context *file_c, char *str, int flag) {
    ft_printf_fd(2, "%sCall: [%d]%s idx: %s[%d]%s\n", RED, file_c->call_flag, RESET, YELLOW, file_c->idx, RESET);
    display_flags(flag);
    ft_printf_fd(2, "for |%s%s%s|\n",CYAN, str, RESET);
}

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
 * args: file, lst_len, call, index, flag
 *      file: t_file obj to display
 *      file_c: t_file context struct:
 *          - lst_len: file's lst_len
 *          - call: call deep,    
 *                          - 0 for only file
 *                          - 1 for first call without file/error before
 *                          - 2 for every next call
 *          - idx: index of file in lst
 *      flag : flag option
 *      no_file: bool no_file
*/
static void display_dir_header(t_file file, t_file_context *file_c, int flag, t_int8 no_file)
{
    int quote = quotes_required(file.name);
    // display_fcontext_flag(file_c, file.name, flag);

    if (file_c->call_flag != 0) { /* if not only file call */

        if (file_c->idx == 0) { /* if first file of list */
            if (file_c->call_flag == 2) /* if is not first directory call */
                fill_buffer("\n\n");
        }
        if (file_c->idx != 0) /* if not first file in lst */
            fill_buffer("\n\n");
    }

    /* if multiple dir (call == 2) or Recurcive option enable */
    if (file_c->call_flag == 2 || has_flag(flag, R_OPTION)\
        || (file_c->call_flag == 1 && file_c->lst_len > 1)) { /* if is first call but file not alone in lst */
        if (quote > NOEFFECT_CHAR)
            display_quote(quote);
        fill_buffer(file.name);
        if (quote > NOEFFECT_CHAR)
            display_quote(quote);
        fill_buffer_char(':');
        if (!no_file)
            fill_buffer_char('\n');
    }
}

static t_int64 get_total_size(t_list *lst)
{
    t_list      *current = lst;
    t_int64     total = 0;

    while (current) {
        t_file *file = current->content;
        total += (file->nb_block / 2);
        current = current->next;
    }
    return (total);
}

/**
 * Display total directort size, take no_file boolean to display \n or not
*/
static int display_total_size(t_file *file, t_int8 no_file)
{
    char        *total_str = ft_ltoa(file->total_size);

    if (!total_str)
        return (MALLOC_ERR);
    multiple_fill_buff("total ", total_str, NULL, NULL);
    if (!no_file)
        fill_buffer_char('\n');
    free(total_str);
    return (0);
}


int ls_only_file_L(t_list *lst, int flag_nb)
{
    if (fill_l_buffer(lst, flag_nb, 0) == MALLOC_ERR)
        return (MALLOC_ERR);
    return (0);
}

/** ls_l_one_dir : ls_function, give in argument to ls() and call manualy in recurcive_ls()
 * 
*/
int ls_l_one_dir(t_file *file, t_context *c, t_file_context *file_c)
{
    t_list *lst = NULL;
    t_int8 local_err = 0;
    // int r_flag = has_flag(c->flag_nb, R_OPTION); /* bool r_flag enable */

    if (file_c->call_flag != 0 && file->type != DIRECTORY)
        return (0);
    lst = get_all_file_struct(file, c->flag_nb, &local_err); /* NEED TO GIVE PTR TO FILE_C HERE */
    if (!lst && local_err == MALLOC_ERR) /* One case where int pointer error is mandatory */
        return (MALLOC_ERR);
    else if (!lst) { /* Here we use NULL return to check if directory can't be read or empty */
        if (local_err == 1) {
            multiple_fill_buff("\nft_ls: cannot open directory '"\
                , file->name, "': Permission denied", NULL);
            if (!has_flag(c->flag_nb, R_OPTION)){
                c->error = NA_CMD_LINE_ERR;
                return (0); /* CMD LINE ERROR */
            }
            update_error(&c->error);
            return (0); /* classic denie error */
        }
    }
    file->total_size = get_total_size(lst);
    display_dir_header(*file, file_c, c->flag_nb, 0);
    if (display_total_size(file, (lst == NULL)) == MALLOC_ERR)
        return (MALLOC_ERR);

    if (lst && fill_l_buffer(lst, c->flag_nb, file_c->call_flag) == MALLOC_ERR)
            return (MALLOC_ERR);
    if (file_c->call_flag == 1)
        ++(file_c->call_flag);
    return (0);
}


int ls_one_dir(t_file *file, t_context *c, t_file_context *file_c)
{
    // int     r_flag = has_flag(c->flag_nb, R_OPTION); /* bool r_flag enable */
    t_list  *lst = NULL;
    t_int8  local_err = 0;

    if (has_flag(c->flag_nb, D_OPTION))
        return (hard_display_d(file));

    // printf("%sCallC: %d idx: %d for %s%s\n", CYAN, call_flag, index, file->name, RESET);
    /* really ugly need to apply mange column here but still working */
    if (file_c->call_flag == 0) {
        int quote = quotes_required(file->name);
        display_quote(quote);
        fill_buffer(file->name);
        display_quote(quote);
        // store_in_buffer(lst, c->flag_nb); // maybe just that
        return (0);
    }

    lst = get_all_file_struct(file, c->flag_nb, &local_err);
    if (!lst && local_err == MALLOC_ERR)
        return (MALLOC_ERR);
    else if (!lst) {
        if (local_err == 1) {
            multiple_fill_buff("\nft_ls: cannot open directory '"\
                , file->name, "': Permission denied", NULL);
            if (!has_flag(c->flag_nb, R_OPTION)){
                c->error = NA_CMD_LINE_ERR;
                return (0); /* CMD LINE ERROR */
            }
            update_error(&c->error);
            return (0); /* classic denie error */
        }
    }

    display_dir_header(*file, file_c, c->flag_nb, (lst == NULL));
    if (lst && store_in_buffer(lst, c->flag_nb) == MALLOC_ERR)
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