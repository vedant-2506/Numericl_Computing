#include "../Include/Iterativemethod.hpp"
#include "../Include/PlotHelper.hpp"
#include "../Include/Solutionprinter.hpp"
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
using namespace std;


IterativeMethod::IterativeMethod() : LinearOperation() {}
IterativeMethod::IterativeMethod(int r, int c) : LinearOperation(r, c) {}

ostream& operator<<(ostream &out, const IterativeMethod &im)
{
    out << "IterativeMethod(" << im.rows << "x" << im.cols << ")\n";
    return out;
}

void loadSystem(const string &Afile, const string &bfile,
    vector<vector<double>> &A, vector<double> &b, int &n)
{
    ifstream finA(Afile), finb(bfile);
    if (!finA || !finb) throw runtime_error("File not found");
    int cA, rb, cb;
    finA >> n >> cA;
    finb >> rb >> cb;
    if (cA != n || rb != n || cb != 1) throw runtime_error("Iterative method: invalid matrix/vector size");
    A.assign(n, vector<double>(n));
    b.assign(n, 0.0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) finA >> A[i][j];
    for (int i = 0; i < n; i++) finb >> b[i];
}

bool isDominantSystem(const vector<vector<double>> &A, int n)
{
    for (int i = 0; i < n; i++)
    {
        double diag = fabs(A[i][i]);
        double sum  = 0.0;
        for (int j = 0; j < n; j++)
            if (j != i) sum += fabs(A[i][j]);
        if (diag < sum) return false;
    }
    return true;
}

bool makeDiagonallyDominantSystem(vector<vector<double>> &A, vector<double> &b)
{
    int n = (int)A.size();
    bool changed = false;
    for (int i = 0; i < n; i++)
    {
        int best = i;
        for (int k = i + 1; k < n; k++)
            if (fabs(A[k][i]) > fabs(A[best][i])) best = k;
        if (best != i) { swap(A[i], A[best]); swap(b[i], b[best]); changed = true; }
    }
    return changed;
}

void checkDominance(const vector<vector<double>> &A, int n,
    ofstream &logOut)
{
    bool isDom = true;
    for (int i = 0; i < n && isDom; i++)
    {
        double diag = fabs(A[i][i]);
        double sum  = 0.0;
        for (int j = 0; j < n; j++)
            if (j != i) sum += fabs(A[i][j]);
        if (diag < sum) isDom = false;
    }
    string msg = isDom
        ? "[INFO] Matrix IS Diagonally Dominant — method will converge."
        : "[INFO] Matrix is NOT strictly Diagonally Dominant — attempting row reordering first.";
    cout << msg << "\n";
    logOut << msg << "\n";
}

void writeHeader(ofstream &log, int n)
{
    string sep = "+---------+";
    for (int i = 0; i < n; i++) sep += "----------------+";
    sep += "-----------------+";

    string hdr = "| Iter    |";
    for (int i = 0; i < n; i++) {
        string v = "     x" + to_string(i+1) + "         ";
        v = v.substr(0, 16);
        hdr += v + "|";
    }
    hdr += "      Error      |";

    cout  << sep << "\n" << hdr << "\n" << sep << "\n";
    log   << sep << "\n" << hdr << "\n" << sep << "\n";
}

void writeRow(ofstream &log, int iter,
    const vector<double> &x, double error)
{
    ostringstream oss;
    oss << "| " << setw(7) << iter << " |";
    for (double xi : x)
        oss << " " << setw(13) << fixed << setprecision(6) << xi << " |";
    oss << " " << setw(14) << scientific << setprecision(5) << error << "  |";
    cout  << oss.str() << "\n";
    log   << oss.str() << "\n";
}

void IterativeMethod::gaussJacobi(
    const string &Afile, const string &bfile,
    int maxIter, double tol)
{
    string augFile = "Output/jacobi_augmented.txt";
    LinearOperation linearOperation;
    linearOperation.generateAugmentedMatrixFile(Afile, bfile, augFile);

    vector<vector<double>> A;
    vector<double> b;
    int n;
    loadSystem(Afile, bfile, A, b, n);

    ofstream logOut("Output/jacobi_log.txt");
    ofstream vecOut("Output/jacobi_vector.txt");

    cout << "\n================================================\n";
    cout <<   "        GAUSS-JACOBI ITERATIVE METHOD          \n";
    cout <<   "================================================\n";
    logOut << "================================================\n";
    logOut << "        GAUSS-JACOBI ITERATIVE METHOD          \n";
    logOut << "================================================\n\n";

    checkDominance(A, n, logOut);
    if (!isDominantSystem(A, n))
    {
        string reorderMsg = "[INFO] Reordering rows to improve diagonal dominance.";
        cout << reorderMsg << "\n";
        logOut << reorderMsg << "\n";
        if (makeDiagonallyDominantSystem(A, b) && isDominantSystem(A, n))
        {
            string readyMsg = "[INFO] Row reordering produced a diagonally dominant system.";
            cout << readyMsg << "\n";
            logOut << readyMsg << "\n";
        }
        else
        {
            string warnMsg = "[WARNING] Could not make the system diagonally dominant; continuing anyway.";
            cout << warnMsg << "\n";
            logOut << warnMsg << "\n";
        }
    }

    vector<double> x(n, 0.0);
    vector<double> xNew(n, 0.0);

    cout   << "\n--- Iteration Table ---\n";
    logOut << "\n--- Iteration Table ---\n";
    writeHeader(logOut, n);

    bool   converged = false;
    int    finalIter = 0;
    double finalErr  = 0.0;

    for (int iter = 1; iter <= maxIter; iter++)
    {
        for (int i = 0; i < n; i++)
        {
            double sigma = 0.0;
            for (int j = 0; j < n; j++)
                if (j != i) sigma += A[i][j] * x[j];
            xNew[i] = (b[i] - sigma) / A[i][i];
        }

        double error = 0.0;
        for (int i = 0; i < n; i++)
            error = max(error, fabs(xNew[i] - x[i]));

        x        = xNew;
        finalIter = iter;
        finalErr  = error;
        writeRow(logOut, iter, x, error);

        if (error < tol) { converged = true; break; }
    }

    string sep = "+---------+";
    for (int i = 0; i < n; i++) sep += "----------------+";
    sep += "-----------------+";
    cout  << sep << "\n";
    logOut << sep << "\n";

    SolutionPrinter solutionPrinter;
    solutionPrinter.printIterative(x, "Gauss-Jacobi",
        converged, finalIter, finalErr, vecOut);

    PlotHelper plotHelper;
    plotHelper.saveAndPlot(x,
        "Output/jacobi_plot.dat",
        "Output/jacobi_plot.gnu",
        "Output/jacobi_graph.png",
        "Gauss-Jacobi - Solution Vector");

    cout << "\n[OUTPUT] Augmented matrix -> Output/jacobi_augmented.txt\n";
    cout <<   "[OUTPUT] Iteration log    -> Output/jacobi_log.txt\n";
    cout <<   "[OUTPUT] Solution vector  -> Output/jacobi_vector.txt\n";
    cout <<   "[OUTPUT] Graph            -> Output/jacobi_graph.png\n";
}







void IterativeMethod::gaussSeidel(
    const string &Afile, const string &bfile,
    int maxIter, double tol)
{
    string augFile = "Output/seidel_augmented.txt";
    LinearOperation linearOperation;
    linearOperation.generateAugmentedMatrixFile(Afile, bfile, augFile);

    vector<vector<double>> A;
    vector<double> b;
    int n;
    loadSystem(Afile, bfile, A, b, n);

    ofstream logOut("Output/seidel_log.txt");
    ofstream vecOut("Output/seidel_vector.txt");


    cout << "\n================================================\n";
    logOut << "       GAUSS-SEIDEL ITERATIVE METHOD           \n";
    logOut << "================================================\n\n";

    checkDominance(A, n, logOut);
    if (!isDominantSystem(A, n))
    {
        string reorderMsg = "[INFO] Reordering rows to improve diagonal dominance.";
        cout << reorderMsg << "\n";
        logOut << reorderMsg << "\n";
        if (makeDiagonallyDominantSystem(A, b) && isDominantSystem(A, n))
        {
            string readyMsg = "[INFO] Row reordering produced a diagonally dominant system.";
            cout << readyMsg << "\n";
            logOut << readyMsg << "\n";
        }
        else
        {
            string warnMsg = "[WARNING] Could not make the system diagonally dominant; continuing anyway.";
            cout << warnMsg << "\n";
            logOut << warnMsg << "\n";
        }
    }

    vector<double> x(n, 0.0);

    cout   << "\n--- Iteration Table ---\n";
    logOut << "\n--- Iteration Table ---\n";
    writeHeader(logOut, n);

    bool   converged = false;
    int    finalIter = 0;
    double finalErr  = 0.0;

    for (int iter = 1; iter <= maxIter; iter++)
    {
        double error = 0.0;

        for (int i = 0; i < n; i++)
        {
            double sigma = 0.0;
            for (int j = 0; j < n; j++)
                if (j != i) sigma += A[i][j] * x[j];
            double xNew = (b[i] - sigma) / A[i][i];
            error = max(error, fabs(xNew - x[i]));
            x[i]  = xNew;
        }

        finalIter = iter;
        finalErr  = error;
        writeRow(logOut, iter, x, error);

        if (error < tol) { converged = true; break; }
    }

    string sep = "+---------+";
    for (int i = 0; i < n; i++) sep += "----------------+";
    sep += "-----------------+";
    cout  << sep << "\n";
    logOut << sep << "\n";

    SolutionPrinter solutionPrinter;
    solutionPrinter.printIterative(x, "Gauss-Seidel",
        converged, finalIter, finalErr, vecOut);

    PlotHelper plotHelper;
    plotHelper.saveAndPlot(x,
        "Output/seidel_plot.dat",
        "Output/seidel_plot.gnu",
        "Output/seidel_graph.png",
        "Gauss-Seidel - Solution Vector");

    cout << "\n[OUTPUT] Augmented matrix -> Output/seidel_augmented.txt\n";
    cout <<   "[OUTPUT] Iteration log    -> Output/seidel_log.txt\n";
    cout <<   "[OUTPUT] Solution vector  -> Output/seidel_vector.txt\n";
    cout <<   "[OUTPUT] Graph            -> Output/seidel_graph.png\n";
}