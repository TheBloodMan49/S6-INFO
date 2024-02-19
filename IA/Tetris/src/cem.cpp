#include "cem.hpp"
#include <functional>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <omp.h>

using namespace std;

cem::cem(const vector<double>& weights, int nb_iterations, int batch_size,
         float elite_fraction, function<int(const vector<double>&)> fitness)
  : optimized_weights(weights), nb_iterations(nb_iterations),
    batch_size(batch_size), fitness(fitness)
{
  elite_size = batch_size * elite_fraction;
}

void cem::run()
{
  const int WEIGHTS_SIZE = optimized_weights.size();
  vector<double> theta_mean = optimized_weights;
  vector<double> theta_std = vector<double>(WEIGHTS_SIZE, 10.0);

  float extra_std = 2;

  vector<vector<double>> thetas(batch_size, vector<double>(WEIGHTS_SIZE));

  vector<pair<double, int>> fitness_index(batch_size);

  vector<normal_distribution<double>> distributions(WEIGHTS_SIZE);

  int best_fitness = fitness(optimized_weights);

  mt19937 generator(chrono::system_clock::now().time_since_epoch().count());

  for (int iter = 0; iter < nb_iterations; ++iter)
    {

      for (int i = 0; i < WEIGHTS_SIZE; ++i)
        {
          distributions[i] = normal_distribution<double>(theta_mean[i],
                                                         theta_std[i]
                                                         + extra_std
                                                         * max(0.0, (1.0 - (double)iter / (nb_iterations >> 1))));
        }

      for (int i = 0; i < batch_size; ++i)
        {
          for (int j = 0; j < WEIGHTS_SIZE; ++j)
            {
              thetas[i][j] = distributions[j](generator);
            }
        }

#pragma omp parallel for schedule(dynamic)
      for (int i = 0; i < batch_size; ++i)
        {
          fitness_index[i] = make_pair(fitness(thetas[i]), i);
        }

      sort(fitness_index.begin(), fitness_index.end(), greater<pair<double, int>>());

      if (fitness_index[0].first > best_fitness)
        {
          best_fitness = fitness_index[0].first;
          optimized_weights = thetas[fitness_index[0].second];
          for (double w : optimized_weights)
            {
              cout << w << " ";
            }
          cout << endl;
        }

      cout << "generation " << iter << ": " << best_fitness << endl;

      for (int i = 0; i < WEIGHTS_SIZE; ++i)
        {
          theta_mean[i] = 0;
          theta_std[i] = 0;
        }

      // MEAN
      for (int i = 0; i < elite_size; ++i)
        {
          for (int j = 0; j < WEIGHTS_SIZE; ++j)
            {
              theta_mean[j] += thetas[fitness_index[i].second][j];
            }
        }

      for (int i = 0; i < WEIGHTS_SIZE; ++i)
        {
          theta_mean[i] /= elite_size;
        }

      // STDEV
      for (int i = 0; i < elite_size; ++i)
        {
          for (int j = 0; j < WEIGHTS_SIZE; ++j)
            {
              float v = thetas[fitness_index[i].second][j] - theta_mean[j];
              theta_std[j] += v * v;
            }
        }

      for (int i = 0; i < WEIGHTS_SIZE; ++i)
        {
          theta_std[i] = sqrt(theta_std[i] / elite_size);
        }
    }
}

vector<double> cem::get_best_weights() const
{
  return optimized_weights;
}
