CURR_DIR := .
BUILD_DIR := $(CURR_DIR)/build
LOGIC_DIR := $(CURR_DIR)/core

RM := rm -f
MK := mkdir -p

EXEC := vimer.app
ARCHIVE := $(CURR_DIR)/$(EXEC)

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
	open ./manual/index.html

create_dirs:
	# @$(RM) -r $(ARCHIVE)
	# @$(MK) $(ARCHIVE)
	# @$(MK) $(ARCHIVE)/src
	# @$(MK) $(ARCHIVE)/src/gui
	# @$(MK) $(ARCHIVE)/src/gui/pic
	# @$(MK) $(ARCHIVE)/src/calclogic
	# @$(MK) $(ARCHIVE)/src/calclogic/tests

cpy_files:
	# cp CMakeLists.txt $(ARCHIVE)/src
	# cp documentation.texi $(ARCHIVE)/src
	# cp Makefile $(ARCHIVE)/src
	# cp $(CURR_DIR)/calclogic/*c $(ARCHIVE)/src/calclogic
	# cp $(CURR_DIR)/calclogic/*h $(ARCHIVE)/src/calclogic
	# cp $(CURR_DIR)/calclogic/Makefile $(ARCHIVE)/src/calclogic
	# cp $(CURR_DIR)/calclogic/tests/* $(ARCHIVE)/src/calclogic/tests
	# cp $(CURR_DIR)/gui/*.cpp $(ARCHIVE)/src/gui
	# cp $(CURR_DIR)/gui/*.h $(ARCHIVE)/src/gui
	# cp $(CURR_DIR)/gui/pic/* $(ARCHIVE)/src/gui/pic

tests:
	@cd $(LOGIC_DIR) ; make test ; cd ./..

gcov_report:
	@cd $(LOGIC_DIR) ; make gcov_report ; cd ./..

clean: uninstall
	@cd $(LOGIC_DIR) ; make clean ; cd ./..
	@$(RM) -r manual/
