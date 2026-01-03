CC := gcc
# Allow the user to pass additional include/link flags (useful on MSYS2)
CPPFLAGS ?=
LDFLAGS ?=

CFLAGS := -std=c99 -O2 -Wall -Wextra -Iinclude $(CPPFLAGS)

# Check for ncurses availability
NCURSES_CFLAGS := $(shell pkg-config --cflags ncursesw 2>/dev/null)
NCURSES_LIBS := $(shell pkg-config --libs ncursesw 2>/dev/null)

# If pkg-config doesn't work, try fallback for MSYS2 UCRT64
ifeq ($(NCURSES_LIBS),)
    # Try MSYS2 UCRT64 path
    NCURSES_LIBS := $(shell [ -d /ucrt64/lib ] && echo "-L/ucrt64/lib -lncursesw" || echo "")
    NCURSES_CFLAGS := $(shell [ -d /ucrt64/include ] && echo "-I/ucrt64/include/ncursesw -DNCURSES_WIDECHAR" || echo "")
endif

ifeq ($(NCURSES_LIBS),)
    HAVE_NCURSES := 0
else
    HAVE_NCURSES := 1
    CFLAGS += $(NCURSES_CFLAGS)
    LDFLAGS += $(NCURSES_LIBS)
endif

# Check for SDL3 availability
SDL_CFLAGS := $(shell pkg-config --cflags sdl3 2>/dev/null)
SDL_LIBS := $(shell pkg-config --libs sdl3 2>/dev/null)

# If pkg-config doesn't work, try fallback for MSYS2 UCRT64
ifeq ($(SDL_LIBS),)
    SDL_LIBS := $(shell [ -d /ucrt64/lib ] && echo "-L/ucrt64/lib -lSDL3" || echo "")
    SDL_CFLAGS := $(shell [ -d /ucrt64/include ] && echo "-I/ucrt64/include/SDL3" || echo "")
endif

ifeq ($(SDL_LIBS),)
    HAVE_SDL3 := 0
else
    HAVE_SDL3 := 1
endif

# Base source files
SRC := src/controller.c src/model.c src/main.c src/highscores.c

# Add view sources based on availability
ifeq ($(HAVE_NCURSES),1)
    SRC += src/view_console.c src/view_menu_console.c
else
    SRC += src/view_console_stub.c
endif

ifeq ($(HAVE_SDL3),1)
	SRC += src/view_sdl.c src/view_menu_sdl.c src/text_bitmap.c
    CFLAGS += $(SDL_CFLAGS)
    LDFLAGS += $(SDL_LIBS)
else
    SRC += src/view_sdl_stub.c
endif

OBJ := $(SRC:.c=.o)

BIN_DIR := build
BIN := $(BIN_DIR)/space_invaders

all: $(BIN)

$(BIN): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: all
ifeq ($(HAVE_SDL3),1)
	$(BIN) --view=sdl
else ifeq ($(HAVE_NCURSES),1)
	TERMINFO=/usr/share/terminfo TERM=xterm-256color $(BIN) --view=console
else
	@echo "Aucune interface graphique disponible."
endif

run-console: all
ifeq ($(HAVE_NCURSES),1)
	TERMINFO=/usr/share/terminfo TERM=xterm-256color $(BIN) --view=console
else
	@echo "ncurses non disponible. Exécutez: make run-sdl"
endif

run-sdl: all
ifeq ($(HAVE_SDL3),1)
	$(BIN) --view=sdl
else
	@echo "SDL3 non disponible."
endif

clean:
	rm -rf $(BIN_DIR) src/*.o

valgrind-console: all
	@echo "=== Valgrind Memory Check (Console) ==="
	@echo "Note: Appuyez sur 'q' pour quitter et voir le rapport"
	@echo "Les fuites SDL/ncurses 'still reachable' sont normales"
	valgrind --leak-check=full --show-leak-kinds=definite,possible --suppressions=valgrind.supp --track-origins=yes $(BIN) --view=console

valgrind-sdl: all
	@echo "=== Valgrind Memory Check (SDL) ==="
	@echo "Note: Fermez la fenêtre pour voir le rapport"
	@echo "Les fuites SDL 'still reachable' sont normales"
	valgrind --leak-check=full --show-leak-kinds=definite,possible --suppressions=valgrind.supp --track-origins=yes $(BIN) --view=sdl

valgrind: valgrind-console valgrind-sdl

valgrind-full: all
	@echo "=== Valgrind Full Report (tous types de fuites) ==="
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=valgrind.supp --track-origins=yes --verbose $(BIN) --view=console

.PHONY: all run run-sdl clean valgrind valgrind-console valgrind-sdl valgrind-full

check-deps:
	@echo "=== Detected Dependencies ==="
	@echo "HAVE_NCURSES=$(HAVE_NCURSES)"
	@echo "NCURSES_CFLAGS=$(NCURSES_CFLAGS)"
	@echo "NCURSES_LIBS=$(NCURSES_LIBS)"
	@echo ""
	@echo "HAVE_SDL3=$(HAVE_SDL3)"
	@echo "SDL_CFLAGS=$(SDL_CFLAGS)"
	@echo "SDL_LIBS=$(SDL_LIBS)"
	@echo ""
	@echo "Final CFLAGS=$(CFLAGS)"
	@echo "Final LDFLAGS=$(LDFLAGS)"
	@echo ""
	@echo "Source files to compile: $(SRC)"

.PHONY: check-deps
