int main()
{
    int arr[] = { 15,14,13,12,11,10,9,8};
    int n = sizeof(arr) / sizeof(arr[0]);
    mySort(arr, n);
    printArray(arr, n);
    return 0;
}
