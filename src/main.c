#include "../ft_ls.h"

t_buff g_buff;

void ft_ls(char **argv, int flag_nb)
{
    t_list *dir_lst = get_dir_args(&argv[1]);
    if (!dir_lst)
    {
        struct stat sb;
        if (lstat(".", &sb) == -1)
        {
            perror("lstat for current dir");
            return ;
        }
        ft_lstadd_front(&dir_lst, ft_lstnew(fill_file_struct(sb, ".")));
        if (!dir_lst)
        {
            perror("Malloc Error ft_ls");
            return ;
        }
    }
    sort_lst(dir_lst, flag_nb);
    t_list *new = NULL;
    if (flag_nb & REVERSE_OPTION)
    {
        reverse_lst(dir_lst, &new);
        free_node_ptr(&dir_lst);
        dir_lst = new;
    }
    if (flag_nb & R_OPTION)
        search_recurcive_dir(dir_lst, flag_nb);
    else if (flag_nb & L_OPTION)
    {
        t_list *current = dir_lst;
        while (current)
        {
            ls_l_one_dir(current->content, flag_nb);
            current = current->next;
        }
    }
    else
    {
        t_list *current = dir_lst;
        while (current)
        {
            ls_one_dir(current->content, flag_nb);
            current = current->next;
        }
    }
    finish_print_buffer();
    new_lstclear(&dir_lst, free);
}

int main (int argc, char** argv)
{
    (void)argc;
    enum e_flag *flag = check_for_flag(argv);
    if (!flag)
        return(2);
    int flag_nb = get_flag(flag);
    free(flag);
    ft_ls(argv, flag_nb);
    return (0);
}