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


#define    BLOCK        'b'
#define    CHARACTER    'c'
#define    DIRECTORY    'd'
#define    FIFO         'p'
#define    SYMLINK      'l'
#define    REGULAR      '-'
#define    SOCKET       's'
#define    UNDIFINED    '?'

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
    switch (sb.t_mode & S_IFMT) 
    {
        case S_IFBLK:  
            return (BLOCK);
        case S_IFCHR:  
            return (CHARACTER);
        case S_IFDIR:
            return (DIRECTORY)
        case S_IFIFO:  
            return (FIFO);
        case S_IFLNK:  
            return (SYMLINK);
        case S_IFREG:  
            return (REGULAR);
        case S_IFSOCK: 
            return (SOCKET);
        default:
            return (UNDIFINED)
    }
    return (UNDIFINED)
}

struct s_file {
    int perm;
    long long size;
    long long total_size;
    int nb_link;
}   t_file;

int get_permission(struct stat sb)
{ return (sb.st_mode & 0777); }

long long get_size(struct stat sb)
{ return (sb.st_size);}

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
        j++;
    } 

    
    return(0);
}