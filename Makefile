NAME			= archiver.exe

CC				= clang++
HEADER_DIR		= includes
HEADER			= includes/archiver.h \
				  includes/Overlap.h \
				  includes/Compressor.h \
				  includes/ParallelCompressor.h \
				  includes/SynchronizedIO.h

CFLAGS			= -I $(HEADER_DIR) -O2

SRC				= 	src/main.cpp \
					src/pack/ParallelCompressor.cpp \
				  	src/pack/Compressor.cpp \
					src/pack/SynchronizedIO.cpp \
				  	src/util/helps.cpp

OBJ = $(patsubst %.cpp,%.o,$(SRC))

%.o: %.cpp $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	
re: fclean all

.PHONY: clean fclean all re
