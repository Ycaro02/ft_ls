#include "../include/ft_ls.h"

void free_node_ptr(t_list **lst)
{
	t_list	*tmp;
	t_list	*current;

	if (lst == NULL || *lst == NULL)
		return ;
	current = *lst;
	tmp = current;
	while (tmp != NULL)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
	*lst = NULL;
}

static void reverse_lst(t_list *lst, t_list **new)
{
    t_list *current;

    current = lst;
    while (current)
    {
        ft_lstadd_front(new, ft_lstnew(current->content));
        current = current->next; 
    }
}

int safe_reverse_lst(t_list **lst,  int* error, int flag_nb)
{
    t_list *reverse = NULL;
    if (flag_nb & F_OPTION)
        return (0);
    reverse_lst(*lst, &reverse);
    if (!reverse)
    {
        if (error)
            *error = MALLOC_ERR;
        return (MALLOC_ERR);
    }
    free_node_ptr(lst);
    *lst = reverse;
    return (0);
}

void sort_by_name(t_list *lst, int flag_nb)
{
    if (!lst)
        return ;
    t_list *head = lst;
    t_list *min = NULL;
    while (lst) 
    {
        if (!min)
            min = lst;
        t_file *current = (t_file *)lst->content;
        t_file *min_file = (t_file *)min->content;
        if (ft_lower_strcmp(current->name, min_file->name) < 0)
            min = lst;
        lst = lst->next;
    }
    t_file *tmp = head->content;
    head->content = min->content;
    min->content = tmp;
    sort_by_name(head->next, flag_nb);
}

t_list *get_min_value(t_list *min, t_list *lst, char option)
{
    t_file *current = (t_file *)lst->content;
    t_file *min_file = (t_file *)min->content;
    if (option == 't')
        if (current->last_change > min_file->last_change)
            min = lst;
    if (option == 'u')
        if (current->last_access > min_file->last_access)
            min = lst;
    if (option == 'c')
        if (current->last_status_change > min_file->last_status_change)
            min = lst;
    return (min);
}

void sort_by_time(t_list *lst, int flag_nb, char option)
{
    if (!lst)
        return ;
    t_list *head = lst;
    t_list *min = NULL;
    while (lst)
    {
        if (!min)
            min = lst;
        min = get_min_value(min, lst, option);
        lst = lst->next;
    }
    t_file *tmp = head->content;
    head->content = min->content;
    min->content = tmp;
    sort_by_time(head->next, flag_nb, option);
}


void sort_lst(t_list *lst, int flag_nb)
{
    if (flag_nb & F_OPTION)
        return ;
    if (flag_nb & L_OPTION)
    {
        if (flag_nb & T_OPTION)
        {
            if (flag_nb & U_OPTION)
                sort_by_time(lst, flag_nb, 'u'); // u
            else if (flag_nb & C_OPTION)
                sort_by_time(lst, flag_nb, 'c'); // c
            else
                sort_by_time(lst, flag_nb, 't'); // t
        }
        else if (flag_nb & U_OPTION || flag_nb & C_OPTION)
            sort_by_name(lst, flag_nb);
        else
            sort_by_name(lst, flag_nb);
    }
    else if (flag_nb & U_OPTION)
            sort_by_time(lst, flag_nb, 'u'); // u -u take priotiry tested with ls -u -ut -utc
    else if (flag_nb & C_OPTION)
            sort_by_time(lst, flag_nb, 'c'); // c
    else if (flag_nb & T_OPTION)
            sort_by_time(lst, flag_nb, 'c'); // c
    else
        sort_by_name(lst, flag_nb);
}
