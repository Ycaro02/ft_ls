#include "../include/ft_ls.h"

void insert_space(int nb)
{
    if (nb > 0)
    {
        while (nb != 0)
        {
            fill_buffer_char(' ');
            nb--;
        }
    }
}

void write_user_name(long user_id, int space, int flag_nb)
{
    if (flag_nb & N_OPTION)
    {
        char *tmp = ft_ltoa(user_id);
        insert_space(space - ft_strlen(tmp));
        fill_buffer(tmp);
        free(tmp);
    }
    else
    {
        struct passwd* user = getpwuid(user_id);
        if (!user)
        {
            perror("getpwuid");
            fill_buffer("unknow");
        }
        else
        {
            fill_buffer(user->pw_name);
            insert_space(space - ft_strlen(user->pw_name));
        }
    }
    if (space != -1)
        fill_buffer_char(' ');
}

void write_group_name(long group_id, int space, int flag_nb)
{
    if (flag_nb & N_OPTION)
    {
        char *tmp = ft_ltoa(group_id);
        insert_space(space - ft_strlen(tmp));
        fill_buffer(tmp);
        free(tmp);
    }
    else
    {
        struct group* group = getgrgid(group_id);
        if (!group)
        {
            perror("getgrgid");
            fill_buffer("unknow");
        }
        else
        {
            fill_buffer(group->gr_name);
            insert_space(space - ft_strlen(group->gr_name));
        }
    }
    if (space != -1)
        fill_buffer_char(' ');
}

static int write_nb_link(long long nb_link, int space)
{
    char *tmp;
    
    tmp = ft_itoa((int)nb_link);
    if (!tmp)
        return (MALLOC_ERR);
    fill_buffer_char(' ');
    insert_space(space - ft_strlen(tmp));
    fill_buffer(tmp);
    free(tmp);
    fill_buffer_char(' ');
    return (0);
}


static int write_symlink(char *path, char *parrent_path, int flag_nb)
{
    char    buff[500];
    char    *tmp;

    fill_buffer_color(path, E_CYAN, flag_nb);
    if (flag_nb & L_OPTION)
    {
        if (parrent_path)
            tmp = join_parent_name(parrent_path, path);
        else
            tmp = ft_strjoin(path, "");
        if (!tmp)
            return (MALLOC_ERR);
        fill_buffer(" -> ");
        int ret = readlink(tmp, buff, 199);
        if (ret == -1)
            perror("readlink");
        else 
        {
            buff[ret] = '\0'; 
            fill_buffer(buff);
        }
        free(tmp);
    }
    return (0);
}

int write_file_name(t_file file, int is_exec, int flag_nb)
{
    if(file.type == SYMLINK)
    {
        if (write_symlink(file.name, file.parrent, flag_nb) == MALLOC_ERR)
            return (MALLOC_ERR);
    }
    else if (file.type == DIRECTORY)
        fill_buffer_color(file.name, E_BLUE, flag_nb);
    else if (is_exec == 0)
        fill_buffer_color(file.name, E_GREEN, flag_nb);
    else
        fill_buffer(file.name);
    // if (option == L_OPTION)
    if (flag_nb & L_OPTION)
        fill_buffer_char('\n');
    else
        fill_buffer(" ");
    return (0);
}


static int write_perm(t_file file, int *is_exec)
{
    char    *tmp;
    int     len;
    int     i;
    
    i = 0;
    tmp = get_perm(file.perm);
    if (!tmp)
        return (MALLOC_ERR);
    len = ft_strlen(tmp);
    if (len <= 1)
        fill_buffer("------");
    else if (len == 2)
        fill_buffer("---");
    while (tmp && tmp[i])
    {
        fill_buffer_perm(tmp[i], is_exec);
        i++;
    }
    free(tmp);
    return (0);
}

static int write_size(long size, int space)
{
    char *tmp;

    tmp = ft_itoa((int)size);
    if (!tmp)
        return (MALLOC_ERR);
    insert_space(space - ft_strlen(tmp));
    fill_buffer(tmp);
    free(tmp);
    fill_buffer_char(' ');
    return (0);
}

static int write_date(t_file file, int* space, int flag_nb)
{
    char **tmp = NULL;
    int i = 0;
    int j = S_MONTH;

    if (flag_nb & U_OPTION)
        tmp = get_printable_date(file.last_access);
    else if (flag_nb & C_OPTION)
        tmp = get_printable_date(file.last_status_change);
    else
        tmp = get_printable_date(file.last_change);
    if (!tmp)
    {
        perror("Malloc");
        return (MALLOC_ERR);
    }
    while (tmp[i])
    {
        insert_space(space[j] - ft_strlen(tmp[i]));
        fill_buffer(tmp[i]);
        fill_buffer_char(' ');
        i++;
        j++;
    }
    ft_free_tab(tmp);
    return (0);
}

int fill_buffer_l_option(t_file file, int *space, int flag_nb)
{   
    int is_exec;

    is_exec = 1;
    fill_buffer_char(file.type);
    if (write_perm(file, &is_exec) == MALLOC_ERR || \
        write_nb_link(file.nb_link, space[S_LINK]) == MALLOC_ERR)
        return (MALLOC_ERR);
    if (!(flag_nb & G_OPTION))
        write_user_name(file.user_id, space[S_USER], flag_nb);
    write_group_name(file.group_id, space[S_GROUP], flag_nb);
    if (write_size(file.size, space[S_SIZE]) == MALLOC_ERR || \
        write_date(file, space, flag_nb) == MALLOC_ERR || \
        write_file_name(file, is_exec, flag_nb) == MALLOC_ERR)
            return (MALLOC_ERR);
    if (flag_nb & Z_OPTION)
        diplay_xattr_acl(&file);
    return (0);
}