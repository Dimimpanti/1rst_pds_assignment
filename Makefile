CC = gcc
CC_FLAGS = -O3
CILKCC = /usr/local/OpenCilk-2.0.0/bin/clang
SEQ_DIR = src/sequential

SEQ_FILES = $(SEQ_DIR)/sequential.c $(SEQ_DIR)/csr_csc_multiplication.c $(SEQ_DIR)/configuration_matrix.c src/read_matrix.c
SEQ_FILES := $(SEQ_FILES:%=$(SEQ_FILES)/%.o)

$(SEQ_DIR)/sequential.out: $(SEQ_FILES)
	@echo
	@echo -e "      Linking..."
	@echo
	@$(CC) $(CC_FLAGS) -o $(SEQ_DIR)/sequential.out $(SEQ_FILES)
	@echo -e "		Build finished successfully!"
	@echo

$(SEQ_DIR)/%.c.o: %.c
	@echo -e "        Compiling: $(<)..."
	@$(CC) $(CC_FLAGS) -c $< -o $@

%.c:

build_sequential: $(SEQ_DIR)/sequential.out
run_sequential: $(SEQ_DIR)/sequential.out
	@ echo
	@ echo "		Running sequential version..."
	@ ./src/sequential/sequential.out
	@ echo
	@ echo


.PHONY: clean
clean: 
	rm -rf *.o *.out