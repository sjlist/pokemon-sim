//
// Created by slist on 7/24/19.
//

#include "probability-helpers.h"
#include <cmath>

using namespace std;

int get_num_samples(int N)
{
    float samples = log(2 * N / DELTA)/(2 * pow(THRESHOLD, 2));
    return ceil(samples);
}