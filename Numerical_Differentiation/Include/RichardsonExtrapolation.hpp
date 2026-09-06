#ifndef RICHARDSON_EXTRAPOLATION_HPP
#define RICHARDSON_EXTRAPOLATION_HPP

#include "NumericalDifferentiation.hpp"

// Implements Richardson Extrapolation using central difference at h and h/2 to cancel O(h^2) truncation error.
class RichardsonExtrapolation : public NumericalDifferentiation
{
public:
    RichardsonExtrapolation() = default;

    // Evaluates Richardson Extrapolation across all benchmark functions at evaluation point x.
    MethodResult processRichardsonExtrapolation(double x = 1.0) const;
};

#endif
