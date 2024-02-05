#include "../include/ft_ls.h"

/** last_char_is_slash
 * Basic check if last char is '/'
*/
static int last_char_is_slash(char *str)
{
    int i = 0;
    
    if (!str)
        return (-1);
    while (str[i])
        i++;
    if (i == 1 && str[0] == '/')
        return (0);
    if (str[i - 1] == '/')
        return (0);
    return (1);
}

/** join_parent_name
 * Join parent name with check to avoid double '/'
*/
char *join_parent_name(char* parent_name, char* path)
{
    char *str = NULL;

    if (last_char_is_slash(parent_name) == 0)
        str = ft_strjoin(parent_name, path);
    else {
        str = ft_strjoin(parent_name, "/");
        str = ft_strjoin_free(str, path, 'f');
    }
    return (str);
}

/** ft_strlen_word
 * Count until ' ' (space) reached
*/
int ft_strlen_word(char *s)
{
	int i = 0;

	while (s && s[i] && s[i] != ' ')
		i++;
	return (i);
}

/** Update_error
 * Update command line exit code, protected NA_CMD_LINE_ERR > NO_ACCESS_ERR
 * if NA_CMD_LINE_ERR is set do nothing else set error to NO_ACCESS_ERR
*/
void update_error(t_int8 *error)
{
     if (*error != NA_CMD_LINE_ERR)
        *error = NO_ACCESS_ERR;
}

/** is_point_dir
 * check if path is starting by '.' return accordate value with flag_nb
*/
int is_point_dir(char *path, int flag_nb, int display)
{
    if (display == 0) {
        if (has_flag(flag_nb, A_OPTION))
            return (1);
        else
            if (path && path[0] == '.')
                return (0);
    }
    else {
        if (has_flag(flag_nb, A_OPTION) && !has_flag(flag_nb, R_OPTION))
            return (1);
        else if (has_flag(flag_nb, R_OPTION) && has_flag(flag_nb, A_OPTION)) {
            if (ft_strcmp(path, ".") == 0 || ft_strcmp(path, "..") == 0)
                return (0);
            return (1);
        }
        if (path && path[0] == '.')
            return (0);
    }
    return (1);
}


void destroy_file(void *file_ptr)
{
    t_file *file = file_ptr;
    if (!file)
        return ;
    if (file->name)
        free(file->name);
    if (file->parrent)
        free(file->parrent);
    if (file->line)
        ft_free_tab(file->line);
}

/** ft_lstclear
 * Clear t_file lst
 * Can be removed to create void destroy_tfile(void *) and give it to ft_lstclear
*/
// void	ft_lstclear(t_list **lst, void (*del)(void*))
// {
// 	t_list	*tmp = NULL, *current = NULL;
//     t_file  *file = NULL;

// 	if (del == NULL || lst == NULL || *lst == NULL)
// 		return ;
// 	current = *lst;
// 	tmp = current;
// 	while (tmp != NULL) {
// 		tmp = current->next;
//         if (current->content) {
//             file = current->content;
//             del(file->name);
//             if (file->parrent)
//                 del(file->parrent);
//             del(current->content);
//         }
//         // if (current->content)
// 		free(current);
// 		current = tmp;
// 	}
// 	*lst = NULL;
// }

/** get_stdout_width
 * Get stdout width for manage column
 * if fail ( for example with redirection ) set it to 80
*/
int get_stdout_width()
{
    struct winsize win;
    int width;
    
    if (ioctl(1, TIOCGWINSZ, &win) != 0)
        return (BASIC_WIDTH); /* default width for redir need to move this */
    width = win.ws_col; 
    return (width);
}