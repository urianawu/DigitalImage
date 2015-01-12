//
//  gaussian.h
//  DigitalImage
//
//  Created by Uriana on 13-10-28.
//  Copyright (c) 2013 Uriana. All rights reserved.
//

#ifndef __DigitalImage__gaussian__
#define __DigitalImage__gaussian__

#include <iostream>
#include <vector>
using namespace std;
class gaussian
{
public:
    vector< vector<double> > produce2dGaussianKernel (int kernelRadius);
    double gauss (double x, double mu, double sigma);

};
#endif /* defined(__DigitalImage__gaussian__) */
