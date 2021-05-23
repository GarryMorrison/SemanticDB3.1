//
// Created by Garry Morrison on 13/09/2020.
//

#include "SigmoidLibrary.h"
#include "../CompoundConstant/ConstantFloat.h"
#include "../Function/misc.h"
#include <random>
#include <chrono>


double clean(const double x) {
    if (x <= 0) { return 0.0; }
    return 1.0;
}

double threshold_filter(const double x, const double t) {
    if (x < t) { return 0.0;}
    return x;
}

double op_threshold_filter(const double x, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return 0.0; }
    // double t = std::dynamic_pointer_cast<ConstantFloat>(parameters[0])->get_float(); // What if the parameter was a ConstantInteger?
    double t = parameters[0]->get_float();
    if (x < t) { return 0.0; }
    return x;
}

double not_threshold_filter(const double x, const double t) {
    if (x <= t ) { return x; }
    return 0.0;
}

double op_not_threshold_filter(const double x, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.empty()) { return x; }  // Is this what we want to return in this case? Maybe 0.0 instead?
    double t = parameters[0]->get_float();
    if (x <= t) { return x; }
    return 0.0;
}

double binary_filter(const double x) {
    if (x <= 0.96) { return 0.0; }
    return 1.0;
}

double sigmoid_in_range(const double x, const double a, const double b) {
    if (a <= x && x <= b) { return x; }
    return 0.0;
}

double op_sigmoid_in_range(const double x, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.size() < 2) { return x; }  // Is this what we want to return in this case? Maybe 0.0 instead?
    double a = parameters[0]->get_float();
    double b = parameters[1]->get_float();
    if (a <= x && x <= b) { return x; }
    return 0.0;
}

double ReLU(const double x) {
    if (x <= 0) { return 0.0; }
    return x;
}

double op_invert(const double x) {
    if (double_eq(x, 0.0)) { return 0.0; }
    return 1 / x;
}

double op_random(const double x, const std::vector<std::shared_ptr<CompoundConstant> > &parameters) {
    if (parameters.size() < 2) { return x; }
    double mu = parameters[0]->get_float();
    double sigma = parameters[1]->get_float();
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();  // A little bit heavy weight to do all this as a sigmoid,
    std::default_random_engine generator(seed);                                   // Since we re-invoke for every ket in the input sequence.
    std::normal_distribution<double> distribution(mu, sigma);                     // It would be faster to implement it as a compound sequence function.
    return x * distribution(generator);
}
