#include "lab1_sequential.h"
#include <math.h>
#include <unordered_set>
#include <random>
#include <algorithm>
#include <iostream>
#include <omp.h>

using namespace std;

#define MAX_ITERATIONS 10000
#define DIMENSION 3
#define MAX_DIST 1000000000.0
#define CHANGE_THRESHOLD 2

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
    std::mt19937 gen(rd());

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

void kmeans_sequential(int num_threads, int N, int K, int* data_points, int** data_point_cluster, int** centroids, int* num_iterations){
	int iterations = 1;
	int points_in_cluster[K];
	int cluster_changes = 0;
	initialize(N, K, data_points, data_point_cluster, centroids);

	omp_set_num_threads(num_threads);

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
		#pragma omp parallel
		{
			int tid = omp_get_thread_num();
		#pragma omp for
			for(int i 0; i < N; i++){
				int cluster = assign_centroid(K, &data_points[i*3 + 0], &centroids[0][(iterations-1)*K*3]);
				if(data_point_cluster[0][i*4 + 3] != cluster){
		#pragma omp atomic 
					cluster_changes++;
				}
				data_point_cluster[0][i*4 + 3] = cluster;
		#pragma omp atomic 
				points_in_cluster[cluster]++;
			}
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
	cout << iterations-1 << "\n";
}