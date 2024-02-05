#ifndef FT_LS_STRUCT_DEF_H
# define FT_LS_STRUCT_DEF_H

# include "basic_define.h"
# include "define_enum.h"
# include <sys/stat.h>
# include <sys/types.h>

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//                                                                            //
//                                  STRUCT                                    //
//                                                                            //
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

/**
 * special_error: Store if special error found in cmd line to adapt display
 *  - for only '-' manage case: 
 *      - if no valid args -> exit code -2, don't active default search in current dir (".")
 *      - else display error and another args normaly
*/
typedef struct s_context { /* Execution context */
    t_int8      error;              /* Exit error code */
    t_int8      special_error;      /* special error to manage -- and special display */
    int         flag_nb;            /* flags value (ls option) */
} t_context;

/** 
 * call_flag: Call flag used in hub ls function
 *  - 0 for file display
 *  - 1 for dir without file before
 *  - 2 for dir with file or another dir before
*/
typedef struct s_file_context { /* File context for each ls call*/
    int         call_flag;
    int         idx;            /* idx: Index of file in lst */
    int         lst_len;        /* lst_len: File's list len */
    int         *space;         /* int array of max column/space for l option else NULL */
    char        *path;          /* ptr to path string */
    char        *parent_path;   /* ptr to parent path string*/
} t_file_context;

// /* File line structure for l option */
// typedef struct s_file_line { /* File info store/build in t_file */
//     char **buff; /* alloc of S_HOUR + 1, string idx matching with e_space enum*/
//     char quote; /* quote if needed, '\0' for default value */
// } t_file_line;

/**
 * File structure, contain lot of struct stat field
*/
typedef struct s_file 
{
    t_int8      type;
    mode_t      perm;
    nlink_t     nb_link;
    uid_t       user_id;
    gid_t       group_id;
    t_int64     size;
    t_int64     total_size;
    blkcnt_t    nb_block;
    dev_t       rdev;
    t_timespec  last_status_change;
    t_timespec  last_access;
    t_timespec  last_change;
    // struct stat *sb;
    char        *name;
    char        *parrent;
    t_int8      quote;
    char        **line; /* alloc of S_MAJOR_SIZE + 1, string idx matching with e_space enum, only for L_OPTION */
} t_file;

/**
 * Buffer on stack to avoid multiple useless call of write
*/
typedef struct s_buff {
	char		buffer[BUFFER_LEN];
    int	        i;  /* buffer index */
}	t_buff;



typedef struct s_args {
    t_list          *simple_file;   /* lst of no directory file */
    t_list          *dir_lst;       /* lst of dir file */
    t_context       c;              /* ls application context */
    t_file_context file_c;         /* file lst context for simple_fie */
} t_args;

/* t_file_line buff content: */
/* Permision string */
/* User owner string or id */
/* Group owner string or id */
/* File size string, or minor + major for BLOCK and CHARACTER file  */
/* nb link string */
/* Month string */
/* Day string */
/* Hour string */

#endif /* FT_LS_STRUCT_DEF_H */