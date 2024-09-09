int main()
{
    int arr[] = { -20,10,0,-5,15,-1};
    int n = sizeof(arr) / sizeof(arr[0]);
    mySort(arr, n);
    printArray(arr, n);
    return 0;
}
