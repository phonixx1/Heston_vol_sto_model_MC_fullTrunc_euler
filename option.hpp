#ifndef __VANILLA_OPTION_H
#define __VANILLA_OPTION_H

#include "payoff.hpp"

class VanillaOption {
 public:
  PayOff* m_pay_off;

  double m_K;
  double m_r;
  double m_T;
  double m_sigma;

  VanillaOption();
  VanillaOption(double K, double r, double T, 
                double sigma, PayOff* pay_off);
   ~VanillaOption();

};

#endif