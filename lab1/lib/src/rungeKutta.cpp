#include "rungeKutta.hpp"


void rungeKuttaSolution(BodyFallMathModel &solution, std::map<double, double>& resultRungeKutta) {
    BodyFallParams params = solution.getParams();

    double timestamp = params.timeStart;
    double vCurrent = 0.0;
    double vPrev = 0.0;

    resultRungeKutta.insert(std::pair<double, double>(timestamp, vCurrent));

    for (int i = 1; i <= (int) (params.timeEnd / params.step); i++) {
        double k1 = params.step * (BodyFallParams::G - params.alpha * (vPrev) / params.m);
        double k2 = params.step * (BodyFallParams::G - params.alpha * (vPrev + 1 / 2 * k1) / params.m);
        double k3 = params.step * (BodyFallParams::G - params.alpha * (vPrev + 1 / 2 * k2) / params.m);
        double k4 = params.step * (BodyFallParams::G - params.alpha * (vPrev + k3) / params.m);

        vCurrent = vPrev + (k1 + 2. * k2 + 2. * k3 + k4) / 6;
        vPrev = vCurrent;
        timestamp += params.step;
        resultRungeKutta.insert(std::pair<double, double>(timestamp, vCurrent));
    }
}

