#include <iostream>
#include <vector>

void InsertionSort(std::vector<int>& arr, const int l, const int r) {
  for (int i = l + 1; i <= r; ++i) {
    const int key = arr[i];
    int j = i - 1;
    while (j >= l && arr[j] > key) {
      arr[j + 1] = arr[j];
      --j;
    }
    arr[j + 1] = key;
  }
}

void Merge(std::vector<int>& arr, const int l, const int mid, const int r, std::vector<int>& temp) {
  int i = l;
  int j = mid + 1;
  int k = l;

  for (int x = l; x <= r; ++x) {
    temp[x] = arr[x];
  }

  while (i <= mid && j <= r) {
    if (temp[i] <= temp[j]) {
      arr[k++] = temp[i++];
    } else {
      arr[k++] = temp[j++];
    }
  }

  while (i <= mid) {
    arr[k++] = temp[i++];
  }
}

void MergeSort(std::vector<int>& arr, const int left, const int right, std::vector<int>& temp) {
  if (left >= right) {
    return;
  }

  if (right - left <= 15) {
    InsertionSort(arr, left, right);
    return;
  }

  const int mid = left + (right - left) / 2;
  MergeSort(arr, left, mid, temp);
  MergeSort(arr, mid + 1, right, temp);

  if (arr[mid] <= arr[mid + 1]) {
    return;
  }
  Merge(arr, left, mid, right, temp);
}

void MergeSort(std::vector<int>& arr) {
  if (arr.empty()) {
    return;
  }
  std::vector<int> temp(arr.size());
  MergeSort(arr, 0, arr.size() - 1, temp);
}


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n = 0;
  std::cin >> n;

  std::vector<int> arr(n);
  for (int i = 0; i < n; i++) {
    std::cin >> arr[i];
  }

  MergeSort(arr);

  for (int i = 0; i < n; i++) {
    std::cout << arr[i] << " ";
  }
}