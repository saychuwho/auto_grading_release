int main()
{
    int arr[] = { 3,3,99,-4,-4};
    int n = sizeof(arr) / sizeof(arr[0]);
    mySort(arr, n);
    printArray(arr, n);
    return 0;
}
