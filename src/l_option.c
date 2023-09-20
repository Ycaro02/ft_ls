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

// static void putnbr_decimal_to_octal(int nbr, int *is_exec)
// {
//         int             b_size = 8;
//         long int        n = nbr;
//         char            *base = "01234567";

//         if (n / b_size != 0)
//                 putnbr_decimal_to_octal(n / b_size, is_exec);
//         fill_buffer_perm(base[n % b_size], is_exec);
// }

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


static char *remove_char(char *str, char c)
{
    int i = 0;
    char* tmp;

    while (str && str[i] && str[i] == c)
        i++;
    if (i < 3 && i != 0)
    {
        tmp = ft_strdup(&str[i]);
        if (!tmp)
            return (NULL);
        free(str);
    }
    else
        tmp = NULL;
    return (tmp);
}

static char *get_perm(int nbr)
{
    char *perm;

    perm = malloc(sizeof(char) * 4);
    perm[0] = '8'; 
    perm[1] = '8'; 
    perm[2] = '8'; 
    perm[3] = '\0';
    convert(perm, nbr, 3);
    char *tmp = remove_char(perm, '8');
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
        return (1);
    if (parent)
    {
        file->parrent = ft_strdup(parent);
        if (!file->parrent)
            return (1);
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
    file->nb_block = sb.st_blocks;
    if (fill_name_and_parent(file, path, parent) == 1)
        return (NULL);
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

static void write_nb_link(long long nb_link, int space)
{
    fill_buffer_char(' ');
    char *tmp = ft_itoa((int)nb_link);
    insert_space(space - ft_strlen(tmp));
    fill_buffer(tmp);
    free(tmp);
    fill_buffer_char(' ');
}


static int write_symlink(char *path, char *parrent_path, int option)
{
    char    buff[200];
    char    *tmp;

    fill_buffer_color(path, E_CYAN);
    if (option == L_OPTION)
    {
        if (parrent_path)
            tmp = join_parent_name(parrent_path, path);
        else
            tmp = ft_strjoin(path, "");
        if (!tmp)
            return (1);
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

void write_file_name(t_file file, int is_exec, int option)
{
    if(file.type == SYMLINK)
        write_symlink(file.name, file.parrent, option);
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
}

static void write_date(time_t *last_change, int* space)
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
        return ;
    }
    while (tmp[i])
    {
        insert_space(space[j] - ft_strlen(tmp[i]));
        fill_buffer(tmp[i]);
        fill_buffer_char(' ');
        i++;
        j++;
    }
    free_tab(tmp);
}

static void write_perm(t_file file, int *is_exec)
{
    char *tmp;
    tmp = get_perm(file.perm);
    int len = ft_strlen(tmp);
    if (len == 1)
        fill_buffer("------");
    else if (len == 2)
        fill_buffer("---");
    int i = 0;
    while (tmp && tmp[i])
    {
        fill_buffer_perm(tmp[i], is_exec);
        i++;
    }
    free(tmp);
}

void fill_buffer_l_option(t_file file, int *space)
{   
    char *tmp;
    int is_exec = 1;
    
    fill_buffer_char(file.type);
    write_perm(file, &is_exec);
    write_nb_link(file.nb_link, space[S_LINK]);
    write_user_name(file.user_id, space[S_USER]);
    write_group_name(file.group_id, space[S_GROUP]);
    tmp = ft_itoa((int)file.size);
    insert_space(space[S_SIZE] - ft_strlen(tmp));
    fill_buffer(tmp);
    free(tmp);
    fill_buffer_char(' ');
    write_date(&file.last_change, space);
    fill_buffer_char(' ');
    write_file_name(file, is_exec, L_OPTION);
}