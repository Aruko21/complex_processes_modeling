#include "euler.hpp"

void eulerSolution(BodyFallMathModel &solution, std::map<double, double> &resultEuler) {
    BodyFallParams params = solution.getParams();
    double timestamp = params.timeStart;
    double vPrev = 0.0;
    double vCurrent = 0.0;

    resultEuler.insert(std::pair<double, double>(timestamp, vCurrent));

    for (int i = 1; i <= (int) (params.timeEnd / params.step); i++) {
        vCurrent = vPrev + params.step * solution.getDiffSolutionPerTime(vPrev);
        vPrev = vCurrent;
        timestamp += params.step;
        resultEuler.insert(std::pair<double, double>(timestamp, vCurrent));
    }
}
