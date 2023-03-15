#include <vector>
#include "statistics.hpp"

class MultiNormalDistribution {
public:
  MultiNormalDistribution(const std::vector<double>& rho, const int N);
  ~MultiNormalDistribution();

  void random_draws(std::vector<std::vector<double>>& dist_draws);

private:
  int N_;
  std::vector<double> rho_;
  std::vector<double> means_;
  std::vector<std::vector<double>> covariance_matrix_;
  StandardNormalDistribution dist_;
};