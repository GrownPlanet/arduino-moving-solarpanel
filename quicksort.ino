/*
void quicksort(int arr[][2], int low, int high) {
  if (low >= high || low < 0) {
    return;
  }
  
  int part_i = partition(arr, low, high);
  quicksort(arr, low, part_i - 1);
  quicksort(arr, part_i + 1, high);
}

int partition(int arr[][2], int low, int high) {
  int pivot = arr[high][1];
  int i = low - 1;

  for (int k = low; k <= high - 1; k++) {
    if (arr[k][1] < pivot) {
      i++;
      swap(arr, i, k);
    }
  }
  
  swap(arr, i+1, high);
  return i + 1;
}

void swap(int arr[][2], int i, int k) {
  int temp = arr[i][0];
  arr[i][0] = arr[k][0];
  arr[k][0] = temp;
    
  temp = arr[i][1];
  arr[i][1] = arr[k][1];
  arr[k][1] = temp;
}
*/
