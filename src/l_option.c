#include "../include/ft_ls.h"

static void fill_buffer_perm(char c, int *is_exec)
{
    int nb = c - 48;
    char r = '-';
    char w = '-';
    char x = '-';
    if (nb & 4)
        r = 'r';
    if (nb & 2)
        w = 'w';
    if (nb & 1)
    {
        x = 'x';
        *is_exec = 0;   
    }
    fill_buffer_char(r);
    fill_buffer_char(w);
    fill_buffer_char(x);
}

static void convert(char* perm, int nbr, int index)
{
        int             b_size = 8;
        long int        n = nbr;
        char            *base = "01234567";

        index -= 1;
        if (n / b_size != 0)
            convert(perm, n / b_size, index);
        perm[index] = base[n % b_size];
}

static char *remove_char(char *str, char c, int *err)
{
    int i = 0;
    char* tmp;

    tmp = NULL;
    while (str && str[i] && str[i] == c)
        i++;
    if (i < 3 && i != 0)
    {
        tmp = ft_strdup(&str[i]);
        if (!tmp)
        {
            *err = MALLOC_ERR;
            return (NULL);
        }
        free(str);
    }
    return (tmp);
}

char *get_perm(int nbr)
{
    char *perm;
    int   err;

    err = 0;
    perm = malloc(sizeof(char) * 4);
    if (!perm)
        return (NULL);
    perm[0] = '8'; 
    perm[1] = '8'; 
    perm[2] = '8'; 
    perm[3] = '\0';
    convert(perm, nbr, 3);
    char *tmp = remove_char(perm, '8', &err);
    if (err == MALLOC_ERR)
        return (NULL);
    if (tmp)
        return (tmp);
    return (perm);
}

char get_type(struct stat sb)
{
    switch (sb.st_mode & S_IFMT) 
    {
        case S_IFBLK:  
            return (BLOCK);
        case S_IFCHR:  
            return (CHARACTER);
        case S_IFDIR:
            return (DIRECTORY);
        case S_IFIFO:  
            return (FIFO);
        case S_IFLNK:  
            return (SYMLINK);
        case S_IFREG:  
            return (REGULAR);
        case S_IFSOCK: 
            return (SOCKET);
        default:
            return (UNDIFINED);
    }
    return (UNDIFINED);
}

static int fill_name_and_parent(t_file *file, char *path, char *parent)
{
    file->name = ft_strdup(path);
    if (!file->name)
        return (MALLOC_ERR);
    if (parent)
    {
        file->parrent = ft_strdup(parent);
        if (!file->parrent)
            return (MALLOC_ERR);
    }
    else
        file->parrent = NULL;
    return (0);
}

t_file *fill_file_struct(struct stat sb, char *path, char *parent)
{
    t_file *file;

    file = malloc(sizeof(t_file));
    if (!file)
        return (NULL);
    file->total_size = -1;
    file->type = get_type(sb);
    file->perm = sb.st_mode & 0777;
    file->size = sb.st_size;
    file->nb_link = sb.st_nlink;
    file->last_status_change = sb.st_ctime;
    file->last_access = sb.st_atime;
    file->last_change = sb.st_mtime;
    file->user_id = sb.st_uid;
    file->group_id = sb.st_gid;
    file->nb_block = sb.st_blocks;
    if (fill_name_and_parent(file, path, parent) == MALLOC_ERR)
        return (NULL);
    return (file);
}

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

void write_user_name(long user_id, int space)
{
    struct passwd* user = getpwuid(user_id);
    if (!user)
    {
        perror("getpwuid");
        fill_buffer("unknow");
    }
    else
    {
        insert_space(space - ft_strlen(user->pw_name));
        fill_buffer(user->pw_name);
    }
    if (space != -1)
        fill_buffer_char(' ');
}

void write_group_name(long group_id, int space)
{
    struct group* group = getgrgid(group_id);
    if (!group)
    {
        perror("getgrgid");
        fill_buffer("unknow");
    }
    else
    {
        insert_space(space - ft_strlen(group->gr_name));
        fill_buffer(group->gr_name);
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


static int write_symlink(char *path, char *parrent_path, int option)
{
    char    buff[500];
    char    *tmp;

    fill_buffer_color(path, E_CYAN);
    if (option == L_OPTION)
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

int write_file_name(t_file file, int is_exec, int option)
{
    if(file.type == SYMLINK)
    {
        if (write_symlink(file.name, file.parrent, option) == MALLOC_ERR)
            return (MALLOC_ERR);
    }
    else if (file.type == DIRECTORY)
        fill_buffer_color(file.name, E_BLUE);
    else if (is_exec == 0)
        fill_buffer_color(file.name, E_GREEN);
    else
        fill_buffer(file.name);
    if (option == L_OPTION)
        fill_buffer_char('\n');
    else
        fill_buffer(" ");
    return (0);
}

static int write_date(time_t *last_change, int* space)
{
    char **tmp;
    int i;
    int j;
    
    j = S_MONTH;
    i = 0;
    tmp = NULL;
    tmp = get_printable_date(last_change);
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

int fill_buffer_l_option(t_file file, int *space, int flag_nb)
{   
    int is_exec;

    is_exec = 1;
    fill_buffer_char(file.type);
    if (write_perm(file, &is_exec) == MALLOC_ERR || \
        write_nb_link(file.nb_link, space[S_LINK]) == MALLOC_ERR)
        return (MALLOC_ERR);
    write_user_name(file.user_id, space[S_USER]);
    write_group_name(file.group_id, space[S_GROUP]);
    if (write_size(file.size, space[S_SIZE]) == MALLOC_ERR || \
        write_date(&file.last_change, space) == MALLOC_ERR || \
        write_file_name(file, is_exec, L_OPTION) == MALLOC_ERR)
        return (MALLOC_ERR);
    if (flag_nb & Z_OPTION)
        diplay_xattr_acl(&file);
    return (0);
}