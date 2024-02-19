#pragma once
#include <functional>
#include <random>

class cem
{
  std::vector<double> optimized_weights;
  int nb_iterations;
  int batch_size;
  std::function<int(const std::vector<double>&)> fitness;
  int elite_size;

public:
  cem(const std::vector<double>& weights, int nb_iterations, int batch_size, float elite_fraction, std::function<int(const std::vector<double>&)> fitness);
  void run();
  std::vector<double> get_best_weights() const;
};
