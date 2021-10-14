#ifndef LAB1_INCLUDE_LIB_BODYFALLMATHMODEL_H
#define LAB1_INCLUDE_LIB_BODYFALLMATHMODEL_H

#include <map>
#include "config.hpp"

struct BodyFallParams {
    static constexpr double G = 9.80665;

    double m = 10.;
    double alpha = 1.;

    double timeStart = 0.0;
    double timeEnd = 10.0;
    double step = 1.0;

    BodyFallParams(ConfigurationSingleton &configuration);

    explicit BodyFallParams(double _step) : step(_step) {}

    BodyFallParams(double _m, double _alpha, double _step, double _timeEnd, double _timeStart = 0.)
        : m(_m), alpha(_alpha), step(_step), timeEnd(_timeEnd), timeStart(_timeStart) {};
};

class BodyFallMathModel {
private:
    BodyFallParams params;

public:
    explicit BodyFallMathModel(BodyFallParams _params) : params(_params) {};

    BodyFallParams getParams();
    void setStep(double step);

    double getAnalyticalSolutionPerTime(double time);
    double getDiffSolutionPerTime(double vPrev);
};


#endif //LAB1_INCLUDE_LIB_BODYFALLMATHMODEL_H
