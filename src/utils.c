#include "../include/ft_ls.h"

void update_error(int *error)
{
     if (*error != NA_CMD_LINE_ERR)
        *error = NO_ACCESS_ERR;
}

int print_error(char *msg, char* str, int error_type, int use_perror)
{
    if (str)
        ft_putstr_fd(str, 2);
    if (use_perror == 0)
        perror(msg);
    else
        ft_putstr_fd(msg, 2);
    return (error_type);
}

int get_lst_len(t_list *lst)
{
    int i;

    i = 0;
    while(lst)
    {
        lst = lst->next;
        i++;
    }
    return (i);
}

int is_point_dir(char *path, int flag_nb)
{
    if (flag_nb & A_OPTION && !(flag_nb & R_OPTION))
        return (1);
    else if (flag_nb & R_OPTION && flag_nb & A_OPTION)
    {
        if (ft_strcmp(path, ".") == 0 || ft_strcmp(path, "..") == 0)
            return (0);
        return (1);
    }
    if (path && path[0] == '.')
        return (0);
    return (1);
}

char *join_parent_name(char* parent_name, char* path)
{
    char *str = NULL;
    if (last_char_is_slash(parent_name) == 0)
        str = ft_strjoin(parent_name, path);
    else
    {
        str = ft_strjoin(parent_name, "/");
        str = ft_strjoin_free(str, path, 'f');
    }
    return (str);
}

int last_char_is_slash(char *str)
{
    if (!str)
        return (-1);
    int i = 0;
    while (str[i])
        i++;
    if (i == 1 && str[0] == '/')
        return (0);
    if (str[i - 1] == '/')
        return (0);
    return (1);
}


void	new_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*tmp;
	t_list	*current;

	if (del == NULL || lst == NULL || *lst == NULL)
		return ;
	current = *lst;
	tmp = current;
	while (tmp != NULL)
	{
		tmp = current->next;
        t_file *file = current->content;
        del(file->name);
        if (file->parrent)
            del(file->parrent);
        del(current->content);
		free(current);
		current = tmp;
	}
	*lst = NULL;
}


int get_stdout_width()
{
    struct winsize win;
    int width;
    
    if (ioctl(1, TIOCGWINSZ, &win) != 0)
        return (-1); 
    width = win.ws_col; 
    return (width);
}

void display_file_lst(t_list *lst)
{
    ft_printf_fd(1, "\nDisplay file lst\n");
    while (lst)
    {
        t_file *file = lst->content;
        ft_printf_fd(1, "%s\n", file->name);
        lst = lst->next;
    }
}
