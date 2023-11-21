CC = gcc
CILKCC = /usr/local/OpenCilk-2.0.0/bin/clang

# Sequential
SEQ_DIR = src/sequential
SEQ_BUILDDIR = build_debug/sequential

SEQ_FILES = $(SEQ_DIR)/sequential.c $(SEQ_DIR)/csr_csc_multiplication.c $(SEQ_DIR)/configuration_matrix.c src/read_matrix.c
SEQ_FILES := $(SEQ_FILES:%=$(SEQ_BUILDDIR)/%.o)

# Pthreads
PTHREAD_DIR = src/p_threads
PTHREAD_BUILDDIR = build_debug/p_threads

PTHREAD_FILES = $(PTHREAD_DIR)/pthread.c $(PTHREAD_DIR)/csr_csc_multiplication_pthread.c $(PTHREAD_DIR)/configuration_matrix_pthread.c src/read_matrix.c
PTHREAD_FILES := $(PTHREAD_FILES:%=$(PTHREAD_BUILDDIR)/%.o)

# OpenMP
OPENMP_DIR = src/open_mp
OPENMP_BUILDDIR = build_debug/open_mp

OPENMP_FILES = $(OPENMP_DIR)/openmp.c $(OPENMP_DIR)/csr_csc_multiplication_openmp.c $(OPENMP_DIR)/configuration_matrix_openmp.c src/read_matrix.c
OPENMP_FILES := $(OPENMP_FILES:%=$(OPENMP_BUILDDIR)/%.o)

# Cilk
CILK_DIR = src/open_cilk
CILK_BUILDDIR = build_debug/open_cilk

CILK_FILES = $(CILK_DIR)/open_cilk.c $(CILK_DIR)/csr_csc_multiplication_cilk.c $(CILK_DIR)/configuration_matrix_cilk.c src/read_matrix.c
CILK_FILES := $(CILK_FILES:%=$(CILK_BUILDDIR)/%.o)


# Arguments
GRAPH = ./graphs/mycielskian13/mycielskian13.mtx
N_THREADS = 16
N_CLUSTERS = 4
CILK_NWORKERS = 16
CILK_TASKS = 32


CC_FLAGS = -O3 -g -Wall


$(SEQ_DIR)/sequential.out: $(SEQ_FILES)
	@echo
	@echo -e "	Linking..."
	@echo
	@$(CC) $(CC_FLAGS) -o $(SEQ_BUILDDIR)/sequential.out $(SEQ_FILES)
	@echo -e "	Build finished successfully!"


$(SEQ_BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo -e "  Compiling: $(<)..."
	@$(CC) $(CC_FLAGS) -c $< -o $@


$(PTHREAD_DIR)/pthread.out: $(PTHREAD_FILES)
	@echo
	@echo -e "	Linking..."
	@echo
	@$(CC) $(CC_FLAGS) -o $(PTHREAD_BUILDDIR)/pthread.out $(PTHREAD_FILES) -lpthread
	@echo -e "	Build finished successfully!"

$(PTHREAD_BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo -e "  Compiling: $(<)..."
	@$(CC) $(CC_FLAGS) -c $< -o $@


$(OPENMP_DIR)/openmp.out: $(OPENMP_FILES)
	@echo
	@echo -e "	Linking..."
	@echo
	@$(CC) $(CC_FLAGS) -o $(OPENMP_BUILDDIR)/openmp.out $(OPENMP_FILES) -fopenmp
	@echo -e "	Build finished successfully!"

$(OPENMP_BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo -e "  Compiling: $(<)..."
	@$(CC) $(CC_FLAGS) -c $< -o $@ -fopenmp

$(CILK_DIR)/open_cilk.out: $(CILK_FILES)
	@echo
	@echo -e "	Linking..."
	@echo
	@$(CILKCC) $(CC_FLAGS) -o $(CILK_BUILDDIR)/open_cilk.out $(CILK_FILES) -fopencilk
	@echo -e "	Build finished successfully!"

$(CILK_BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo -e "  Compiling: $(<)..."
	@$(CILKCC) $(CC_FLAGS) -c $< -o $@ -fopencilk

%.c:

build_sequential: $(SEQ_DIR)/sequential.out
run_sequential: $(SEQ_DIR)/sequential.out
	@ echo
	@ echo "	Running sequential version..."
	@ $(SEQ_BUILDDIR)/sequential.out $(GRAPH) $(N_CLUSTERS)
	@ echo
	@ echo


build_pthread: $(PTHREAD_DIR)/pthread.out
run_pthread: $(PTHREAD_DIR)/pthread.out
	@ echo
	@ echo "	Running pthread version..."
	@ $(PTHREAD_BUILDDIR)/pthread.out $(GRAPH) $(N_CLUSTERS) $(N_THREADS)
	@ echo
	@ echo

build_openmp: $(OPENMP_DIR)/openmp.out
run_openmp: $(OPENMP_DIR)/openmp.out
	@ echo
	@ echo "	Running openmp version..."
	@ $(OPENMP_BUILDDIR)/openmp.out $(GRAPH) $(N_CLUSTERS)
	@ echo
	@ echo

build_cilk: $(CILK_DIR)/open_cilk.out
run_cilk: $(CILK_DIR)/open_cilk.out
	@ echo
	@ echo "	Running open cilk version..."
	@ CILK_NWORKERS=$(CILK_NWORKERS) $(CILK_BUILDDIR)/open_cilk.out $(GRAPH) $(N_CLUSTERS) $(CILK_TASKS)
	@ echo
	@ echo

.PHONY: clean
clean:
	@echo
	@echo -e "	Cleaning..."
	@echo
	@rm -rf $(SEQ_BUILDDIR)
	@rm -rf $(PTHREAD_BUILDDIR)
	@rm -rf $(OPENMP_BUILDDIR)
	@rm -rf $(CILK_BUILDDIR)
	@echo -e "	Clean finished successfully!"
	@echo
	