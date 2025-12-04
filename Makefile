CPPFLAGS = -Wall -Wextra -g -std=c++17

test: test.cpp material.hpp material.cpp ray.hpp ray.cpp scene.hpp scene.cpp shape.hpp vector.hpp vector.cpp color.hpp color.cpp
	g++ $(CPPFLAGS) -o test test.cpp material.cpp ray.cpp scene.cpp vector.cpp color.cpp

run: test
	./test

leaks: test
	leaks --atExit -- ./test

val-leaks: test 
	valgrind --leak-check=yes ./test

clean:
	rm -f test
	rm -f image.ppm
	rm -f *.o
	rm -rf *.dSYM
