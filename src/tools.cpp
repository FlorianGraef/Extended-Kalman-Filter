#include <iostream>
#include "tools.h"

#define SMALL_NO 0.0001
#define VERY_SMALL_NO 0.0000001

using namespace std;
using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
    VectorXd rmse(4);
    rmse << 0, 0, 0, 0;
    // Check the validity of the following inputs:
    // The estimation vector size should not be zero
    if (estimations.size() == 0) {
        cout << "Input is empty" << endl;
        return rmse;
    }
    // The estimation vector size should equal ground truth vector size
    if (estimations.size() != ground_truth.size()) {
        cout << "Invalid estimation or ground_truth. Data should have the same size" << endl;
        return rmse;
    }
    // Accumulate squared residuals
    for (unsigned int i = 0; i < estimations.size(); ++i) {
        VectorXd residual = estimations[i] - ground_truth[i];
        // Coefficient-wise multiplication
        residual = residual.array() * residual.array();
        rmse += residual;
    }

    // Calculate the mean
    rmse = rmse / estimations.size();
    rmse = rmse.array().sqrt();
    return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd &x_state) {
    // Code from lectures quizes
    float px = x_state(0);
    float py = x_state(1);
    float vx = x_state(2);
    float vy = x_state(3);
    MatrixXd Hj(3, 4);

    if (fabs(px) < SMALL_NO and fabs(py) < SMALL_NO) {
        px = SMALL_NO;
        py = SMALL_NO;
    }

    float px_py = px * px + py * py;
    // avoid division by zero
    if (fabs(px_py) < VERY_SMALL_NO) {
        px_py = VERY_SMALL_NO;
    }
    float sqrt_px_py = sqrt(px_py);
    float px_py_sqrt_px_py = (px_py * sqrt_px_py);
    // Compute the Jacobian
    Hj << (px / sqrt_px_py), (py / sqrt_px_py), 0, 0,
            -(py / px_py), (px / px_py), 0, 0,
            py * (vx * py - vy * px) / px_py_sqrt_px_py, px * (px * vy - py * vx) / px_py_sqrt_px_py, px / sqrt_px_py, py / sqrt_px_py;
    return Hj;
}