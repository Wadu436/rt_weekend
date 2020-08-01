CPP=clang++
CPPFLAGS=-I . -Wall -O3 -fsanitize=address -fno-omit-frame-pointer -std=c++17 
#CPPFLAGS=-I . -I lodepng -Wall -g -Og
#CPPFLAGS=-I . -Wall -g -O3
DEPS = src/color.h src/vec3.h

DEPS = src/camera.h src/color.h src/hittable.h src/hittable_list.h src/material.h src/ray.h src/rtweekend.h src/sphere.h src/vec3.h
OBJ = src/main.o

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

main: $(OBJ)
	$(CPP) -o $@ $^ $(CPPFLAGS)

.PHONY: clean run

run: main
	rm -f img.ppm
	./main >> img.ppm

clean:
	rm -f main
	find . -type f -name '*.o' -delete