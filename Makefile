CPPFLAGS = -Wall -Wextra -g -std=c++17

test: test.cpp material.hpp material.cpp ray.hpp shape.hpp shape.cpp vector.hpp vector.cpp scene.hpp color.hpp color.cpp
	g++ $(CPPFLAGS) -o test test.cpp material.cpp shape.cpp vector.cpp color.cpp

run: test
	./test

clean:
	rm -f test material.o ray.o shape.o vector.o scene.o color.o test