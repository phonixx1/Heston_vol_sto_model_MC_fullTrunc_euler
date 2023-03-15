#include "statistics.hpp"
#include "MultiNormalDistribution.hpp"
#include <vector>
#include <random>
#include <iostream>


MultiNormalDistribution::MultiNormalDistribution(const std::vector<double>& rho, const int N)
  : N_(N), rho_(rho), dist_() {
  means_.resize(N_, 0.0);
  covariance_matrix_.resize(N_, std::vector<double>(N_, 0.0));
  for (int i = 0; i < N_; i++) {
    covariance_matrix_[i][i] = 1.0;
    for (int j = i + 1; j < N_; j++) {
      covariance_matrix_[i][j] = rho_[j+i-1];
      covariance_matrix_[j][i] = rho_[j+i-1];
    }
  }
  if(rho.size()!=((N_-1)*(N_-1) + (N_ -1 ))/2)
  {
    std::cout << "Dimension error" <<std::endl;
  }
  
}

MultiNormalDistribution::~MultiNormalDistribution() {}

void MultiNormalDistribution::random_draws(std::vector<std::vector<double>>& dist_draws) {
  // Check that the input vector has the correct size
  if (dist_draws[0].size() != N_ || dist_draws.size() % 2 != 0) {
    std::cout << N_ << std::endl;
    std::cout << dist_draws[0].size() << std::endl;

    std::cout << "Invalid input vector size." << std::endl;
    return;
  }
  
  int M=dist_draws.size();
  // Generate N independent standard normal draws
  std::vector<double> uniform_draws(M*N_, 0.0);
  std::vector<double> standard_draws(M*N_, 0.0);
  
  for (int i=0; i<(uniform_draws.size()); i++) {
    uniform_draws[i] = (rand()+1) / (static_cast<double>(RAND_MAX)+1);
  }
  dist_.random_draws(uniform_draws,standard_draws);
  for (int i = 0; i < M; i++) {
    for (int j=0;j<N_;j++){
        dist_draws[i][j]=standard_draws[i*N_+j];
        
    }
  }
  // Apply the Cholesky decomposition to the covariance matrix
  std::vector<std::vector<double>> L(N_, std::vector<double>(N_, 0.0));
  
  for (int i = 0; i < N_; i++) {
    for (int j = 0; j <= i; j++) {
        double sum = 0;
        for (int k = 0; k < j; k++)
            sum += L[i][k] * L[j][k];

        if (i == j)
            L[i][j] = sqrt(covariance_matrix_[i][i] - sum);
        else
            L[i][j] = (1.0 / L[j][j] * (covariance_matrix_[i][j] - sum));
        }
    }
  
  // Multiply the independent standard normal draws by the Cholesky decomposition
  for (int n=0; n<M;n++){
    std::vector<double> temp(N_,0.0);
    for (int i = 0; i < N_; i++) {
        for (int j = 0; j <= i; j ++) {
            temp[i] += L[i][j]*dist_draws[n][j];
            
        }   
    }
    dist_draws[n] = temp;
    
  }
}
