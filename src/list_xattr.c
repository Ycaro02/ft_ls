#include "../include/ft_ls.h"

/* listxattr: List attributes of the file pointed to by PATH into the user-supplied
   buffer LIST (which is SIZE bytes big).  Return 0 on success, -1 for
   errors.  */
int list_xattr(char *path, char *list)
{
    ft_bzero(list, BUFSIZ);
    int listen_len = listxattr(path, list, BUFSIZ);
    if (listen_len == -1)
        return (-1);
    return (listen_len);
}

static int display_acl(t_file file, char *str, char* full_name)
{
    acl_t acl = acl_get_file(full_name, ACL_TYPE_ACCESS);
    char *text = NULL;
    
    if (!acl) {
        ft_printf_fd(2, "ACL NULL for %s\n", full_name);
        return (-1);
    }
    multiple_fill_buff(str, "\n# owner : ", NULL, NULL);
    write_user_name(file.user_id, -1, 0);
    multiple_fill_buff("\n", "# group : ", NULL, NULL);
    write_group_name(file.group_id, -1, 0);
    text = acl_to_text(acl, NULL);
    multiple_fill_buff("\n", text, NULL, NULL);
    acl_free(text);
    acl_free(acl);
    return (0);
}

static int fill_acl_attr(t_file *file, char *str, char *value, char *tmp)
{
    ft_bzero(value, BUFSIZ);
    if (ft_strncmp(str, "system.posix_acl_", 17) == 0)
        display_acl(*file, str, tmp);
    else {
        if (getxattr(tmp, str, value, BUFSIZ) == -1) {
            ft_printf_fd(2, "Erorr getxattr for %s\n", tmp);
            free(tmp);
            return (-1);
        }
        multiple_fill_buff(str, "=\"", NULL, NULL);
        multiple_fill_buff(value, "\"\n", NULL, NULL);
    }
    return (0);
}

static char *get_new_path(t_file *file)
{
    char *tmp;

    tmp = NULL;
    if (file->parrent)
        tmp = join_parent_name(file->parrent, file->name);
    else
        tmp = ft_strjoin(file->name, "");
    if (!tmp)
        return (NULL);
    return (tmp);
}

int diplay_xattr_acl(t_file *file)
{
    char value[BUFSIZ];
    char list[BUFSIZ];
    int i = 0, listen_len = 0;
    char *tmp = NULL;

    tmp = get_new_path(file);
    if (!tmp)
        return (MALLOC_ERR);
    listen_len = list_xattr(tmp, list);
    if (listen_len == -1) {
        free(tmp);
        return (-1);
    }
    while (i < listen_len) {
        if (i == 0) {
            fill_buffer("\n\n# file :");
            write_file_name(*file, COLOR_OPTION, -1);
            fill_buffer("\n");
            // multiple_fill_buff("\n\n# file : ", file->name, "\n", NULL);
        }
        if (fill_acl_attr(file, &list[i], value, tmp) == -1) {
            free(tmp);
            return (-1);
        }
        i += ft_strlen_word(&list[i]) + 1;
    }
    free(tmp);
    return (0);
}

int check_acl(t_file *file)
{
    char list[BUFSIZ];
    int i = 0, listen_len = 0;
    char *tmp = NULL;
//
    tmp = get_new_path(file);
    if (!tmp)
        return (MALLOC_ERR);
    listen_len = list_xattr(tmp, list);
    if (listen_len == -1) {
        free(tmp);
        return (1);
    }
    while (i < listen_len) {
        if (ft_strncmp(&list[i], "system.posix_acl_", 17) == 0) {
            free(tmp);
            return (0);
        }
        i += ft_strlen_word(&list[i]) + 1;
    }
    free(tmp);
    return (1);   
}

int check_lst_acl(t_list *lst)
{
    int ret = 0;

    while (lst) {
        ret = check_acl(lst->content);
        if (ret == 0 || ret == MALLOC_ERR)
            break;
        lst = lst->next;
    }
    return (ret);
}
