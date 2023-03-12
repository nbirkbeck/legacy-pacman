
OBJS=level.o compat.o sound.o pacman.o
FLAGS=-g -DENABLE_NETWORK=1 -DENABLE_AI=1


all: pacman ${OBJS}
clean:
	rm -fv *.o pacman

%.o: %.cc
	g++ ${FLAGS} -c -o $@ $<

pacman: ${OBJS}
	g++ ${FLAGS} -o $@ ${OBJS} -lSDL2 -lpthread

pacman.html: Makefile pacman.cc level.cc compat.cc sound.cc
	emcc -O2 -o pacman.html -sASYNCIFY -sSDL2_IMAGE_FORMATS='["bmp"]' -s USE_SDL_IMAGE=2 -sSDL2_MIXER_FORMATS='["wav"]' -sUSE_SDL=2 --embed-file textures --embed-file sounds --embed-file levels  pacman.cc level.cc compat.cc sound.cc
