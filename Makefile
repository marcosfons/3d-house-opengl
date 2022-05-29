CC=gcc
CFLAGS=-g -Wall -lm -lGL -lGLU -lglut
LDLIBS=

MKDIR_P=mkdir -p

EXE=main

SRC=src
BIN=bin
OBJ=build

SRCS=$(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))


.PHONY: help
help: ## Show a help message
help:
	@echo -e 'Usage: make [\033[36mtarget\033[0m]\ntargets:'
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
		sort | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "    \033[36m%-8s\033[0m %s\n", $$1, $$2}'

.PHONY: debug
debug: ## Build an executable to debug 
debug: $(BIN)/$(EXE)

.PHONY: release
release: ## Build a release executable
release: CFLAGS += -O2 -DNDEBUG
release: clean
release: $(BIN)/$(EXE)

.PHONY: clean
clean: ## Clean all builded files
	$(RM) -r $(BIN) $(OBJ)


$(OBJ)/%.o: $(SRC)/%.c $(OBJ)
	@$(MKDIR_P) $(@D)
	$(CC) $(CFLAGS) $(LDLIBS) -c $< -o $@

$(BIN)/$(EXE): $(OBJS) | $(BIN)
	$(CC) $(CFLAGS) $(LDLIBS) $^ -o $@


# Directories
$(BIN):
	$(MKDIR_P) $@

$(OBJ):
	$(MKDIR_P) $@

