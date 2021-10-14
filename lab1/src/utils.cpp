#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <sstream>
#include "utils.hpp"
#include "config.hpp"


SolveFunction getFuncFromLib(const std::string& libPath, const std::string& funcName) {
    char *error;
    HMODULE dll = LoadLibrary(libPath.c_str());

    if (!dll) {
        std::cerr << "Ошибка при подключении библиотеки " << libPath << std::endl;
        throw std::runtime_error("dll lib open error");
    }

    auto solveFunction = reinterpret_cast<SolveFunction>(
            reinterpret_cast<uintptr_t>(GetProcAddress(dll, funcName.c_str()))
    );

    if (solveFunction == nullptr) {
        FreeLibrary(dll);
        std::cerr << "Error while loading " << funcName << " function from dll library" << std::endl;
        throw std::runtime_error("dll func get error");
    }

    return solveFunction;
}

void solveAnalytic(BodyFallMathModel solution, std::map<double, double>& result) {
    result.clear();

    BodyFallParams params = solution.getParams();

    double timestamp = params.timeStart;

    for (int i = 0; i <= (int) (params.timeEnd / params.step); ++i) {
        double tmpV = solution.getAnalyticalSolutionPerTime(timestamp);

        result.insert(std::pair<double, double>(timestamp, tmpV));
        timestamp += params.step;
    }
}

void getIntervalParams(ConfigurationSingleton &configuration, BodyFallParams &leftParams, BodyFallParams &rightParams,
                       const std::string &param, const std::string &mode = "interval") {
    if (mode == "interval") {
        if (param == "mass") {
            leftParams.m = configuration.getVariableDouble("MASS_LEFT");
            rightParams.m = configuration.getVariableDouble("MASS_RIGHT");
        } else if (param == "alpha") {
            leftParams.alpha = configuration.getVariableDouble("ALPHA_LEFT");
            rightParams.alpha = configuration.getVariableDouble("ALPHA_RIGHT");
        }
    } else if (mode == "dispersion") {
        double mathExp = 0.0;
        double deviation = 0.0;

        if (param == "mass") {
            mathExp = configuration.getVariableDouble("MASS_MATH_EXPECTED");
            deviation = std::sqrt(configuration.getVariableDouble("MASS_DISPERSION"));
            leftParams.m = mathExp - deviation;
            rightParams.m = mathExp + deviation;
        } else if (param == "alpha") {
            mathExp = configuration.getVariableDouble("ALPHA_MATH_EXPECTED");
            deviation = std::sqrt(configuration.getVariableDouble("ALPHA_DISPERSION"));
            leftParams.alpha = mathExp - deviation;
            rightParams.alpha = mathExp + deviation;
        }
    } else {
        throw std::runtime_error(std::string("unknown mode: ") + mode);
    }
}

void solveInterval(ConfigurationSingleton& configuration, SolveFunction solver, std::map<double, double> &resultLeft,
                   std::map<double, double> &resultRight) {
    BodyFallParams leftParams(configuration);
    BodyFallParams rightParams(configuration);

    std::string intervalParam = configuration.getVariable("INTERVAL_PARAM");
    std::string intervalMode = configuration.getVariable("INTERVAL_MODE");

    getIntervalParams(configuration, leftParams, rightParams, intervalParam, intervalMode);

    solver(BodyFallMathModel(leftParams), resultLeft);
    solver(BodyFallMathModel(rightParams), resultRight);
}

void writeResult(const std::string& filename, const std::map<double, double>& result) {
    std::ofstream file(filename, std::ofstream::out | std::ofstream::trunc);

    file << "t\tV(t)" << std::endl;
    for (auto& it : result) {
        file << it.first << '\t' << it.second << std::endl;
    }
}

void plotSolutions(ConfigurationSingleton &configuration,
                   std::string analyticalFile, std::string eulerFile, std::string rungeKuttaFile) {
    std::string gnuscriptFile = configuration.getVariable("GNUPLOT_FILENAME");

    std::ofstream gnuscript(gnuscriptFile);

    gnuscript << "set term wxt title 'V(t) plot'" << std::endl;
    gnuscript << "set xlabel 'Time t'" << std::endl;
    gnuscript << "set ylabel 'Velocity V(t)'" << std::endl;
    gnuscript << "set key right bottom" << std::endl;
    gnuscript << "set grid" << std::endl;

    gnuscript << "plot '" << analyticalFile << "' using 1:2 with l lc rgb 'red' lt 1 lw 1.5 title 'Analytical Solution', \\" << std::endl;
    gnuscript << "'" << eulerFile << "' using 1:2 with l lc rgb 'green' lt 2 lw 1.5 title 'Euler Solution', \\" << std::endl;
    gnuscript << "'" << rungeKuttaFile << "' using 1:2 with l lc rgb 'blue' lt 4 lw 1.5 title 'Runge Kutta Solution'" << std::endl;
    gnuscript << "pause -1" << std::endl;

    system((std::string("gnuplot ") + gnuscriptFile).c_str());
}

std::string getStringFromDouble(double variable) {
    std::ostringstream stringStream;
    stringStream.precision(1);
    stringStream << std::fixed << variable;
    return stringStream.str();
}

void plotIntervalSolution(ConfigurationSingleton &configuration, const std::string &leftResultFile, const std::string &rightResultFile) {
    BodyFallParams leftParams(configuration);
    BodyFallParams rightParams(configuration);

    std::string intervalParam = configuration.getVariable("INTERVAL_PARAM");
    std::string intervalMode = configuration.getVariable("INTERVAL_MODE");

    getIntervalParams(configuration, leftParams, rightParams, intervalParam, intervalMode);

    std::string firstLabel(intervalParam);
    std::string secondLabel(intervalParam);

    if (intervalParam == "mass") {
        firstLabel += " = " + getStringFromDouble(leftParams.m);
        secondLabel += " = " + getStringFromDouble(rightParams.m);
    } else if (intervalParam == "alpha") {
        firstLabel += " = " + getStringFromDouble(leftParams.alpha);
        secondLabel += " = " + getStringFromDouble(rightParams.alpha);
    }

    std::string gnuscriptFile = configuration.getVariable("GNUPLOT_INTERVAL_FILENAME");
    std::ofstream gnuscript(gnuscriptFile);

    gnuscript << "set term wxt title 'V(t) plot'" << std::endl;
    gnuscript << "set xlabel 'Time t'" << std::endl;
    gnuscript << "set ylabel 'Velocity V(t)'" << std::endl;
    gnuscript << "set key right bottom" << std::endl;
    gnuscript << "set grid" << std::endl;

    gnuscript << "plot '" << leftResultFile << "' using 1:2 with l lc rgb 'red' lt 1 lw 1.5 title '" << firstLabel << "', \\" << std::endl;
    gnuscript << "'" << rightResultFile << "' using 1:2 with l lc rgb 'blue' lt 2 lw 1.5 title '" << secondLabel << "'" << std::endl;
    gnuscript << "pause -1" << std::endl;

    system((std::string("gnuplot ") + gnuscriptFile).c_str());
}

double findMaxStep(ConfigurationSingleton &configuration, SolveFunction solveAnalytic, SolveFunction solveFunction) {
    double curStep = configuration.getVariableDouble("STEP_MAX");

    double epsMax = configuration.getVariableDouble("EPS_MAX");
    size_t maxCount = configuration.getVariableInt("MAX_COUNT");

    BodyFallParams params(curStep);
    BodyFallMathModel curSolution(params);

    std::map<double, double> analyticResult;
    std::map<double, double> functionResult;

    double curL2 = 0.0;
    bool isOver = false;
    size_t counter = 0;

    // Выяснить погрешность при изначально заданном шаге
    solveAnalytic(curSolution, analyticResult);
    solveFunction(curSolution, functionResult);

    curL2 = findNormL2(analyticResult, functionResult);

    // Текущий шаг оказался слишком большим
    if (curL2 > epsMax) {
        while (curL2 > epsMax && !isOver) {
            curStep *= 0.5;
            curSolution.setStep(curStep);

            solveAnalytic(curSolution, analyticResult);
            solveFunction(curSolution, functionResult);

            curL2 = findNormL2(analyticResult, functionResult);

            ++counter;
            if (counter > maxCount) {
                isOver = true;
            }
        }
    } else {
        while (curL2 < epsMax && !isOver) {
            curStep *= 1.2;
            curSolution.setStep(curStep);

            solveAnalytic(curSolution, analyticResult);
            solveFunction(curSolution, functionResult);

            curL2 = findNormL2(analyticResult, functionResult);

            ++counter;
            if (counter > maxCount) {
                isOver = true;
            }
        }
        curStep /= 1.2;
    }

    return curStep;
}

double findNormL2(std::map<double, double>& analyticResult, std::map<double, double>& result) {
    double sum = 0.0;
    double timestamps = result.size();
    for (auto const& point : result) {
        if (point.second == 0) {
            --timestamps;
        } else {
            sum += std::pow((analyticResult[point.first] - point.second) / point.second, 2);
        }
    }

    return std::sqrt(sum / timestamps);
}

void writeMaxSteps(double maxStepEuler, double maxStepRungeKutta) {
    std::ofstream file("max_steps.csv", std::ofstream::out | std::ofstream::trunc);

    if (!file.is_open()) {
        throw std::runtime_error("couldn't open file for writing");
    }

    file << "euler" << '\t' << "runge_kutta" << std::endl;
    file << maxStepEuler << '\t' << maxStepRungeKutta << std::endl << std::flush;
    file.close();
}

std::pair<double, double> findStableVelocity(ConfigurationSingleton &configuration) {
    double epsDiff = configuration.getVariableDouble("STABLE_VELOCITY_EPS");
    BodyFallParams params(configuration);
    BodyFallMathModel curSolution(params);

    double timestamp = params.timeStart;
    double previousVelocity = curSolution.getAnalyticalSolutionPerTime(timestamp);
    timestamp += params.step;
    double curVelocity = curSolution.getAnalyticalSolutionPerTime(timestamp);

    while (std::fabs(curVelocity - previousVelocity) > epsDiff) {
        std::cout << "check: " << std::fabs(curVelocity - previousVelocity) << std::endl;
        previousVelocity = curVelocity;
        timestamp += params.step;
        curVelocity = curSolution.getAnalyticalSolutionPerTime(timestamp);
    }

    return std::pair<double, double>(timestamp - params.step, previousVelocity);
}