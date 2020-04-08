NAME			= archiver.exe

CC				= g++
HEADER_DIR		= includes
HEADER			= includes/archiver.h includes/Overlap.h

CFLAGS			= -I $(HEADER_DIR) -O2

SRC				= src/main.cpp \
				  src/pack/Compressor.cpp

OBJ = $(patsubst %.cpp,%.o,$(SRC))

%.o: %.cpp $(HEADER)
	g++ -c $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	
re: fclean all

.PHONY: clean fclean all re
