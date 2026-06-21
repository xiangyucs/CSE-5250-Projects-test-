#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "CycleTimer.h"

#define SEED 7
#define SAMPLE_RATE 1e-2

using namespace std;

// Main compute functions
extern void kMeansThread(double *data, double *clusterCentroids,
                      int *clusterAssignments, int M, int N, int K,
                      double epsilon);
extern double dist(double *x, double *y, int nDim);

// Utilities
extern void logToFile(string filename, double sampleRate, double *data,
                      int *clusterAssignments, double *clusterCentroids, int M,
                      int N, int K);
extern void writeData(string filename, double *data, double *clusterCentroids,
                      int *clusterAssignments, int *M_p, int *N_p, int *K_p,
                      double *epsilon_p);
extern void readData(string filename, double **data, double **clusterCentroids,
                     int **clusterAssignments, int *M_p, int *N_p, int *K_p,
                     double *epsilon_p);

// Functions for generating data
double randDouble() {
  return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

void initData(double *data, int M, int N) {
  int K = 10;
  double *centers = new double[K * N];

  // Gaussian noise
  double mean = 0.0;
  double stddev = 0.5;
  std::default_random_engine generator;
  std::normal_distribution<double> normal_dist(mean, stddev);

  // Randomly create points to center data around
  for (int k = 0; k < K; k++) {
    for (int n = 0; n < N; n++) {
      centers[k * N + n] = randDouble();
    }
  }

  // Even clustering
  for (int m = 0; m < M; m++) {
    int startingPoint = rand() % K; // Which center to start from
    for (int n = 0; n < N; n++) {
      double noise = normal_dist(generator);
      data[m * N + n] = centers[startingPoint * N + n] + noise;
    }
  }

  delete[] centers;
}

void initCentroids(double *clusterCentroids, int K, int N) {
  // Initialize centroids (close together - makes it a bit more interesting)
  for (int n = 0; n < N; n++) {
    clusterCentroids[n] = randDouble();
  }
  for (int k = 1; k < K; k++) {
    for (int n = 0; n < N; n++) {
      clusterCentroids[k * N + n] =
          clusterCentroids[n] + (randDouble() - 0.5) * 0.1;
    }
  }
}

int main() {
  srand(SEED);

  int M, N, K;
  double epsilon;

  double *data;
  double *clusterCentroids;
  int *clusterAssignments;

  // ---------------------------------------------------------------------------
  // CSUSB (CSE5250) change: zero-cloud, NO Stanford network required.
  // The original assignment shipped a large data.dat on an internal university
  // network. Here we auto-generate an equivalent dataset on the first
  // run if data.dat is missing, then cache it to disk so every later run (and
  // grading) reads the exact same bytes. Generation is fully deterministic
  // (srand(SEED) above + fixed RNG), so every student gets identical data,
  // which keeps grading fair. You can also pre-build the file with the bundled
  // generator: `make gen && ./gen_data`.
  //
  // Size knob: DATA_M is the number of points. The default is laptop-friendly
  // (~160MB, runs in seconds). Set DATA_M = 1000000 to reproduce the original
  // ~800MB workload exactly.
  // ---------------------------------------------------------------------------
  const int DATA_M = 200000;   // points      (original: 1000000)
  const int DATA_N = 100;      // dimensions per point
  const int DATA_K = 10;       // number of clusters

  {
    ifstream probe("./data.dat", ios::in | ios::binary);
    bool haveFile = probe.good();
    probe.close();

    if (!haveFile) {
      cout << "data.dat not found -- generating it locally "
              "(CSUSB: no download needed)..." << endl;

      M = DATA_M;
      N = DATA_N;
      K = DATA_K;
      epsilon = 0.1;

      data = new double[(size_t)M * N];
      clusterCentroids = new double[(size_t)K * N];
      clusterAssignments = new int[M];

      initData(data, M, N);
      initCentroids(clusterCentroids, K, N);

      for (int m = 0; m < M; m++) {
        double minDist = 1e30;
        int bestAssignment = -1;
        for (int k = 0; k < K; k++) {
          double d = dist(&data[m * N], &clusterCentroids[k * N], N);
          if (d < minDist) {
            minDist = d;
            bestAssignment = k;
          }
        }
        clusterAssignments[m] = bestAssignment;
      }

      // Cache to disk so subsequent runs and grading read identical data.
      writeData("./data.dat", data, clusterCentroids, clusterAssignments, &M,
                &N, &K, &epsilon);

      delete[] data;
      delete[] clusterCentroids;
      delete[] clusterAssignments;
    }
  }

  // We grade using data.dat (now guaranteed to exist); read it here:
  readData("./data.dat", &data, &clusterCentroids, &clusterAssignments, &M, &N,
           &K, &epsilon);

  printf("Running K-means with: M=%d, N=%d, K=%d, epsilon=%f\n", M, N,
         K, epsilon);

  // Log the starting state of the algorithm
  logToFile("./start.log", SAMPLE_RATE, data, clusterAssignments,
            clusterCentroids, M, N, K);

  double startTime = CycleTimer::currentSeconds();
  kMeansThread(data, clusterCentroids, clusterAssignments, M, N, K, epsilon);
  double endTime = CycleTimer::currentSeconds();
  printf("[Total Time]: %.3f ms\n", (endTime - startTime) * 1000);

  // Log the end state of the algorithm
  logToFile("./end.log", SAMPLE_RATE, data, clusterAssignments,
            clusterCentroids, M, N, K);

  delete[] data;
  delete[] clusterCentroids;
  delete[] clusterAssignments;
  return 0;
}