#include <iostream>
#include <string>

using namespace std;

string funcC(int n)
{
    string add = "";

    if (n<2)
        return to_string(n);
    
    int share = n / 2;
    int remain = n % 2;

    return funcC(share) + to_string(remain);
}

int main()
{
    cout << funcC(0) << endl;
}
