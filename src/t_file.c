#include "../include/ft_ls.h"


static int fill_name_and_parent(t_file *file, char *path, char *parent)
{
    file->name = ft_strdup(path);
    if (!file->name)
        return (MALLOC_ERR);
    if (parent) {
        file->parrent = ft_strdup(parent);
        if (!file->parrent) {
            free(file->name);
            return (MALLOC_ERR);
        }
    }
    else
        file->parrent = NULL;
    return (0);
}

int check_for_quote(char *str)
{
    int quote = quotes_required(str);
    return (quote > NOEFFECT_CHAR ? quote : NORMAL_CHAR);
}

/** 
 * 
*/
// static int update_space_max(int idx, int test_value)
// {

// }

// static void update_max_len_by_field(t_file *file, int *space, int flag)
// {
//     if (has_flag(flag_nb, N_OPTION)) {
//          get_user_id_len;
//          get_group_id_len;
//     } 
//     else {
//          get_user_name_len;
//          get_group_name_len;
//     }
//      get_len_size;
//      get_len_nb_link;
//      get_len_date_month;
//      get_len_date_day;
//      get_len_date_hour;

//      get_minor_size;
//      get_major_size;
//      get_len_name_quote;
// }
/*
    Need to change all l option storage logic, if l_option build line here
        - create line structure 
        - fill it with all actual "write info" 
            - update int Array Space in concequences
        - just need to fill buffer in new write
    This change avoid double allocation for all string and special function call
    like getpwuid ...
*/
/* int * space */

/**
 * Call in: 
 *          default_file_struct -> check_args -> np
 *          check_args -> np
 *          check_for_fill_struct in main ls function one -> np
 *          create_new_file(recurcive routine) -> like cmd args create
 *          display_symlink -> np for this 
*/
/* maybe we can remove parent and path and fill it after call, 3 call in parse, one in recurcive and one in write name no realy a pain */
// t_file *fill_file_struct(struct stat *sb, char *path, char *parent, int symlink, t_context *c, t_file_context *file_c)
t_file *fill_file_struct(struct stat *sb, char *path, char *parent, int symlink, t_file_context *file_c)
{
    t_file *file;
 
    file = ft_calloc(sizeof(t_file), 1);
    if (!file)
        return (NULL);
    file->total_size = -1;
    file->type = symlink == TRUE ? SYMLINK : get_type(*sb);

    // file->sb = sb; /* STOP FREE SB */

    file->perm = sb->st_mode;
    file->size = sb->st_size;
    file->nb_link = sb->st_nlink;
    file->last_status_change = sb->st_ctim;
    file->last_access = sb->st_atim;
    file->last_change = sb->st_mtim;
    file->user_id = sb->st_uid;
    file->group_id = sb->st_gid;
    file->nb_block = sb->st_blocks;
    file->rdev = sb->st_rdev;
     if (file_c) {
        printf("%sGot file_c for %s manage space here%s\n",GREEN, path, RESET);
        // file->line = build_file_line(file, c, file_c);
        // build file->line with write l_option logi: so we need flag
            // in build line if (get_size_bytype > space[type]):
                // update space[type]
                // like manage_space but don't need to realloc all string 
                // or iter x time on lst, use build line before to store it to avoid iteration after store

    }


    if (fill_name_and_parent(file, path, parent) == MALLOC_ERR)
        return (NULL);
    file->quote = check_for_quote(file->name);
    return (file);
}
