#
#		Filename : Makefile
#		Description:  to test on Windows and Linux.
#		Created:  23-06-19 | Revision: 30-06-19.
#		Compiler:  GNU Compiler - GCC.
#		Author:  Samuel Jacquet
#

# -------------------------------------------------------------------
#  Compiler choice
# -------------------------------------------------------------------

CC = gcc

# -------------------------------------------------------------------
#  project structure
# -------------------------------------------------------------------

SRC_PATH = src
BUILD_PATH = build
ifeq ($(OS),Windows_NT)
	INC_PATH = include
	LIB_PATH = lib\win32
	FMOD_PATH = lib\win32\fmod_vc.lib
	EXE = release\win32\prog.exe
else
	INC_PATH = include
	LIB_PATH = lib/linux
	# FMOD_PATH = lib/linux/libfmod.so
	EXE = release/linux/prog
endif

# -------------------------------------------------------------------
#  Linking
# -------------------------------------------------------------------

# Compilation flags
CFLAGS = -I $(INC_PATH) -std=c99 -Wall# -Wextra -pedantic
LDFLAGS = -L $(LIB_PATH)
#lib\fmod_vc.lib

# os conditions :
# WINDOWS = yes
# ifeq($(WINDOWS),yes)
ifeq ($(OS),Windows_NT)
	CFLAGS += -lfreeglut -lopengl32 -lglu32 -lDevIL -lILU -lILUT -lfmod_vc
else
	CFLAGS += -lglut -lGL -lGLU -lIL -lILU -lILUT -lSOIL -lm #-lfmod
endif

# GDB debugger options
DEBUG_FLAG = -g

# -------------------------------------------------------------------
#  Listing
# -------------------------------------------------------------------

# listing all source files (.c) in the directory :
SRC = $(wildcard src/*.c)
# listing all object files (.o) from source files :
OBJ = $(subst src,build,$(SRC:.c=.o))
#OBJ = $(subst $(SRC_DIR),$(BUILD_DIR),$(src:.c=.o))

# -------------------------------------------------------------------
#  Compilation
# -------------------------------------------------------------------

.PHONY: default all clean

default: $(EXE)
all: default # command that will link recursively.

$(BUILD_PATH)/%.o : $(SRC_PATH)/%.c # @echo Creating object.
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS) ${DEBUG_FLAG}

.PRECIOUS: $(EXE) $(OBJ)

$(EXE) : $(OBJ) # @echo Creating exe binary.
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
# $@ = $(EXE) ; $^ = $(OBJ)

# Special variables :
# $@ = target name.
# $< = the name of the first dependency (main.o is created with main.c and not with player.c).
# $^ = dependencies list.
# $() = reference a make variable.

# -------------------------------------------------------------------
#  Additional rules
# -------------------------------------------------------------------

# A valgrind target to check for memory leaks.
valgrind:
	valgrind --leak-check=yes $(EXEC) >& valgrind.out
	@echo valgrind has been created.

vars:
	@echo CC = $(CC)
	@echo SRC = $(SRC)
	@echo OBJ = $(OBJ)
	@echo EXE = $(EXE)
	@echo CFLAGS = $(CFLAGS)
	@echo DEBUG_FLAG = $(DEBUG_FLAG)
	@echo INC_PATH = $(INC_PATH)
	@echo LIB_PATH = $(LIB_PATH)
	@echo FMOD_PATH = $(FMOD_PATH)
	@echo EXE = $(EXE)

.PHONY : clean $(FMOD_PATH)
# Clean up all build targets and executable file (rm command for Linux).

clean:
ifeq ($(OS),Windows_NT)
	-del ${EXE} release\win32\*.exe $(BUILD_PATH)\*.o
else
	-rm -rf ${EXE} _linux_release/*.exe $(BUILD_PATH)/*.o $(BUILD_PATH)/*.dSYM
endif

zip:
ifeq ($(OS),Windows_NT)
	# Download 7-Zip from the https://www.7-zip.org/
	# To add the path to 7z.exe to your PATH environment variable.
	7z a -tzip Jacquets_JumpingBanana.zip makefile src\*.c includelude\*.h
else
	# sudo apt-get install zip
	zip -r Jacquets_JumpingBanana.zip *.c *.h Makefile $(SRC_PATH)/*/* $(INC_PATH)/*/*
endif

link:
	ldd prog

# Trying to generate a link with fmod.
test:
	sudo chown root /home/sam/Bureau/1908/190819/lib/linux/libfmod.so
	sudo chgrp root /home/sam/Bureau/1908/190819/lib/linux/libfmod.so
	sudo mkdir /usr/include/fmod
	sudo cp /home/sam/Bureau/1908/190819/include/FMOD/fmod.h /home/sam/Bureau/1908/190819/include/FMOD/fmod_errors.h /usr/include/fmod
# -------------------------------------------------------------------
#  Other compilation command.
# -------------------------------------------------------------------

# glfw: gcc src/LibrariesTests/_glfw.c -o src/LibrariesTests/prog -I include/GLFW -I include/GLFW/glad -L lib -lglfw
# sdl: gcc src/LibrariesTests/sdl.c -o src/LibrariesTests/prog -I include/SDL -L lib -lmingw32 -lSDL2main -lSDL2
fmod:
ifeq ($(OS),Windows_NT)
	@echo FMOD compilation command (Windows).
	gcc src\audio.c -o release\win32\song -D_FMOD $(CFLAGS) $(FMOD_PATH) $(LDFLAGS) ${DEBUG_FLAG}
else
	gcc src/audio.c -o release/linux/song -D_FMOD $(CFLAGS) $(LDFLAGS) -ln -s libfmod.so -Wall -std=c99
endif

render:
ifeq ($(OS),Windows_NT)
	@echo DevIL compilation command (Windows).
	gcc src/render.c -o release\win32\prog -D_RENDER $(CFLAGS) $(LDFLAGS) ${DEBUG_FLAG}
else
	@echo SOIL compilation command (Linux).
	gcc src/render.c -o render -D_RENDER -I./include/ -L./lib/linux/ -lglut -lGL -lGLU -lSOIL -lm -Wall -std=c99
endif

draw:
ifeq ($(OS),Windows_NT)
	@echo Draw compilation command (Windows).
	gcc src/draw.c src/write.c src/render.c -o release\win32\prog -D_DRAW $(CFLAGS) $(LDFLAGS) ${DEBUG_FLAG}
else

endif

random:
ifeq ($(OS),Windows_NT)
	@echo Random compilation command (Windows).
	gcc src/draw.c src/write.c src/render.c -o release\win32\prog -D_RANDOM $(CFLAGS) $(LDFLAGS) ${DEBUG_FLAG}
else

endif

score:
	@echo STRUCT TM on C compilation.
	gcc src/score.c src/timeTM.c -o release\win32\prog -D_POINTEUR_CHASSEUR -I $(INC_PATH) -std=c99 -Wall

time:
ifeq ($(OS),Windows_NT)
	@echo STRUCT TM on C compilation.
	gcc src/timeTM.c -o release\win32\prog -D_STRUCT_TM -I $(INC_PATH) -std=c99 -Wall
else

endif

map:
ifeq ($(OS),Windows_NT)
	@echo display a simple matrice.
	gcc src/map.c src/render.c src/draw.c src/write.c -o release\win32\prog -D_MAP $(CFLAGS) ${DEBUG_FLAG} $(LDFLAGS)
else

endif

map2:
ifeq ($(OS),Windows_NT)
	@echo display a simple matrice.
	gcc src/map.c src/render.c -o release\win32\prog -D_MAP_bis $(CFLAGS) ${DEBUG_FLAG} $(LDFLAGS)
else

endif

game:
ifeq ($(OS),Windows_NT)
	@echo Saving test.
	gcc -D_SAVEDATA src/game.c src/draw.c src/render.c src/map.c src/write.c src/timeTM.c src/score.c src/audio.c -o release\win32\prog -D_SAVEGAME $(CFLAGS) ${DEBUG_FLAG} $(LDFLAGS)
else

endif

event:
ifeq ($(OS),Windows_NT)
	@echo Collision test.
	gcc src/event.c src/game.c src/draw.c src/render.c src/map.c src/write.c src/audio.c src/timeTM.c src/score.c -o release\win32\prog -D_EVENT $(CFLAGS) ${DEBUG_FLAG} $(LDFLAGS)
else
	@echo Collision test.
	gcc src/event.c src/game.c src/draw.c src/render.c src/map.c src/write.c src/audio.c src/timeTM.c src/score.c -o release/linux/prog -D_EVENT $(CFLAGS) ${DEBUG_FLAG} $(LDFLAGS)
endif


# -------------------------------------------------------------------
#  Linux environement installation for other program.
# -------------------------------------------------------------------

in:
	#sudo cp lib/linux/libfmod.so /usr/local/lib/
	#sudo cp includelude/FMOD2/*.h /usr/local/includelude/
	#sudo cp /usr/lib/x86_64-linux-gnu/*.so lib/linux/
	#sudo cp /usr/includelude/IL/*.h includelude/IL/IL/
	#sudo cp /usr/includelude/GL/*.h includelude/GL/GL/
	#sudo cp /usr/local/lib/libSOIL.a  lib/linux/
sudo:
	sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev

help:
	@echo Programme Jumping Banana
	@echo -------------------------------------------------------------------------------
	@echo Available options :
	@echo make			: compiling source codes and creating the final executable.
	@echo prog.exe		: running on windows.
	@echo prog			: running on Linux.
	@echo make valgrind		: running a programming tool for memory debugging.
	@echo make vars    		: to display variables.
	@echo make biblio		: to display bibliographies.
	@echo make clean   		: Clean up all build targets and executable file.
	@echo make zip			: to zip files.
	@echo -------------------------------------------------------------------------------

biblio:
	@echo Make Bibliography 06-07-19 :
	@echo https://www.gnu.org/software/make/manual/html_node/Echoing.html
	@echo https://www.gnu.org/software/make/manual/html_node/Text-Functions.html
	@echo https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
	@echo Pre-defined Compiler Macros :
	@echo https://sourceforge.net/p/predef/wiki/OperatingSystems/
	@echo https://openclassrooms.com/forum/sujet/le-quot-cleanquot-de-mon-makefile-ne-fonctionne-pas-comme-43154
	@echo https://stackoverflow.com/questions/22545029/makefile-how-to-correctly-includelude-header-file-and-its-directory
	@echo https://www.youtube.com/watch?v=2cZiMBfp-C0&t
	@echo https://www.youtube.com/watch?v=-riHEHGP2DU&t
	@echo https://www.youtube.com/watch?v=Lwx9rSgwoDg&t
	@echo https://stackoverflow.com/questions/18180060/how-to-zip-a-file-using-cmd-line/18180154
