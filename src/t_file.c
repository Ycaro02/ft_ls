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

    // if (quote > NOEFFECT_CHAR) {
    //     return (quote);
    // return (NORMAL_CHAR);
    // char *tmp = NULL;
    //     if (quote == ADD_SIMPLE_QUOTE_CHAR) {
    //         tmp = ft_strjoin_free("'", str, 's');
    //         tmp = ft_strjoin_free(tmp, "'", 'f');
    //         str = tmp;
    //     }
    //     else {
    //         tmp = ft_strjoin_free("\"", str, 's');
    //         tmp = ft_strjoin_free(tmp, "\"", 'f');
    //         str = tmp;
    //     }
    // }





/*  Need to refact, call stat here, check file type, if link call lstat instead 
    Need to store space here to, each field will check for him
    Give space array here to check and store*/

t_file *fill_file_struct(struct stat *sb, char *path, char *parent, int symlink)
{
    t_file *file;

    file = ft_calloc(sizeof(t_file), 1);
    if (!file)
        return (NULL);
    file->total_size = -1;
    if (symlink == 0)
        file->type = get_type(*sb);
    else
        file->type = SYMLINK;
    
    file->perm = sb->st_mode & 0777;
    file->size = sb->st_size;
    file->nb_link = sb->st_nlink;
    file->last_status_change = sb->st_ctim;
    file->last_access = sb->st_atim;
    file->last_change = sb->st_mtim;
    file->user_id = sb->st_uid;
    file->group_id = sb->st_gid;
    file->nb_block = sb->st_blocks;
    file->rdev = sb->st_rdev;
    file->quote = check_for_quote(file->name);
    if (fill_name_and_parent(file, path, parent) == MALLOC_ERR)
        return (NULL);
    return (file);
}