#include "../include/ft_ls.h"

/**
 * Basic display quote
*/
void display_quote(t_int8 quote)
{
    if (quote == ADD_SIMPLE_QUOTE_CHAR)
        fill_buffer_char('\'');
    else if (quote == ADD_DOUBLE_QUOTE_CHAR)
        fill_buffer_char('\"');
    else
        fill_buffer_char(' ');
}

/**
* parse special char
*/
int is_special_char(char c)
{
    /* all no care char */
    if (c == '@' || c == '%' || c == '-'\
        || c == '_' || c == '+' || c == '.' || c == ',' || c == ':')
        return (NOEFFECT_CHAR);
    /* special rule */
    if ( c == '{' || c == '}')
        return (BRACKET_CHAR);
    if (c == '#')
        return (DIEZE_CHAR);
    if (c == '\'')
        return (ADD_DOUBLE_QUOTE_CHAR);
    /* alway add quote. special rule for ' or "*/
    if (c == '!' || c == '$' || c == '^' || c == '&' || c == '*' || c == '('\
        || c == ')' || c == '=' || c == '<' || c == '>' || c == '?' || c == ';'\
        || c == '[' || c == ']' || c == '`' || c == '~' || c == '\"')
            return (ADD_SIMPLE_QUOTE_CHAR);
    return (NORMAL_CHAR);
}

int quotes_required(char *str)
{
    int i = 0;
    int ret = NORMAL_CHAR;
    int tmp = NORMAL_CHAR;

    while (str && str[i]) {
        tmp = is_special_char(str[i]);
        if (tmp == DIEZE_CHAR)
            tmp = i == 0 ? ADD_SIMPLE_QUOTE_CHAR : NORMAL_CHAR;
        else if (tmp == BRACKET_CHAR) 
            tmp = (ft_strlen(str) == 1 ? ADD_SIMPLE_QUOTE_CHAR : NORMAL_CHAR);
        if (tmp != NORMAL_CHAR) {
            if (ret < ADD_DOUBLE_QUOTE_CHAR)
                ret = tmp;
        }
        ++i;
    }
    return (ret);
}