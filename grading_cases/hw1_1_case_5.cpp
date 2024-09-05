int main()
{
    int arr[] = { 2000000,1999999,2000001,1999998};
    int n = sizeof(arr) / sizeof(arr[0]);
    mySort(arr, n);
    printArray(arr, n);
    return 0;
}
