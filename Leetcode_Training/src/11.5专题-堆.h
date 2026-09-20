#pragma once
#include <algorithm> // make_heap, push_heap, pop_heap, sqrt, nth_element
#include <numeric>   // accumulate
#include <vector>
#include <queue>     // priority_queue
#include <unordered_map>    
#include <tuple>

using namespace std;

// 问题待定：
/*
sxxx：xxx
*/

/*
模板题：
1.xxx：no.x

*/

// 堆（优先队列）：基础 + 进阶 + 第K小/大 + 重排元素 + 反悔堆 + 懒删除堆 + 对顶堆（滑动窗口第 K 小/大）

// 五、堆 (5)

// 【5.1】基础 (2)
// 熟悉priority_queue的基本使用和性质，并额外熟悉原地堆化的使用（make_heap)
/*
2558.从数量最多的堆取走礼物：给你一个整数数组 gifts ，表示各堆礼物的数量。每一秒，你需要执行以下操作：
选择礼物数量最多的那一堆。如果不止一堆都符合礼物数量最多，从中选择任一堆即可。
将堆中的礼物数量减少到堆中原来礼物数量的平方根，向下取整。返回在 k 秒后剩下的礼物数量。

703.数据流中的第 K 大元素：设计一个找到数据流中第 k 大元素的类（class）。
注意是排序后的第 k 大元素，不是第 k 个不同的元素。
请实现 KthLargest 类：
KthLargest(int k, int[] nums) 使用整数 k 和整数流 nums 初始化对象。
int add(int val) 将 val 插入数据流 nums 后，返回当前数据流中第 k 大的元素。
*/
// ---------------------
// 模板1：熟悉priority_queue + queue的基本使用和性质，可以对比着看
namespace s2558m1
{   // 之前用过一点优先队列，这是自己写出来的
    class Solution {
    public:
        long long pickGifts(vector<int>& gifts, int k) {
            priority_queue<int, vector<int>> pq;
            for (int x : gifts) {
                pq.push(x);
            }

            while (k--) {
                int x = pq.top();
                pq.pop();
                x = static_cast<int>(sqrt(x));// 这里不进行显式类型转换也会发生隐式类型转换
                pq.push(x);
            }

            long long ans = 0;
            while (!pq.empty()) {
                ans += pq.top();
                pq.pop();
            }
            return ans;
        }
    };
}
namespace s2558o1
{   // 用原地堆化可以降低空间占用（但注意面试时要问问是否可以修改输入数组）
    class Solution {
    public:
        long long pickGifts(vector<int>& gifts, int k) {
            make_heap(gifts.begin(), gifts.end());
            // 额外优化：当堆顶元素为1时，后续k再减小堆内元素也不会再改变
            while (k-- && gifts[0] > 1) {
                // 弹出堆顶并移到末尾
                pop_heap(gifts.begin(), gifts.end());
                gifts.back() = sqrt(gifts.back());
                // 把末尾元素入堆
                push_heap(gifts.begin(), gifts.end());
            }
            return accumulate(gifts.begin(), gifts.end(), 0LL);
        }
    };
}

// 模板2：优先队列经典TopK题，利用堆性质设计一个类，可以随时返回第K大的元素
namespace s703o1
{   // 用最小堆（小根堆）储存nums中的k个元素，堆顶一定就是k个元素中最小的，也就是第k大的元素
    class KthLargest {
    private:
        priority_queue<int, vector<int>, greater<int>> pqMin;
        int k;

    public:
        KthLargest(int k, vector<int>& nums) : k(k) {
            for (int x : nums) {
                add(x);
            }
        }

        int add(int val) {
            pqMin.push(val);
            if (pqMin.size() > k) {
                pqMin.pop();// 此时堆顶存放的是第k + 1大的元素，不需要，pop掉
            }
            return pqMin.top();
        }
    };
}
// ---------------------
// 【5.2】进阶 ()
// 
/*


*/
// ---------------------

// ---------------------
// 【5.3】第 K 小/大 (2)
// 
/*
215.数组中的第K个最大元素：给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。
请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。
你必须设计并实现时间复杂度为 O(n) 的算法解决此问题。

347.前 K 个高频元素：给你一个整数数组 nums 和一个整数 k ，请你返回其中出现频率前 k 高的元素。
你可以按 任意顺序 返回答案。
*/
// ---------------------
// 模板题3：最简单的思路是小顶堆，但是时间复杂度超了。常规做法是快速选择法，是快速排序的变体（也可以用三路优化）
namespace s215o1
{   // 最简单的用优先队列(最小堆)的做法，但是不仅时间复杂度O(nlogn)，也用额外的O(k)空间
    int findKthLargest(vector<int>& nums, int k) {
        // 创建一个最小堆：使用 priority_queue 并指定比较函数为 greater<int>
        // 这样堆顶总是最小的元素
        priority_queue<int, vector<int>, greater<int>> minHeap;

        // 遍历数组中的每个元素
        for (int num : nums) {
            // 将当前元素推入堆中
            minHeap.push(num);

            // 如果堆的大小超过 k，弹出堆顶（最小的元素）
            // 这样可以确保堆中只保留最大的 k 个元素
            if (minHeap.size() > k) {
                minHeap.pop();
            }
        }
        /*  
        // 下面的生成堆做法，push和pop的操作更少，只在必要时操作堆，效率更高
        for (int num : nums) {
            if (minHeap.size() < k) {
                minHeap.push(num);
            } else {
                if (num > minHeap.top() {
                    minHeap.pop();
                    minHeap.push(num);
                }
            }
        }
        */

        // 此时堆顶就是第 k 个最大的元素
        return minHeap.top();
    }   
}
namespace s215o2
{   // 更常用的做法是快速选择法（QuickSelect），本版是最简单最容易记的三路选择版本，时间复杂度O(n)，空间复杂度O(logn)
    // 基于三路快速排序，将数组分成三份，不是基于分治法（继续处理大于小于的两份）
    // 而是减治法，“只”处理需要的那“一份”，相较于快速排序，因为中途只要找到了值就可以提前退出
    // 所以时间复杂度从O(nlogn)减小到了O(n)，算法思想是相同的，只是因为应用场景不同时间复杂度发生变化
    // o2写法虽然好记，但是用到了额外的数组空间记录三路，更多的优化见o3和o4版本
    // 但是如果面试时要求不能修改输入数组，就直接用o2解法
    class Solution {
    public:
        int findKthLargest(vector<int>& nums, int k) {
            return quickSelect(nums, k);
        }

    private:
        // 注意：这里k代表nums中的topk大，后续递归更改nums为small时，k也需要相应改变
        int quickSelect(vector<int>& nums, int k) {
            // 随机选择基准数
            int pivot = nums[rand() % nums.size()];
            // 将大于、小于、等于 pivot 的元素划分至 big, small, equal 中
            vector<int> big, equal, small;
            for (int num : nums) {
                if (num > pivot)
                    big.push_back(num);
                else if (num < pivot)
                    small.push_back(num);
                else
                    equal.push_back(num);
            }
            // 第 k 大元素在 big 中，递归划分
            if (k <= big.size())
                return quickSelect(big, k);
            // 第 k 大元素在 small 中，递归划分
            if (nums.size() - small.size() < k)
                return quickSelect(small, k - nums.size() + small.size());
            // 第 k 大元素在 equal 中，直接返回 pivot
            return pivot;
        }
    };
}
namespace s215o3
{   // 用0.5排序算法s0_5_5o3中的三路选择模板进行修改，避免了使用额外三路数组，但是仍然有O(logn)的递归栈调用
    class Solution {
    public:
        int findKthLargest(vector<int>& nums, int k) {
            int n = nums.size();
            return quickSelect(nums, 0, n - 1, k);
        }

    private:
        // 参数k说明求[low, high]中的第k大值，所以如果向小于pivot的区间递归时，要修改k
        int quickSelect(vector<int>& nums, int low, int high, int k) {
            if (low == high) return nums[low];

            int pivotIndex = low + rand() % (high - low + 1);
            swap(nums[low], nums[pivotIndex]);

            int pivot = nums[low];

            // 三路分区：维护三个指针
            int lt = low;      // 小于pivot的右边界
            int gt = high;     // 大于pivot的左边界
            int i = low + 1;   // 当前检查的元素

            while (i <= gt) {
                if (nums[i] < pivot) {
                    swap(nums[i], nums[lt]);
                    ++i;
                    ++lt;
                }
                else if (nums[i] > pivot) {
                    swap(nums[i], nums[gt]);
                    --gt;
                }
                else {
                    ++i;
                }
            }

            // 现在数组分为：[low, lt-1] < pivot, [lt, gt] == pivot, [gt+1, high] > pivot
            int greaterCnt = high - gt;
            int equalCnt = gt - lt + 1;
            if (k <= greaterCnt) {
                return quickSelect(nums, gt + 1, high, k);
            }
            else if (k <= greaterCnt + equalCnt) {
                return pivot;
            }
            else {
                // 已经可以从k个中去掉greaterCnt + equalCnt了，这些元素比topk都要大
                return quickSelect(nums, low, lt - 1, k - greaterCnt - equalCnt);
            }
        }
    };
}
namespace s215o4
{   // 进一步优化，省去了递归栈的空间开销，做到空间复杂度O(1)，也是我目前觉得最合适的面试写法
    class Solution {
    public:
        int findKthLargest(vector<int>& nums, int k) {
            int n = nums.size();
            int low = 0, high = n - 1;

            while (low <= high) {
                // 随机选择基准避免最坏情况
                int pivotIdx = low + rand() % (high - low + 1);
                swap(nums[low], nums[pivotIdx]);

                int pivot = nums[low];

                // 三路分区：维护三个指针
                int lt = low;    // 小于pivot的右边界
                int gt = high;   // 大于pivot的左边界
                int i = low + 1; // 当前检查的元素

                while (i <= gt) {
                    if (nums[i] < pivot) {
                        swap(nums[i], nums[lt]);
                        ++i;
                        ++lt;
                    }
                    else if (nums[i] > pivot) {
                        swap(nums[i], nums[gt]);
                        --gt;
                        // 注意：这里不增加i，因为交换过来的新元素需要重新检查
                    }
                    else {
                        ++i;// 等于pivot，直接跳过
                    }
                }

                // 现在数组分为：[low, lt-1] < pivot, [lt, gt] == pivot, [gt+1, high] > pivot
                int greaterCnt = high - gt;
                int equalCnt = gt - lt + 1;
                if (k <= greaterCnt) {
                    low = gt + 1;// 更新查询范围
                }
                else if (k <= greaterCnt + equalCnt) {
                    return pivot; // k 在等于 pivot 的区间内
                }
                else {
                    k -= greaterCnt + equalCnt;
                    high = lt - 1;// 更新查询范围，左边的greaterCnt + equalCnt个元素也要从k个里面去除
                }
            }
            return -1; // 无效输入
        }
    };
}
namespace s215o5
{   // 库函数写法，仅作了解
    /*
    // 默认使用 小于号 < 进行比较
    void nth_element (RandomAccessIterator first,
                     RandomAccessIterator nth,
                     RandomAccessIterator last);

    // 可以自定义比较规则
    void nth_element (RandomAccessIterator first,
                     RandomAccessIterator nth,
                     RandomAccessIterator last,
                     Compare comp);
    // 或调用库比较函数
    // 如：找第3大的数（索引为2），使用 greater<int>() 使大的数排在前面
    std::nth_element(v.begin(), v.begin() + 2, v.end(), std::greater<int>());

    nth_element把序列分成了三个部分，并且保证了中间那个元素（即 nth 指向的元素）处于绝对正确的位置
    可以用来求TOPK大/小问题，或者是中位数
    */
    class Solution {
    public:
        int findKthLargest(vector<int>& nums, int k) {
            // 其内部实现通常就是快速选择法，时间复杂度O(n)
            nth_element(nums.begin(), nums.end() - k, nums.end());
            return nums[nums.size() - k];
        }
    };
}

// 模板题4：TopK变体，权重从大小换成频次，最小堆依旧能做，但是更好的做法是桶排序，可以实现O(n)时空复杂度
namespace s347m1
{   // 这份垃圾代码没想到真通过了，总时间复杂度O(n + nlogk)，空间复杂度O(n + k)
    class Solution {
    public:
        vector<int> topKFrequent(vector<int>& nums, int k) {
            // 生成频率图，时间复杂度O(n)
            unordered_map<int, int> freqMp;
            for (int num : nums) {
                ++freqMp[num];
            }

            // 生成最小堆，时间复杂度O(k + (n - k)logk)，化简后为O(nlogk)
            auto cmp = [](const auto& a, const auto& b) { return a.second > b.second; };
            /* 如果改成下面这样的自定义比较规则，则可以节省优先队列的空间，直接定义成int, vector<int>
               但是题干要求的是只返回频率Map的key，不需要重复，比如[1, 1, 1, 1], k = 2;
               只需要返回{1}，而不是{1, 1}，所以最小堆做法还是用原本的cmp才行
            auto cmp = [&](const int a, const int b) {
                return freqMap[a] > freqMap[b];
            };
            */
            priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> minHeap(cmp);
            for (auto& p : freqMp) {
                minHeap.push(p);

                if (minHeap.size() > k) {
                    minHeap.pop();
                }
            }

            // 生成答案，时间复杂度为O(klogk)
            vector<int> ans;
            ans.reserve(k);
            while (!minHeap.empty()) {
                ans.push_back(minHeap.top().first);
                minHeap.pop();
            }
            return ans;
        }
    };
}
namespace s347o1 
{   // 利用元素频率天然小于数组大小，根据频次确定桶的数量实现O(n)时空复杂度
    // 解法具备桶排序的思想和框架，但每个桶内元素不进行排序
    class Solution {
    public:
        vector<int> topKFrequent(vector<int>& nums, int k) {
            unordered_map<int, int> freqMap;
            int mxFreq = 0;
            for (int num : nums) {
                ++freqMap[num];
                mxFreq = max(mxFreq, freqMap[num]);
            }

            // 也可以直接定义桶的大小为n + 1，因为最大频率不超过数组长度
            // 如果数组很大但最大频次很小，可以稍微优化空间，就像下面这样
            vector<vector<int>> buckets(mxFreq + 1);
            for (auto& [val, freq] : freqMap) {
                buckets[freq].push_back(val);
            }

            vector<int> ans;
            ans.reserve(k);
            for (int i = mxFreq; i > 0 && ans.size() < k; --i) {
                ans.insert(ans.end(), buckets[i].begin(), buckets[i].end());
            }
            /* 
            注意题目保证答案唯一，一定会出现某次 insert 后 ans.size() 恰好等于 k 的情况，所以可以直接insert
            但如果答案不唯一，就要像下面这样写，通过内外循环双重判断条件确保只从buckets中取k个元素
            for (int i = mxFreq; i > 0 && ans.size() < k; --i) {
                for (int num : buckets[i]) {
                    ans.push_back(num);
                    if (ans.size() == k) break; // 桶内可能存在多个相同频率的元素
                }
            }
            */
            return ans;
        }
    };
}

// 模板题5：经典k路归并，用小顶堆做，同时记录k行
namespace s373m1
{   // 看了思路后自己写出来的，更优化的写法见o1
    class Solution {
    public:
        vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
            vector<vector<int>> ans;
            auto cmp = [](const auto& a, const auto& b) {return a[0] > b[0]; };
            priority_queue<vector<int>, vector<vector<int>>, decltype(cmp)> pq(cmp);
            int m = nums1.size(), n = nums2.size();

            for (int i = 0; i < m; ++i) {
                pq.push({ nums1[i] + nums2[0], i, 0 });
            }

            while (k > 0) {
                auto vec = pq.top();
                int xy = vec[0], i = vec[1], j = vec[2];
                pq.pop();

                ans.push_back({ nums1[i], nums2[j] });
                --k;

                if (j + 1 < n) {
                    pq.push({ nums1[i] + nums2[j + 1], i, j + 1 });
                }
            }

            return ans;
        }
    };
}
namespace s373o1
{   // 时间：O(k·log(min(m,k)))（加上建堆的 O(min(m,k))）
    // 空间：O(min(m, k))

    class Solution {
    public:
        vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
            int m = nums1.size(), n = nums2.size();
            vector<vector<int>> ans;

            // 堆的大小由m决定，可以在mn中选更小的，进行优化，但需要记得把答案中的数对反转一下
            // 且要注意，n < k的条件也是必要的，如果k更小，那么交换mn并不会使堆变小
            if (m > n && n < k) {
                vector<vector<int>> res = kSmallestPairs(nums2, nums1, k);
                for (auto& p : res) swap(p[0], p[1]);   // ← 把每对换回 {nums1元素, nums2元素}
                return res;
            }

            // 小顶堆，元素 = {和, 行号 i, 列号 j}
            // 用tuple代替二维数组，且直接用自带的greater<Node>就够了，不用麻烦写cmp
            using Node = tuple<int, int, int>;
            priority_queue<Node, vector<Node>, greater<Node>> pq;

            // 第一步：把每一行的第一个元素放进堆
            // 关键优化：只需要前 min(m, k) 行就够了（原因见下面的说明）
            for (int i = 0; i < min(m, k); ++i) {
                pq.emplace(nums1[i] + nums2[0], i, 0);
            }

            // 第二步：弹出最小的作为答案，并补上它右边的邻居
            while (!pq.empty() && k > 0) {// 这里的!pq.empty()纯是代码习惯，这题不加也对
                auto [sum, i, j] = pq.top();
                pq.pop();

                ans.push_back({ nums1[i], nums2[j] });
                --k;

                if (j + 1 < n) {
                    pq.emplace(nums1[i] + nums2[j + 1], i, j + 1);
                }
            }

            return ans;
        }
    };
}
// ---------------------
// 【5.4】重排元素 ()
// 
/*

*/
// ---------------------


// ---------------------
// 【5.5】反悔堆 ()
// 
/*

*/
// ---------------------


// ---------------------
// 【5.6】懒删除堆 ()
// 
/*

*/
// ---------------------



// ---------------------
// 【5.7】对顶堆（滑动窗口第 K 小/大） (1)
// 
/*
295.数据流的中位数：中位数是有序整数列表中的中间值。如果列表的大小是偶数，则没有中间值，中位数是两个中间值的平均值。
例如 arr = [2,3,4] 的中位数是 3 。
例如 arr = [2,3] 的中位数是 (2 + 3) / 2 = 2.5 。
实现 MedianFinder 类:
MedianFinder() 初始化 MedianFinder 对象。
void addNum(int num) 将数据流中的整数 num 添加到数据结构中。
double findMedian() 返回到目前为止所有元素的中位数。与实际答案相差 10-5 以内的答案将被接受。
*/
// ---------------------
// 模板题5：大小堆结合求中位数
namespace s295m1
{   // 企图用库函数蒙混过关，无奈超时了，因为findMedian()如果设计成O(n)时间复杂度必超时
    class MedianFinder {
    private:
        vector<int> nums;
    public:
        MedianFinder() {

        }

        void addNum(int num) {
            nums.push_back(num);
        }

        double findMedian() {
            int n = nums.size();
            if (n % 2) {
                int k = n / 2 + 1;
                nth_element(nums.begin(), nums.end() - k, nums.end());
                return nums[n - k];
            }
            else {
                int k1 = n / 2;
                int k2 = n / 2 + 1;
                nth_element(nums.begin(), nums.end() - k1, nums.end());
                int m1 = nums[n - k1];
                nth_element(nums.begin(), nums.end() - k2, nums.end());
                int m2 = nums[n - k2];
                return (m1 + m2) / 2.0;
            }
        }
    };
}
namespace s295o1
{   // 大小堆，采用中位数将数组分为两个集合的思维
    // addNum时间复杂度O(logn)，n为数据流的元素个数，查找为O(1)
    // 空间复杂度为O(n)
    class MedianFinder {
    private:
        priority_queue<int> left;                               // 最大堆
        priority_queue<int, vector<int>, greater<int>> right;   // 最小堆
    public:
        MedianFinder() {

        }

        void addNum(int num) {
            // 规定left中元素全部小于right中元素
            // 且left堆的大小最多比right多1
            // 自行分类讨论，当left和right大小相同/不相同时，加入元素有4种情况，可以合并成2种
            if (left.size() == right.size()) {
                right.push(num);
                left.push(right.top());
                right.pop();
            }
            else {
                left.push(num);
                right.push(left.top());
                left.pop();
            }
        }

        double findMedian() {
            if (left.size() != right.size()) {
                return left.top();
            }
            else {
                return (left.top() + right.top()) / 2.0;
            }
        }
    };
}
