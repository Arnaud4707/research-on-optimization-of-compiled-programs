NAME = custom_grep

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g3 -O2

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

debug: $(OBJ)
	$(CXX) $(CXXFLAGS) -fno-omit-frame-pointer $(OBJ) -o $(NAME)

re: fclean all

.PHONY: all clean fclean re debug