CC = gcc
CILKCC = /usr/local/OpenCilk-2.0.0/bin/clang

# Sequential
SEQ_DIR = src/sequential
SEQ_BUILDDIR = build_debug/sequential

SEQ_FILES = $(SEQ_DIR)/sequential.c $(SEQ_DIR)/csr_csc_multiplication.c src/configuration_matrix.c src/read_matrix.c
SEQ_FILES := $(SEQ_FILES:%=$(SEQ_BUILDDIR)/%.o)

# Sequential V2
SEQ_V2_DIR = src/sequential_v2
SEQ_V2_BUILDDIR = build_debug/sequential_v2

SEQ_V2_FILES = $(SEQ_V2_DIR)/sequential_v2.c $(SEQ_V2_DIR)/graph_minor.c src/configuration_matrix.c src/read_matrix.c
SEQ_V2_FILES := $(SEQ_V2_FILES:%=$(SEQ_V2_BUILDDIR)/%.o)

# Pthreads
PTHREAD_DIR = src/p_threads
PTHREAD_BUILDDIR = build_debug/p_threads

PTHREAD_FILES = $(PTHREAD_DIR)/pthread.c $(PTHREAD_DIR)/csr_csc_multiplication_pthread.c src/configuration_matrix.c src/read_matrix.c
PTHREAD_FILES := $(PTHREAD_FILES:%=$(PTHREAD_BUILDDIR)/%.o)

# OpenMP
OPENMP_DIR = src/open_mp
OPENMP_BUILDDIR = build_debug/open_mp

OPENMP_FILES = $(OPENMP_DIR)/openmp.c $(OPENMP_DIR)/csr_csc_multiplication_openmp.c src/configuration_matrix.c src/read_matrix.c
OPENMP_FILES := $(OPENMP_FILES:%=$(OPENMP_BUILDDIR)/%.o)

# Cilk
CILK_DIR = src/open_cilk
CILK_BUILDDIR = build_debug/open_cilk

CILK_FILES = $(CILK_DIR)/open_cilk.c $(CILK_DIR)/csr_csc_multiplication_cilk.c src/configuration_matrix.c src/read_matrix.c
CILK_FILES := $(CILK_FILES:%=$(CILK_BUILDDIR)/%.o)


# Arguments
RI = ri2010
NJ = nj2010
PA = pa2010
CA = ca2010
TX = tx2010
USA = road_usa

GRAPH = ./graphs/$(RI).mtx
CLUSTER = ./graphs/clusters/$(RI).txt
N_THREADS = 16
N_CLUSTERS = 4
CILK_NWORKERS = 16
CILK_TASKS = 64


CC_FLAGS = -O0 -g -Wall # -O3

all: build_sequential build_pthread build_openmp build_cilk

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


$(SEQ_V2_DIR)/sequential_v2.out: $(SEQ_V2_FILES)
	@echo
	@echo -e "	Linking..."
	@echo
	@$(CC) $(CC_FLAGS) -o $(SEQ_V2_BUILDDIR)/sequential_v2.out $(SEQ_V2_FILES)
	@echo -e "	Build finished successfully!"


$(SEQ_V2_BUILDDIR)/%.c.o: %.c
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
	@ $(SEQ_BUILDDIR)/sequential.out $(GRAPH) $(CLUSTER)
	@ echo
	@ echo


build_pthread: $(PTHREAD_DIR)/pthread.out
run_pthread: $(PTHREAD_DIR)/pthread.out
	@ echo
	@ echo "	Running pthread version..."
	@ $(PTHREAD_BUILDDIR)/pthread.out $(GRAPH) $(CLUSTER) $(N_THREADS)
	@ echo
	@ echo

build_openmp: $(OPENMP_DIR)/openmp.out
run_openmp: $(OPENMP_DIR)/openmp.out
	@ echo
	@ echo "	Running openmp version..."
	@ $(OPENMP_BUILDDIR)/openmp.out $(GRAPH) $(CLUSTER) $(N_THREADS)
	@ echo
	@ echo

build_cilk: $(CILK_DIR)/open_cilk.out
run_cilk: $(CILK_DIR)/open_cilk.out
	@ echo
	@ echo "	Running open cilk version..."
	@ CILK_NWORKERS=$(CILK_NWORKERS) $(CILK_BUILDDIR)/open_cilk.out $(GRAPH) $(CLUSTER) $(CILK_TASKS)
	@ echo
	@ echo

build_sequential_V2: $(SEQ_V2_DIR)/sequential_v2.out
run_sequential_V2: $(SEQ_V2_DIR)/sequential_v2.out
	@ echo
	@ echo "	Running sequential version V2..."
	@ $(SEQ_V2_BUILDDIR)/sequential_v2.out $(GRAPH) $(CLUSTER)
	@ echo
	@ echo

run_all: run_sequential run_pthread run_openmp run_cilk

.PHONY: clean
clean:
	@echo
	@echo -e "	Cleaning..."
	@echo
	@rm -rf $(SEQ_BUILDDIR)
	@rm -rf $(SEQ_V2_BUILDDIR)
	@rm -rf $(PTHREAD_BUILDDIR)
	@rm -rf $(OPENMP_BUILDDIR)
	@rm -rf $(CILK_BUILDDIR)
	@echo -e "	Clean finished successfully!"
	@echo
	