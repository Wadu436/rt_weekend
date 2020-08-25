CPP=clang++
#CPPFLAGS=-I . -O0 -Wall -pthread -std=c++17 -g -fsanitize=address -fno-omit-frame-pointer
CPPFLAGS=-I . -O3 -Wall -pthread -std=c++17
DEPS = src/camera.h src/color.h src/hittable.h src/hittable_list.h src/material.h src/moving_sphere.h src/ray.h src/rtweekend.h src/sphere.h src/texture.h src/vec3.h
OBJ = src/main.o src/material.o src/moving_sphere.o src/sphere.o src/color.o src/aabb.o src/hittable_list.o src/bvh.o

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

main: $(OBJ)
	$(CPP) -o $@ $^ -O3 $(CPPFLAGS)

.PHONY: clean run

run: main
	rm -f img.ppm
	./main >> img.ppm

clean:
	rm -f main
	find . -type f -name '*.o' -delete

benchmark: main
	rm -f img_scene1.png && ./main -w 800 -h 450 -s 250 -t 1 --scene 1 | pnmtopng >> img_scene1.png
