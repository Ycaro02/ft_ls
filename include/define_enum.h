#ifndef LS_DEFINE_H
# define LS_DEFINE_H

# define    RED "\033[1;31m"
# define    GREEN "\033[1;32m"
# define    YELLOW "\033[1;33m"
# define    BLUE "\033[1;34m"
# define    PURPLE "\033[1;35m"
# define    CYAN "\033[1;36m"
# define    RESET "\033[0m"

# define     BUFFER_LEN 10000
# define     PRINT_SIZE 8000

# define     L_FLAG_CHAR 'l'
# define     R_FLAG_CHAR 'R'
# define     REVERSE_FLAG_CHAR 'r'
# define     A_FLAG_CHAR 'a'
# define     T_FLAG_CHAR 't'
# define     ACL_EXT_Z_FLAG 'z'

# define     BLOCK        'b'
# define     CHARACTER    'c'
# define     DIRECTORY    'd'
# define     FIFO         'p'
# define     SYMLINK      'l'
# define     REGULAR      '-'
# define     SOCKET       's'
# define     UNDIFINED    '?'
# define     EXEC         'x'

# define    NEW 0
# define    OLD 1

# define    NB_FLAG 7

enum e_flag  {
    UNKNOW=0,
    L_OPTION=1,
    A_OPTION=2,
    T_OPTION=4,
    REVERSE_OPTION=8,
    R_OPTION=16,
    Z_OPTION=32,
};

enum e_space  {
    S_USER,
    S_GROUP,
    S_SIZE,
    S_LINK,
    S_MONTH,
    S_DAY,
    S_HOUR,
};

enum e_color  {
    E_NONE,
    E_RED,
    E_GREEN,
    E_YELLOW,
    E_BLUE,
    E_PURPLE,
    E_CYAN,
};

enum e_error {
    ANY_ERR,
    NO_ACCESS_ERR,
    NA_CMD_LINE_ERR,
    MALLOC_ERR=-10,
};

#endif