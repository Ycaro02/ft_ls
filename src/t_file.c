#include "../include/ft_ls.h"

/* for last x perm (other) S_IXOTH, STICKY
        if just exec:   x
        if just sticky: T
        if sticky + x:  t
        
        for second x perm (group) S_IXGRP, GID
        if just exec:   x
        if just GID: S
        if GID + x:  s

        for first x perm (group) S_IXGRP, SUID
        if just exec:   x
        if just GID: S
        if GID + x:  s
    */

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


char *perm_to_string(mode_t mode, char type)
{
    char *perm = ft_calloc(sizeof(char), 11);
    if (!perm)
        return (NULL);
    char exe_tmp = '-';

    for (int i = 0; i < 11; ++i)
        perm[i] = '-';
    perm[10] = '\0';
    perm[0] = type;

    
    if (mode & S_IRUSR)         /* first R */
        perm[1] = 'r';
    if (mode & S_IWUSR)         /* first W */
        perm[2] = 'w';

    if (mode & S_IXUSR)         /* first X */
        exe_tmp = 'x';
    
    if (mode & S_ISUID) {       /* UID */
        if (exe_tmp == 'x')
            exe_tmp = 's';
        else
            exe_tmp = 'S';
    }

    perm[3] = exe_tmp;
    exe_tmp = '-';

    if (mode & S_IRGRP)         /* second R */
        perm[4] = 'r';
    
    if (mode & S_IWGRP)         /* second W */
        perm[5] = 'w';


    if (mode & S_IXGRP)         /* second x */
        exe_tmp = 'x';
    
    if (mode & S_ISGID) {       /* GID */
        if (exe_tmp == 'x')
            exe_tmp = 's';
        else
            exe_tmp = 'S';
    }
    perm[6] = exe_tmp;
    exe_tmp = '-';
    
    if (mode & S_IROTH)         /* last r 4 */
        perm[7] = 'r';
    if (mode & S_IWOTH)         /* last w 2 */
        perm[8] = 'w';

    if (mode & S_IXOTH)         /* last x 1 */
        exe_tmp = 'x';

    if (mode & __S_ISVTX)  {    /* STICKY */
        if (exe_tmp == 'x')
            exe_tmp = 't';
        else
            exe_tmp = 'T';
    }

    perm[9] = exe_tmp;
    return (perm);
}

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
    
    // file->perm = sb->st_mode & 0777;
    file->perm = sb->st_mode;

    // if (sb->st_mode & S_IWUSR)
    //     printf("le propriétaire a le droit d'écriture: sb->st_mode & S_IWUSR [%d] \n", sb->st_mode & S_IWUSR);

    // char* perm = perm_to_string(sb->st_mode, file->type);
    // printf("Perm: %s for -> %s\n", perm, path);

    file->size = sb->st_size;
    file->nb_link = sb->st_nlink;
    file->last_status_change = sb->st_ctim;
    file->last_access = sb->st_atim;
    file->last_change = sb->st_mtim;
    file->user_id = sb->st_uid;
    file->group_id = sb->st_gid;
    file->nb_block = sb->st_blocks;
    file->rdev = sb->st_rdev;
    if (fill_name_and_parent(file, path, parent) == MALLOC_ERR)
        return (NULL);
    file->quote = check_for_quote(file->name);
    return (file);
}

// #if defined __USE_MISC || defined __USE_XOPEN
    // S_ISVTX /* sticky bits*/
// #endif

    // if (mode & S_IRWXU) /* full first */
    // if (mode & S_IRWGRP)/* full second */
    // if (mode & S_IRWXO) /* fulll last 7 */