#pragma once
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// 问题待定：
/*
s2563o2：相向三指针
*/

/*
模板题：
1.回文串常见字符串函数isalnum以及tolower的应用：125
2.减少乘法判断运算次数的技巧：977
3.双指针永远是固定指针起点更好处理，正难则反：658
4.排序 + 相向双指针，每移动一次指针，相当于知道O(n)的信息，可将暴力循环O(n^2)的时间复杂度降到O(n)：167
5.相向双指针可以解决两数之和=、>=、<等判断条件的计数，但若是一个区间，则可以拆分为两次相向双指针：2563
6.s167相向双指针的进阶，先固定一个nums[i]，然后让后面的两个数凑成0 - nums[i]，剩下就和s167一样：15
7.比较两个指针移动的后果，只移动可能更新答案的那一端：11
8.接雨水，在为每个高度加左右木板的基础上，用相向双指针，将空间复杂度降至O(1)：42
9.插入排序法，原地修改：75
10.数字搬家/原地哈希，O(1)空间复杂度，涉及循环追踪和交换：1920
11.原地哈希应用，不涉及数字搬家，只是将数组当作哈希简单使用：442
*/

// 单序列双指针：相向双指针 + 同向双指针 + 背向双指针 + 原地修改

// 【3.1】相向双指针 (19)
// 一般left指向头部，right指向尾端，向中间逼近，滑动窗口属于同向双指针
/*
344.反转字符串：编写一个函数，其作用是将输入的字符串反转过来。输入字符串以字符数组 s 的形式给出。
不要给另外的数组分配额外的空间，你必须原地修改输入数组、使用 O(1) 的额外空间解决这一问题。

3643.垂直翻转子矩阵：给你一个 m x n 的整数矩阵 grid，以及三个整数 x、y 和 k。
整数 x 和 y 表示一个 正方形子矩阵 的左上角下标，整数 k 表示该正方形子矩阵的边长。
你的任务是垂直翻转子矩阵的行顺序。返回更新后的矩阵。

125.验证回文串：如果在将所有大写字符转换为小写字符、并移除所有非字母数字字符之后，
短语正着读和反着读都一样。则可以认为该短语是一个 回文串 。字母和数字都属于字母数字字符。
给你一个字符串 s，如果它是 回文串 ，返回 true ；否则，返回 false 。

1750.删除字符串两端相同字符后的最短长度：给你一个只包含字符 'a'，'b' 和 'c' 的字符串 s ，
你可以执行下面这个操作（5 个步骤）任意次：选择字符串 s 一个 非空 的前缀，这个前缀的所有字符都相同。
选择字符串 s 一个 非空 的后缀，这个后缀的所有字符都相同。前缀和后缀在字符串中任意位置都不能有交集。
前缀和后缀包含的所有字符都要相同。同时删除前缀和后缀。
请你返回对字符串 s 执行上面操作任意次以后（可能 0 次），能得到的 最短长度 。
*/
// ---------------------

namespace s344m1
{
    class Solution {
    public:
        void reverseString(vector<char>& s) {
            int left = 0, right = s.size() - 1;
            while (left < right) {
                swap(s[left++], s[right--]);
            }
        }
    };
}

namespace s3643m1
{
    class Solution {
    public:
        vector<vector<int>> reverseSubmatrix(vector<vector<int>>& grid, int x, int y, int k) {
            int row1 = x, row2 = x + k - 1;
            while (row1 < row2) {
                for (int i = 0; i < k; ++i) {
                    swap(grid[row1][y + i], grid[row2][y + i]);
                }
                ++row1; --row2;
            }
            return grid;
        }
    };
}

// 模板题1：回文串常见字符串函数isalnum以及tolower的应用
namespace s125o1
{   // bool isalnum(c)，如果是字母或数字则返回true
    // int tolower(c)，如果是大写字符会转成小写，其他不变
    class Solution {
    public:
        bool isPalindrome(string s) {
            int left = 0, right = s.size() - 1;
            while (left < right) {
                if (!isalnum(s[left])) {
                    ++left;
                }
                else if (!isalnum(s[right])) {
                    --right;
                }
                else if (tolower(s[left]) != tolower(s[right])) {
                    return false;
                }
                else {
                    ++left; --right;
                }
            }
            return true;
        }
    };
}

namespace s1750m1
{   // 还有优化空间
    class Solution {
    public:
        int minimumLength(string s) {
            int n = s.size();
            int left = 0, right = n - 1;
            while (left < right && s[left] == s[right]) {
                while (left < n - 1 && s[left + 1] == s[left]) {
                    ++left;// left和right移动可以互相逼近，而不是跟s的端点比
                }
                while (right > 0 && s[right - 1] == s[right]) {
                    --right;
                }
                ++left;
                --right;

            }
            return left <= right ? (right - left + 1) : 0;
        }
    };
}
namespace s1750o1
{
    class Solution {
    public:
        int minimumLength(string s) {
            int left = 0, right = s.size() - 1;
            while (left < right && s[left] == s[right]) {
                char ch = s[left];
                // 删除左边连续的 ch
                while (left <= right && s[left] == ch) {
                    ++left;// 假如可以删完，在这段循环中, left将变成right + 1
                }
                // 删除右边连续的 ch
                while (left <= right && s[right] == ch) {
                    --right;
                }
            }
            return right - left + 1;// 细节，如果全部删完，left + 1 = right，也没问题
        }
    };
}

namespace s2105m1
{   // 写的有些啰嗦
    class Solution {
    public:
        int minimumRefill(vector<int>& plants, int capacityA, int capacityB) {
            int l = 0, r = plants.size() - 1;
            int ans = 0;
            int tempA = capacityA;
            int tempB = capacityB;
            while (l < r) {
                if (tempA >= plants[l]) {
                    tempA -= plants[l];
                    ++l;
                }
                else {
                    tempA = capacityA - plants[l];
                    ++ans;
                    ++l;
                }
                if (tempB >= plants[r]) {
                    tempB -= plants[r];
                    --r;
                }
                else {
                    tempB = capacityB - plants[r];
                    ++ans;
                    --r;
                }
            }
            if (l == r) {
                if (max(tempA, tempB) < plants[l]) {
                    ++ans;
                }
            }
            return ans;
        }
    };
}
namespace s2105o1
{   // 思路不变，优化了tempA， tempB的判断逻辑
    class Solution {
    public:
        int minimumRefill(vector<int>& plants, int capacityA, int capacityB) {
            int l = 0, r = plants.size() - 1;
            int ans = 0;
            int tempA = capacityA;
            int tempB = capacityB;
            while (l < r) {
                if (tempA < plants[l]) {
                    tempA = capacityA;
                    ++ans;
                }
                tempA -= plants[l++];
                if (tempB < plants[r]) {
                    tempB = capacityB;
                    ++ans;
                }
                tempB -= plants[r--];
            }
            if (l == r) {
                if (max(tempA, tempB) < plants[l]) {
                    ++ans;
                }
            }
            return ans;
        }
    };
}

// 模板题2：减少乘法判断运算次数的技巧
namespace s977o1
{   // 绝对值最大值一定出现在nums两端
    class Solution {
    public:
        vector<int> sortedSquares(vector<int>& nums) {
            int n = nums.size();
            int left = 0, right = n - 1, pos = n - 1;
            vector<int> ans(n);
            while (left <= right) {
                if (-nums[left] > nums[right]) {// 等效于nums[left] * nums[left] > nums[right] * nums[right]
                    ans[pos--] = nums[left] * nums[left];
                    ++left;
                }
                else {
                    ans[pos--] = nums[right] * nums[right];
                    --right;
                }
            }
            return ans;
        }
    };
}
namespace s977o2
{   // 写法更简洁
    class Solution {
    public:
        vector<int> sortedSquares(vector<int>& nums) {
            int n = nums.size();
            vector<int> ans(n);
            int i = 0, j = n - 1;
            for (int p = n - 1; p >= 0; p--) {
                int x = nums[i], y = nums[j];
                if (-x > y) {
                    ans[p] = x * x;
                    i++;
                }
                else {
                    ans[p] = y * y;
                    j--;
                }
            }
            return ans;
        }
    };
}

// 模板题3：双指针永远是固定指针起点更好处理，正难则反
namespace s658m1
{   // 二分查找处下标为right，左边为left，指针背向移动，逐个判断元素并加入ans中，最后进行sort排序，边界判断比较麻烦
    class Solution {
    public:
        vector<int> findClosestElements(vector<int>& arr, int k, int x) {
            auto it = lower_bound(arr.begin(), arr.end(), x);
            int i = it - arr.begin(), n = arr.size();
            int left = i - 1, right = i;
            int diffl = INT_MAX, diffr = INT_MAX;
            int pos = 0;
            vector<int> ans(k);
            while (k) {
                diffl = left >= 0 ? abs(x - arr[left]) : INT_MAX;
                diffr = right < n ? abs(x - arr[right]) : INT_MAX;
                ans[k-- - 1] = diffl <= diffr ? arr[left--] : arr[right++];
            }
            sort(ans.begin(), ans.end());
            return ans;
        }
    };
}
namespace s658o1
{   // 正难则反，指针从中间向两边扩张导致最后要排序，那么就让指针从arr两端向中间逼近
    // 选中n - k个元素排除掉，剩下的k个元素本来就已经是排好序了的
    class Solution {
    public:
        vector<int> findClosestElements(vector<int>& arr, int k, int x) {
            int n = arr.size();
            int removeNum = n - k;
            int left = 0, right = n - 1;
            while (removeNum) {
                int diffl = abs(arr[left] - x);
                int diffr = abs(arr[right] - x);
                if (diffl <= diffr) {
                    --right;
                }
                else {
                    ++left;
                }
                --removeNum;
            }
            return vector<int>(arr.begin() + left, arr.begin() + right + 1);
        }
    };
}

// 和s658类似，但不需要逆向思维
namespace s1471m1
{
    class Solution {
    public:
        vector<int> getStrongest(vector<int>& arr, int k) {
            sort(arr.begin(), arr.end());
            int n = arr.size();
            int m = arr[(n - 1) / 2];
            int left = 0, right = n - 1;
            vector<int> ans(k);
            while (k) {
                int diff1 = abs(arr[left] - m);
                int diff2 = abs(arr[right] - m);
                if (diff2 >= diff1) {
                    ans[k - 1] = arr[right--];
                }
                else {
                    ans[k - 1] = arr[left++];
                }
                --k;
            }
            return ans;
        }
    };
}

// 模板题4：排序 + 相向双指针，每移动一次指针，相当于知道O(n)的信息，可将暴力循环O(n^2)的时间复杂度降到O(n)
namespace s167o1
{
    class Solution {
    public:
        vector<int> twoSum(vector<int>& numbers, int target) {
            int left = 0, right = numbers.size() - 1;
            while (left < right) {
                int s = numbers[left] + numbers[right];
                if (s == target) {
                    break;
                }
                else if (s > target) {
                    --right;
                }
                else {
                    ++left;
                }
            }
            return { left + 1, right + 1 };// 该题目的题干已经说了一定有答案，所以这样写， 
            // 但是如果不一定有答案，就要准备return {}的分支
        }
    };
}

// 套模板4
namespace s633m1
{
    class Solution {
    public:
        bool judgeSquareSum(int c) {
            long long left = 0, right = sqrt(c);// 右端点为开平方
            while (left <= right) {
                long long sum = left * left + right * right;
                if (sum == c) {
                    return true;
                }
                sum > c ? --right : ++left;
            }
            return false;
        }
    };
}
namespace s633o1
{
    class Solution {
    public:
        bool judgeSquareSum(int c) {
            int a = 0, b = sqrt(c);
            while (a <= b) {
                if (a * a == c - b * b) {// 避免溢出的技巧，不用long long内存占用更低
                    return true;
                }
                if (a * a < c - b * b) {
                    ++a;
                }
                else {
                    --b;
                }
            }
            return false;
        }
    };
}

// 模板题5：相向双指针可以迅速解决两数之和=、>=、<等判断条件的计数，但若是一个区间，则可以拆分为两次相向双指针
// m1见专题9.1.2二分查找-进阶，比m2写法简洁很多
// 相向三指针模板题（见专题8.5）：用left - 1和right - 1下标，解决边界问题（如果直接用l, r为下标，--后下标可能为-1）
// o2,o3为两次相向双指针，计算nums[j] <= upper - nums[i]和nums[j] < lower - nums[i]的个数，两者相减即为答案
// 三种方法都是O(nlogn)时间复杂度
namespace s2563m2
{   
    class Solution {
    public:
        long long countFairPairs(vector<int>& nums, int lower, int upper) {
            sort(nums.begin(), nums.end());
            long long ans = 0;
            int n = nums.size();
            int left = 0, right = n - 1;
            for (int i = 0; i < n; ++i) {
                int left = lower - nums[i];
                int right = upper - nums[i];
                auto it1 = lower_bound(nums.begin(), nums.end(), left);
                auto it2 = upper_bound(nums.begin(), nums.end(), right);
                // 可以只从nums.begin() + i + 1开始二分，这样下面的判断都用不上，直接ans += it2 - it1;
                if (it2 == nums.begin()) {
                    break;
                }
                else if (it1 == nums.end()) {
                    continue;
                }
                if (it2 <= (nums.begin() + i + 1)) break;
                ans += min(it2 - it1, it2 - (nums.begin() + i + 1));
            }
            return ans;
        }
    };
}
namespace s2563o1
{
    class Solution {
    public:
        long long countFairPairs(vector<int>& nums, int lower, int upper) {
            sort(nums.begin(), nums.end());
            long long ans = 0;
            int l = nums.size(), r = l;
            for (int i = 0; i < nums.size(); ++i) {
                while (r && nums[r - 1] > upper - nums[i]) {
                    r--;// 相当于找二分查找的右端点
                }
                while (l && nums[l - 1] >= lower - nums[i]) {
                    l--;// 相当于找二分查找的左端点
                }
                if (r <= i) break;// 当二分查找出来的区间在左端点左边时，退出循环，不用继续找了
                ans += r - max(l, i + 1);
            }
            return ans;
        }
    };
}
namespace s2563o2
{   /* 枚举左边的 i，统计右边有多少个合法的 j
    枚举 i, 计算满足 j > i 且 nums[j] <= upper - nums[i] 的 j 的个数，记作 count(upper)。
    枚举 i, 计算满足 j > i 且 nums[j] < lower - nums[i]，也即nums[j] <= lower - 1 - nums[i]的j的个数
    记作count(lower - 1)，答案就是 count(upper) - count(lower - 1)。
    计算count(upper)的过程是O(n)的，注意，当i == j时要直接退出循环
    此时意味着对当前i，在右边找不到一个满足nums[j] <= upper - nums[i]的j
    而随着继续循环num[i]的增大，这个不等式将更加无法满足，而且如果继续循环j - i将变成负数
    */
    class Solution {
    public:
        long long countFairPairs(vector<int>& nums, int lower, int upper) {
            sort(nums.begin(), nums.end());
            auto count = [&](int upper) {
                long long res = 0;
                int j = nums.size() - 1;
                for (int i = 0; i < nums.size(); i++) {
                    while (j > i && nums[j] > upper - nums[i]) {
                        j--;
                    }
                    if (i == j) break;
                    res += j - i;
                }
                return res;// count函数的范围值是对每个i的的遍历nums[j] <= upper - nums[i]的总和
                };
            return count(upper) - count(lower - 1);// 将多个减法拆分成多个加法和一次减法
        }
    };
}
namespace s2563o3
{   // 依旧是两次相向双指针，但是写法跟模板更接近
    class Solution {
    public:
        long long countFairPairs(vector<int>& nums, int lower, int upper) {
            sort(nums.begin(), nums.end());
            auto count = [&](int upper) {
                long long res = 0;
                int i = 0, j = nums.size() - 1;
                while (i < j) {
                    if (nums[i] + nums[j] <= upper) {
                        res += j - i;
                        i++;
                    }
                    else {
                        j--;
                    }
                }
                return res;
                };
            return count(upper) - count(lower - 1);
        }
    };
}

// 相当于s2563的简化版，s2824和ls28一模一样
namespace s2824m1
{
    class Solution {
    public:
        int countPairs(vector<int>& nums, int target) {
            sort(nums.begin(), nums.end());
            int i = 0, j = nums.size() - 1;
            int ans = 0;
            while (i < j) {
                if (nums[i] + nums[j] >= target) {
                    --j;
                }
                else {
                    ans += j - i;
                    ++i;
                }
            }
            return ans;
        }
    };
}
namespace ls28m1
{
    class Solution {
    public:
        int purchasePlans(vector<int>& nums, int target) {
            const int mod = 1e9 + 7;
            long long ans = 0;
            int i = 0, j = nums.size() - 1;
            sort(nums.begin(), nums.end());
            while (i < j) {
                if (nums[i] + nums[j] > target) {
                    --j;
                }
                else {
                    ans += j - i;
                    ++i;
                }
            }
            return ans % mod;
        }
    };
}

// 模板题6：s167相向双指针的进阶，先固定一个nums[i]，然后让后面的两个数凑成0 - nums[i]，剩下就和s167一样
namespace s15o1
{   // 先排序，此时意味着三个下标i < j < k，等于相向双指针逐步随着i的遍历移动
    // 但是这一题比较棘手的是不允许出现重复的三元组，所以要考虑进行去重剪枝
    // 另外还能加入两个优化，进一步提升速度
    class Solution {
    public:
        vector<vector<int>> threeSum(vector<int>& nums) {
            sort(nums.begin(), nums.end());

            vector<vector<int>> ans;        // sort的时间复杂度为O(nlogn)
            int n = nums.size();
            for (int i = 0; i < n - 2; ++i) {// i遍历到n-3即可，因为后面需要留两个位置给j和k
                int x = nums[i];            // 外层循环固定第一个数O(n),内层双指针O(n)，总共O(n^2)
                if (i > 0 && x == nums[i - 1]) continue; 
                // 跳过重复数字(跳过重复数字这一步，要放在优化之前，省的重复进行优化的判定）
                if (x + nums[i + 1] + nums[i + 2] > 0) break; 
                // 优化一，当前最小值组合已超0，比if(x > 0) break的剪枝能力更强
                if (x + nums[n - 2] + nums[n - 1] < 0) continue; 
                // 优化二，当前最大值组合不足0

                int j = i + 1, k = n - 1;
                while (j < k) {
                    int s = x + nums[j] + nums[k];
                    if (s > 0) {
                        --k;
                    }
                    else if (s < 0) {
                        ++j;
                    }
                    else { // 三数之和为 0
                        ans.push_back({ x, nums[j], nums[k] });
                        for (++j; j < k && nums[j] == nums[j - 1]; ++j); // 跳过重复数字
                        // 在需要处理重复结果的题目中，排序后判断相邻元素是通用解法。
                        // 注意要先处理元素再判断重复，而不是先过滤重复元素。
                        // 超级简洁的写法，相当于：
                        //++j；while (j < k && nums[j] == nums[j-1]) ++j;
                        for (--k; k > j && nums[k] == nums[k + 1]; --k); // 跳过重复数字
                    }
                }
            }
            return ans;
        }
    };
}

// 与s15类似
namespace s16m1
{   // 用diff1记录比target大的偏差，diff2记录比target小的偏差
    // 个人感觉比o1的灵神写法要好一些，不用一直在循环内部更新ans
    class Solution {
    public:
        int threeSumClosest(vector<int>& nums, int target) {
            sort(nums.begin(), nums.end());
            int n = nums.size();
            int diff1 = INT_MAX, diff2 = diff1;
            for (int i = 0; i < n - 2; ++i) {
                int x = nums[i];
                if (i > 0 && nums[i - 1] == x) continue;// 优化1
                int s = x + nums[i + 1] + nums[i + 2];
                if (s > target) {
                    diff1 = min(diff1, s - target);
                    break;// 优化2
                }
                s = x + nums[n - 1] + nums[n - 2];
                if (s < target) {
                    diff2 = min(diff2, target - s);
                    continue;// 优化3
                }
                int comp = target - x;
                int j = i + 1, k = n - 1;
                while (j < k) {
                    s = nums[j] + nums[k];
                    if (s == comp) return target;
                    if (s > comp) {
                        diff1 = min(diff1, s - comp);
                        --k;
                    }
                    else {
                        diff2 = min(diff2, comp - s);
                        ++j;
                    }
                    
                }
            }
            return diff1 < diff2 ? target + diff1 : target - diff2;
        }
    };
}
namespace s16o1
{   // 三数之和的变体，需要一个min_diff来记录三数之和的差值，同时两个剪枝优化的思路也比较相似
    class Solution {
    public:
        int threeSumClosest(vector<int>& nums, int target) {
            sort(nums.begin(), nums.end());
            int ans = 0, n = nums.size();
            int min_diff = INT_MAX;
            for (int i = 0; i < n - 2; i++) {
                int x = nums[i];
                if (i > 0 && x == nums[i - 1]) {
                    continue; // 优化三
                }

                // 优化一
                int s = x + nums[i + 1] + nums[i + 2];
                if (s > target) { // 后面无论怎么选，选出的三个数的和不会比 s 还小
                    if (s - target < min_diff) {
                        ans = s; // 由于下面直接 break，这里无需更新 min_diff
                    }
                    break;
                }

                // 优化二
                s = x + nums[n - 2] + nums[n - 1];
                if (s < target) { // x 加上后面最大的两个数都不超过 target
                    if (target - s < min_diff) {
                        min_diff = target - s;
                        ans = s;
                    }
                    continue;
                }

                // 双指针
                int j = i + 1, k = n - 1;
                while (j < k) {
                    s = x + nums[j] + nums[k];
                    if (s == target) {
                        return target;
                    }
                    if (s > target) {
                        if (s - target < min_diff) { // s 与 target 更近
                            min_diff = s - target;
                            ans = s;
                        }
                        // 这里也可以去跳过重复的nums[j]和nums[k]，但是没有也差不多
                    }
                    else { // s < target
                        if (target - s < min_diff) { // s 与 target 更近
                            min_diff = target - s;
                            ans = s;
                        }
                    }
                }
            }
            return ans;
        }
    };
}

namespace s18m1
{   // 四数之和：先固定第一个数a，那么剩下的三个数就转为了三数之和问题
    // 但是此时三数之和的target变成了target - nums[a]
    // 其他剪枝操作都是和三数之和一样的
    class Solution {
    public:
        vector<vector<int>> fourSum(vector<int>& nums, int target) {
            vector<vector<int>> ans;
            int n = nums.size();
            sort(nums.begin(), nums.end());
            for (int a = 0; a < n - 3; ++a) {
                long long x = nums[a];
                if (a > 0 && x == nums[a - 1]) continue;
                if (x + nums[a + 1] + nums[a + 2] + nums[a + 3] > target) break;
                if (x + nums[n - 3] + nums[n - 2] + nums[n - 1] < target) continue;
                for (int b = a + 1; b < n - 2; ++b) {
                    long long y = nums[b];
                    if (b > a + 1 && y == nums[b - 1]) continue;
                    // 这里注意不要忘了，比如[2,2,2,2,2]，target = 8,如果这里没去重
                    // 那么第一个2与【第2,3,5】【第3,4,5】个2都会组成元组，那么此时就重复了
                    if (x + y + nums[b + 1] + nums[b + 2] > target) break;
                    if (x + y + nums[n - 2] + nums[n - 1] < target) continue;
                    int c = b + 1, d = n - 1;
                    while (c < d) {
                        long long s = x + y + nums[c] + nums[d];
                        if (s > target) {
                            --d;
                        }
                        else if (s < target) {
                            ++c;
                        }
                        else {
                            ans.push_back({ int(x), int(y), nums[c], nums[d] });
                            for (++c; c < d && nums[c] == nums[c - 1]; ++c);
                            for (--d; c < d && nums[d] == nums[d + 1]; --d);
                        }
                    }

                }
            }
            return ans;
        }
    };
}

// 类似三数之和s15，但是右往左遍历，从左往右遍历会漏掉元素
namespace s611m1
{   // 能组成三角形 等价于 剩余两边之和大于最长边
    class Solution {
    public:
        int triangleNumber(vector<int>& nums) {
            int ans = 0, n = nums.size();
            sort(nums.begin(), nums.end());
            for (int k = n - 1; k >= 2; --k) {
                int z = nums[k];
                if (nums[0] + nums[1] > z) {
                    ans += (k + 1) * k * (k - 1) / 6;
                    break;  // 优化一，从k + 1个元素里选3个组合，C(n,k)公式：n! / (k! * (n - k)!)
                }
                if (nums[k - 2] + nums[k - 1] <= z) continue;// 优化二
                int i = 0, j = k - 1;
                while (i < j) {
                    int sum = nums[i] + nums[j];
                    if (sum > z) {
                        ans += j - i;
                        --j;
                    }
                    else {
                        ++i;
                    }
                }
            }
            return ans;
        }
    };
}

// m1见专题10.1.1贪心策略-从最小/最大开始贪心，m1和m2思路差不多
namespace s948m2
{   
    class Solution {
    public:
        int bagOfTokensScore(vector<int>& tokens, int power) {
            if (tokens.empty()) return 0;
            int n = tokens.size();
            int left = 0, right = n - 1, ans = 0;
            sort(tokens.begin(), tokens.end());
            while (left <= right) {
                while (left <= right && power - tokens[left] >= 0) {
                    power -= tokens[left];
                    ++ans;
                    ++left;
                }
                if (ans > 0 && left < right) {
                    power += tokens[right] - tokens[left];
                    ++left;
                    --right;
                }
                else {
                    break;
                }
            }
            return ans;
        }
    };
}

// 模板题7：比较两个指针移动的后果，只移动可能更新答案的那一端
namespace s11m1
{   // 永远移动较矮的那一边，双指针遍历一次逐步更新答案
    // 如果保留较短的那根线：
    //          1.在left和right间找到了比短线更长的线，因为木桶原理，宽度减小，会比原来体积少
    //          2.在left和right中找到了比短线更短的线，更是比原来体积少
    // 所以只有可能保留left和right中的较长线，移动指针才有可能得到更大的答案
    class Solution {
    public:
        int maxArea(vector<int>& height) {
            int n = height.size();
            int left = 0, right = n - 1;
            int ans = 0;

            while (left < right) {
                int span = right - left;
                if (height[left] < height[right]) {
                    ans = max(ans, span * height[left]);
                    ++left;
                }
                else {
                    ans = max(ans, span * height[right]);
                    --right;
                }
            }
            return ans;
        }
    };
}

// 模板题8：接雨水
// o1：前/后缀最大值数组，判断每个高度的左右两边的木板高度，时间复杂度O(n)，但需要两个额外数组，空间复杂度O(n)
// o2：在o1为每个高度加左右木板的基础上，用相向双指针，将空间复杂度降至O(1)
namespace s42o1
{   // 前后缀分解做法好理解，也好写，但空间复杂度不是最优的
    class Solution {
    public:
        int trap(vector<int>& height) {
            int n = height.size();
            // preMx[i]表示preMx[0]到preMx[i]的最大值，闭区间
            vector<int> preMx(n);
            preMx[0] = height[0];
            // 端点不取，因为端点一定不能接雨水（虽然这里就算取了也不影响答案）
            for (int i = 1; i < n; ++i) {
                preMx[i] = max(height[i], preMx[i - 1]);
            }

            // sufMx[i]表示sufMx[i]到sufMx[n - 1]的最大值
            vector<int> sufMx(n);
            sufMx[n - 1] = height[n - 1];
            for (int i = n - 2; i >= 0; --i) {
                sufMx[i] = max(height[i], sufMx[i + 1]);
            }

            int ans = 0;
            for (int i = 0; i < n; ++i) {
                // 两边前后缀的最小值就是水位线，接雨水的量就是水位线减去柱子高度
                ans += min(preMx[i], sufMx[i]) - height[i];
            }
            return ans;
        }
    };
}
namespace s42o2
{
    class Solution {
    public:
        int trap(vector<int>& height) {
            int n = height.size();
            int preMx = 0, sufMx = 0, ans = 0;
            int left = 0, right = n - 1;

            while (left < right) { 
                // 只要一端找到了所有高度的最大值，就不会再移动了
                // 不需要写left <= right，那个位置一定是循环的终点，接不了水
                preMx = max(preMx, height[left]);
                sufMx = max(sufMx, height[right]);
                if (preMx < sufMx) {
                    ans += preMx - height[left];
                    ++left;
                }
                else {
                    ans += sufMx - height[right];
                    --right;
                }
            }
            return ans;
        }
    };
}
// ---------------------
// 【3.2】同向双指针 (1)
/*
611.有效三角形的个数：给定一个包含非负整数的数组 nums ，返回其中可以组成三角形三条边的三元组个数。
*/
// ---------------------
// m1为相向双指针的做法，o1为同向双指针
namespace s611o1
{  // 能组成三角形 等价于 剩余两边之差小于最短边
   // 这意味着，排序后固定a时，b和c之间不能离太远，也即用同向双指针解决，类似滑动窗口
    class Solution {
    public:
        int triangleNumber(vector<int>& nums) {
            sort(nums.begin(), nums.end());
            int n = nums.size(), ans = 0;
            for (int i = 0; i < n - 2; ++i) {
                if (nums[i] == 0) continue;// 必须加这个，否则对nums = [..., 0, 0, 0]，j会在下面while里越界或超过k
                int j = i + 1;
                for (int k = i + 2; k < n; ++k) {// 固定k，枚举j，反之不行
                    while (nums[k] - nums[j] >= nums[i]) {
                        ++j;
                    }
                    ans += k - j;
                }
            }
            return ans;
        }
    };
}
// ---------------------
// 【3.3】背向双指针 (0)
/*
*/
// ---------------------

// ---------------------
// 【3.4】原地修改 (12)
// 题目解出来都不难，但这类题型要求空间复杂度为O(1)
/*
27.移除元素：给你一个数组 nums 和一个值 val，你需要 原地 移除所有数值等于 val 的元素。元素的顺序可能发生改变。
然后返回 nums 中与 val 不同的元素的数量。假设 nums 中不等于 val 的元素数量为 k，要通过此题，您需要执行以下操作：
更改 nums 数组，使 nums 的前 k 个元素包含不等于 val 的元素。nums 的其余元素和 nums 的大小并不重要。返回 k。

26.删除有序数组中的重复项：给你一个 非严格递增排列 的数组 nums ，请你 原地 删除重复出现的元素，
使每个元素 只出现一次 ，返回删除后数组的新长度。元素的 相对顺序 应该保持 一致 。然后返回 nums 中唯一元素的个数。
考虑 nums 的唯一元素的数量为 k ，你需要做以下事情确保你的题解可以被通过：
更改数组 nums ，使 nums 的前 k 个元素包含唯一元素，并按照它们最初在 nums 中出现的顺序排列。
nums 的其余元素与 nums 的大小不重要。返回 k 。
*/
// ---------------------

namespace s27m1
{	// 双指针法，时间复杂度O(n)
    // 最多会遍历数组两遍
    class Solution {
    public:
        int removeElement(vector<int>& nums, int val) {
            int slowIndex = 0;
            for (int fastIndex = 0; fastIndex < nums.size(); fastIndex++) {
                if (nums[fastIndex] != val) {
                    nums[slowIndex++] = nums[fastIndex];
                }
            }
            return slowIndex;
        }
    };
}
namespace s27m2
{   // 库函数做法
    class Solution {
    public:
        int removeElement(vector<int>& nums, int val) {
            return remove(nums.begin(), nums.end(), val) - nums.begin();
        }
    };
}
namespace s27o1
{	// 这个随便看看就好
    // 优化双指针法，两个指针分别位于数组的两侧，普通双指针法位于数组同侧
    // 最多只会遍历数组一遍
    class Solution {
    public:
        int removeElement(vector<int>& nums, int val) {
            int left = 0, right = nums.size();
            while (left < right) {
                if (nums[left] == val) {
                    nums[left] = nums[right - 1];
                    --right;
                }
                else {
                    ++left;
                }
            }
            return left;
        }
    };
}

namespace s26m1
{	// 双指针
    class Solution {
    public:
        int removeDuplicates(vector<int>& nums) {
            int left = 0;
            for (int right = 1; right < nums.size(); ++right)
            {
                if (nums[right] != nums[right - 1])
                {	// 如果没碰到重复元素，那么对left指针进行更新
                    nums[++left] = nums[right];
                }
            }
            return left + 1;
        }
    };
}
namespace s26m2
{   // 库函数做法
    class Solution {
    public:
        int removeDuplicates(vector<int>& nums) {
            return unique(nums.begin(), nums.end()) - nums.begin();
        }
    };
}

// m1：类似s26，但是用cnt进行记录是否出现超过两次
// o1：用栈思考，将nums数组当作栈，stackSize表示栈的大小，初始值为2
namespace s80m1
{
    class Solution {
    public:
        int removeDuplicates(vector<int>& nums) {
            int slow = 0, fast = 1;
            int cnt = 0;
            for (int fast = 1; fast < nums.size(); ++fast) {
                if (nums[fast] != nums[slow]) {
                    nums[++slow] = nums[fast];
                    cnt = 0;// 用cnt记录次数
                }
                else if (++cnt <= 1) {// 如果要求最多重复三次，这里改成<= 2即可
                    nums[++slow] = nums[fast];
                }
            }
            return slow + 1;
        }
    };
}
namespace s80o1
{   // 用nums[stackSize - 2]记录栈顶下方的那个数
    class Solution {
    public:
        int removeDuplicates(vector<int>& nums) {
            int stackSize = 2;// 栈的大小，前两个元素默认保留
            int n = nums.size();
            for (int i = 2; i < n; ++i) {
                if (nums[i] != nums[stackSize - 2]) {
                    ;// 和栈顶下方元素比较
                    nums[stackSize++] = nums[i];// 入栈
                }
            }
            return min(stackSize, n);// 可能nums.size() == 1,也可以在最前面进行判断
        }
    };
}

namespace s283m1
{	// 思路为先用双指针先把非零的元素移到数组前面，然后再遍历剩余的元素，全部赋零
    class Solution {
    public:
        void moveZeroes(vector<int>& nums) {
            int slow = 0, n = nums.size();
            for (int fast = 0; fast < n; ++fast) {
                if (nums[fast] != 0) {
                    nums[slow++] = nums[fast];
                }
            }
            for (; slow < n; ++slow) {
                nums[slow] = 0;
            }
        }
    };

}
namespace s283m2
{   // 库函数做法，思路和m1一样先移动后赋0，remove的底层也是快慢双指针
    class Solution {
    public:
        void moveZeroes(vector<int>& nums) {
            auto it = remove(nums.begin(), nums.end(), 0);
            for (; it != nums.end(); ++it) {
                *it = 0;
            }
        }
    };
}
namespace s283o1
{	// 用swap来移动元素，代码更简洁，效率视交换次数，如果元素全部非零
    // 那么所有元素都要和自己交换一次，速度可能没有m1快
    class Solution {
    public:
        void moveZeroes(vector<int>& nums) {
            int slow = 0, n = nums.size();
            for (int fast = 0; fast < n; ++fast) {
                if (nums[fast] != 0) {
                    swap(nums[slow++], nums[fast]);
                }
            }
        }
    };
}

// m1用swap，同向双指针，至少要遍历两次
// o1用相向双指针，只用遍历一次
namespace s905m1
{
    class Solution {
    public:
        vector<int> sortArrayByParity(vector<int>& nums) {
            int slow = 0, n = nums.size();
            for (int fast = 0; fast < n; ++fast) {
                if (nums[fast] % 2 == 0) {
                    swap(nums[slow++], nums[fast]);
                }
            }
            return nums;
        }
    };
}
namespace s905o1
{
    class Solution {
    public:
        vector<int> sortArrayByParity(vector<int>& nums) {
            int left = 0, right = nums.size() - 1;
            while (left < right) {
                if (nums[left] % 2 == 0) {
                    ++left;//寻找左边的奇数
                }
                else if (nums[right] % 2 == 1) {
                    --right;// 寻找右边的偶数
                }
                else {
                    swap(nums[left], nums[right]);
                    ++left;// 交换后问题变成 [left+1,right-1] 的子问题
                    --right;
                }
            }
            return nums;
        }
    };
}

// m1：模仿s905o1
namespace s922m1
{   // 将left，和right改成同向的也行，思想类似
    class Solution {
    public:
        vector<int> sortArrayByParityII(vector<int>& nums) {
            int n = nums.size();
            int left = 0, right = n - 1;
            while (left < n) {// 因为题干说了必定奇偶个数必定55开，如果全部都是在正确的位置上，一定是left先越界
                if (nums[left] % 2 == 0) {// 先判定的left，如果先判定right，那么while循环条件改成right > 0 
                    left += 2;
                }
                else if (nums[right] % 2 == 1) {
                    right -= 2;
                }
                else {
                    swap(nums[left], nums[right]); 
                    left += 2;
                    right -= 2;
                }
            }
            return nums;
        }
    };
}

// 模板题9：将少数几种固定值进行排序（荷兰国旗问题），最容易想到的方法是两次双指针swap遍历或计数排序，但可以优化成单次遍历
// m1为两次双指针swap遍历，o1为插入排序法原地修改
namespace s75m1
{
    class Solution {
    public:
        void sortColors(vector<int>& nums) {
            int r = 0, n = nums.size();
            for (int i = 0; i < n; ++i) {
                if (nums[i] == 0) {
                    swap(nums[r++], nums[i]);
                }
            }
            for (int j = r; j < n; ++j) {
                if (nums[j] == 1) {
                    swap(nums[r++], nums[j]);
                }
            }
        }
    };
}
namespace s75m2
{   // 计数排序做法，也要遍历两次
    class Solution {
    public:
        void sortColors(vector<int>& nums) {
            vector<int> cnt(3, 0);
            for (int num : nums) {
                ++cnt[num];
            }

            int index = 0;
            for (int i = 0; i < 3; ++i) {
                while (cnt[i]-- > 0) {
                    nums[index++] = i;
                }
            }
        }
    };
}
namespace s75o1
{
    class Solution {
    public:
        void sortColors(vector<int>& nums) {
            // 如果要向一个已经按照0, 1, 2排列的数列里插入一个0，那么：
            // p0指向下一个应该放置0的位置，在[0, p0-1]都是0
            // p1指向下一个应该放置1的位置，在[p0, p1 - 1]都是1
            // i是当前遍历到的元素的索引，[p1,i]的区域在处理时会被暂时标记成2，或者包含那些尚未被正确归位的0和1的原始值

            // 相当于给三种元素各自计数
            // 先把当前位置无脑变成2，然后p0和p1追在i后面完成各自覆盖1或0的使命
            int p0 = 0, p1 = 0;
            // 先默认[0, i - 1]是有序的
            for (int i = 0; i < nums.size(); i++) {
                int x = nums[i];// 保存当前元素原始值
                nums[i] = 2;// 假设当前元素是2
                // 如果x是2，那没有变化
                // 如果x是0或1，那么这个2是临时的占位符，后续会被覆盖

                if (x <= 1) {
                    // 当前位置原始值是0或1
                    // 在p1指向的位置放一个1
                    // 原始在p1处的值，之前被临时改为2，现在被1覆盖了
                    // p1向右移动，为下一个1预留空间
                    nums[p1++] = 1;
                }
                // 【注意】这里不能改成if else，就是要两个if都判断，哪怕是x == 0也得把p1++一次
                if (x == 0) {
                    // 当前位置原始值为0
                    // 在p0指向的位置放一个0
                    // 原始在p0处的值，之前被临时改为2和1，现在被0覆盖了
                    // p0向右移动，为下一个0预留空间
                    nums[p0++] = 0;
                }
            }
        }
    };
}

// 模板题10：数字搬家/原地哈希，O(1)空间复杂度，涉及循环追踪和交换，出现概率和实用性较低
namespace s1920m1
{   // 非原地修改，空间复杂度为O(n)
    class Solution {
    public:
        vector<int> buildArray(vector<int>& nums) {
            int n = nums.size();
            vector<int> ans(n);
            for (int i = 0; i < n; ++i) {
                ans[i] = nums[nums[i]];
            }
            return ans;
        }
    };
}
namespace s1920o1
{   // 每个位置都有对应的一个元素，只是被置换打乱了，通过循环追踪进行还原
    // [1, 2, 0, 4, 3]，第一个循环链是[1, 2, 0]，结束后开始第二个循环链[4, 3]，答案为[2,0,1,3,4]
    class Solution {
    public:
        vector<int> buildArray(vector<int>& nums) {
            int n = nums.size();
            for (int i = 0; i < n; ++i) {
                int num = nums[i];// 记录原始值
                if (num < 0) continue;// 已经处理过了
                int curIndex = i;
                while (i != nums[curIndex]) {// 当前小循环链没有追踪回到起点
                    int nxtIndex = nums[curIndex];// 找到当前元素对应的追踪搬家位置
                    nums[curIndex] = ~nums[nxtIndex];// 把nxt处元素搬到当前cur处并取反，要+1后再取反，因为nums[i]可能为0
                    curIndex = nxtIndex;// 将cur处的原始值当成当前下标继续追踪
                }
                nums[curIndex] = ~num;// 回到这一组小循环链的起点后，把原始值 x=nums[i] 搬过来结束小循环链
            }
            for (int& x : nums) {
                x = ~x;// 复原
            }
            return nums;
        }

    };
}

// 模板题11：原地哈希应用，o1解法不涉及数字搬家，只是将数组当作哈希简单使用，比较好理解
// o2解法和1920类似，将每个数字搬到对应的位置，但和模板题10还是有所不同
// s442可以将s448和s41联系起来，有两种解法，s448是数组当作哈希表，s41是数字搬家
namespace s442o1
{   // 将数组当成哈希表，遍历每个元素，把其当作key，查询其对应的value，查到后就置为负数
    // 如果能查询后发现为负数，就说明是第二次查询到，对应的key出现过两次（题干中信息为元素只出现一次或二次）
    // 只是将数组当作哈希表，不会对数组本身元素顺序进行改变，直接看上去还是乱的
    class Solution {
    public:
        vector<int> findDuplicates(vector<int>& nums) {
            vector<int> ans;

            for (int x : nums) {
                int key = abs(x) - 1;// 这里也要取绝对值，因为可能num值为负数
                if (nums[key] > 0) {
                    nums[key] *= -1;// 将当前key指向的value标为负数
                }
                else {
                    ans.push_back(key + 1);// 不要push_back(num)，因为此时num可能是负数
                }
            }
            return ans;
        }
    };
}
namespace s442o2
{   // 把每个数字放到它"应该在的位置"(即nums[i]应当放在nums[nums[i]-1]处)
    // 只要没找到数字会一直在当前元素位置产生swap
    // 比如431278231，在i = 0的位置要换好几次
    // 431278321 -> 231478321 -> 321478321 -> 123478321，之后就是连续continue调到i = 5处
    // 发现重复元素后就将其标记为负数，表示这个数是重复的，后面不用再处理
    // 最终数组中各元素会各自归位，对应位子上如果缺少没有对应元素，那么应该是放了一个重复的负数
    // 比如 1, 2, 3, 4, -2, 6, -1
    class Solution {
    public:
        vector<int> findDuplicates(vector<int>& nums) {
            int n = nums.size();
            vector<int> ans;
            for (int i = 0; i < n; ++i) {
                int num = nums[i];         
                int index = num - 1;// 当前元素对应的正确下标

                // num < 0说明num处元素已经处理过了，index == i说明数字已经放在对的位置上了，比如在索引3的位置上数字是4
                if (num < 0 || index == i) continue;  

                // 如果代码能运行到这里，说明要么发生了重复，要么需要换位，且index != i
                if (nums[index] == num) {   // 如果发生重复，比如[3,1,3,4,2]，3应该在nums[2]   
                    ans.push_back(num);       // 想要进行换位，但查询过去发现nums[2] == 3，相换的位置上已经相同元素了
                    nums[i] *= -1;          // 说明重复，将当前元素标记为负数，说明是重复元素，后面也不用再处理
                }
                else {
                    swap(nums[index], nums[i]);// 将当前i处的元素x换到x - 1处，放到其应该在的位置
                    --i;// 把元素换过来之后，换来的元素还没处理过，--i用来抵消++i，接着处理
                }
            }
            return ans;
        }
    };
}

// 模板题11衍生，解法和s442o1类似，比较好理解，依旧是原地修改，将数组当作哈希表，用负数标记是否访问过
namespace s448o1
{   //逐个遍历元素，将其当作key，将key下标处的元素值value改成负数，代表已经访问过
    //那么遍历结束后，元素值依旧为正数的地方代码没有对应的key来访问它，也就是正数元素的下标是缺少的
    class Solution {
    public:
        vector<int> findDisappearedNumbers(vector<int>& nums) {
            vector<int> ans;
            int n = nums.size();
            for (int i = 0; i < n; ++i) {
                int num = nums[i];
                int index = abs(num) - 1;
                if (nums[index] > 0) {
                    nums[index] *= -1;
                }

            }
            for (int i = 0; i < n; ++i) {
                if (nums[i] > 0) {
                    ans.push_back(i + 1);
                }
            }
            return ans;
        }
    };
}

// 原地哈希修改，集大成之题，将每个元素放到原本应该在的位置上，然后再遍历一次下标，对应不上的就是答案
namespace s41o1
{   // 这道题没有值域在[1, n]的条件了，值域在[INT_MIN, INT_MAX]
    // 所以不能用数组当哈希表的做法了，因为会下标越界，只能用数字搬家的做法
    class Solution {
    public:
        int firstMissingPositive(vector<int>& nums) {
            int n = nums.size();
            // 从下标0，数值1开始一个个搬家，
            for (int i = 0; i < n; ++i) {// 极端情况在i = 0时就把所有元素都归位了，但总时间复杂度仍为O(n)
                // while的判断条件也要注意，不要写nums[i] - 1 != i，可能越界
                while (nums[i] != i + 1) {// 逐个把其他的元素换过来并将其归位（i = 0时对应1，第一个正数）
                    if (nums[i] > n || nums[i] <= 0 || nums[i] == nums[nums[i] - 1]) {
                        // 前面两个条件排除[1, n]之外的nums[i]，转换成普通的数字搬家
                        // 因为是找第一个缺失的正数，所以当nums[i] <= 0 或 > n时，值都是没有意义的（不在答案范围内）
                        // 或者从下面swap代码角度看，不break就越界了
                        
                        // nums[i] == nums[nums[i] - 1]意味着换过来也等于白换，比如[1, 3, 3]中的3，3组合
                        // 也即swap的二者是相同的，且其一原本就在正确的位置上，如果不break就死循环
                        // 本质swap是不断通过换其他位置的元素过来看是否能将其复原，如果换过来的是同样的数
                        // 自然就不需要进行swap了，否则会因为一直满足外面的while条件陷入死循环
                        // 只要出现break就说明找到了当前i下循环搬家的链条已经断了，往下一个i尝试，退出while循环
                        break;
                    }
                    swap(nums[i], nums[nums[i] - 1]);
                }
            }
            /*
            其实就相当于这段
            for (int i = 0; i < n; ++i) {
                int num = nums[i];
                int index = num - 1;// 但这里注意，num[i]的范围是(int)-2^32 ~ 2^32 - 1，这里再减1可能会溢出int范围，解决办法是在进入while前判断一下index是否在[1, n]内
                while (i != index) {// 所以还是按照上面的写法来，虽然可读性差些，但是稳健些
                    num = nums[i];
                    index = num - 1;
                    if (index < 0 || index >= n || nums[index] == num) {
                        break;
                    }
                    swap(nums[i], nums[index]);
                }
            }
            */
            // 第一个搬家没对应上的就是答案
            for (int i = 0; i < n; ++i) {
                if (nums[i] != i + 1) {
                    return i + 1;
                }
            }
            // 说明[1, n]的元素都归位了，那么最小的缺失的正数就是n + 1
            return n + 1;
        }
    };
}

// 模板题11衍生，但如果不能修改原数组的话，需要新的解法，见o1，当作基环树（图），思路同s142 环形链表II
namespace s287m1
{
    class Solution {
    public:
        int findDuplicate(vector<int>& nums) {
            int n = nums.size();
            for (int i = 0; i < n; ++i) {
                int num = nums[i];
                int index = abs(num) - 1;
                if (nums[index] < 0) {
                    return index + 1;// 第二次访问到，说明重复
                }
                nums[index] *= -1;
            }
            return -1;
        }
    };
}
namespace s287o1
{   // 相当于有向图，等效于数组哈希的另一种角度，把每个元素当成链表节点，nums[i]指向nums[nums[i]]
    // 如果有一个重复元素，那么这个链表一定是有环的，而环的入口出下标的元素就是重复元素

    // 每个节点的入度，就是这个节点在 nums 中的出现次数。重复元素的入度大于1
    // 在每个节点的出度都是 1 的情况下，n+1 个点 n+1 条边的有向图，又叫内向基环森林，每个连通块都恰好有一个环
    // 由于 nums[i]≥1，所以节点 0 的入度是 0，不在环上。
    // 从节点 0 出发，进入基环，基环的入环口，就是入度大于 1 的节点。
    // 以nums = [1, 3, 4, 2, 2]为例，nums[i]指向nums[nums[i]]
    // 其结构为：0->1->3->2-><-4，环入口即重复元素2
    // 
    // 注意：如果从非 0 节点出发，可能无法找到答案，比如[1,2,1,3,4]，可以得到0-1-2和3-4两段链表
    // 如果从节点3或4出发，只会在3,4节点直接无限循环，得到两个入度为1的节点，无法找到入度为2的节点1
    // 只要从节点0出发，就相当于从链表的头开始出发，一定不会漏掉答案
    // （哪怕只能遍历一部分数组，数组在中间断开成好几段，入度为2的点也一定在最左边断掉的那段）

    // 既然是找环的入口，那么可以利用s142环形链表II的思路，用快慢指针遍历节点，下面是两者的对应关系：
    // 链表节点         node	                i（注意，如果是从0开始的，node不是对应nums[i]，但一旦进入环，那就是nums[i]）
    // 下一个节点    node.next	               nums[i]（这个操作对应关系只是相对于i的，真正的对应关系是将node当作nums的下标）
    // 头节点          head	                    0       （可以看看代码就明白了，循环追踪生成环路）
    // 入环口	                重复元素

    // 代码逻辑同 142. 环形链表 II
    class Solution {
    public:
        int findDuplicate(vector<int>& nums) {
            int slow = 0, fast = 0; // 0 一定不在环上，适合作为起点
            while (true) {// 这个循环可以改成do while版本，见m2
                slow = nums[slow]; // 等价于 slow = slow.next
                fast = nums[nums[fast]]; // 等价于 fast = fast.next.next
                if (fast == slow) { // 快慢指针移动到同一个节点
                    break;
                }
            }

            int head = 0; // 再用一个指针，从起点出发
            while (slow != head) {
                slow = nums[slow];
                head = nums[head];
            }
            return slow; // 入环口即重复元素，如果这里还是不确定，可以用[1,2,1]这个例子快速验证一下（得画图）
            // 最后结构是0 -> 1 <-> 2，slow和fast在2相遇，然后head = 0, slow = 2时各走一步到1，也就是答案1
            // 除了0之外，环路上的元素因为是套nums取值外壳循环追踪而来，都是nums数组中的元素值，也就是答案求的，直接返回slow
        }
    };
}
namespace s287m2
{   // 改成do-while版本，while true确实有点扎眼
    class Solution {
    public:
        int findDuplicate(vector<int>& nums) {
            int slow = 0;
            int fast = 0;
            do {
                fast = nums[nums[fast]];
                slow = nums[slow];
            } while (slow != fast);

            int head = 0;
            while (head != slow) {
                slow = nums[slow];
                head = nums[head];
            }
            return slow;
        }
    };
}