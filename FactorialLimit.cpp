#include <iostream>
#include<climits>
using namespace std;

int main() {

    int n = 1;
    int fact_int = 1;
    unsigned int fact_uint = 1;
    long int fact_long = 1;

    cout << "Finding factorial limits...\n\n";

    // INT
    while (true) {
        if (fact_int > INT_MAX / (n + 1))
            break;
        fact_int *= (++n);
    }
    cout << "Maximum n for int = " << n
         << " , factorial = " << fact_int << endl;

    // UNSIGNED INT
    n = 1;
    while (true) {
        if (fact_uint > UINT_MAX / (n + 1))
            break;
        fact_uint *= (++n);
    }
    cout << "Maximum n for unsigned int = " << n
         << " , factorial = " << fact_uint << endl;

    // LONG INT
    n = 1;
    while (true) {
        if (fact_long > LONG_MAX / (n + 1))
            break;
        fact_long *= (++n);
    }
    cout << "Maximum n for long int = " << n
         << " , factorial = " << fact_long << endl;

    return 0;
}
