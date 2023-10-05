#ifndef FT_LS_DEFINE_ENUM_H
# define FT_LS_DEFINE_ENUM_H

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//                                                                            //
//                                DEFINE                                      //
//                                                                            //
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//-------------------------------
//      COLOR                  //
//-------------------------------
# define    RED "\033[1;31m"
# define    GREEN "\033[1;32m"
# define    YELLOW "\033[1;33m"
# define    BLUE "\033[1;34m"
# define    PURPLE "\033[1;35m"
# define    CYAN "\033[1;36m"
# define    RESET "\033[0m"
//-------------------------------
//      BUFFER                 //
//-------------------------------
# define     BUFFER_LEN 10000
# define     PRINT_SIZE 8000
//-------------------------------
//      FILE_TYPE              //
//-------------------------------
# define     BLOCK        'b'
# define     CHARACTER    'c'
# define     DIRECTORY    'd'
# define     FIFO         'p'
# define     SYMLINK      'l'
# define     REGULAR      '-'
# define     SOCKET       's'
# define     UNDIFINED    '?'
# define     EXEC         'x'
//-------------------------------
//      --HELP                 //
//-------------------------------
# define HELP_STR  "\
"YELLOW"Usage: ./ft_ls [OPTION]... [FILE]...\n\
List information about the FILEs (the current directory by default).\n\
Sort by alphabetical order by default\n\
Exit code:"RESET"\n\
"GREEN"0"RESET"  : Can access all file\n\
"PURPLE"1"RESET"  : Cannot access subdirectory\n\
"RED"2"RESET"  : Cannot access command-line argument\n\
"YELLOW"Option:"RESET"\n\
"CYAN"-l"RESET" : List detail option\n\
"CYAN"-a"RESET" : List hiden file\n\
"CYAN"-r"RESET" : Reverse sort\n\
"CYAN"-R"RESET" : Search recurcive subdir\n\
"CYAN"-t"RESET" : Sort by last file modification\n\
"CYAN"-u"RESET" : Sort by last access time\n\
"CYAN"-c"RESET" : Sort by last status change\n\
"CYAN"-f"RESET" : Don't sort, enable -a option\n\
"CYAN"-z"RESET" : List extended attr and acccess control list\n\
"CYAN"-g"RESET" : Like -l but don't list owner \n\
"CYAN"-d"RESET" : List only directory, no content\n\
"CYAN"-n"RESET" : Like -l but list id instead of name\n"

//-------------------------------
//     TIME                     // (after or before 6month ago)
//-------------------------------
# define    NEW 0
# define    OLD 1
//-------------------------------
//      FLAG                   //
//-------------------------------
# define     L_FLAG_CHAR 'l'
# define     A_FLAG_CHAR 'a'
# define     T_FLAG_CHAR 't'
# define     REVERSE_FLAG_CHAR 'r'
# define     R_FLAG_CHAR 'R'
# define     ACL_EXT_Z_FLAG 'z'
# define     U_FLAG_CHAR 'u'
# define     C_FLAG_CHAR 'c'
# define     G_FLAG_CHAR 'g'
# define     G_FLAG_CHAR 'g'
# define     F_FLAG_CHAR 'f'
# define     D_FLAG_CHAR 'd'
# define     N_FLAG_CHAR 'n'
# define     NB_FLAG 14
# define     ALL_FLAG "latrRzucgfdnG"
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//                                                                            //
//                                  ENUM                                      //
//                                                                            //
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//-------------------------------
//      FLAG                   //
//-------------------------------
enum e_flag  {
    UNKNOW=0,
    L_OPTION=1,
    A_OPTION=2,
    T_OPTION=4,
    REVERSE_OPTION=8,
    R_OPTION=16,
    Z_OPTION=32,
    U_OPTION=64,
    C_OPTION=128,
    G_OPTION=256,
    F_OPTION=512,
    D_OPTION=1024,
    N_OPTION=2048,
    COLOR_OPTION=4096,
};
//-------------------------------
//      SPACE                   // For -l option
//-------------------------------
enum e_space  {
    S_PERM,
    S_USER,
    S_GROUP,
    S_SIZE,
    S_LINK,
    S_MONTH,
    S_DAY,
    S_HOUR,
};
//-------------------------------
//      COLOR                   //
//-------------------------------
enum e_color  {
    E_NONE,
    E_RED,
    E_GREEN,
    E_YELLOW,
    E_BLUE,
    E_PURPLE,
    E_CYAN,
};
//-------------------------------
//      ERROR                   //
//-------------------------------
enum e_error {
    ANY_ERR,
    NO_ACCESS_ERR,
    NA_CMD_LINE_ERR,
    MALLOC_ERR=-10,
};

#endif /* FT_LS_DEFINE_ENUM_H */