#ifndef LAB1_INCLUDE_LIB_RUNGEKUTTA_H
#define LAB1_INCLUDE_LIB_RUNGEKUTTA_H

#include <map>
#include "bodyFallMathModel.hpp"

extern "C" void rungeKuttaSolution(BodyFallMathModel &solution, std::map<double, double>& resultRungeKutta);

#endif //LAB1_INCLUDE_LIB_RUNGEKUTTA_H
