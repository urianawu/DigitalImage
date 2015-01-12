//
//  gaussian.cpp
//  DigitalImage
//
//  Created by Uriana on 13-10-28.
//  Copyright (c) 2013 Uriana. All rights reserved.
//

#include "gaussian.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;


double gaussian::gauss (double x, double mu, double sigma) {
    return exp( -(((x-mu)/(sigma))*((x-mu)/(sigma)))/2.0 );
}

vector< vector<double> > gaussian::produce2dGaussianKernel (int kernelRadius) {
    // get kernel matrix
    vector< vector<double> > kernel2d ( 2*kernelRadius+1,
                                       vector<double>(2*kernelRadius+1)
                                       );
    
    // determine sigma
    double sigma = kernelRadius/2.;
    
    // fill values
    double sum = 0;
    for (int row = 0; row < kernel2d.size(); row++)
        for (int col = 0; col < kernel2d[row].size(); col++) {
            kernel2d[row][col] = gauss(row, kernelRadius, sigma) *
            gauss(col, kernelRadius, sigma);
            sum += kernel2d[row][col];
        }
    
    // normalize
    for (int row = 0; row < kernel2d.size(); row++)
        for (int col = 0; col < kernel2d[row].size(); col++)
            kernel2d[row][col] /= sum;
    
    return kernel2d;
}
