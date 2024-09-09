#include <iostream>

using namespace std;

void mySort(int arr[], int n) {
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < n-1; j++) {
			if (arr[j] > arr[j + 1]) {
				int b_num = arr[j];
				int s_num = arr[j + 1];
				arr[j + 1] = b_num;
				arr[j] = s_num;
			}
		}
	}
}

void printArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
}

int main()
{
    int arr[] = { -20,10,0,-5,15,-1};
    int n = sizeof(arr) / sizeof(arr[0]);
    mySort(arr, n);
    printArray(arr, n);
    return 0;
}
