#include <iostream>

using namespace std;

void funcA(int n)
{
    int i = 0;

    for (i=0;i<n;i++)
        cout << "*";
    cout << endl;

    if (n>1)
        funcA(n-1);
}

int main()
{
    funcA(2);
}
