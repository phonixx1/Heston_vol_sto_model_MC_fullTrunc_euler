#include "statistics.hpp"
#include "MultiNormalDistribution.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include "heston.hpp"
#include "payoff.hpp"
#include "option.hpp"

std::vector<std::vector<double>> generate_normal_correlation_paths(double rho, 
                                        std::vector<std::vector<double>>& cor_spot_vol_normals) {
    unsigned vals = cor_spot_vol_normals.size();
    std::vector<double> rho1 = { rho };
    int N = 2;
    
    MultiNormalDistribution dist(rho1,N);
    dist.random_draws(cor_spot_vol_normals);
    std::vector<std::vector<double>> t_cor_spot_vol_normals(cor_spot_vol_normals[0].size(), std::vector<double>(cor_spot_vol_normals.size(), 0.0));
    for (int j = 0; j < cor_spot_vol_normals.size(); j++)
    {
        for (int i=0;i<cor_spot_vol_normals[0].size();i++)
        {
            t_cor_spot_vol_normals[i][j] = cor_spot_vol_normals[j][i];
        }
        
        
    }
    return t_cor_spot_vol_normals;
  
}

int main(int argc, char **argv) {
    // First we create the parameter list
    // Note that you could easily modify this code to input the parameters
    // either from the command line or via a file
    unsigned num_sims = 100000;   // Number of simulated asset paths
    unsigned num_intervals = 1000;  // Number of intervals for the asset path to be sampled 

    double S_0 = 100.0;    // Initial spot price
    double K = 100.0;      // Strike price
    double r = 0.0319;     // Risk-free rate
    double v_0 = 0.010201; // Initial volatility 
    double T = 1.00;       // One year until expiry

    double rho = -0.7;     // Correlation of asset and volatility
    double kappa = 6.21;   // Mean-reversion rate
    double theta = 0.019;  // Long run average volatility
    double xi = 0.61;      // "Vol of vol"

    // Create the PayOff, Option and Heston objects
    PayOff* pPayOffCall = new PayOffCall(K);
    VanillaOption* pOption = new VanillaOption(K, r, T,v_0, pPayOffCall);
    HestonEuler hest_euler(pOption, kappa, theta, xi, rho);

    // Create the spot and vol initial normal and price paths

    // Vector of initial spot normal draws and Vector of initial correlated vol normal draws in each dimension 
    std::vector<std::vector<double>> init_cor_spot_vol_normals(num_intervals, std::vector<double>(2, 0.0));
    std::vector<std::vector<double>> t_cor_spot_vol_normals(2, std::vector<double>(num_intervals, 0.0));
    std::vector<double> spot_prices(num_intervals, S_0);  // Vector of initial spot prices
    std::vector<double> vol_prices(num_intervals, v_0);   // Vector of initial vol prices
    // Monte Carlo options pricing
    double payoff_sum = 0.0;
    for (unsigned i=0; i<num_sims; i++) {

        std::cout << "Calculating path " << i+1 << " of " << num_sims << std::endl; 
        t_cor_spot_vol_normals = generate_normal_correlation_paths(rho, init_cor_spot_vol_normals);
        hest_euler.calc_vol_path(t_cor_spot_vol_normals[0], vol_prices);
        hest_euler.calc_spot_path(t_cor_spot_vol_normals[1], vol_prices, spot_prices);
        
        payoff_sum += pOption->m_pay_off->operator()(spot_prices[num_intervals-1]);
    }
    double option_price = (payoff_sum / static_cast<double>(num_sims)) * exp(-r*T);
    std::cout << "Option Price: " << option_price << std::endl;
    

    // Free memory
    delete pOption;
    delete pPayOffCall;

    return 0;
}


  