#include "../include/ft_ls.h"
#include <sys/acl.h>


int ft_strlen_word(char *s)
{
	int i =0;
	while (s && s[i] && s[i] != ' ')
		i++;
	return (i);
}

static int list_xattr(char *path, char *list)
{
    ft_bzero(list, BUFSIZ);
    int listen_len = listxattr(path, list, BUFSIZ);
    if (listen_len == -1)
    {
        ft_printf_fd(2, "Listxattr error for %s\n", path);
        return (-1);
    }
    return (listen_len);
}

static int display_acl(t_file file, char *str)
{
    acl_t acl = acl_get_file(file.name, ACL_TYPE_ACCESS);
    fill_buffer(str);
    fill_buffer("\n# owner : ");
    write_user_name(file.user_id, -1);
    fill_buffer_char('\n');
    fill_buffer("# group : ");
    write_group_name(file.group_id, -1);
    fill_buffer_char('\n');
    if (!acl)
        ft_printf_fd(2, "ACL NULL\n");
    char *text = acl_to_text(acl, NULL);
    fill_buffer(text);
    fill_buffer_char('\n');
    acl_free(text);
    acl_free(acl);
    return (0);
}

int diplay_xattr(t_file *file)
{
    char value[BUFSIZ];
    char list[BUFSIZ];
    int i = 0;
    int res =0;

    int listen_len = list_xattr(file->name, list);
    if (listen_len == -1)
        return (-1);
    fill_buffer("# file: ");
    fill_buffer(file->name);
    fill_buffer_char('\n');
    while (i < listen_len)
    {
        ft_bzero(value, BUFSIZ);
        if (strncmp(&list[i], "system.posix_acl_", 17) == 0)
            display_acl(*file, &list[i]);
        else
        {
            fill_buffer(&list[i]);
            fill_buffer("=\"");
            res = getxattr(file->name, &list[i], value, BUFSIZ);
            if (res == -1)
            {
                ft_printf_fd(2, "Erorr getxattr\n");
                return (1);
            }
            fill_buffer(value);
            fill_buffer("\"\n");
        }
        i += ft_strlen_word(&list[i]) + 1;
    }
    return (0);
}