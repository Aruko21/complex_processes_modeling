#include "bodyFallMathModel.hpp"
#include "config.hpp"

#include <cmath>

BodyFallParams::BodyFallParams(ConfigurationSingleton &configuration) {
    m = configuration.getVariableDouble("MASS");
    alpha = configuration.getVariableDouble("ALPHA");
    timeStart = configuration.getVariableDouble("TIME_START");
    timeEnd = configuration.getVariableDouble("TIME_END");
    step = configuration.getVariableDouble("STEP");
}

BodyFallParams BodyFallMathModel::getParams() {
    return this->params;
}

void BodyFallMathModel::setStep(double step) {
    this->params.step = step;
}

double BodyFallMathModel::getAnalyticalSolutionPerTime(double time) {
    return this->params.m * BodyFallParams::G / this->params.alpha *
          (1 - exp(-this->params.alpha * time / this->params.m));
}

double BodyFallMathModel::getDiffSolutionPerTime(double vPrev) {
    return BodyFallParams::G - this->params.alpha * vPrev / this->params.m;
}
