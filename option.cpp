#include "option.hpp"

VanillaOption::VanillaOption() {}

VanillaOption::VanillaOption(double K, double r, double T, 
                             double sigma, PayOff* pay_off) : 
  m_K(K), m_r(r), m_T(T), m_sigma(sigma), m_pay_off(pay_off) {}

VanillaOption::~VanillaOption() {}