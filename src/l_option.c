#include "../include/ft_ls.h"

inline static void insert_space(int nb)
{
    if (nb > 0) {
        while (nb != 0) {
            fill_buffer_char(' ');
            nb--;
        }
    }
}

static int write_date(t_file *file, int* space, int flag_nb)
{
    char **tmp = NULL;
    int i = 0;
    int j = S_MONTH;

    if (file->type == -2) {
        multiple_fill_buff(file->line[S_DAY], file->line[S_MONTH], file->line[S_HOUR], NULL);
    }
    if (has_flag(flag_nb, U_OPTION))
        tmp = get_printable_date(file->last_access);
    else if (has_flag(flag_nb, C_OPTION))
        tmp = get_printable_date(file->last_status_change);
    else
        tmp = get_printable_date(file->last_change);
    if (!tmp) {
        ft_printf_fd(2, "Malloc error\n");
        return (MALLOC_ERR);
    }
    while (tmp[i]) {
        insert_space(space[j] - ft_strlen(tmp[i]));
        fill_buffer(tmp[i]);
        fill_buffer_char(' ');
        i++;
        j++;
    }
    ft_free_tab(tmp);
    return (0);
}

static int write_size(t_file *file, int *space)
{
    // printf("name %s size: %d minor:%d major: %d\n", file->name, space[S_SIZE], space[S_MINOR_SIZE], space[S_MAJOR_SIZE]);
    if (file->type == CHARACTER || file->type == BLOCK) {
        // printf("for %s size :%ld\n", file->name, (space[S_MAJOR_SIZE] - ft_strlen(size)));
        if (space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] > space[S_SIZE]) {
            insert_space(space[S_MAJOR_SIZE] - ft_strlen(file->line[S_MAJOR_SIZE]));
            fill_buffer(file->line[S_MAJOR_SIZE]);
            fill_buffer(", ");
            insert_space((space[S_MINOR_SIZE])- ft_strlen(file->line[S_MINOR_SIZE]));
            fill_buffer(file->line[S_MINOR_SIZE]);
        }
        else {
            int pad_len = space[S_MINOR_SIZE] + ft_strlen(file->line[S_MAJOR_SIZE]);
            insert_space(space[S_SIZE] - pad_len);
            fill_buffer(file->line[S_MAJOR_SIZE]);
            fill_buffer(", ");
            insert_space(space[S_MINOR_SIZE] - ft_strlen(file->line[S_MINOR_SIZE]) - 2);
            fill_buffer(file->line[S_MINOR_SIZE]);
        }
        fill_buffer_char(' ');
        // ft_printf_fd(2, "MINOR :%d\n", minor(file->rdev));
        // ft_printf_fd(2, "MAJOR :%d\n", major(file->rdev));
        return (0);
    }
    else { /* just classic size */
        if (space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] != 0\
            && space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] + 2 > space[S_SIZE])
            insert_space(space[S_MAJOR_SIZE] + space[S_MINOR_SIZE] + 2 - ft_strlen(file->line[S_SIZE]));
        else {
            // if ((int)ft_strlen(file->line[S_SIZE]) != space[S_SIZE])
            // if ((int)ft_strlen(file->line[S_SIZE]) == space[S_SIZE])
                // insert_space(space[S_SIZE] - 2 - ft_strlen(file->line[S_SIZE])); // DEVIL - 1
            // else
                insert_space(space[S_SIZE] - ft_strlen(file->line[S_SIZE])); // DEVIL - 1
        }
        fill_buffer(file->line[S_SIZE]);
    }
    fill_buffer_char(' ');
    return (0);
}

static int write_file_line(t_file *file, t_context *c, t_file_context *file_c)
{
    /* display perm string and pad */
    fill_buffer(file->line[S_PERM]);
    insert_space(file_c->space[S_PERM] - ft_strlen(file->line[S_PERM]));
    /* display nb_link string and pad */
    fill_buffer_char(' ');
    insert_space(file_c->space[S_LINK] - ft_strlen(file->line[S_LINK]));
    multiple_fill_buff(file->line[S_LINK], " ", NULL, NULL);
    /* display user id/name string and pad */
    if (!has_flag(c->flag_nb, G_OPTION)) {
        fill_buffer(file->line[S_USER]);
        insert_space(file_c->space[S_USER] - ft_strlen(file->line[S_USER]));
        if (file_c->space[S_USER] != -1)
            fill_buffer_char(' ');
    }
    /* display group id/name string and pad */
    fill_buffer(file->line[S_GROUP]);
    insert_space(file_c->space[S_GROUP] - ft_strlen(file->line[S_GROUP]));
    if (file_c->space[S_GROUP] != -1)
        fill_buffer_char(' ');
    /* display size/MAJOR_MINOR string and pad */
    write_size(file, file_c->space);
    /* display date string and pad */
    if (file->type != -2 && write_date(file, file_c->space, c->flag_nb) == MALLOC_ERR)
        return (MALLOC_ERR);
    if (write_file_name(file, c, file_c, file_c->space[S_NAME_QUOTE]) == MALLOC_ERR)
        return (MALLOC_ERR);
    return (0);
}

/** fill_buffer_l_option
 * HUB caller for all write functopm for l_option
 * Need to rework this to jsut display file_line
*/
int fill_buffer_l_option(t_file *file, t_context *c, t_file_context *file_c)
{
    int err = write_file_line(file, c, file_c);
    if (has_flag(c->flag_nb, Z_OPTION))
        diplay_xattr_acl(file);
    return (err);
}