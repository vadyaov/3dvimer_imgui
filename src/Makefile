CURR_DIR := .
BUILD_DIR := $(CURR_DIR)/build
LOGIC_DIR := $(CURR_DIR)/core

RM := rm -f
MK := mkdir -p

EXEC := vimer.app
ARCHIVE := $(CURR_DIR)/vimer

all: tests install run

install:
	@$(MK) $(BUILD_DIR)
	cmake $(CURR_DIR) -S $(CURR_DIR) -B $(BUILD_DIR)
	@cd $(BUILD_DIR) ; make ; cd ./..

uninstall:
	@$(RM) -r $(BUILD_DIR)

run:
	$(BUILD_DIR)/$(EXEC)

dist: create_dirs cpy_files
	tar cvzf $(ARCHIVE).tgz $(ARCHIVE)
	mv $(ARCHIVE).tgz $(CURR_DIR)/..
	$(RM) -r $(ARCHIVE)

dvi:
	makeinfo documentation.texi --html
	open ./doc/index.html

create_dirs:
	@$(RM) -r $(ARCHIVE)
	@$(MK) $(ARCHIVE)
	@$(MK) $(ARCHIVE)/src
	@$(MK) $(ARCHIVE)/src/core
	@$(MK) $(ARCHIVE)/src/imgui
	@$(MK) $(ARCHIVE)/src/shaders
	@$(MK) $(ARCHIVE)/src/models
	@$(MK) $(ARCHIVE)/src/core/tests

cpy_files:
	cp CMakeLists.txt $(ARCHIVE)/src
	cp documentation.texi $(ARCHIVE)/src
	cp Makefile $(ARCHIVE)/src
	cp main.cpp $(ARCHIVE)/src
	cp main.hpp $(ARCHIVE)/src
	cp savefile.vimer $(ARCHIVE)/src
	cp $(CURR_DIR)/core/*c $(ARCHIVE)/src/core
	cp $(CURR_DIR)/core/*h $(ARCHIVE)/src/core
	cp $(CURR_DIR)/core/Makefile $(ARCHIVE)/src/core
	cp $(CURR_DIR)/core/tests/* $(ARCHIVE)/src/core/tests
	cp $(CURR_DIR)/imgui/* $(ARCHIVE)/src/imgui
	cp $(CURR_DIR)/models/* $(ARCHIVE)/src/models
	cp $(CURR_DIR)/shaders/* $(ARCHIVE)/src/shaders

tests:
	@cd $(LOGIC_DIR) ; make test ; cd ./..

gcov_report:
	@cd $(LOGIC_DIR) ; make gcov_report ; cd ./..

clean: uninstall
	@cd $(LOGIC_DIR) ; make clean ; cd ./..
	@$(RM) -r doc/
	@$(RM) imgui.ini
	@$(RM) ScreenShot.jpeg
	@$(RM) ScreenShot.bmp
	@$(RM) -r imgui/CMakeFiles/
	@$(RM) imgui/cmake_install.cmake
	@$(RM) imgui/Makefile
	@$(RM) imgui/libimgui.a
