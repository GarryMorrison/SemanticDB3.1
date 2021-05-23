//
// Created by Garry Morrison on 13/09/2020.
//

#ifndef SDB3_1_SIGMOIDLIBRARY_H
#define SDB3_1_SIGMOIDLIBRARY_H

#include <vector>
#include <memory>
#include "../CompoundConstant/CompoundConstant.h"

double clean(const double x);
double threshold_filter(const double x, const double t);
double op_threshold_filter(const double x, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
double not_threshold_filter(const double x, const double t);
double op_not_threshold_filter(const double x, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
double binary_filter(const double x);
double sigmoid_in_range(const double x, const double a, const double b);
double op_sigmoid_in_range(const double x, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);
double ReLU(const double x);
double op_invert(const double x);
double op_sigmoid_random(const double x, const std::vector<std::shared_ptr<CompoundConstant> > &parameters);  // Deprecated.

#endif //SDB3_1_SIGMOIDLIBRARY_H
