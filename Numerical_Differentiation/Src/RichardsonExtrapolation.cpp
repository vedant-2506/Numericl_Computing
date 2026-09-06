#include "../Include/RichardsonExtrapolation.hpp"

#include <cmath>

// Evaluates Richardson Extrapolation across all benchmark test functions at evaluation point x.
MethodResult RichardsonExtrapolation::processRichardsonExtrapolation(double x) const
{
    return buildMethodResult(
        "Richardson Extrapolation",
        "Output/RichardsonExtrapolation.txt",
        "RichardsonExtrapolation",
        x,
        [this](const TestFunctionSpec &spec, double evaluationPoint, double h) {
            DifferentiationSample sample{};
            sample.h = h;

            // Uses extended precision internally to reduce round-off during Richardson extrapolation.
            const long double x_ld = static_cast<long double>(evaluationPoint);
            const long double h_ld = static_cast<long double>(h);
            const long double h_half_ld = h_ld / 2.0L;

            // D(h) calculated using central difference at step size h in long double.
            const long double d_h_ld = (spec.functionLong(x_ld + h_ld) - spec.functionLong(x_ld - h_ld)) / (2.0L * h_ld);

            // D(h/2) calculated using central difference at step size h/2 in long double.
            const long double d_h_half_ld = (spec.functionLong(x_ld + h_half_ld) - spec.functionLong(x_ld - h_half_ld)) / (2.0L * h_half_ld);

            // Richardson Extrapolation formula: R(h) = (4 * D(h/2) - D(h)) / 3.
            const long double r_h_ld = (4.0L * d_h_half_ld - d_h_ld) / 3.0L;

            sample.functionAtPlus = static_cast<double>(d_h_ld);        // Stores D(h) for output table formatting
            sample.functionAtMinus = static_cast<double>(d_h_half_ld);   // Stores D(h/2) for output table formatting
            sample.numericalDerivative = static_cast<double>(r_h_ld);    // R(h)
            return sample;
        });
}
