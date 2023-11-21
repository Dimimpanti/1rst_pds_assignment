## 1rst_pds_assignment


### Dependencies

#### Make

The project is using make tools for building and running the code.

#### Open_Cilk

You can install OpenCilk by following the instructions of the official [website](https://www.opencilk.org/doc/users-guide/install/#installing-using-a-tarball).

`IMPORTANT!` In order for the Makefile to work, you need to set [this](Makefile#L2)variable to the path of the OpenCilk installation in your system.


### Usage

The `Makefile` is located in the root directory of the project. The following targets are available:

- `make` or `make all` : Builds the project.
- `make run_sequential` : Runs the sequential version of the code.
- `make run_pthread` : Runs the pthread version of the code.
- `make run_cilk` : Runs the open cilk version of the code.
- `make run_openmp` : Runs the open mp version of the code.

#### Arguments and default values

The following arguments are available for the code for each target:

##### Sequential

- `GRAPH` : The relative path of the graph file. Default value: none.
- `N_CLUSTERS` : The number of clusters to divide the nodes. Default value: 4.

Example: 
```
make run_sequential GRAPH=./path/to/graph/file.mtx N_CLUSTERS=8
```

##### Pthread

- `GRAPH` : The relative path of the graph file. Default value: none.
- `N_THREADS` : The number of threads to use. Default value: 8.
- `N_CLUSTERS` : The number of clusters to divide the nodes. Default value: 4.

Example: 
```
make run_pthread GRAPH=./path/to/graph/file.mtx N_THREADS=8 N_CLUSTERS=8
```

##### Cilk

- `GRAPH` : The relative path of the graph file. Default value: none.
- `CILK_WORKERS` : The number of workers to use. Default value: 8.
- `N_CLUSTERS` : The number of clusters to divide the nodes. Default value: 4.
- `CILK_TASKS` : The number of tasks to divide the work. Default value: 32. `IMPORTANT!` This argument denote the requested number of tasks, not the actual number of tasks that will be created. The actual number of tasks will be the closest possible to the requested number.

Example: 
```
make run_cilk GRAPH=./path/to/graph/file.mtx CILK_WORKERS=8 N_CLUSTERS=8 CILK_TASKS=32
```

##### OpenMP

- `GRAPH` : The relative path of the graph file. Default value: none.
- `N_THREADS` : The number of threads to use. Default value: 8.
- `N_CLUSTERS` : The number of clusters to divide the nodes. Default value: 4.

Example: 
```
make run_openmp GRAPH=./path/to/graph/file.mtx N_THREADS=8 N_CLUSTERS=8
```
