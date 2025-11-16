FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

COMMON_INCLUDES		= -I./COMMON/headers
CONFIG_INCLUDES 	= -I./CONFIG
STK_INCLUDES		= -I./STACK/headers
TREE_INCLUDES		= -I./TREE/headers
AKINATOR_INCLUDES   = -I./AKINATOR/headers
SFML_INCLUDES 		= -I./SFML/headers -I./SFML/src -I/opt/homebrew/include -Wno-deprecated-declarations -L/opt/homebrew/lib -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio

COMMON_FILES   = COMMON/IsBadPtr.cpp COMMON/LineCounter.cpp COMMON/logger.cpp COMMON/SizeFile.cpp COMMON/TXTreader.cpp
TREE_FILES 	   = TREE/TreeFunc.cpp
AKINATOR_FILES = AKINATOR/AkinFunc.cpp AKINATOR/DataReader.cpp AKINATOR/GenGraphs.cpp
SFML_FILES	   = SFML/start_game.cpp SFML/find_mode.cpp SFML/TextField.cpp SFML/final_find.cpp SFML/define_mode.cpp SFML/cmp_mode.cpp


all: help

akinator: main_akinator.cpp $(SFML_FILES) $(COMMON_FILES) $(TREE_FILES) $(AKINATOR_FILES)
	@echo "-----------------------------------------------------------------------------------------"
	g++ -o akinator_program $(FLAGS) main_akinator.cpp $(SFML_INCLUDES) $(COMMON_INCLUDES) $(CONFIG_INCLUDES) $(STK_INCLUDES) $(TREE_INCLUDES) $(AKINATOR_INCLUDES) $(SFML_FILES) $(COMMON_FILES) $(TREE_FILES) $(AKINATOR_FILES)
	@echo "-----------------------------------------------------------------------------------------"

run-akinator: akinator
	./akinator_program

run: run-akinator

clean:
	rm -f akinator_program

help:
	@echo "Available commands:"
	@echo ""
	@echo "  make akinator                 - compile a akinator"
	@echo "  make run-akinator             - compile and run akinator"
	@echo "  make run                      - compile and run akinator"
	@echo ""
	@echo "  make clean                    - remove compiled programs"

.PHONY: akinator run-akinator run clean help