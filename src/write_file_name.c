#include "../include/ft_ls.h"

// static void display_symlink(struct stat *sb, int flag_nb, char *new, int sym_bool)
// {
//     t_file *file = fill_file_struct(sb, new, NULL, sym_bool);
//     if (file) {
//         write_file_name(*file, flag_nb, -1);
//         free(file->name);
//         free(file);
//     } 
//     free(sb);
// }

// static char* remove_last_word(char *str, char sep)
// {
//     int i = (int)ft_strlen(str);
//     int save_len = i;
//     char *new = NULL;

//     while (i > 0) {
//         if (i == save_len && str[i] == sep)
//             --i;
//         else if (str[i] == sep)
//             break ;
//         --i;
//     }
//     str[i] = '\0';
//     new = ft_calloc(sizeof(char), i+1);
//     if (new)
//         ft_strcpy(new, str, i+1);
//     return (new);
// }


// /* check got namne for local directory
//    check for absolute path (begin by slash or add it)
//    or remove last word to go to .. */
// static void stat_symlink(char* buff, char *parrent_path, char* path, int flag_nb)
// {
//     int         sym_bool = 0;
//     char        *new = NULL;
//     struct stat *sb = check_for_stat(buff, flag_nb, &sym_bool);

//     // ft_printf_fd(2, "%sFor path: %s, parent |%s| buff|%s|%s\n", CYAN, path, parrent_path, buff, RESET);
//     /* check for buff real path */
//     if (sb)
//         display_symlink(sb, flag_nb, buff, sym_bool);
//     else { /* else check for add parrent path or / before */
//         if (parrent_path && ft_strcmp(parrent_path, "/") == 0)
//             new = ft_strjoin(parrent_path, buff);
//         else {
//             if (parrent_path && parrent_path[ft_strlen(parrent_path) -1] != '/') 
//                 new = ft_strjoin(parrent_path, "/");
//             new = ft_strjoin_free(new, buff, 'f');
//         }
//         sb = check_for_stat(new, flag_nb, &sym_bool);
//         /* check for new path with parrent */
//         if (sb)
//             display_symlink(sb, flag_nb, buff, sym_bool);
//         else { /* else check to remove last word of path, this will search in current dir */
//             if (new)
//                 free(new);
//             new = remove_last_word(path, '/');
//             sb = check_for_stat(new, flag_nb, &sym_bool);
//             if (sb)
//                 display_symlink(sb, flag_nb, buff, sym_bool);
//             else
//                 multiple_fill_buff(" ", buff, NULL, NULL);
//         }
//         free(new);
//     }
// }

// static int write_symlink(char *path, char *parrent_path, int flag_nb, int space)
// {
//     char    buff[500];
//     char    *tmp;
//     int quote = quotes_required(path);

//     fill_buffer_color(path, E_CYAN, flag_nb, space, quote);
//     if (has_flag(flag_nb, L_OPTION))
//     {
        
//         if (parrent_path && path[0] != '/') /* avoid /bin/bin case*/
//             tmp = join_parent_name(parrent_path, path);
//         else
//             tmp = ft_strjoin(path, "");

//         if (!tmp)
//             return (MALLOC_ERR);
//         if (space == 0)
//             fill_buffer_char(' ');
//         fill_buffer("->");
//         int ret = readlink(tmp, buff, 199);
//         if (ret == -1) {
//             printf("For tmp: %s:", tmp);
//             perror(" readlink");
//         }
//         else {
//             buff[ret] = '\0';
//             stat_symlink(buff, parrent_path, path, flag_nb);
//         }
//         free(tmp);
//     }
//     return (0);
// }

/** Write_file_name
 * Core function to display file name, manage color quote ...
*/
int write_file_name(t_file file, int flag_nb, int space)
{
    int perm_color = E_NONE;

    if (is_executable_file(file.perm))
        perm_color = E_GREEN;
    if (is_full_perm(file.perm))
        perm_color = E_FILL_GREEN;
    else if (file.type == FIFO)
        perm_color = E_YELLOW_BLACK;
    else if (file.perm & S_ISUID)
        perm_color = E_FILL_RED;
    else if (file.perm & S_ISGID)
        perm_color = E_FILL_YELLOW;
    else if (file.type == SYMLINK)
        perm_color = E_CYAN;
    else if (file.type == CHARACTER || file.type == BLOCK)
        perm_color = E_YELLOW;
    else if (file.type == DIRECTORY)
        perm_color = E_BLUE;
    /* space -1 against recurcive SYMLINK display */
    // if(file.type == SYMLINK && space != -1) {
    //     if (write_symlink(file.name, file.parrent, flag_nb, space) == MALLOC_ERR)
    //         return (MALLOC_ERR);
    // }
    // else
    fill_buffer_color(file.name, perm_color, flag_nb, space, file.quote);
    if (!has_flag(flag_nb, L_OPTION) && space == 0)
        fill_buffer(" ");
    return (0);
}

