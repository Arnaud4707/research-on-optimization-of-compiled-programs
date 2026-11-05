NAME = custom_grep

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g3 -O2 -fno-omit-frame-pointer

SRC = grep.cpp

OBJ = $(SRC:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re debug