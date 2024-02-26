#include <bits/stdc++.h>

int partition(std::vector<int>& nums, int left, int right);

void quick_sort(std::vector<int>& nums, int left, int right) {
    if (left < right) {
        auto mid = partition(nums, left, right);
        quick_sort(nums, left, mid - 1);
        quick_sort(nums, mid + 1, right);
    }
}

std::vector<int> sortArray(std::vector<int>& nums) {
    const int n = nums.size();
    if (n < 2) return nums;
    quick_sort(nums, 0, n - 1);
    return nums;
}

int partition(std::vector<int>& nums, int left, int right) {
    int pivot = nums[left];
    int i = left, j = right;
    while (i < j) {
        while (i < j && nums[j] >= pivot) --j;
        nums[i] = nums[j];
        while (i < j && nums[i] <= pivot) ++i;
        nums[j] = nums[i];
    }
    nums[i] = pivot;
    return i;
}

int main() {
    std::vector<int> nums{ 3, 5, -1, 2, 3, 42, 3,1,3, 4, 2, 4, 5, 8, 2, 3,4 ,5, 3,5,2,3};
    auto res = sortArray(nums);

    for (int i = 0; i < res.size(); ++i) {
        std::cout << nums[i] << " \n"[i + 1 == res.size()];
    }
    return 0;
}