##
## EPITECH PROJECT, 2025
## rush
## File description:
## Main Makefile
##

#Executable names
NAME	=	my_ftp

TESTS_NAME	=	unit_tests.out

# Folders name
SRCDIR	=	./src/
SRCCOM	=	./src/commands/
GL	=	./sources/globals/
MISC	=	./sources/miscellaneous/
TESTS	=	./tests/

#Sources
MAIN	=	src/main.c
SRC	=	\
	$(SRCDIR)set_server.c	\
	$(SRCDIR)get_dir.c	\
	$(SRCDIR)loop.c	\
	$(SRCDIR)handle_clients.c	\
	$(SRCDIR)check_command.c	\
	$(SRCDIR)is_auth.c	\
	$(SRCCOM)user_command.c	\
	$(SRCCOM)pass_command.c	\
	$(SRCCOM)quit_command.c	\
	$(SRCCOM)pwd_command.c	\
	$(SRCCOM)noop_command.c	\
	$(SRCCOM)help_command.c	\
	$(SRCCOM)cdup_command.c	\
	$(SRCCOM)cwd_command.c	\
	$(SRCCOM)pasv_command.c	\
	$(SRCCOM)list_command.c	\

TESTS_SRC	=	$(TESTS)tests.c	\
				$(SRC)


#Headers folder
INCLUDES	=	-I./includes/	\


#GCC Flags
ERROR	=	-Wall -Wextra -Wshadow #-Werror
#Compilation Flags
CFLAGS	+=	$(ERROR) -I$(INCLUDES) -g
#Pre Compilation
CC	:=	gcc

OBJ	=	$(SRC:.c=.o)
DEP	=	$(SRC:.c=.d)	\
		$(MAIN:.c=.d)

#Libraries
LIBS	=	./libs/
LLIBS	=	-L$(LIBS)
LIB_	=	#$(LIBS)put a path to a library here



#Compilation Flags
FLAGS	=	$(CFLAGS) $(LLIBS)
#Test Compilation Flags
UNIT_FLAGS	=	$(FLAGS) -lcriterion --coverage



all:	$(NAME)

$(NAME): $(OBJ) $(MAIN:.cpp=.o) $(LIB_)
	$(CC) -o $(NAME) $(OBJ) $(MAIN:.cpp=.o) $(FLAGS)


-include $(DEP)
%.o: %.cpp
	$(COMPILE.cpp) $< -o $@ -MMD -MF $*.d -MT $@ $(CFLAGS)


clean:
	rm -f $(OBJ)
	rm -f $(MAIN:.c=.o)
	rm -f $(DEP)
	rm -f *.gcno
	rm -f *.gcda
	rm -f vgcore.*

libclean: clean

fclean:	libclean
	rm -f $(NAME)
	rm -f $(TESTS_NAME)
	find -name "lib*.a" -delete

re:	fclean all

tests_run:	unit_tests
	./$(TESTS_NAME)

unit_tests:
	$(CC) -o $(TESTS_NAME) $(TESTS_SRC) $(UNIT_FLAGS)

gcovr:	tests_run
	gcovr --exclude tests/
	gcovr --exclude tests/ --txt-metric branch

.PHONY:	all clean	libclean	\
		fclean	re	remake		\
		tests_run	unit_tests	gcovr
