#include <iostream>
#include <windows.h>
#include <map>
#include <string>
#include "utils.hpp"
#include "config.hpp"


int main(int argc, const char** argv) {
    ConfigurationSingleton& configuration = ConfigurationSingleton::getInstance();

    BodyFallParams params(configuration);
    BodyFallMathModel solution(params);

    std::map<double, double> analyticResult;
    std::map<double, double> eulerResult;
    std::map<double, double> rungeKuttaResult;

    auto solveEuler = getFuncFromLib("lib/libeuler.dll", "eulerSolution");
    auto solveRungeKutta = getFuncFromLib("lib/librungekutta.dll", "rungeKuttaSolution");

    solveAnalytic(solution, analyticResult);
    solveEuler(solution, eulerResult);
    solveRungeKutta(solution, rungeKuttaResult);

    // Запись в файл результатов расчетов
    writeResult("analytical_solution.csv", analyticResult);
    writeResult("euler_solution.csv", eulerResult);
    writeResult("rungeKutta_solution.csv", rungeKuttaResult);

    std::map<double, double> leftResult;
    std::map<double, double> rightResult;

    solveInterval(configuration, solveAnalytic, leftResult, rightResult);
    writeResult("interval_left_solution.csv", leftResult);
    writeResult("interval_right_solution.csv", rightResult);
    // Поиск максимального шага
    double maxStepEuler = findMaxStep(configuration, solveAnalytic, solveEuler);
    double maxStepHeun = findMaxStep(configuration, solveAnalytic, solveRungeKutta);

    // Запись в файл максимального шага
    writeMaxSteps(maxStepEuler, maxStepHeun);
    // Построение графиков с результатами расчетов
    plotSolutions(configuration, "analytical_solution.csv", "euler_solution.csv", "rungeKutta_solution.csv");

//    plotIntervalSolution(configuration, "interval_left_solution.csv", "interval_right_solution.csv");

//    std::pair<double, double> stableVelocity = findStableVelocity(configuration);
//    std::cout << "Stable velocity found at timestamp: " << stableVelocity.first << "s. with V: " << stableVelocity.second << std::endl;

    return 0;
}
