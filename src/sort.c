#include "../ft_ls.h"

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

void reverse_lst(t_list *lst, t_list **new)
{
    t_list *current;

    current = lst;
    while (current)
    {
        ft_lstadd_front(new, ft_lstnew(current->content));
        current = current->next; 
    }
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
        if (lower_strcmp(current->name, min_file->name) < 0)
            min = lst;
        lst = lst->next;
    }
    t_file *tmp = head->content;
    head->content = min->content;
    min->content = tmp;
    sort_by_name(head->next, flag_nb);
}

void sort_by_time(t_list *lst, int flag_nb)
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
        if (current->last_change > min_file->last_change)
            min = lst;
        lst = lst->next;
    }
    t_file *tmp = head->content;
    head->content = min->content;
    min->content = tmp;
    sort_by_time(head->next, flag_nb);
}


void sort_lst(t_list *lst, int flag_nb)
{
    if (flag_nb & T_OPTION)
        sort_by_time(lst, flag_nb);
    else
        sort_by_name(lst, flag_nb);
}