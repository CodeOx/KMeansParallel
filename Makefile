all:
	g++ -o seq main_sequential.c lab1_io.c lab1_sequential.cpp -fopenmp
	g++ -o pthread main_pthread.c lab1_io.c lab1_pthread.cpp -fopenmp -lpthread
	g++ -o omp main_omp.c lab1_io.c lab1_omp.cpp -fopenmp
seq:
	g++ -o seq main_sequential.c lab1_io.c lab1_sequential.cpp -fopenmp
pthread:
	g++ -o pthread main_pthread.c lab1_io.c lab1_pthread.cpp -fopenmp -lpthread
omp:
	g++ -o omp main_omp.c lab1_io.c lab1_omp.cpp -fopenmp
clean:
	rm omp seq pthread
	echo Clean done
