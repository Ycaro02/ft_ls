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


unsigned int    ft_check_base(char *str);

void    ft_putchar(char c) { write(1, &c, 1); }

unsigned int    ft_check_base(char *str)
{
        int     i;
        int     j;

        i = 0;
        j = 0;
        if (ft_strlen(str) <= 1)
                return (1);
        while (str[i])
        {
                if (str[i] < 32 || str[i] > 126 || str[i] == '+' || str[i] == '-')
                        return (1);
                j = i + 1;
                while (str[j])
                {
                        if (str[i] != str[j])
                                j++;
                        else if (str[i] == str [j])
                                return (1);
                }
                i++;
        }
        return (ft_strlen(str));
}

void    ft_putnbr_base(int nbr, char *base)
{
        int                     b_size;
        long int        n;

        n = nbr;
        b_size = ft_check_base(base);
        if (b_size == 1)
                return ;
        if (n < 0)
        {
                ft_putchar('-');
                n = -n;
        }
        if (n / b_size != 0)
                ft_putnbr_base(n / b_size, base);
        ft_putchar(base[n % b_size]);
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
        ft_putnbr_base(sb.st_mode & 0777, "01234567");
        j++;
    } 
    return(0);
}