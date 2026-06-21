// gen_data.cpp  (CSUSB / CSE5250 addition)
// -----------------------------------------------------------------------------
// Standalone generator for prog6's data.dat. This removes the need to download
// the large data file from an internal network -- students just run:
//
//     make gen
//     ./gen_data            # writes ./data.dat
//     ./gen_data 1000000    # optional: original-size (~800MB) workload
//
// (You usually don't even need this: `kmeans` auto-generates data.dat on its
//  first run if the file is missing. This tool is here if you want to build the
//  dataset ahead of time or change its size.)
//
// Generation is fully deterministic (fixed seeds), so every student gets the
// exact same bytes -- grading stays fair. The format matches utils.cpp's
// writeData: [int M][int N][int K][double epsilon][double data MxN]
//            [double centroids KxN][int assignments M].
// -----------------------------------------------------------------------------
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

using namespace std;

#define SEED 7

static double randDouble() {
  return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

// Identical to main.cpp's initData (10 true clusters + Gaussian noise).
static void initData(double *data, int M, int N) {
  int K = 10;
  double *centers = new double[K * N];
  double mean = 0.0, stddev = 0.5;
  std::default_random_engine generator;
  std::normal_distribution<double> normal_dist(mean, stddev);

  for (int k = 0; k < K; k++)
    for (int n = 0; n < N; n++)
      centers[k * N + n] = randDouble();

  for (int m = 0; m < M; m++) {
    int startingPoint = rand() % K;
    for (int n = 0; n < N; n++) {
      double noise = normal_dist(generator);
      data[m * N + n] = centers[startingPoint * N + n] + noise;
    }
  }
  delete[] centers;
}

// Identical to main.cpp's initCentroids (clusters start close together).
static void initCentroids(double *clusterCentroids, int K, int N) {
  for (int n = 0; n < N; n++)
    clusterCentroids[n] = randDouble();
  for (int k = 1; k < K; k++)
    for (int n = 0; n < N; n++)
      clusterCentroids[k * N + n] =
          clusterCentroids[n] + (randDouble() - 0.5) * 0.1;
}

static double dist(double *x, double *y, int nDim) {
  double accum = 0.0;
  for (int i = 0; i < nDim; i++)
    accum += pow((x[i] - y[i]), 2);
  return sqrt(accum);
}

static void writeData(const string &filename, double *data,
                      double *clusterCentroids, int *clusterAssignments,
                      int *M_p, int *N_p, int *K_p, double *epsilon_p) {
  int M = *M_p, N = *N_p, K = *K_p;
  ofstream dataFile(filename, ios::out | ios::binary);
  dataFile.write((char *)M_p, sizeof(int));
  dataFile.write((char *)N_p, sizeof(int));
  dataFile.write((char *)K_p, sizeof(int));
  dataFile.write((char *)epsilon_p, sizeof(double));
  dataFile.write((char *)data, sizeof(double) * (size_t)M * N);
  dataFile.write((char *)clusterCentroids, sizeof(double) * (size_t)K * N);
  dataFile.write((char *)clusterAssignments, sizeof(int) * (size_t)M);
  dataFile.close();
}

int main(int argc, char **argv) {
  srand(SEED);

  int M = 200000;  // laptop-friendly default (~160MB). Original was 1000000.
  int N = 100;
  int K = 10;
  double epsilon = 0.1;
  if (argc > 1) M = atoi(argv[1]);

  cout << "Generating data.dat with M=" << M << ", N=" << N << ", K=" << K
       << " ..." << endl;

  double *data = new double[(size_t)M * N];
  double *clusterCentroids = new double[(size_t)K * N];
  int *clusterAssignments = new int[M];

  initData(data, M, N);
  initCentroids(clusterCentroids, K, N);

  for (int m = 0; m < M; m++) {
    double minDist = 1e30;
    int best = -1;
    for (int k = 0; k < K; k++) {
      double d = dist(&data[m * N], &clusterCentroids[k * N], N);
      if (d < minDist) {
        minDist = d;
        best = k;
      }
    }
    clusterAssignments[m] = best;
  }

  writeData("./data.dat", data, clusterCentroids, clusterAssignments, &M, &N,
            &K, &epsilon);

  delete[] data;
  delete[] clusterCentroids;
  delete[] clusterAssignments;
  cout << "Wrote ./data.dat" << endl;
  return 0;
}
