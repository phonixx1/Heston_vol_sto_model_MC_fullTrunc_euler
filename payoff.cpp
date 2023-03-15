#include "payoff.hpp"

PayOff::PayOff() {}

// ==========
// PayOffCall
// ==========

// Constructor with single strike parameter
PayOffCall::PayOffCall(const double& K) { m_K = K; }

// Over-ridden operator() method, which turns PayOffCall into a function object
double PayOffCall::operator() (const double& S) const {
  return std::max(S-m_K, 0.0); // Standard European call pay-off
}

// =========
// PayOffPut
// =========

// Constructor with single strike parameter
PayOffPut::PayOffPut(const double& K) { m_K = K;}

// Over-ridden operator() method, which turns PayOffPut into a function object
double PayOffPut::operator() (const double& S) const {
  return std::max(m_K-S, 0.0); // Standard European put pay-off
}
