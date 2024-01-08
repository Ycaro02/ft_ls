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

// if in string -> simple quote: don't take in special char
// ! $ ^ & * ( ) = < > ? ; [ ] ` ~
// SEMICOLONE ARE HEREEEE
// cant / EXCLU DIR
// ' : add ""
// " : add ''

// special logic, alway simple quote when alone { || }
// remove simple quote whenn add ever char : 
 // {{ : no simple quote != ## : simple quote
// at begin simple quote on: # 


// no care: @ % - _ + . , :
// simple double point
int is_special_char(char c)
{
    /* all no care char */
    if (c == '@' || c == '%' || c == '-'\
        || c == '_' || c == '+' || c == '.' || c == ',' || c == ':')
        return (0);
    if ( c == '{' || c == '}' || c == '#')
        return (0);
    return (1);
}

int special_char_gestion(char *current, char* min)
{
    
    return (1);
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
        if (special_char_gestion(current->name, min_file->name) == 0)
            min = lst;
        else if (ft_lower_strcmp(current->name, min_file->name) < 0)
            min = lst;
        // ft_printf_fd(2, "curr %s, min %s\n", current->name, min_file->name);
        lst = lst->next;
    }
    t_file *tmp = head->content;
    head->content = min->content;
    min->content = tmp;
    sort_by_name(head->next, flag_nb);
}

static t_list *get_precise_value(t_list *min, t_list* lst, t_timespec current, t_timespec min_file)
{
    t_file *current_tmp = lst->content;
    t_file *min_tmp = min->content;

    if (current.tv_sec > min_file.tv_sec)
        min = lst;
    if (current.tv_sec == min_file.tv_sec)
    {
        if (current.tv_nsec > min_file.tv_nsec)
            min = lst;
        if (current.tv_nsec == min_file.tv_nsec)    // if sec and microsec are equal sort by name
            if (ft_lower_strcmp(current_tmp->name, min_tmp->name) < 0)
                min = lst;
    }
    return (min);
}

static t_list *get_min_value(t_list *min, t_list *lst, char option)
{
    t_file *current = (t_file *)lst->content;
    t_file *min_file = (t_file *)min->content;
    if (option == T_FLAG_CHAR)
        min = get_precise_value(min , lst, current->last_change, min_file->last_change);
    else if (option == U_FLAG_CHAR)
        min = get_precise_value(min , lst, current->last_access, min_file->last_access);
    else if (option == C_FLAG_CHAR)
        min = get_precise_value(min , lst, current->last_status_change, min_file->last_status_change);
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
                sort_by_time(lst, flag_nb, U_FLAG_CHAR);
            else if (flag_nb & C_OPTION)
                sort_by_time(lst, flag_nb, C_FLAG_CHAR);
            else
                sort_by_time(lst, flag_nb, T_FLAG_CHAR);
        }
        else
            sort_by_name(lst, flag_nb);
    }
    else if (flag_nb & U_OPTION)
            sort_by_time(lst, flag_nb, U_FLAG_CHAR); // u -u take priotiry
    else if (flag_nb & C_OPTION)
            sort_by_time(lst, flag_nb, C_FLAG_CHAR); // c
    else if (flag_nb & T_OPTION)
            sort_by_time(lst, flag_nb, T_FLAG_CHAR); // c
    else
        sort_by_name(lst, flag_nb);
}