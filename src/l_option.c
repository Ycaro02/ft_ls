#include "../ft_ls.h"


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

static void putnbr_decimal_to_octal(int nbr, int *is_exec)
{
        int             b_size = 8;
        long int        n = nbr;
        char            *base = "01234567";

        if (n / b_size != 0)
                putnbr_decimal_to_octal(n / b_size, is_exec);
        fill_buffer_perm(base[n % b_size], is_exec);
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

t_file *fill_file_struct(struct stat sb, char *path, char *parent)
{
    t_file *file;

    file = malloc(sizeof(t_file));
    if (!file)
    {
        perror("Malloc");
        return (NULL);
    }
    file->total_size = -1;
    file->type = get_type(sb);
    file->perm = sb.st_mode & 0777;
    file->size = sb.st_size;
    file->nb_link = sb.st_nlink;
    file->last_change = sb.st_mtime;
    file->user_id = sb.st_uid;
    file->group_id = sb.st_gid;
    file->name = ft_strdup(path);
    file->nb_block = sb.st_blocks;
    file->parrent = ft_strdup(parent);
    return (file);
}

static void insert_space(int nb)
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

static void write_user_name(long user_id, int space)
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
    fill_buffer_char(' ');
}

static void write_group_name(long group_id, int space)
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
    fill_buffer_char(' ');
}

static void write_nb_link(long long nb_link, int *space)
{
    fill_buffer_char(' ');
    char *tmp = ft_itoa((int)nb_link);
    insert_space(space[4] - ft_strlen(tmp));
    fill_buffer(tmp);
    free(tmp);
    fill_buffer_char(' ');
}


static void write_symlink(char *path, char *parrent_path)
{
    char    *buff;
    char    *tmp;
    int     ret;

    buff = ft_calloc(sizeof(char), 200);
    if (!buff)
        return;
    tmp = join_parent_name(parrent_path, path);
    if (!tmp)
    {
        free(buff);
        return ;
    }
    fill_buffer_color(path, E_CYAN);
    fill_buffer(" -> ");
     ret = readlink(tmp, buff, 199);
    if (ret == -1)
        perror("readlink");
    else 
        fill_buffer_color(buff, E_BLUE);
    free(buff);
    free(tmp);
}


static void write_file_name(t_file file, int is_exec)
{
    fill_buffer_char(' ');
    if(file.type == SYMLINK)
        write_symlink(file.name, file.parrent);
    else if (file.type == DIRECTORY)
        fill_buffer_color(file.name, E_BLUE);
    else if (is_exec == 0)
        fill_buffer_color(file.name, E_GREEN);
    else
        fill_buffer(file.name);
    fill_buffer_char('\n');
}

void fill_buffer_l_option(t_file file, int *space)
{   
    char *tmp;
    int is_exec = 1;
    
    tmp = ft_itoa((int)file.size);
    fill_buffer_char(file.type);
    putnbr_decimal_to_octal(file.perm, &is_exec);
    write_nb_link(file.nb_link, space);
    write_user_name(file.user_id, space[0]);
    write_group_name(file.group_id, space[1]);
    insert_space(space[2] - ft_strlen(tmp));
    fill_buffer(tmp);
    free(tmp);
    fill_buffer_char(' ');
    tmp = get_printable_date(&file.last_change);
    fill_buffer(tmp);
    free(tmp);
    write_file_name(file, is_exec);
}

static int get_len_size(t_file file)
{
    char    *tmp;
    int     max;

    tmp = ft_itoa((int)file.size);
    max = ft_strlen(tmp);
    free(tmp);
    return (max);
}

static int get_group_name_len(t_file file)
{
    struct group* group = getgrgid(file.group_id);
    if (!group)
    {
        perror("getgrgid");
        return (-1);
    }
    int nb = ft_strlen(group->gr_name);
    return (nb);
}

static int get_user_name_len(t_file file)
{
    struct passwd* user = getpwuid(file.user_id);
    if (!user)
    {
        perror("getpwuid");
        return (-1);
    }
    int nb = ft_strlen(user->pw_name);
    return (nb);
}


static int get_len_date(t_file file)
{
    char *tmp = get_printable_date(&file.last_change);
    int nb = ft_strlen(tmp);
    free(tmp);
    return (nb);
}

static int get_len_nb_link(t_file file)
{
    char *tmp = ft_itoa(file.nb_link);
    int nb = ft_strlen(tmp);
    free(tmp);
    return (nb);
}


int get_nb_space(t_list *lst, int(*get_len_info)(t_file))
{
    t_file *file;
    t_list *current = lst;
    int max = -1;

    file = NULL;
    while (current)
    {
        file = current->content;
        int tmp = get_len_info(*file);
        if (tmp > max)
            max = tmp;
        current = current->next;
    }
    return (max);
}

int *get_all_space(t_list *lst)
{
    int *array;

    array = malloc(sizeof(int )* 5);
    if (!array)
    {
        new_lstclear(&lst, free);
        perror("Malloc");
        exit(1);
    }
    array[0] = get_nb_space(lst, get_user_name_len);
    array[1] = get_nb_space(lst, get_group_name_len);
    array[2] = get_nb_space(lst, get_len_size);
    array[3] = get_nb_space(lst, get_len_date);
    array[4] = get_nb_space(lst, get_len_nb_link);
    return (array);
}
