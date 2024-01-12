CFLAGS = -Wall -Werror -pedantic -g

TARGET = $(BIN_DIR)/MyShell

BIN_DIR = bin
OBJ_DIR = obj
INC_DIR = inc
LIB_DIR = lib
SRC_DIR = src

$(TARGET) : $(OBJ_DIR)/MyShell.o $(OBJ_DIR)/Executors.o $(LIB_DIR)/libjobcontrol.a $(LIB_DIR)/libutilities.a
	mkdir -p $(BIN_DIR)
	gcc $(CFLAGS) $(OBJ_DIR)/MyShell.o $(OBJ_DIR)/Executors.o -L./$(LIB_DIR) -ljobcontrol -lutilities -o $(TARGET)

$(OBJ_DIR)/MyShell.o : $(SRC_DIR)/MyShell.c $(INC_DIR)/MyShell.h
	mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $(SRC_DIR)/MyShell.c -o $(OBJ_DIR)/MyShell.o

$(OBJ_DIR)/Executors.o : $(SRC_DIR)/Executors.c $(INC_DIR)/Executors.h
	gcc $(CFLAGS) -c $(SRC_DIR)/Executors.c -o $(OBJ_DIR)/Executors.o

$(OBJ_DIR)/JobControl.o : $(SRC_DIR)/Job/JobControl.c $(INC_DIR)/Job/JobControl.h
	gcc $(CFLAGS) -c $(SRC_DIR)/Job/JobControl.c -o $(OBJ_DIR)/JobControl.o

$(OBJ_DIR)/JobList.o : $(SRC_DIR)/Job/JobList.c $(INC_DIR)/Job/JobList.h
	gcc $(CFLAGS) -c $(SRC_DIR)/Job/JobList.c -o $(OBJ_DIR)/JobList.o

$(OBJ_DIR)/Utilities.o : $(SRC_DIR)/Utilities/Utilities.c $(INC_DIR)/Utilities/Utilities.h
	gcc $(CFLAGS) -c $(SRC_DIR)/Utilities/Utilities.c -o $(OBJ_DIR)/Utilities.o

$(LIB_DIR)/libjobcontrol.a : $(OBJ_DIR)/JobControl.o $(OBJ_DIR)/JobList.o
	mkdir -p $(LIB_DIR)
	ar rs $(LIB_DIR)/libjobcontrol.a $(OBJ_DIR)/JobControl.o $(OBJ_DIR)/JobList.o

$(LIB_DIR)/libutilities.a : $(OBJ_DIR)/Utilities.o
	ar rs $(LIB_DIR)/libutilities.a $(OBJ_DIR)/Utilities.o

.PHONY: clean
clean:
	rm -f -r $(OBJ_DIR)
	rm -f -r $(BIN_DIR)
	rm -f -r $(LIB_DIR)