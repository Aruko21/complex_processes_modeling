#ifndef LAB1_INCLUDE_LIB_EULER_H
#define LAB1_INCLUDE_LIB_EULER_H

#include <map>
#include "bodyFallMathModel.hpp"

extern "C" void eulerSolution(BodyFallMathModel &setParams, std::map<double, double>& resultEuler);

#endif //LAB1_INCLUDE_LIB_EULER_H
