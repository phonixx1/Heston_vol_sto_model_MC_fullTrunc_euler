#ifndef __PAY_OFF_HPP
#define __PAY_OFF_HPP

#include <algorithm> // This is needed for the std::max comparison function, used in the pay-off calculations

class PayOff {
 public:
  PayOff(); // Default (no parameter) constructor
  virtual ~PayOff() {}; // Virtual destructor
  // Overloaded () operator, turns the PayOff into an abstract function object
  virtual double operator() (const double& S) const = 0;
};

class PayOffCall : public PayOff {
 private:
  double m_K; // Strike price

 public:
  PayOffCall(const double& K);
  virtual ~PayOffCall() {};
  // Virtual function is now over-ridden (not pure-virtual anymore)
  virtual double operator() (const double& S) const;
};

class PayOffPut : public PayOff {
 private:
  double m_K; // Strike

 public:
  PayOffPut(const double& K);
  virtual ~PayOffPut() {};
  virtual double operator() (const double& S) const;
};

#endif