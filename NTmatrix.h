/*****************************************************************************
 * File: NTmatrix.h
 * Author: Nathan Toner
 * Last modified: November 30, 2011
 * 
 * NTmatrix objectName();  Null object, defines n = m = 1, and
 *                         mat = trans = vect = 0
 * NTmatrix objectName( n, m );    Defines a matrix of zeros with n rows and m
 *                                 columns
 * NTmatrix objectName( n, m, vect );  Defines a matrix with values passed by
 *                                     vector in column-major order with n
 *                                     rows and m columns
 * 
 * Header file for NTmatrix class. The objects of this class include members
 * defining rows and columns (n and m), the values of the matrix in matrix
 * form (mat), and the values of the matrix in a vector in column-major order
 * (vect). Operations defined are +, -, *, = and != (transposition operator).
 * Also, the member function zero() zeros out the matrix when called.
 * 
 * Note: Matrices must be passed to this class in column-major order, i.e. as
 * a vector of length n*m with the elements of column one occupying the first
 * n elements, then column 2 the next n, etc. up to column m. This is done for
 * easier compatibility with cBlas and Lapack functions.
 * 
 * Ex: vector[ 2 * 2 ] = {1, 3, 2, 4} is equivalent to
 * matrix[2][2] = {{1, 2}, {3, 4}}
 * 
 * Use the first one (vector).
*****************************************************************************/ 

#ifndef __NTmatrix_h__
#define __NTmatrix_h__

class NTmatrix {
public:
    // Define data elements (number of rows/columns, elements of the matrix)
    int n;          // number of rows in the matrix
    int m;          // number of columns in the matrix
    double* vect;   // values stored in column-major order, vector form
    double** mat;   // values in the matrix

    // Define constructors and destructor
    NTmatrix();
    NTmatrix(int, int);
    NTmatrix(int, int, char);
    NTmatrix(int, int, double*);
    NTmatrix(int, int, char, double*);
    ~NTmatrix();

    // Define operations between matrices
    NTmatrix operator+(NTmatrix&);
    NTmatrix operator-(NTmatrix&);
    NTmatrix operator*(NTmatrix&);
    NTmatrix operator*(double);
    NTmatrix operator*(int);
    NTmatrix& operator=(NTmatrix);
    NTmatrix& operator!=(NTmatrix&); // transpose assignment operator
    
    NTmatrix inverse();
    // Define other functions between matrices
    int allocate();         // allocates memory for the matrix
    int assign(double *);   // function to assign values to 'mat' and 'trans'
    int assign(double **);  // function to assign values to 'mat' and 'trans'
                            // from double** (still doesn't work!)
    int diag(double *);     // assign values as diagonal entries
    int zero();             // zeros out the values in a NTmatrix object
    double trace();         // calculates the trace of a square matrix
    
    int printMat();         // print the values of 'mat'
    int printTrans();       // print the values of 'trans'
    int printVect();        // print the values of 'vect' in column-major
                            // order

    // Would like to add an invert function in the near future, but not sure
    // how to do this yet. Maybe include some calls to cBlas or Lapack
    // functions for inversion and diagonalization?
};

#endif

