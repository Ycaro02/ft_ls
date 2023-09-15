#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include "src/libft/libft.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include <grp.h>
#include <pwd.h>

#define    BLOCK        'b'
#define    CHARACTER    'c'
#define    DIRECTORY    'd'
#define    FIFO         'p'
#define    SYMLINK      'l'
#define    REGULAR      '-'
#define    SOCKET       's'
#define    UNDIFINED    '?'

typedef struct s_file 
{
    int perm;
    long long size;
    long long total_size;
    int nb_link;
    char type;
    time_t last_change;
    long user_id;
    long group_id;
} t_file;

void    putnbr_decimal_to_octal(int nbr)
{
        int             b_size = 8;
        long int        n = nbr;
        char            *base = "01234567";

        if (n / b_size != 0)
                putnbr_decimal_to_octal(n / b_size);
        write(1, &base[n % b_size], 1);
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

t_file fill_file_struct(struct stat sb)
{
    t_file file;

    file.total_size = -1;
    file.type = get_type(sb);
    file.perm = sb.st_mode & 0777;
    file.size = sb.st_size;
    file.nb_link = sb.st_nlink;
    file.last_change = sb.st_mtime;
    file.user_id = sb.st_uid;
    file.group_id = sb.st_gid;
    return (file);
}

char *get_user_name(long user_id)
{
    struct passwd* user = getpwuid(user_id);
    char *name = strdup(user->pw_name);
    return (name);
}

char *get_group_name(long group_id)
{
    struct group* group = getgrgid(group_id);
    char *name = strdup(group->gr_name);
    return (name);
}

int main(int argc, char**argv)
{
    int j = 1;
    if (argc == 1)
        return (1);
    struct stat     sb;
    while (j < argc)   
    {
        if (lstat(argv[j], &sb) == -1)
        {
            perror("lstat faillure");
            return (1);
        }
        switch (sb.st_mode & S_IFMT) 
        {
            case S_IFBLK:  
                printf("block device\n");            
                break;
            case S_IFCHR:  
                printf("character device\n");        
                break;
            case S_IFDIR:  
                printf("directory\n");               
                break;
            case S_IFIFO:  
                printf("FIFO/pipe\n");               
                break;
            case S_IFLNK:  
                printf("symlink\n");                 
                break;
            case S_IFREG:  
                printf("regular file\n");            
                break;
            case S_IFSOCK: 
                printf("socket\n");                  
                break;
            default:       
                printf("unknown?\n");                
                break;
    }
        printf("Argv[%d]:                 %s\n", j, argv[j]);
        printf("I-node number:            %ld\n", (long) sb.st_ino);
        printf("Mode:                     %lo (octal)\n",(unsigned long) sb.st_mode);
        printf("Link count:               %ld\n", (long) sb.st_nlink);
        printf("Ownership:                UID=%ld   GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);
        printf("Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
        printf("File size:                %lld bytes\n", (long long) sb.st_size);
        printf("Blocks allocated:         %lld\n", (long long) sb.st_blocks);
        printf("Last status change:       %s", ctime(&sb.st_ctime));
        printf("Last file access:         %s", ctime(&sb.st_atime));
        printf("Last file modification:   %s", ctime(&sb.st_mtime));
        write(1, "My res:\n", ft_strlen("my_res\n"));
        putnbr_decimal_to_octal(sb.st_mode & 0777);
        struct passwd* user = getpwuid(sb.st_uid);
        printf("name        = %s\n", user->pw_name);
        printf("pass        = %s\n", user->pw_passwd);
        printf("user id     = %d\n", user->pw_uid);
        printf("group id    = %d\n", user->pw_gid);
        printf("gecos       = %s\n", user->pw_gecos);
        printf("dir common  = %s\n", user->pw_dir);
        printf("shell       = %s\n", user->pw_shell);
        struct group* tmp = getgrgid(sb.st_gid);
        printf("group name  = %s\n", tmp->gr_name);
        struct s_file file = fill_file_struct(sb);
        printf("perm %d\n", file.perm);
        printf("size %lld\n", file.size);
        printf("size %lld\n", file.total_size);
        printf("link %d\n", file.nb_link);
        printf("type %c\n", file.type);
        printf("last_change %s\n", ctime(&file.last_change));
        printf("userId %ld\n", file.user_id);
        printf("last_change %ld\n", file.group_id);
        j++;
    } 

    return(0);
}

