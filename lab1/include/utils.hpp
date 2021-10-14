#ifndef LAB1_INCLUDE_LIB_UTILS_H
#define LAB1_INCLUDE_LIB_UTILS_H

#include "bodyFallMathModel.hpp"
#include "config.hpp"

static constexpr char GNUPLOT_FILENAME[] = "gnuplot.gp";

using SolveFunction = void (*) (BodyFallMathModel, std::map<double, double>&);

SolveFunction getFuncFromLib(const std::string& libPath, const std::string& funcName);
void solveAnalytic(BodyFallMathModel solution, std::map<double, double>& result);
void writeResult(const std::string& filename, const std::map<double, double>& result);
void plotSolutions(ConfigurationSingleton &configuration, std::string analyticalFile, std::string eulerFile, std::string rungeKuttaFile);
double findMaxStep(ConfigurationSingleton &configuration, SolveFunction solveAnalytic, SolveFunction solveFunction);
double findNormL2(std::map<double, double>& analyticResult, std::map<double, double>& result);
void writeMaxSteps(double maxStepEuler, double maxStepRungeKutta);
std::pair<double, double> findStableVelocity(ConfigurationSingleton &configuration);
void getIntervalParams(
        ConfigurationSingleton &configuration,
        BodyFallParams &leftParams,
        BodyFallParams &rightParams,
        const std::string &param,
        const std::string &mode
);
void solveInterval(
        ConfigurationSingleton& configuration,
        SolveFunction solver,
        std::map<double, double> &resultLeft,
        std::map<double, double> &resultRight
);
void plotIntervalSolution(ConfigurationSingleton &configuration, const std::string &leftResultFile, const std::string &rightResultFile);
std::string getStringFromDouble(double variable);

#endif //LAB1_INCLUDE_LIB_UTILS_H
