#include "src/libft/libft.h"

#include <stdio.h>

void fill_buff(char *buff, int *buff_i, char *str)
{
    if (!str)
        return ;
    int i = 0;
    if (*buff_i != 0)
        (*buff_i)--;
    while (str[i])
    {
        buff[*buff_i] = str[i];
        i++;
        (*buff_i)++;
        if (*buff_i > 10)
        {
            printf("\nbuffer =\n%s", buff);
            ft_bzero(buff, *buff_i);
            *buff_i = 0;
        }
    }
    buff[*buff_i] = '\n';
    (*buff_i)++;
    buff[*buff_i] = '\0';
    (*buff_i)++;
}

int main (int argc, char **argv)
{
    char buff[100000];
    int i = 0;
    int j = 0;
    while (j < argc)
    {
        fill_buff(buff, &i, argv[j]);
        j++;
    }
    if (i != 0)
        printf("\nbuffer =\n%s", buff);
}