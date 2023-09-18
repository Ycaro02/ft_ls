#include "../ft_ls.h"

void ls_one_dir(t_file *file, int flag_nb)
{
    fill_buffer(file->name);
    fill_buffer(":\n");
    t_list *lst = get_all_file_struct(file, flag_nb);
    if (!lst)
        return ;
    store_in_buffer(lst, flag_nb);
    fill_buffer("\n");
}

static void display_dir_header(t_file file)
{
    fill_buffer_char('\n');
    fill_buffer(file.name);
    fill_buffer(":\n");
    char *total_str = ft_itoa(file.total_size);
    fill_buffer("total ");
    fill_buffer(total_str);
    fill_buffer("\n");
    free(total_str);
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

void ls_l_one_dir(t_file *file, int flag_nb)
{
    t_list *lst = NULL;
    if (file->type == DIRECTORY)
      lst = get_all_file_struct(file, flag_nb);
    else
        return ;
    if (!lst)
        return ;
    file->total_size = get_total_size(lst);
    display_dir_header(*file);
    t_list *new = NULL;
    if (flag_nb & REVERSE_OPTION)
    {
        reverse_lst(lst, &new);
        free_node_ptr(&lst);
        lst = new;
    }
    t_list *current = lst;
    int *space = get_all_space(current);
    while (current)
    {
        fill_buffer_l_option(*(t_file *)current->content, space);
        current = current->next;
    }
    free(space);
    new_lstclear(&lst, free);
}