CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

PTHEAD_FLAG = -pthread

NAME = philo_bonus

SRCS_DIR = srcs/
SRCS_FILE =	philo_bonus.c \
			utils_bonus.c	\
			more_bonus.c	\
			philo_bonus_ft_atoi.c	\

SRCS = $(addprefix $(SRCS_DIR), $(SRCS_FILE))
OBJS = $(SRCS:%.c=%.o)

$(NAME) :	$(OBJS)
			$(CC) $(CFLAGS) $(PTHEAD_FLAG) -o $(NAME) $(OBJS)

all		:	$(NAME)

clean	:	
			$(RM) $(OBJS)

fclean	:	clean
			$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re

