NAME = libft.a
SRC =	ft_memset.c \
		ft_atoi.c \
		ft_bzero.c \
		ft_calloc.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_split.c \
		ft_strchr.c \
		ft_strjoin.c \
		ft_strlcat.c \
		ft_strlcpy.c \
		ft_strlen.c \
		ft_strncmp.c \
		ft_strnstr.c \
		ft_strrchr.c \
		ft_strtrim.c \
		ft_substr.c \
		ft_tolower.c \
		ft_toupper.c \
		ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_strdup.c \
		ft_itoa.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_strmapi.c \
		ft_striteri.c \
		ft_ltoa.c \
		ft_free_tab.c \
		ft_strjoin_free.c \
		ft_lower_strcmp.c \
		ft_strcmp.c \
		printf_fd/ft_intfunction.c \
		printf_fd/ft_printf.c \
		printf_fd/ft_put_and_count.c \

BONUS = ft_lstnew.c \
		ft_lstadd_front.c \
		ft_lstsize.c \
		ft_lstlast.c \
		ft_lstadd_back.c \
		ft_lstdelone.c \
		ft_lstclear.c \
		ft_lstiter.c \
		ft_lstmap.c \

BOBJ = $(BONUS:.c=.o)

OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

all : $(NAME)

bonus : $(BOBJ) $(OBJ)
	ar rcs $(NAME) $^

so:
	$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRC)
	gcc -nostartfiles -shared -o libft.so $(OBJ) $(BOBJ)

%.o : %.c
	$(CC) ${CFLAGS} -c $< -o $@

$(NAME): $(OBJ)
	ar rcs -o $(NAME) $(OBJ)

clean :
	rm -f $(OBJ) $(BOBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re bonus

