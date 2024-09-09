int main()
{
    int arr[] = { 8,8,8,8,8,8,8};
    int n = sizeof(arr) / sizeof(arr[0]);
    mySort(arr, n);
    printArray(arr, n);
    return 0;
}
