#include "../include/ft_ls.h"

t_buff g_buff;

int ls(t_list * lst, int flag_nb,  int (*ls_function)(t_file*, int, int, int*, int, int), int* error, int call_flag)
{
    t_list *current = lst;
    int idx = 0;
    int lst_len = get_lst_len(lst) - 1;
    int err = 0;
    while (current) {
        err = ls_function(current->content, flag_nb, lst_len, error, call_flag, idx);
        ++idx;
        if (err == MALLOC_ERR)
            break ;
        current = current->next;
    }
    if (has_flag(flag_nb, D_OPTION) && !has_flag(flag_nb, L_OPTION))
        fill_buffer_char('\n');
    return (err);
}

static int ls_only_dir(t_list *dir_lst, int flag_nb)
{
    t_list *current = dir_lst;
    int *space = get_all_space(dir_lst, flag_nb);
    int err = 0;
    while (current) {
        err = fill_buffer_l_option(*(t_file *)current->content, space, flag_nb);
        if (err == MALLOC_ERR) {
            free(space);
            return (MALLOC_ERR);
        }
        current = current->next;
    }
    free(space);
    return (0);
}


void call_ls(t_list *dir_lst, int flag_nb, int *error, int call_flag)
{
    int err = 0;

    if (call_flag != 0 && has_flag(flag_nb, R_OPTION) && !has_flag(flag_nb, D_OPTION))
        err = search_recurcive_dir(dir_lst, flag_nb, error);
    else if (call_flag != 0 && has_flag(flag_nb, L_OPTION) && has_flag(flag_nb, D_OPTION))
        err = ls_only_dir(dir_lst, flag_nb);
    else if (has_flag(flag_nb, L_OPTION))
        err = ls(dir_lst, flag_nb, ls_l_one_dir, error, call_flag);
    else
        err = ls(dir_lst, flag_nb, ls_one_dir, error, call_flag);

    if (!has_flag(flag_nb, D_OPTION) || (has_flag(flag_nb, L_OPTION) && has_flag(flag_nb, D_OPTION)))
        new_lstclear(&dir_lst, free);
    else
        free_node_ptr(&dir_lst);
    
    if (err == MALLOC_ERR) {
        ft_printf_fd(2, "Malloc error exit\n");
        exit(MALLOC_ERR);
    }
    // finish_print_buffer();
}

static int basic_sort_lst(t_list *lst, int flag, int *error)
{
    sort_lst(lst, flag);
    if (!lst)
        return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
    if (has_flag(flag , REVERSE_OPTION)) {
        if (safe_reverse_lst(&lst, error, flag) == MALLOC_ERR) {
            new_lstclear(&lst, free);
            return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
        }
    }
    return (0);
}

int ft_ls(char **argv, int flag_nb, int* error)
{
    t_list *dir_lst, *simple_file = NULL;
    
    dir_lst = get_dir_args(&argv[1], error, flag_nb, &simple_file);
    // if (simple_file != NULL) {
    //     printf("simple file not null\n");
    // }
    // else
    //     printf("simple file NULL\n");
        
    /* Error management */
    if (!dir_lst && *error == MALLOC_ERR)
        return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
    else if (!dir_lst)
        return (*error);

    /* sort */
    if (basic_sort_lst(dir_lst, flag_nb, error) == 1)
        return (1);
    if (simple_file){
        if (basic_sort_lst(dir_lst, flag_nb, error) == 1)
            return (1);
        call_ls(simple_file, flag_nb, error, 0);
        // new_lstclear(&simple_file, free);
    }

    // sort_lst(dir_lst, flag_nb);
    // if (!dir_lst)
    //     return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
    // if (has_flag(flag_nb , REVERSE_OPTION)) {
    //     if (safe_reverse_lst(&dir_lst, error, flag_nb) == MALLOC_ERR) {
    //         new_lstclear(&dir_lst, free);
    //         return (print_error("Malloc error\n", NULL, MALLOC_ERR, 1));
    //     }
    // }

    call_ls(dir_lst, flag_nb, error, 1);
    return (*error);
}

static int check_display_help(int argc, char**argv)
{
    if (argc >= 2) {
        if (ft_strcmp(argv[1], "--help") == 0) {
            ft_printf_fd(1, HELP_STR);
            return (0);
        }
    }
    return (1);
}

int main (int argc, char **argv)
{
    int error;
    int flag_nb;

    error = 0;
    if (check_display_help(argc, argv) == 0)
        return (0);
    ft_bzero(g_buff.buffer, BUFFER_LEN - 1);

    flag_nb = parse_flag(argc, argv);

    error = ft_ls(argv, flag_nb, &error);
    finish_print_buffer();
    return (error);
}

int quotes_required(char *str)
{
    int i = 0;
    int ret = NORMAL_CHAR;
    int tmp = NORMAL_CHAR;

    while (str && str[i]) {
        tmp = is_special_char(str[i]);
        if (tmp == DIEZE_CHAR)
            tmp = i == 0 ? ADD_SIMPLE_QUOTE_CHAR : NORMAL_CHAR;
        else if (tmp == BRACKET_CHAR) 
            tmp = (ft_strlen(str) == 1 ? ADD_SIMPLE_QUOTE_CHAR : NORMAL_CHAR);
        if (tmp != NORMAL_CHAR) {
            if (ret < ADD_DOUBLE_QUOTE_CHAR)
                ret = tmp;
        }
        ++i;
    }
    return (ret);
}

struct stat *check_for_stat(char* name, int flag, int *save_symlink)
{
    struct stat *sb = ft_calloc(sizeof(struct stat), 1);

    if (!sb)
        return (NULL);
    if (lstat(name, sb) == -1) {
            // printf("parrent path: %s\n", name);
            // perror("lstat");
            return (NULL);
    }
    
    *save_symlink = get_type(*sb) == SYMLINK;

    if (!has_flag(flag, L_OPTION)) {
        if (stat(name, sb) == -1) {
            // printf("path: %s\n", name);
            // perror("stat error");
            return (NULL);
        }
    }

    return (sb);
}