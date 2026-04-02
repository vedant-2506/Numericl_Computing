#include "../Include/Iterativemethod.hpp" // include ../Include/Iterativemethod.hpp
#include "../Include/PlotHelper.hpp" // include ../Include/PlotHelper.hpp
#include "../Include/Solutionprinter.hpp" // include ../Include/Solutionprinter.hpp
#include <fstream> // include fstream
#include <vector> // include vector
#include <cmath> // include cmath
#include <iomanip> // include iomanip
#include <sstream> // include sstream
#include <stdexcept> // include stdexcept
using namespace std;                              // use standard namespace


IterativeMethod::IterativeMethod() : LinearOperation() {}  // define function
IterativeMethod::IterativeMethod(int r, int c) : LinearOperation(r, c) {}  // define function

ostream& operator<<(ostream &out, const IterativeMethod &im)  // stream input/output operation
{
    out << "IterativeMethod(" << im.rows << "x" << im.cols << ")\n";  // check equality or comparison
    return out;                                   // return result from function
}

void loadSystem(const string &Afile, const string &bfile,  // declare and initialize object
    vector<vector<double>> &A, vector<double> &b, int &n)  // declare variable
{
    ifstream finA(Afile), finb(bfile);            // handle file stream operation
    if (!finA || !finb) throw runtime_error("File not found");  // check condition
    int cA, rb, cb;                               // declare variable
    finA >> n >> cA;                              // check equality or comparison
    finb >> rb >> cb;                             // check equality or comparison
    if (cA != n || rb != n || cb != 1) throw runtime_error("Iterative method: invalid matrix/vector size");  // check condition
    A.assign(n, vector<double>(n));               // check equality or comparison
    b.assign(n, 0.0);                             // call function or method
    for (int i = 0; i < n; i++)                   // iterate over elements
        for (int j = 0; j < n; j++) finA >> A[i][j];  // iterate over elements
    for (int i = 0; i < n; i++) finb >> b[i];     // iterate over elements
}

bool isDominantSystem(const vector<vector<double>> &A, int n)  // declare and initialize object
{
    for (int i = 0; i < n; i++)                   // iterate over elements
    {
        double diag = fabs(A[i][i]);              // declare and initialize object
        double sum  = 0.0;                        // declare and assign variable
        for (int j = 0; j < n; j++)               // iterate over elements
            if (j != i) sum += fabs(A[i][j]);     // check condition
        if (diag < sum) return false;             // check condition
    }
    return true;                                  // return result from function
}

bool makeDiagonallyDominantSystem(vector<vector<double>> &A, vector<double> &b)  // declare and initialize object
{
    int n = (int)A.size();                        // declare and initialize object
    bool changed = false;                         // declare and assign variable
    for (int i = 0; i < n; i++)                   // iterate over elements
    {
        int best = i;                             // declare and assign variable
        for (int k = i + 1; k < n; k++)           // iterate over elements
            if (fabs(A[k][i]) > fabs(A[best][i])) best = k;  // check condition
        if (best != i) { swap(A[i], A[best]); swap(b[i], b[best]); changed = true; }  // check condition
    }
    return changed;                               // return result from function
}

void checkDominance(const vector<vector<double>> &A, int n,  // declare and initialize object
    ofstream &logOut)                             // handle file stream operation
{
    bool isDom = true;                            // declare and assign variable
    for (int i = 0; i < n && isDom; i++)          // iterate over elements
    {
        double diag = fabs(A[i][i]);              // declare and initialize object
        double sum  = 0.0;                        // declare and assign variable
        for (int j = 0; j < n; j++)               // iterate over elements
            if (j != i) sum += fabs(A[i][j]);     // check condition
        if (diag < sum) isDom = false;            // check condition
    }
    string msg = isDom                            // declare and assign variable
        ? "[INFO] Matrix IS Diagonally Dominant — method will converge."  // access array or vector element
        : "[INFO] Matrix is NOT strictly Diagonally Dominant — attempting row reordering first.";  // call function or method
    cout << msg << "\n";                          // check equality or comparison
    logOut << msg << "\n";                        // check equality or comparison
}

void writeHeader(ofstream &log, int n)     // declare and initialize object
{
    string sep = "+---------+";                   // declare and assign variable
    for (int i = 0; i < n; i++) sep += "----------------+";  // iterate over elements
    sep += "-----------------+";                  // update variable with operation

    string hdr = "| Iter    |";                   // declare and assign variable
    for (int i = 0; i < n; i++) {                 // iterate over elements
        string v = "     x" + to_string(i+1) + "         ";  // declare and initialize object
        v = v.substr(0, 16);                      // assign value to variable
        hdr += v + "|";                           // update variable with operation
    }
    hdr += "      Error      |";                  // update variable with operation

    cout  << sep << "\n" << hdr << "\n" << sep << "\n";  // check equality or comparison
    log   << sep << "\n" << hdr << "\n" << sep << "\n";  // check equality or comparison
}

void writeRow(ofstream &log, int iter,     // declare and initialize object
    const vector<double> &x, double error)        // declare variable
{
    ostringstream oss;                            // execute statement
    oss << "| " << setw(7) << iter << " |";       // check equality or comparison
    for (double xi : x)                           // iterate over elements
        oss << " " << setw(13) << fixed << setprecision(6) << xi << " |";  // check equality or comparison
    oss << " " << setw(14) << scientific << setprecision(5) << error << "  |";  // check equality or comparison
    cout  << oss.str() << "\n";                   // check equality or comparison
    log   << oss.str() << "\n";                   // check equality or comparison
}

void IterativeMethod::gaussJacobi(                // define function
    const string &Afile, const string &bfile,     // declare variable
    int maxIter, double tol)                      // declare variable
{
    string augFile = "Output/jacobi_augmented.txt";  // declare and assign variable
    LinearOperation linearOperation;               // create helper object
    linearOperation.generateAugmentedMatrixFile(Afile, bfile, augFile);  // declare function

    vector<vector<double>> A;                     // check equality or comparison
    vector<double> b;                             // check equality or comparison
    int n;                                        // declare variable
    loadSystem(Afile, bfile, A, b, n);            // call function or method

    ofstream logOut("Output/jacobi_log.txt");     // call function or method
    ofstream vecOut("Output/jacobi_vector.txt");  // call function or method

    cout << "\n================================================\n";  // check equality or comparison
    cout <<   "        GAUSS-JACOBI ITERATIVE METHOD          \n";  // check equality or comparison
    cout <<   "================================================\n";  // check equality or comparison
    logOut << "================================================\n";  // check equality or comparison
    logOut << "        GAUSS-JACOBI ITERATIVE METHOD          \n";  // check equality or comparison
    logOut << "================================================\n\n";  // check equality or comparison

    checkDominance(A, n, logOut);                 // call function or method
    if (!isDominantSystem(A, n))                  // check condition
    {
        string reorderMsg = "[INFO] Reordering rows to improve diagonal dominance.";  // declare and assign variable
        cout << reorderMsg << "\n";              // check equality or comparison
        logOut << reorderMsg << "\n";            // check equality or comparison
        if (makeDiagonallyDominantSystem(A, b) && isDominantSystem(A, n))  // check condition
        {
            string readyMsg = "[INFO] Row reordering produced a diagonally dominant system.";  // declare and assign variable
            cout << readyMsg << "\n";           // check equality or comparison
            logOut << readyMsg << "\n";         // check equality or comparison
        }
        else
        {
            string warnMsg = "[WARNING] Could not make the system diagonally dominant; continuing anyway.";  // declare and assign variable
            cout << warnMsg << "\n";            // check equality or comparison
            logOut << warnMsg << "\n";          // check equality or comparison
        }
    }

    vector<double> x(n, 0.0);                     // check equality or comparison
    vector<double> xNew(n, 0.0);                  // check equality or comparison

    cout   << "\n--- Iteration Table ---\n";      // check equality or comparison
    logOut << "\n--- Iteration Table ---\n";      // check equality or comparison
    writeHeader(logOut, n);                       // call function or method

    bool   converged = false;                     // declare and assign variable
    int    finalIter = 0;                         // declare and assign variable
    double finalErr  = 0.0;                       // declare and assign variable

    for (int iter = 1; iter <= maxIter; iter++)   // iterate over elements
    {
        for (int i = 0; i < n; i++)               // iterate over elements
        {
            double sigma = 0.0;                   // declare and assign variable
            for (int j = 0; j < n; j++)           // iterate over elements
                if (j != i) sigma += A[i][j] * x[j];  // check condition
            xNew[i] = (b[i] - sigma) / A[i][i];   // assign value to variable
        }

        double error = 0.0;                       // declare and assign variable
        for (int i = 0; i < n; i++)               // iterate over elements
            error = max(error, fabs(xNew[i] - x[i]));  // assign value to variable

        x        = xNew;                          // assign value to variable
        finalIter = iter;                         // assign value to variable
        finalErr  = error;                        // assign value to variable
        writeRow(logOut, iter, x, error);         // call function or method

        if (error < tol) { converged = true; break; }  // check condition
    }

    string sep = "+---------+";                   // declare and assign variable
    for (int i = 0; i < n; i++) sep += "----------------+";  // iterate over elements
    sep += "-----------------+";                  // update variable with operation
    cout  << sep << "\n";                         // check equality or comparison
    logOut << sep << "\n";                        // check equality or comparison

    SolutionPrinter solutionPrinter;             // create printer object
    solutionPrinter.printIterative(x, "Gauss-Jacobi",  // define function
        converged, finalIter, finalErr, vecOut);  // execute statement

    PlotHelper plotHelper;                       // create plot helper object
    plotHelper.saveAndPlot(x,                    // define function
        "Output/jacobi_plot.dat",                 // execute statement
        "Output/jacobi_plot.gnu",                 // execute statement
        "Output/jacobi_graph.png",                // execute statement
        "Gauss-Jacobi - Solution Vector");        // execute statement

    cout << "\n[OUTPUT] Augmented matrix -> Output/jacobi_augmented.txt\n";  // check equality or comparison
    cout <<   "[OUTPUT] Iteration log    -> Output/jacobi_log.txt\n";  // check equality or comparison
    cout <<   "[OUTPUT] Solution vector  -> Output/jacobi_vector.txt\n";  // declare variable
    cout <<   "[OUTPUT] Graph            -> Output/jacobi_graph.png\n";  // check equality or comparison
}







void IterativeMethod::gaussSeidel(                // define function
    const string &Afile, const string &bfile,     // declare variable
    int maxIter, double tol)                      // declare variable
{
    string augFile = "Output/seidel_augmented.txt";  // declare and assign variable
    LinearOperation linearOperation;               // create helper object
    linearOperation.generateAugmentedMatrixFile(Afile, bfile, augFile);  // declare function

    vector<vector<double>> A;                     // check equality or comparison
    vector<double> b;                             // check equality or comparison
    int n;                                        // declare variable
    loadSystem(Afile, bfile, A, b, n);            // call function or method

    ofstream logOut("Output/seidel_log.txt");     // call function or method
    ofstream vecOut("Output/seidel_vector.txt");  // call function or method

    
    cout << "\n================================================\n";  // check equality or comparison
    logOut << "       GAUSS-SEIDEL ITERATIVE METHOD           \n";  // check equality or comparison
    logOut << "================================================\n\n";  // check equality or comparison

    checkDominance(A, n, logOut);                 // call function or method
    if (!isDominantSystem(A, n))                  // check condition
    {
        string reorderMsg = "[INFO] Reordering rows to improve diagonal dominance.";  // declare and assign variable
        cout << reorderMsg << "\n";              // check equality or comparison
        logOut << reorderMsg << "\n";            // check equality or comparison
        if (makeDiagonallyDominantSystem(A, b) && isDominantSystem(A, n))  // check condition
        {
            string readyMsg = "[INFO] Row reordering produced a diagonally dominant system.";  // declare and assign variable
            cout << readyMsg << "\n";           // check equality or comparison
            logOut << readyMsg << "\n";         // check equality or comparison
        }
        else
        {
            string warnMsg = "[WARNING] Could not make the system diagonally dominant; continuing anyway.";  // declare and assign variable
            cout << warnMsg << "\n";            // check equality or comparison
            logOut << warnMsg << "\n";          // check equality or comparison
        }
    }

    vector<double> x(n, 0.0);                     // check equality or comparison

    cout   << "\n--- Iteration Table ---\n";      // check equality or comparison
    logOut << "\n--- Iteration Table ---\n";      // check equality or comparison
    writeHeader(logOut, n);                       // call function or method

    bool   converged = false;                     // declare and assign variable
    int    finalIter = 0;                         // declare and assign variable
    double finalErr  = 0.0;                       // declare and assign variable

    for (int iter = 1; iter <= maxIter; iter++)   // iterate over elements
    {
        double error = 0.0;                       // declare and assign variable

        for (int i = 0; i < n; i++)               // iterate over elements
        {
            double sigma = 0.0;                   // declare and assign variable
            for (int j = 0; j < n; j++)           // iterate over elements
                if (j != i) sigma += A[i][j] * x[j];  // check condition
            double xNew = (b[i] - sigma) / A[i][i];  // declare and initialize object
            error = max(error, fabs(xNew - x[i]));  // assign value to variable
            x[i]  = xNew;                         // assign value to variable
        }

        finalIter = iter;                         // assign value to variable
        finalErr  = error;                        // assign value to variable
        writeRow(logOut, iter, x, error);         // call function or method

        if (error < tol) { converged = true; break; }  // check condition
    }

    string sep = "+---------+";                   // declare and assign variable
    for (int i = 0; i < n; i++) sep += "----------------+";  // iterate over elements
    sep += "-----------------+";                  // update variable with operation
    cout  << sep << "\n";                         // check equality or comparison
    logOut << sep << "\n";                        // check equality or comparison

    SolutionPrinter solutionPrinter;             // create printer object
    solutionPrinter.printIterative(x, "Gauss-Seidel",  // define function
        converged, finalIter, finalErr, vecOut);  // execute statement

    PlotHelper plotHelper;                       // create plot helper object
    plotHelper.saveAndPlot(x,                    // define function
        "Output/seidel_plot.dat",                 // execute statement
        "Output/seidel_plot.gnu",                 // execute statement
        "Output/seidel_graph.png",                // execute statement
        "Gauss-Seidel - Solution Vector");        // execute statement

    cout << "\n[OUTPUT] Augmented matrix -> Output/seidel_augmented.txt\n";  // check equality or comparison
    cout <<   "[OUTPUT] Iteration log    -> Output/seidel_log.txt\n";  // check equality or comparison
    cout <<   "[OUTPUT] Solution vector  -> Output/seidel_vector.txt\n";  // declare variable
    cout <<   "[OUTPUT] Graph            -> Output/seidel_graph.png\n";  // check equality or comparison
}