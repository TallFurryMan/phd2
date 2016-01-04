/*
 * Copyright 2014-2015, Max Planck Society.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* Created by Stephan Wenninger <stephan.wenninger@tuebingen.mpg.de> and
 * Edgar Klenske <edgar.klenske@tuebingen.mpg.de>
 */

#include "covariance_functions.h"
#include "math_tools.h"

namespace covariance_functions
{

    /* PeriodicSquareExponential */
    PeriodicSquareExponential::PeriodicSquareExponential() :
    hyperParameters(Eigen::VectorXd::Zero(4)), extraParameters(Eigen::VectorXd::Ones(1)*std::numeric_limits<double>::max()) { }

    PeriodicSquareExponential::PeriodicSquareExponential(const Eigen::VectorXd& hyperParameters_) :
    hyperParameters(hyperParameters_), extraParameters(Eigen::VectorXd::Ones(1)*std::numeric_limits<double>::max()) { }

    MatrixStdVecPair PeriodicSquareExponential::evaluate(
        const Eigen::VectorXd& x,
        const Eigen::VectorXd& y)
    {

        double lsSE0 = exp(hyperParameters(0));
        double svSE0 = exp(2 * hyperParameters(1));
        double lsP  = exp(hyperParameters(2));
        double svP  = exp(2 * hyperParameters(3));

        double plP  = exp(extraParameters(0));

        // Work with arrays internally, convert to matrix for return value.
        // This is because all the operations act elementwise.

        // Compute Distances
        Eigen::ArrayXXd squareDistanceXY = math_tools::squareDistance(
            x.transpose(), y.transpose());
        Eigen::ArrayXXd distanceXY = squareDistanceXY.sqrt();

        // Square Exponential Kernel
        Eigen::ArrayXXd E0 = squareDistanceXY / pow(lsSE0, 2);
        Eigen::ArrayXXd K0 = svSE0 * (-0.5 * E0).exp();

        // Periodic Kernel
        Eigen::ArrayXXd P1 = (M_PI * distanceXY / plP);
        Eigen::ArrayXXd S1 = P1.sin() / lsP;
        Eigen::ArrayXXd Q1 = S1.square();
        Eigen::ArrayXXd K1 = svP * (-2 * Q1).exp();

        // Combined Kernel
        Eigen::MatrixXd K = K0 + K1;

        // Derivatives
        std::vector<Eigen::MatrixXd> derivatives(4);

        derivatives[0] = K0 * E0;
        derivatives[1] = 2 * K0;
        derivatives[2] = 4 * K1 * Q1;
        derivatives[3] = 2 * K1;

        return std::make_pair(K, derivatives);
    }

    void PeriodicSquareExponential::setParameters(const Eigen::VectorXd& params)
    {
        this->hyperParameters = params;
    }

    void PeriodicSquareExponential::setExtraParameters(const Eigen::VectorXd& params)
    {
        this->extraParameters = params;
    }

    const Eigen::VectorXd& PeriodicSquareExponential::getParameters() const
    {
        return this->hyperParameters;
    }

    const Eigen::VectorXd& PeriodicSquareExponential::getExtraParameters() const
    {
        return this->extraParameters;
    }

    int PeriodicSquareExponential::getParameterCount() const
    {
        return 4;
    }

    int PeriodicSquareExponential::getExtraParameterCount() const
    {
        return 1;
    }


    /* PeriodicSquareExponential2 */
    PeriodicSquareExponential2::PeriodicSquareExponential2() :
        hyperParameters(Eigen::VectorXd::Zero(6)), extraParameters(Eigen::VectorXd::Ones(1)*std::numeric_limits<double>::max()) { }

    PeriodicSquareExponential2::PeriodicSquareExponential2(const Eigen::VectorXd& hyperParameters_) :
        hyperParameters(hyperParameters_), extraParameters(Eigen::VectorXd::Ones(1)*std::numeric_limits<double>::max()) { }

    MatrixStdVecPair PeriodicSquareExponential2::evaluate(
        const Eigen::VectorXd& x,
        const Eigen::VectorXd& y)
    {

        double lsSE0 = exp(hyperParameters(0));
        double svSE0 = exp(2 * hyperParameters(1));
        double lsP  = exp(hyperParameters(2));
        double svP  = exp(2 * hyperParameters(3));
        double lsSE1 = exp(hyperParameters(4));
        double svSE1 = exp(2 * hyperParameters(5));

        double plP  = exp(extraParameters(0));

        // Work with arrays internally, convert to matrix for return value.
        // This is because all the operations act elementwise.

        // Compute Distances
        Eigen::ArrayXXd squareDistanceXY = math_tools::squareDistance(
                                               x.transpose(), y.transpose());
        Eigen::ArrayXXd distanceXY = squareDistanceXY.sqrt();

        // Square Exponential Kernel
        Eigen::ArrayXXd E0 = squareDistanceXY / pow(lsSE0, 2);
        Eigen::ArrayXXd K0 = svSE0 * (-0.5 * E0).exp();

        // Periodic Kernel
        Eigen::ArrayXXd P1 = (M_PI * distanceXY / plP);
        Eigen::ArrayXXd S1 = P1.sin() / lsP;
        Eigen::ArrayXXd Q1 = S1.square();
        Eigen::ArrayXXd K1 = svP * (-2 * Q1).exp();

        // Square Exponential Kernel
        Eigen::ArrayXXd E2 = squareDistanceXY / pow(lsSE1, 2);
        Eigen::ArrayXXd K2 = svSE1 * (-0.5 * E2).exp();

        // Combined Kernel
        Eigen::MatrixXd K = K0 + K1 + K2;

        // Derivatives
        std::vector<Eigen::MatrixXd> derivatives(6);

        derivatives[0] = K0 * E0;
        derivatives[1] = 2 * K0;
        derivatives[2] = 4 * K1 * Q1;
        derivatives[3] = 2 * K1;
        derivatives[4] = K2 * E2;
        derivatives[5] = 2 * K2;

        return std::make_pair(K, derivatives);
    }

    void PeriodicSquareExponential2::setParameters(const Eigen::VectorXd& params)
    {
        this->hyperParameters = params;
    }

    void PeriodicSquareExponential2::setExtraParameters(const Eigen::VectorXd& params)
    {
        this->extraParameters = params;
    }

    const Eigen::VectorXd& PeriodicSquareExponential2::getParameters() const
    {
        return this->hyperParameters;
    }

    const Eigen::VectorXd& PeriodicSquareExponential2::getExtraParameters() const
    {
        return this->extraParameters;
    }

    int PeriodicSquareExponential2::getParameterCount() const
    {
        return 6;
    }

    int PeriodicSquareExponential2::getExtraParameterCount() const
    {
        return 1;
    }

}  // namespace covariance_functions