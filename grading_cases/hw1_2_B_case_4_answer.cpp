#include <iostream>

using namespace std;

void funcB(int n)
{
    int i = 0;
    for (i=0;i<n-1;i++)
        cout << " ";
    cout << "*****" << endl;

    if (n>1)
        funcB(n-1);
}

int main()
{
    funcB(7);
}