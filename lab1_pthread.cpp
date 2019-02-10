#include "lab1_sequential.h"
#include <math.h>
#include <unordered_set>
#include <random>
#include <algorithm>
#include <iostream>
#include <pthread.h>
#include <stdint.h>

using namespace std;

#define MAX_ITERATIONS 10000
#define DIMENSION 3
#define MAX_DIST 1000000000.0
#define CHANGE_THRESHOLD 2

/********** global variables ************/
pthread_mutex_t lock1,lock2;
int N_gl; int K_gl; int* data_points_gl; int** data_point_cluster_gl; int** centroids_gl;
int iterations;
int* points_in_cluster;
int cluster_changes;
int num_threads_gl;
/****************************************/

// measures distance between points x and y
// d : dimension
double distance(int* x, int* y, int d=DIMENSION){
	double s = 0;
	for(int i = 0; i < d; i++){
		s += pow(x[i] - y[i],2);
	}
	return pow(s,0.5);
}

//chooses k random numbers from 0 to N-1 withoud repetition
std::unordered_set<int> choose_random(int N, int K, std::mt19937& gen)
{
    std::unordered_set<int> elems;
    for (int r = N - K; r < N; ++r) {
        int v = std::uniform_int_distribution<>(1, r)(gen);
        if (!elems.insert(v).second) {
            elems.insert(r);
        }   
    }
    return elems;
}

// initializes the data_point_cluster and centroids
void initialize(int N, int K, int * data_points, int** data_point_cluster, int** centroids){
	*data_point_cluster = (int *)malloc(sizeof(int)*N*4);
	*centroids = (int *)malloc(sizeof(int)*K*3*MAX_ITERATIONS);

	// initialize data_point_cluster
	for(int i = 0; i < N; i++){
		data_point_cluster[0][i*4 + 0] = data_points[i*3 + 0];
		data_point_cluster[0][i*4 + 1] = data_points[i*3 + 1];
		data_point_cluster[0][i*4 + 2] = data_points[i*3 + 2];
		data_point_cluster[0][i*4 + 3] = 0;
	}

	//intitalize centroids using forgy method
	//choosing k points at random
	std::random_device rd;
    std::mt19937 gen(1234);

    unordered_set<int> elems = choose_random(N, K, gen);

    vector<int> points(elems.begin(), elems.end());
    std::shuffle(points.begin(), points.end(), gen);

	for(int i = 0; i < K; i++){
		centroids[0][i*3 + 0] = data_points[points[i]*3 + 0];
		centroids[0][i*3 + 1] = data_points[points[i]*3 + 1];
		centroids[0][i*3 + 2] = data_points[points[i]*3 + 2];
	}
}

int assign_centroid(int K, int* x, int* centroids){
	double min_dist = MAX_DIST;
	int cluster = 0;
	double dist;
	for(int i = 0; i < K; i++){
		dist = distance(x, &centroids[i*3 + 0]);
		if(dist < min_dist){
			min_dist = dist;
			cluster = i;
		}
	}
	return cluster;
}

void* assgin_cluster(void *tid){
	int id = (intptr_t)tid;
	for(int i = id*(N_gl/num_threads_gl); i < (id+1)*(N_gl/num_threads_gl); i++){
		int cluster = assign_centroid(K_gl, &data_points_gl[i*3 + 0], &centroids_gl[0][(iterations-1)*K_gl*3]);
		if(data_point_cluster_gl[0][i*4 + 3] != cluster){
			pthread_mutex_lock(&lock1);
			cluster_changes++;
			pthread_mutex_unlock(&lock1);
		}
		data_point_cluster_gl[0][i*4 + 3] = cluster;
		pthread_mutex_lock(&lock2);
		points_in_cluster[cluster]++;
		pthread_mutex_unlock(&lock2);
	}
	pthread_exit(NULL);
}

void kmeans_pthread(int num_threads, int N, int K, int* data_points, int** data_point_cluster, int** centroids, int* num_iterations){
	pthread_t kmeans_thr[num_threads];
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);

	iterations = 1;
	points_in_cluster = (int*)malloc(sizeof(int)*K);
	cluster_changes = 0;
	initialize(N, K, data_points, data_point_cluster, centroids);
	num_threads_gl = num_threads; N_gl = N; K_gl = K; data_points_gl = data_points; data_point_cluster_gl = data_point_cluster; centroids_gl = centroids;

	while(iterations < MAX_ITERATIONS){
		cluster_changes = 0;
		//intitalize points in each cluster to 0;
		//initialize centroid in this iteration
		for(int i = 0; i < K; i++){
			points_in_cluster[i] = 0;
			centroids[0][iterations*K*3 + i*3 + 0] = 0;
			centroids[0][iterations*K*3 + i*3 + 1] = 0;
			centroids[0][iterations*K*3 + i*3 + 2] = 0;
		}

		//assigning clusters to all points
		int t;
		for(t=0; t < num_threads; t++){
			pthread_create(&kmeans_thr[t], NULL, assgin_cluster, (void *)(intptr_t)t);
		}
		for(int i=0; i < num_threads; i++){
			pthread_join(kmeans_thr[i], NULL);
		}

		//recomputing centroids for each cluster
		for(int i = 0; i < N; i++){
			int cluster = data_point_cluster[0][i*4 + 3];
			centroids[0][iterations*K*3 + cluster*3 + 0] += data_point_cluster[0][i*4 + 0];
			centroids[0][iterations*K*3 + cluster*3 + 1] += data_point_cluster[0][i*4 + 1];
			centroids[0][iterations*K*3 + cluster*3 + 2] += data_point_cluster[0][i*4 + 2];
		}
		for(int i = 0; i < K; i++){
			if(points_in_cluster[i] <= 0){
				cout << "Error: points in cluster " << i << " less than 0\n";
			}
			centroids[0][iterations*K*3 + i*3 + 0] /= points_in_cluster[i];
			centroids[0][iterations*K*3 + i*3 + 1] /= points_in_cluster[i];
			centroids[0][iterations*K*3 + i*3 + 2] /= points_in_cluster[i];	
		}

		iterations++;

		if(cluster_changes < CHANGE_THRESHOLD){
			break; 
		}

	}
	*num_iterations = iterations-1;
	cout << num_threads << ":" << N << ":" << K << ":" << iterations-1 << ":";
}
