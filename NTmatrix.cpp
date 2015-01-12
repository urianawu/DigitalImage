/*****************************************************************************
 * File: NTmatrix.cpp
 * Author: Nathan Toner
 * Last modified: January 26, 2012
 *
 * Note: Matrices must be passed to this class in column-major order, i.e. as
 * a vector of length n*m with the elements of column one occupying the first
 * n elements, then column 2 the next n, etc. up to column m. This is done for
 * easier compatability with cblas and lapack functions.
 *
 * Ex: 
 * vector[ 2 * 2 ] = {1, 3, 2, 4} is equivalent to 
 * matrix[2][2] = {{1, 2}, {3, 4}}
 *
 * Use the first one (vector).
 *
 * On June 8, 2011: removed the previous way of keeping track of transpose.
 * Now use operator "!=" to assign one object of class NTmatrix to the
 * transpose of another object of class NTmatrix. In this way, you can
 * multiply an object by its transpose by first assigning the transpose to a
 * new object, then multiplying the two together. Tested and it seems to work.
 * 
 * On November 30, 2011: added method for constructing diagonal matrices and
 * identity matrices more easily, as well as functions for zeroing out
 * non-zero matrices and for determining the trace of a matrix.
*****************************************************************************/

#include <stdlib.h>	// needed for 'exit' function if nothing else
#include <iostream>	// needed in 'print' member function
#include <assert.h>	// memory allocation error checking
#include "NTmatrix.h"	// defines the NTmatrix class and the prototypes for
                        // the member functions

using namespace std;

/*****************************************************************************
 * Develop the constructors and destructors for the class NTmatrix
*****************************************************************************/
NTmatrix::NTmatrix() {
    // Allocate memory for each variable, and terminate if not allocated
    // successfully
    mat = new double*;
    mat[0] = new double;
    vect = new double;
    assert(mat != 0);
    assert(vect != 0);

    // Assign values to the values pointed by our pointers
    n = 1;
    m = 1;
    **mat = 0;
    *vect = 0;
}

NTmatrix::NTmatrix(int rows, int cols) {
    // Assign values to n & m, then call allocate() to allocate memory to our
    // matrices.
    n = rows;
    m = cols;
    allocate();

    // Put some values in the matrix initially, which can be reassigned later
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            mat[i][j] = 0;
            vect[ j * n + i ] = 0;
        }
    }
}

NTmatrix::NTmatrix(int rows, int cols, char flag) {
    // Assign values to n & m, then call allocate() to allocate memory to our
    // matrices.
    n = rows;
    m = cols;
    allocate();

    // Based on the flag value, initialize the matrix to either zeros are the
    // identity.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            mat[i][j] = 0;
            vect[j*n + i] = 0;
        }
    }

    // If flag is "I" or "D" and the matrix is square, initialize as an
    // identity matrix.
    if ((flag == 'I' || flag == 'D') && n == m) {
        for (int i = 0; i < n; i++) {
            mat[i][i] = 1;
            vect[i*n + i] = 1;
        }
    }
    else if ((flag == 'I' || flag == 'D') && n != m) {
        cout << "\n\nMatrix dimensions incompatible, cannot initialize non-square identity matrix!\n\n";
        exit(1);
    }
}

NTmatrix::NTmatrix(int rows, int cols, double* values) {
    // Assign values to n & m, then call allocate() to allocate memory to our
    // matrices.
    n = rows;
    m = cols;
    allocate();

    // Assign 'values' to 'mat' and 'vect'
    assign(values);
}

NTmatrix::NTmatrix(int rows, int cols, char flag, double* values) {
    // Assign values to n & m, then call allocate() to allocate memory to our
    // matrices.
    n = rows;
    m = cols;
    allocate();

    // If 'flag' is 1 and the matrix is square, assign 'values' as diagonal
    // entries to 'mat' and 'vect'. If flag != 1, just assign as above.
    if (flag == 'D' && n == m)
        diag(values);
    else if (flag == 'D' && n != m) {
        cout << "\n\nMatrix not square. Diagonal matrices must be square!\n\n";
        exit(1);
    }
    else if (flag != 'D')
        assign(values);
}

NTmatrix::~NTmatrix() {
    // Use 'delete' for memory allocated with 'new'
    for (int i = 0; i < n; i++)
        delete [] mat[i];
    delete [] mat;
    delete [] vect;

    /*  I think we need to do the above regardless of the dimension of the
     *  object to ensure we free up all of the memory that was set aside.
    if (n > 1 && m > 1) {
        for (int i = 0; i < n; i++)
            delete [] mat[i];
        delete [] mat;
        delete [] vect;
    } else if ((n > 1 && m == 1) || (n == 1 && m > 1)) {
        delete [] mat;
        delete [] vect;
    } else if (n == 1 && m == 1) {
        delete mat;
        delete vect;
    }
    */
}

int NTmatrix::allocate() {
    // Dynamically allocate memory for an n x m matrix 'mat' and an
    // n*m-element vector 'vect'. No values to assign in this constructor.
    mat = new double*[n];
    for (int i = 0; i < n; i++)
        mat[i] = new double[m];

    vect = new double[ n * m ];

    // Kill the program if we run out of memory (things are not allocated
    // properly). Note that we do not have to display something to the screen
    // here because the program will terminate if this happens. Honestly I'm
    // not sure how to display something to the screen and still use the
    // 'assert' commands, and I kinda like them :)
    assert(mat != 0);
    assert(vect != 0);

    return 1;
}

int NTmatrix::assign(double* values) {
    // Assign values to 'mat' and 'vect' from 'values' given in column-major
    // order
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            mat[i][j] = values[ j * n + i ];
            if ( &vect != &values )
                vect[ j * n + i ] = values[ j * n + i ];
        }
    }

    return 1;
}

int NTmatrix::diag(double* values) {
    // Assign values from 'values' to the diagonal entries of 'mat' and
    // 'vect'. 'values' is given in column-major order.
    if (n == m) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i == j)
                    mat[i][i] = values[i];
                else if (i != j)
                    mat[i][j] = 0;
                if (&vect != &values)
                    vect[j*n + i] = mat[i][j];
            }
        }
    }
    else {
        cout << "\n\nMatrix not square. Diagonal matrices must be square!\n\n";
        exit(1);
    }

    return 1;
}

int NTmatrix::assign(double** values) {
    // Assign values to 'mat', 'trans', and 'vect' from 'values' given as a
    // matrix
    // Note, this does not seem to be working still. Any help would be
    // appreciated.
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            if ( &mat != &values )
                mat[i][j] = values[i][j];
            vect[ j * n + i ] = values[i][j];
        }
    }

    return 1;
}

/*****************************************************************************
 * Define member functions
*****************************************************************************/
int NTmatrix::zero() {
    // Zero out the values in a NTmatrix object
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++)
            vect[j*n + i] = 0;
    }
    assign(vect);

    return 1;
}

double NTmatrix::trace() {
    // Return the trace of the matrix (if square) as a double
    double temp = 0.0;
    if (n == m) {
        for (int i = 0; i < n; i++)
            temp += mat[i][i];
    }
    else if (n != m) {
        cout << "\n\nMatrix not square. Trace is not defined on non-square matrices!\n\n";
        exit(1);
    }
    
    return temp;
}

int NTmatrix::printMat() {
    // Print the matrix to the screen (as a matrix, not a vector)
    cout << "mat =\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (j < m - 1)
                cout << mat[i][j] << "\t";
            else
                cout << mat[i][j] << "\n";
        }
    }
    cout << "\n\n";

    return 1;
}

int NTmatrix::printTrans() {
    // Print the transpose to the screen (as a matrix, not a vector)
    cout << "trans =\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (j < n - 1)
                cout << mat[j][i] << "\t";
            else
                cout << mat[j][i] << "\n";
        }
    }
    cout << "\n\n";

    return 1;
}

int NTmatrix::printVect() {
    // Print the vector to the screen in column-major order
    cout << "vect = (column-major order)\n";
    for (int i = 0; i < (n * m); i++)
        cout << vect[i] << "   ";
    cout << "\n\n";

    return 1;
}
NTmatrix NTmatrix::inverse() {
    double det = mat[0][0] * ( mat[1][1] *  mat[2][2] -  mat[2][1] *  mat[1][2]) -
                 mat[0][1] * ( mat[1][0] *  mat[2][2] -  mat[1][2] *  mat[2][0]) +
                 mat[0][2] * ( mat[1][0] *  mat[2][1] -  mat[1][1] *  mat[2][0]);
    double invdet = 1 / det;
    
    NTmatrix minv(n,m); // inverse of matrix
    minv.mat[0][0] = (double)( mat[1][1] *  mat[2][2] -  mat[2][1] *  mat[1][2]) * invdet;
    minv.mat[0][1] = (double)( mat[0][2] *  mat[2][1] -  mat[0][1] *  mat[2][2]) * invdet;
    minv.mat[0][2] = (double)( mat[0][1] *  mat[1][2] -  mat[0][2] *  mat[1][1]) * invdet;
    minv.mat[1][0] = ( mat[1][2] *  mat[2][0] -  mat[1][0] *  mat[2][2]) * invdet;
    minv.mat[1][1] = ( mat[0][0] *  mat[2][2] -  mat[0][2] *  mat[2][0]) * invdet;
    minv.mat[1][2] = ( mat[1][0] *  mat[0][2] -  mat[0][0] *  mat[1][2]) * invdet;
    minv.mat[2][0] = ( mat[1][0] *  mat[2][1] -  mat[2][0] *  mat[1][1]) * invdet;
    minv.mat[2][1] = ( mat[2][0] *  mat[0][1] -  mat[0][0] *  mat[2][1]) * invdet;
    minv.mat[2][2] = ( mat[0][0] *  mat[1][1] -  mat[1][0] *  mat[0][1]) * invdet;
    minv.assign(minv.mat);
    
    return minv;
}
/*****************************************************************************
 * Define operators between objects of type NTmatrix
*****************************************************************************/
NTmatrix NTmatrix::operator+(NTmatrix& param) {
    // Define the addition operator between two objects of this class

    // If the matrices are the same size][and dimension >= 1 x 1,
    // add each element separately.
    NTmatrix temp(n, m, vect);
    if (n == param.n && m == param.m && n >= 1) {
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n; i++) {
                //temp.mat[i][j] += param.mat[i][j];
                temp.vect[ j * n + i ] += param.vect[ j * n + i ];
            }
        }
    }
    // If C = A + B where A is an n x m matrix and B is an n x 1 vector,
    // and m >= 1, add the vector B to each column of A.
    else if (n == param.n && param.m == 1 && m >= 1) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                //temp.mat[i][j] += param.mat[i][0];
                temp.vect[ j * n + i ] += param.mat[i][0];
            }
        }
    }
    // If neither of the above are true, dimensions are incompatible
    // (or I have to do more coding)
    else {
        cout << "\n\nCould not add matrices! Dimensions not compatible.\n\n";
        exit(1);
    }
    temp.assign(temp.vect);

    return temp;
}

NTmatrix NTmatrix::operator-(NTmatrix& param) {
    // Define the subtraction operator between two objects of this class

    // If the matrices are the same size, and dimension >= 1 x 1,
    // subtract each element separately.
    NTmatrix temp(n, m, vect);
    if (n == param.n && m == param.m && n >= 1) {
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n; i++) {
                //temp.mat[i][j] -= param.mat[i][j];
                temp.vect[ j * n + i ] -= param.vect[ j * n + i ];
            }
        }
    }
    // If C = A + B where A is an n x m matrix and B is an n x 1 vector,
    // and m >= 1, subtract the vector B to each column of A.
    else if (n == param.n && param.m == 1 && m >= 1) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                //temp.mat[i][j] -= param.mat[i][0];
                temp.vect[ j * n + i ] -= param.vect[i];
            }
        }
    }
    // If neither of the above are true, dimensions are incompatible
    // (or I have to do more coding)
    else {
        cout << "\n\nCould not subtract matrices! Dimensions not compatible.\n\n";
        exit(1);
    }
    temp.assign(temp.vect);

    return temp;
}

NTmatrix NTmatrix::operator*(NTmatrix& param) {
    // Define the multiplication operator between two objects of this class
    NTmatrix temp(n, param.m);
    if (m == param.n) {
        // Find the value of temp.mat through matrix multiplication
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < param.m; j++) {
                for (int k = 0; k < param.n; k++)
                    temp.mat[i][j] += mat[i][k] * param.mat[k][j];

                temp.vect[ j * n + i ] = temp.mat[i][j];
            }
        }
    }
    else {
        cout << "\n\nCould not multiply matrices! Dimensions not compatible.\n\n";
        exit(1);
    }

    return temp;
}

NTmatrix NTmatrix::operator*(double param) {
    // Define multiplication by a constant (double)
    NTmatrix temp(n, m);
    if (param != 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                temp.mat[i][j] = param * mat[i][j];
                temp.vect[j*n + i] = temp.mat[i][j];
            }
        }
    }

    return temp;
}

NTmatrix NTmatrix::operator*(int param) {
    // Define multiplication by a constant (int)
    NTmatrix temp(n, m);
    if (param != 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                temp.mat[i][j] = param * mat[i][j];
                temp.vect[j*n + i] = temp.mat[i][j];
            }
        }
    }

    return temp;
}

NTmatrix& NTmatrix::operator=(NTmatrix param) {
    // Assign the value on the right side to the left side.
    // Note: Can only be used when the right side is an expression, for
    // copying an object, use "object1.assign(object2.vect)"
    n = param.n;
    m = param.m;
    allocate();
    assign(param.vect);

    return *this;
}

NTmatrix& NTmatrix::operator!=(NTmatrix& param) {
    // Define the transpose operator between two object of this class
    m = param.n;
    n = param.m;
    allocate();
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++)
            vect[j * n + i] = param.mat[j][i];
    }
    assign(vect);

    return *this;
}

