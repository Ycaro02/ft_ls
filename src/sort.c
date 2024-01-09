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
# define NORMAL_CHAR            0
# define NOEFFECT_CHAR          1   // @ % - _ + . , :
# define BRACKET_CHAR           2   // { }
# define DIEZE_CHAR             3   // #
# define ADD_SIMPLE_QUOTE_CHAR  4   // ! $ ^ & * ( ) = < > ? ; [ ] ` ~ "
# define ADD_DOUBLE_QUOTE_CHAR  5   // '

int is_special_char(char c)
{
    /* all no care char */
    if (c == '@' || c == '%' || c == '-'\
        || c == '_' || c == '+' || c == '.' || c == ',' || c == ':')
        return (NOEFFECT_CHAR);
    /* special rule */
    if ( c == '{' || c == '}')
        return (BRACKET_CHAR);
    if (c == '#')
        return (DIEZE_CHAR);
    if (c == '\'')
        return (ADD_DOUBLE_QUOTE_CHAR);
    /* alway add quote. special rule for ' or "*/
    if (c == '!' || c == '$' || c == '^' || c == '&' || c == '*' || c == '('\
        || c == ')' || c == '=' || c == '<' || c == '>' || c == '?' || c == ';'\
        || c == '[' || c == ']' || c == '`' || c == '~' || c == '\"')
            return (ADD_SIMPLE_QUOTE_CHAR);
    return (NORMAL_CHAR);
}

# define TOKEN_NO_CASE_SENSITIVE 0
# define TOKEN_CHECK_SPE_CHAR 1

int special_strcmp(char *s1, char *s2, int flag)
{
    char first = 0;
    char second = 0;
    int i = 0;
    int j = 0;
    /*  skip special char before first count char */
    while (s1[i] && is_special_char(s1[i]) != NORMAL_CHAR)
        i++;
    while (s2[j] && is_special_char(s2[j]) != NORMAL_CHAR)
        j++;
    /* compare all 'count' char */
    while (s1[i] || s2[j])
    {
        if (is_special_char(s1[i]) != NORMAL_CHAR)
            i++;
        else if (is_special_char(s2[j]) != NORMAL_CHAR)
            j++;
        else 
        {
            first = s1[i];
            second = s2[j];
            if (flag == TOKEN_NO_CASE_SENSITIVE)
            {
                first = ft_tolower(s1[i]);
                second = ft_tolower(s2[j]);
            }
            if (first != second)
                return (first - second);
            if (s1[i])
                i++;
            if (s2[j])
                j++;
        }
    }
    /* skip all special char after count char */
    while (s1[i] && is_special_char(s1[i]) != NORMAL_CHAR)
        i++;
    while (s2[j] && is_special_char(s2[j]) != NORMAL_CHAR)
        j++;
    return (first - second);
}

int special_char_gestion(char *current, char* min)
{
    /*  if is same letter no case sentive no special char exemple: ..a == _A */
    int ret = special_strcmp(current, min, TOKEN_NO_CASE_SENSITIVE); 
    if (ret == 0)
    {
        // ft_printf_fd(2, "without case ret 0 for %s == %s\n", current, min);
        /*  if we are here we are on egality on same letter, we need to check the case
            ret 0 for ..a == -a 
        */
        ret = special_strcmp(current, min, -1);
        if (ret == 0)
        {
            // int len_1 = ft_strlen(current);
            // int len_2 = ft_strlen(min);
            if (ft_strcmp(current, min) < 0)
                return (1);
        }
        else if (ret > 0) // ret > 0 because we want to print a before A
            return (1);
            // ft_printf_fd(2, "with case ret 0 for %s == %s\n", current, min);
    }
    else if (ret < 0)
        return (1);
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
        if (special_char_gestion(current->name, min_file->name) == 1)
            min = lst;
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