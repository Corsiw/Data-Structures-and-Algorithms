#include<iostream>
#include <vector>

size_t Merge(std::vector<int>& arr, const int l, const int mid, const int r) {
  const std::vector<int> left_intervals(arr.begin() + l, arr.begin() + mid + 1);
  const std::vector<int> right_intervals(arr.begin() + mid + 1, arr.begin() + r + 1);

  int i = 0;
  int j = 0;
  int cur = l;
  size_t res = 0;

  while (i < left_intervals.size() && j < right_intervals.size()) {
    if (left_intervals[i] <= right_intervals[j]) {
      arr[cur++] = left_intervals[i++];
    } else {
      arr[cur++] = right_intervals[j++];
      res += left_intervals.size() - i;
    }
  }

  while (i < left_intervals.size()) {
    arr[cur++] = left_intervals[i++];
  }
  while (j < right_intervals.size()) {
    arr[cur++] = right_intervals[j++];
  }
  return res;
}

size_t MergeSort(std::vector<int>& arr, int l, int r) {
  if (l >= r) {
    return 0;
  }
  const int m = l + (r - l) / 2;
  size_t k = 0;
  k += MergeSort(arr, l, m);
  k += MergeSort(arr, m + 1, r);
  k += Merge(arr, l, m, r);
  return k;
}

size_t MergeSignificant(std::vector<int>& arr, const int l, const int mid, const int r) {
  const std::vector<int> left_intervals(arr.begin() + l, arr.begin() + mid + 1);
  const std::vector<int> right_intervals(arr.begin() + mid + 1, arr.begin() + r + 1);

  size_t res = 0;
  int k = 0;
  for (int i = 0; i < left_intervals.size(); i++) {
    while (k < right_intervals.size() && left_intervals[i] > 2LL * right_intervals[k]) {
      k++;
    }
    res += k;
  }

  int i = 0;
  int j = 0;
  int cur = l;

  while (i < left_intervals.size() && j < right_intervals.size()) {
    if (left_intervals[i] <= right_intervals[j]) {
      arr[cur++] = left_intervals[i++];
    } else {
      arr[cur++] = right_intervals[j++];
    }
  }

  while (i < left_intervals.size()) {
    arr[cur++] = left_intervals[i++];
  }
  while (j < right_intervals.size()) {
    arr[cur++] = right_intervals[j++];
  }
  return res;
}

size_t MergeSortSignificant(std::vector<int>& arr, const int l, const int r) {
  if (l >= r) {
    return 0;
  }
  const int m = l + (r - l) / 2;
  size_t k = 0;
  k += MergeSortSignificant(arr, l, m);
  k += MergeSortSignificant(arr, m + 1, r);
  k += MergeSignificant(arr, l, m, r);
  return k;
}

int main() {
  std::vector<int> arr = {1, 3, 4, 2, 5};
  std::cout << MergeSort(arr, 0, arr.size() - 1) << std::endl;

  arr = {5,3,2,4,1};
  std::cout << MergeSortSignificant(arr, 0, arr.size() - 1) << std::endl;
}