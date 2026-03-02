#ifndef ADVANCED_OPERATION_HPP
#define ADVANCED_OPERATION_HPP

#include "BasicOperation.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

class AdvancedOperation {
public:
    int rows, cols;
    vector<vector<double>> mat;

    AdvancedOperation();                             // default constructor
    AdvancedOperation(int r, int c);                 // parameterized constructor
    AdvancedOperation(const AdvancedOperation &m);   // copy constructor
 
    void readFromFile(ifstream &fin);                // read matrix from file
    void displayToFile(ofstream &fout) const;        // display matrix to file

    AdvancedOperation add(const AdvancedOperation &other) const;     // addition
    AdvancedOperation subtract(const AdvancedOperation &other) const;    // subtraction

    static void generateAugmentedAdvancedOperationFile(const string &leftFile,
                                                       const string &rightFile,
                                                       const string &outputFile);   // Generate augmented matrix 

    // Legacy/static interface expected by main.cpp
    static void generateAugmentedMatrixFile(const string &leftFile,
                                            const string &rightFile,
                                            const string &outputFile);

    static void gaussianEliminationWithPivoting(BasicOperation &matrix,
                                                ofstream &matrixOut,
                                                ofstream &vectorOut);

    void gaussianEliminationWithPivoting(ofstream &AdvancedOperationOut,
                                        ofstream &vectorOut);
};

#endif // ADVANCED_OPERATION_HPP
