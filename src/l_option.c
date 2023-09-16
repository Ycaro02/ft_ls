#include "../ft_ls.h"


static void fill_buffer_perm(char c)
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
        x = 'x';
    fill_buffer_char(r);
    fill_buffer_char(w);
    fill_buffer_char(x);
}


static void putnbr_decimal_to_octal(int nbr)
{
        int             b_size = 8;
        long int        n = nbr;
        char            *base = "01234567";

        if (n / b_size != 0)
                putnbr_decimal_to_octal(n / b_size);
        // write(1, &base[n % b_size], 1);
        fill_buffer_perm(base[n % b_size]);
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

t_file *fill_file_struct(struct stat sb, char *path)
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
    return (file);
}

static void write_user_name(long user_id)
{
    struct passwd* user = getpwuid(user_id);
    fill_buffer(user->pw_name);
}

static void write_group_name(long group_id)
{
    struct group* group = getgrgid(group_id);
    fill_buffer(group->gr_name);
}

void fill_buffer_l_option(t_file file)
{   
    fill_buffer(&file.type);
    putnbr_decimal_to_octal(file.perm);
    fill_buffer_char(' ');
 
    char *tmp = ft_itoa((int)file.nb_link);
    fill_buffer(tmp);
    free(tmp);
   
    fill_buffer_char(' ');
    
    write_user_name(file.user_id);
   
    fill_buffer_char(' ');
    
    write_group_name(file.group_id);
    
    fill_buffer_char(' ');
    tmp = ft_itoa((int)file.size);
    fill_buffer(tmp);
    free(tmp);
    fill_buffer_char(' ');
    
    tmp = ft_itoa((int)file.total_size);
    fill_buffer(tmp);
    fill_buffer_char(' ');
    free(tmp);
    
    tmp = get_printable_date(&file.last_change, 0);
    fill_buffer(tmp);
    free(tmp);
    fill_buffer_char(' ');
    fill_buffer(file.name);
    fill_buffer_char(' ');
}


void display_file_struct(t_file file)
{
        printf("perm %d\n", file.perm);
        printf("size %lld\n", file.size);
        printf("size %lld\n", file.total_size);
        printf("link %d\n", file.nb_link);
        printf("type %c\n", file.type);
        printf("last_change %s\n", ctime(&file.last_change));
        printf("userId %ld\n", file.user_id);
        printf("last_change %ld\n", file.group_id);
}
