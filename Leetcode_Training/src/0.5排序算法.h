#pragma once
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <ctime>

using namespace std;

// 选择排序（Selection Sort）：每次遍历找到未排序部分的最小元素，放到已排序部分的末尾
// 平均/最好/最坏时间复杂度O(n^2)/O(n^2)/O(n^2)	空间复杂度O(1)	不稳定排序			
namespace s0_5_1o1
{   // 最简单的暴力排序算法，原始序列的有序性不能影响算法性能，也会破坏相同元素之间的相对顺序，不稳定
    // 选择排序不稳定（可能改变相等元素的顺序），但交换次数少，适合数据移动成本高的场景
    void sort(vector<int>& nums) {
        int n = nums.size();
        // sortedIndex 是一个分割线
        // 索引 < sortedIndex 的元素都是已排序的
        // 索引 >= sortedIndex 的元素都是未排序的
        // 初始化为 0，表示整个数组都是未排序的
        int sortedIndex = 0;
        while (sortedIndex < n) {
            // 找到未排序部分 [sortedIndex, n) 中的最小值
            int minIndex = sortedIndex;
            for (int i = sortedIndex + 1; i < n; ++i) {
                if (nums[i] < nums[minIndex]) {
                    minIndex = i;
                }
            }
            // 交换最小值和 sortedIndex 处的元素
            swap(nums[minIndex], nums[sortedIndex]);

            // sortedIndex 后移一位
            sortedIndex++;
        }
    }
}
namespace s0_5_1m1
{   // 自己凭借理解写的
    class Solution {
    public:
        vector<int> sortArray(vector<int>& nums) {
            int n = nums.size();

            for (int i = 0; i < n; ++i) {
                int mnIndex = i;
                for (int j = i + 1; j < n; ++j) {
                    if (nums[j] < nums[mnIndex]) {
                        mnIndex = j;
                    }
                }
                swap(nums[i], nums[mnIndex]);
            }
            return nums;
        }
    };
}

// 冒泡排序（Bubble Sort）：重复遍历数组，比较相邻元素，如果顺序错误就交换，直到整个数组有序，将最大/最小元素冒泡出来
// 平均/最好/最坏时间复杂度O(n^2)/O(n)/O(n^2)	空间复杂度O(1)	稳定排序	
namespace s0_5_2o1
{   // 不断比较相邻两个元素，将不符合升序/降序原则的元素进行调换
    // 既可以逆序遍历冒最小元素，也可以升序遍历冒最大元素，见sortVector1
    void sortVector(vector<int>& nums) {
        int n = nums.size();
        int sortedIndex = 0;
        while (sortedIndex < n) {
            // 寻找 nums[sortedIndex..] 中的最小值
            // 同时将这个最小值逐步移动到 nums[sortedIndex] 的位置

            bool swapped = false; // 优化：如果某轮没有交换，说明已有序
            for (int i = n - 1; i > sortedIndex; --i) {
                if (nums[i] < nums[i - 1]) {// 注意是 < 而不是 <=，因为可以保持相同元素之间的相对顺序和稳定性
                    swap(nums[i], nums[i - 1]);
                    swapped = true;
                }
            }
            if (!swapped) break; // 提前退出，最好情况下是数组原本有序，时间复杂度O(n)，只进行一轮for循环

            sortedIndex++;
        }
    }

    // 正序遍历冒最大值的泡
    void sortVector1(vector<int>& nums) {
        int n = nums.size();
        int sortedIndex = n - 1;

        while (sortedIndex >= 0) {
            for (int i = 0; i < sortedIndex; ++i) {
                if (nums[i] > nums[i + 1]) {
                    swap(nums[i], nums[i + 1]);
                }
            }
            --sortedIndex;
        }
    }
}
namespace s0_5_2m1
{   // 凭借自己理解写的版本，i和j的方向和o1是反的，但思路相同
    class Solution {
    public:
        vector<int> sortArray(vector<int>& nums) {
            int n = nums.size();

            for (int i = n - 1; i >= 1; --i) {
                bool swapped = false;
                for (int j = 0; j < i; ++j) {
                    if (nums[j] > nums[j + 1]) {
                        swap(nums[j], nums[j + 1]);
                        swapped = true;
                    }
                }
                if (!swapped) break;
            }

            return nums;
        }
    };
}

// 插入排序（Insertion Sort）：像扑克牌整理手牌，将数组分为已排序和未排序部分，逐个将未排序元素插入到已排序部分的正确位置
// 平均/最好/最坏时间复杂度O(n^2)/O(n)/O(n^2)	空间复杂度O(1)	稳定排序
// 在小数组（一般n <= 16）时有优势：顺序查找 + 节点指针修改/元素交换，没有递归调用，没有函数栈开销，内存访问非常局部化，CPU 缓存友好
// 虽然O(n^2)比O（nlogn)大，但对小数组常数因子极小，不少工业排序实现都采用混合排序策略：大数组用快排/归并，小片段切换到插入排序
namespace s0_5_3o1
{
    // 对选择排序进一步优化，向左侧有序数组中插入元素
    // 插入排序对小规模或部分有序数据高效，是许多高级算法（如TimSort）的基础
    // o1是基于swap实现的插入，每次swap需要进行3次赋值，但是逻辑清晰，更容易理解
    // o2版本是直接连续覆盖，每次覆盖只需要一次赋值，性能稍微强一点
    void insertSort(vector<int>& nums) {
        int n = nums.size();
        // 维护 [0, sortedIndex) 是有序数组
        int sortedIndex = 0;
        while (sortedIndex < n) {
            // 将 nums[sortedIndex] 插入到有序数组 [0, sortedIndex) 中
            for (int i = sortedIndex; i > 0; i--) {
                if (nums[i] < nums[i - 1]) {
                    swap(nums[i], nums[i - 1]);
                    // 数组原本有序性越好，算法越快，最好情况O(n)
                }
                else {
                    break;
                    // 比冒泡强的地方就在这，默认左边有序，只要不需要swap就说明已经排完序了，直接换下一个sortedIndex
                }
            }
            sortedIndex++;
        }
    }
}
namespace s0_5_3o2
{   
    void insertSortOptimized(vector<int>& nums) {
        int n = nums.size();

        for (int i = 1; i < n; ++i) {
            int temp = nums[i];  // 保存当前元素
            int j = i;
            // 每次都只插入一个元素，左边半区[0, i)都是已经排好序的
            // 找到插入位置并移动元素（注意这里nums[j - 1]是和temp比较）！！！！！
            // 只要nums[j - 1]不满足 > nums[i]，就说明当前需要插入的元素比左边半区所有元素都大
            // 已经是在正确的位置了，while循环退出，然后num[j]自己赋值自己一次，相当于o1写法的break
            while (j > 0 && nums[j - 1] > temp) {
                nums[j] = nums[j - 1];  // 向后移动元素
                --j;
            }

            nums[j] = temp;  // 插入到正确位置
            /* 也就相当于下面这段
            int j = sortedIndex;
            for ( ; j > 0; --j) {
                if (nums[j - 1] > temp) {
                    nums[j] = nums[j - 1];
                } else {
                    break;
                }
            }
            nums[j] = temp; // 插入到正确位置
            */
        }
    }
}

// 希尔排序（Shell Sort）：将数组元素按一定间隔分组，对每组进行插入排序，然后逐渐缩小间隔，直到间隔为1，最终完成排序
// 平均/最好/最坏时间复杂度O(n^1.5)/O(nlogn)/O(n^2)	        空间复杂度O(1)	        不稳定排序
namespace s0_5_4o1
{   // 是插入排序的改进，利用插入排序在部分有序数组上效率高的优点，减少元素的移动次数，从而提升整体性能，突破O(n^2)
    // 在分组上进行插入排序就相当于逐层预处理，每处理完一次当前间隔下的所有分组，原数组有序性都会有一定的提升

    // 希尔排序比简单插入排序快，尤其适合中等大小的数组，但不是稳定排序，
    // 间隔序列的选择会影响性能，这里用了简单的序列(初始间隔为数组长度的一半，然后每次减半，直到间隔为1)
    // 时间复杂度平均情况下大约是O(n^1.5)，还可以尝试其他序列（如Hibbard序列）来优化，见o1_extension

    // 这里内层的插入排序用的是s0_5_3o2的赋值版本
    void shellSort(vector<int>& arr) {
        int n = arr.size();

        // 开始使用较大的间隔（gap），然后逐步缩小
        for (int gap = n / 2; gap > 0; gap /= 2) {
            // 对每个间隔分组进行插入排序
            for (int i = gap; i < n; ++i) {
                int temp = arr[i];  // 保存当前元素
                int j = i;

                // 在组内进行插入排序：将元素插入到正确位置
                while (j >= gap && arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];  // 将较大的元素向后移动
                    j -= gap;
                }
                arr[j] = temp;  // 插入保存的元素到正确位置
            }
        }
    }
}
namespace s0_5_4o1_extension
{   // 知道Shell排序存在优化的gaps序列即可，不需要记住细节，用到再查便是
    void gaps(int n) {
        // o1用到的Shell原始gap序列，实现简单，但是低效，且最坏情况时间复杂度还是O(n^ 2)
        for (int gap = n / 2; gap > 0; gap /= 2) {
            // 插入排序
        };
        
        // Hibbard序列: 1, 3, 7, 15, 31, 63, ... (2^k - 1)
        // 时间复杂度：O(n^(3/2))，可避免最坏情况，比较通用
        vector<int> gaps;
        int k = 1;
        while ((1 << k) - 1 < n) {  // 1<<k 等于 2^k
            gaps.push_back((1 << k) - 1);
            k++;
        }

        // Knuth序列: 1, 4, 13, 40, 121, ... ((3^k - 1)/2)
        // 时间复杂度：O(n^(3/2))，理论最优，适合性能要求高的场景
        vector<int> gaps_Knuth;
        int k_Knuth = 1;
        int gap_Knuth = 1;
        while (gap_Knuth < n) {
            gaps_Knuth.push_back(gap_Knuth);
            gap_Knuth = gap_Knuth * 3 + 1;  // 相当于 (3^(k+1) - 1)/2
        }
    }

    // Sedgewick序列: 1, 5, 19, 41, 109, ...
    // 时间复杂度：O(n^(4/3))，实际表现最佳，适合大规模数据
    vector<int> generateSedgewickGaps(int n) {
        vector<int> gaps;
        int i = 0;
        int gap;

        do {
            if (i % 2 == 0) {
                gap = 9 * (1 << i) - 9 * (1 << (i / 2)) + 1;
            }
            else {
                gap = 8 * (1 << i) - 6 * (1 << ((i + 1) / 2)) + 1;
            }
            if (gap < n) gaps.push_back(gap);
            i++;
        } while (gap < n);

        reverse(gaps.begin(), gaps.end());
        return gaps;
    }
}

// 快速排序（Quick Sort）：分治法策略。选择一个枢轴元素，将数组分为小于和大于枢轴的两部分，递归排序。常用“分区”操作
// 平均/最好/最坏时间复杂度O(nlogn)/O(nlogn)/O(n^2)	    空间复杂度O(logn)（平均递归栈）	    不稳定排序
// 可拓展为随机快速排序（随机选择基准避免最坏情况）以及三路快速排序（对含大量重复元素的数据提速巨大）
namespace s0_5_5o1
{   // 平均时间复杂度：O(n log n) – 分区平衡平均时，递归深度log n
    // 最好时间复杂度：O(n log n) – 枢轴(pivot)总是中位数，将数组二分，分区平衡
    // 最坏时间复杂度：O(n^2) – 如果枢轴总是最小或最大元素（如已排序数组），分区不平衡（比如正序时，枢轴左边总是没有元素）
    // （是最好还是最坏情况的判断依据是分区函数是否起到了不断二分数组的分区作用）
    // 快速排序在实践中非常快（通常是最快的），最坏情况也可通过随机化枢轴避免（也即随机快速排序，见o2）
 
    // 分区函数：返回基准的最终位置（以下版本为Hoare和Lomuto分区方案）

    // Hoare：双指针i,j从两端向中间移动，基准可以灵活选择，交换次数相较于Lomuto分区方案通常更少
    // 代码复杂度更高一点，时间复杂度平均O(n log n)，常数因子较小
    int partitionHoare(vector<int>& arr, int low, int high) {
        int pivot = arr[low]; // 选择第一个元素作为基准
        int i = low + 1;      // 指向第一个大于基准的元素
        int j = high;         // 指向最后一个元素

        while (i <= j) {// 【注意！！！】自己写很容易把判断条件写成i < j
            // 比如区间[1, 2]，如果判断条件是i < j，此时因为i = j = 1，就会退出循环,之后swap i和j上的元素，区间变成[2, 1]
            // i最大为high + 1, j最小为low

            // 从左向右找第一个大于基准的元素
            while (i <= high && arr[i] <= pivot) i++;

            // 从右向左找第一个小于基准的元素
            // 这里可以改成while (arr[j] > pivot)，因为arr[j]不可能“严格大于”[low, high]中所有元素
            // 至少pivot本身不能大于pivot，运行到j = low时一定会退出循环
            while (j >= low && arr[j] > pivot) j--;// 
            if (i < j) {
                swap(arr[i], arr[j]); // 交换元素，确保左边小，右边大
            }
        }
        // 将基准放到正确位置
        swap(arr[low], arr[j]);
        return j; // 返回基准的位置
    }

    // Lomuto：单指针(i)遍历，j标记边界，基准总是第一个元素，交换次数可能更多(稳定增加j)
    // 代码简单直观（英雄视频里的写法），时间复杂度平均O(n log n)，但常数因子较大
    int partitionLomuto(vector<int>& arr, int low, int high) {
        int pivot = arr[low];           // 基准选择第一个元素
        int j = low + 1;           // j指向下一个应该交换的位置

        for (int i = j; i <= high; i++) {
            if (arr[i] <= pivot) {
                swap(arr[i], arr[j]);    // 将小元素交换到j左边
                ++j;
            }
        }

        swap(arr[low], arr[j - 1]);    // 将基准放到正确位置
        return j - 1;              // 更新基准位置
    }

    // 快速排序主函数（每轮递归，都会至少确定pivot是在arr的正确位置上，当low = high，分区内仅一个元素时停止递归）
    void quickSort(vector<int>& arr, int low, int high) {
        if (low < high) {
            int pivotIndex = partitionHoare(arr, low, high); // 分区
            quickSort(arr, low, pivotIndex - 1);  // 递归排序左半部分
            quickSort(arr, pivotIndex + 1, high); // 递归排序右半部分
        }
    }
}
namespace s0_5_5o2
{   // 随机快速排序（Randomized Quicksort）：在分区时随机选择一个元素作为基准，而不是固定选择第一个元素
    // 大大降低最坏情况发生的概率，因为随机化使得基准选择更均匀。平均时间复杂度仍为 O(n log n)，但更稳定
    int partitionLomutoRand(vector<int>& arr, int low, int high) {
        // 只加了下面这两行，每次都是在[low, high]中随机选一个数放在low的位置上，每次分区都带有随机性
        
        int randomIndex = low + rand() % (high - low + 1); // 随机选择索引
        swap(arr[low], arr[randomIndex]); // 将随机元素换到开头作为基准

        // 以下用Lumoto分区做示例，Hoare分区方案也是类似的：

        int pivot = arr[low];           // 基准选择第一个元素
        int j = low + 1;           // j指向下一个应该交换的位置

        for (int i = j; i <= high; i++) {
            if (arr[i] <= pivot) {
                swap(arr[i], arr[j]);    // 将小元素交换到j左边
                ++j;
            }
        }

        swap(arr[low], arr[j - 1]);    // 将基准放到正确位置
        return j - 1;              // 更新基准位置
    }

    void QuickSort(vector<int> arr, int low, int high) {
        // ...
        // 递归调用
    }

    void SortArray(vector<int> arr) {
        // 在整个程序中只调用一次，重置全局随机数，否则，每次使用rand得到的随机数序列是相同的
        srand(time(NULL));  
        // 注意，不能在递归函数QuickSort内部使用上述语句
        // 如果递归调用很快，time(NULL) 可能返回相同的时间戳，导致多次递归使用相同的随机数序列
        // 我们希望每次 partition 都使用不同的随机数，rand() 的连续调用正好满足这个需求
        // 如果每次递归都重置种子，反而会导致问题，整个程序只需要重置一次
    }
}
namespace s0_5_5o3
{   // 当数组中存在大量重复元素时，快速排序和随机快速排序都会存在大量的递归调用导致超时
    // 可以用三路快速排序（3-Way Quicksort）进行解决：
    // 将数组分成三部分：小于基准、等于基准和大于基准。适用于有大量重复元素的数组，能减少递归调用。
    // 并且重复元素越多，性能越好，时间复杂度最好情况可接近 O(n)
    void threeWayQuickSort(vector<int>& nums, int low, int high) {
        if (low >= high) return;
        // 如果是快速选择排序(Quick Select)，这里就是if (low > high) return，因为只有一个元素时也需进入循环取值并返回

        // 对于小数组使用插入排序优化（这个点是可选的，也且也不一定是正向优化）
        if (high - low + 1 <= 16) {
            insertionSort(nums, low, high);
            return;
        }

        // 随机选择基准避免最坏情况（基准选取还有很多种方式，这里不再展开了）
        int randomIndex = low + rand() % (high - low + 1);
        swap(nums[low], nums[randomIndex]);

        int pivot = nums[low];
        int lt = low;      // 小于pivot的右边界（lt = less than，小于基准值的右边界）
        int gt = high;     // 大于pivot的左边界（gt = greater than，大于基准值的左边界）
        int i = low + 1;   // 当前检查的元素

        while (i <= gt) {
            if (nums[i] < pivot) {
                swap(nums[i], nums[lt]);
                ++lt;
                ++i;
                // 第一次交换过来的值一定是pivot，也就是等于pivot的值，这里必须要++i
                // 因为lt和i总是同步增大的，i一定比lt大1，之后这个等于pivot的值可以正常交换到后续的i处
            }
            else if (nums[i] > pivot) {
                swap(nums[i], nums[gt]);
                --gt;
                // 注意：这里不增加i，因为交换过来的新元素需要重新检查
            }
            else {
                ++i;  // 等于pivot，直接跳过
            }
        }

        // 现在数组分为：[low, lt-1] < pivot, [lt, gt] == pivot, [gt+1, high] > pivot
        threeWayQuickSort(nums, low, lt - 1);  // 递归排序小于部分
        threeWayQuickSort(nums, gt + 1, high); // 递归排序大于部分
        // 等于部分已经在正确位置，不需要排序
    }

    void insertionSort(vector<int>& nums, int low, int high) {
        for (int i = low + 1; i <= high; i++) {
            int key = nums[i];
            int j = i - 1;
            while (j >= low && nums[j] > key) {
                nums[j + 1] = nums[j];
                j--;
            }
            nums[j + 1] = key;
        }
    }
}

// 归并排序（Merge Sort）：分治法策略。将数组递归分成两半，分别排序后合并。合并过程是线性的
// 平均/最好/最坏时间复杂度O(nlogn)/O(nlogn)/O(nlogn)	空间复杂度O(n)（若用递归还要O(logn)栈空间）	稳定排序
namespace s0_5_6o1
{   // 每次分割都是O(log n)层递归，底数为2，每层合并需要O(n)时间，总计：O(n log n)

    // o1是递归版本（自顶向下）， o2是迭代版本（自底向上）
    // 两种方法的merge函数（合并两个有序数组）是相同的

    // 递归地将数组对半分割，直到每个子数组只有一个元素（自然有序），然后合并这些子数组
    // 空间复杂度为：O(n + log n) = O(n)
    class Solution {
    private:
        void mergeSortRecursive(vector<int>& arr, vector<int>& temp, int left, int right) {
            if (left >= right) return;

            int mid = left + (right - left) / 2;

            // 分治：递归排序左右两部分，闭区间
            mergeSortRecursive(arr, temp, left, mid);
            mergeSortRecursive(arr, temp, mid + 1, right);

            merge(arr, temp, left, mid, right);
        }

        // 合并两个有序数组
        void merge(vector<int>& arr, vector<int>& temp, int left, int mid, int right) {
            int i = left;       // 左子数组起始索引，包含[left, mid]
            int j = mid + 1;    // 右子数组起始索引，包含[mid + 1, right]
            int k = left;       // 临时数组起始索引

            // 合并两个有序数组
            while (i <= mid && j <= right) {
                if (arr[i] <= arr[j]) { 
                    // 因为临时数组用的是左边，所以这里是<=，碰到相同元素优先在[left, mid]里选
                    // 也就不会因为覆盖相同元素进而改变相同元素的相对位置
                    temp[k++] = arr[i++];// 优先选择左半部分的元素
                }
                else {
                    temp[k++] = arr[j++];// 只有当右边更小时才选择右边
                }
            }

            // 复制剩余元素
            while (i <= mid) temp[k++] = arr[i++];
            while (j <= right) temp[k++] = arr[j++];

            // 将临时数组拷贝回原始数组，这一步不能忘了，否则永远使用的都是原始arr，后面直接当成升序数组合并就会出错
            for (int idx = left; idx <= right; ++idx) {
                arr[idx] = temp[idx];
            }
        }
    public:
        vector<int> sortArray(vector<int>& nums) {
            int n = nums.size();
            if (n <= 1) return nums;

            vector<int> temp(n);
            mergeSortRecursive(nums, temp, 0, n - 1);

            return nums;
        }
    }; 
}
namespace s0_5_6o2
{
    // 从最小的子数组（大小为1）开始，两两合并，然后再合并更大的子数组，直到整个数组有序
    class Solution {
    private:
        // 合并两个有序数组
        void merge(vector<int>& arr, vector<int>& temp, int left, int mid, int right) {
            int i = left;    // 左子数组起始索引
            int j = mid + 1; // 右子数组起始索引
            int k = left;    // 临时数组起始索引

            // 合并两个有序数组
            while (i <= mid && j <= right) {
                if (arr[i] <= arr[j]) {
                    // 因为临时数组用的是左边，所以这里是<=，也就不会覆盖相同元素进而改变相同元素的相对位置
                    temp[k++] = arr[i++];
                }
                else {
                    temp[k++] = arr[j++];
                }
            }

            // 复制剩余元素
            while (i <= mid) temp[k++] = arr[i++];
            while (j <= right) temp[k++] = arr[j++];

            // 将临时数组拷贝回原始数组
            for (int idx = left; idx <= right; ++idx) {
                arr[idx] = temp[idx];
            }
        }

    public:
        vector<int> sortArray(vector<int>& nums) {
            int n = nums.size();
            if (n <= 1) return nums;

            vector<int> temp(n); // 辅助数组

            // 从size=1开始，逐步扩大合并的区间大小
            for (int size = 1; size < n; size *= 2) {
                for (int left = 0; left < n - 1; left += 2 * size) {
                    int mid = min(left + size - 1, n - 1);
                    int right = min(left + 2 * size - 1, n - 1);
                    merge(nums, temp, left, mid, right);
                }
            }
            return nums;
        }
    };
}

// 堆排序：（Heap Sort）：利用二叉堆数据结构。先构建最大堆（或最小堆），然后反复取出堆顶元素（最大/最小）并通过下沉调整堆
// 平均/最好/最坏时间复杂度O(nlogn)/O(nlogn)/O(nlogn)	    空间复杂度O(1)	    不稳定排序
namespace s0_5_7
{
    // 平均时间复杂度：O(n log n) – 建堆O(n)，每次调整堆O(log n)，共n次。
    // 最好时间复杂度：O(n log n) – 同平均情况。
    // 最坏时间复杂度：O(n log n) – 稳定，堆操作总是对数时间。
    // 堆排序能实现原地排序，不稳定，适合需要保证最坏情况性能的场景
    // 在数据量较小的情况下常数因子较大，可能不如快速排序或插入排序快。缓存局部性较差（跳跃访问）

    // 大顶堆，基于完全二叉树实现，完全二叉树的性质之一是可以用线性数组唯一的表示、存储这颗树
    // 下标为i的结点的：
    // 父节点为(i - 1) / 2      【注：这只对向下取整的除法成立，且公式中下标从0开始】
    // 左儿子下标为2 * i + 1
    // 右儿子为2 * i + 2

    // heapify这一步的时间复杂度为O(logn)
    // n: 数组（堆）的大小
    // i: 当前需要进行堆化的节点索引
    void heapify(vector<int>& arr, int n, int i) {
        int largest = i;       // 初始化最大值索引为当前节点
        int left = 2 * i + 1;  // 左孩子索引
        int right = 2 * i + 2; // 右孩子索引

        // 如果左孩子存在且大于当前最大值
        if (left < n && arr[left] > arr[largest]) {
            largest = left;
        }
        // 如果右孩子存在且大于当前最大值
        if (right < n && arr[right] > arr[largest]) {
            largest = right;
        }

        // 如果最大值不是当前节点，则需要交换并递归堆化
        if (largest != i) {
            swap(arr[i], arr[largest]);
            // 递归地对被交换下去的子树进行堆化
            heapify(arr, n, largest);
        }
    }

    void heapSort(vector<int>& arr, int n) {
        // 第一步：建堆（构建最大堆）,整个建堆过程的时间复杂度是 O(n)，而不是直觉上的O(nlogn)
        // 从最后一个非叶子节点开始，从右到左，从下到上进行堆化
        // 最后一个非叶子节点的索引是 n/2 - 1    (最后一个节点下标是n - 1，套入公式可得其父节点为n/2 - 1)
        for (int i = n / 2 - 1; i >= 0; --i) {
            heapify(arr, n, i); // 构建最大堆
        }
        // 此时，arr[0] 是最大值

        // 第二步：排序
        // 逐个从堆顶取出元素（最大值）放到数组末尾
        for (int i = n - 1; i > 0; --i) {
            // 将当前根节点（最大值）移到数组末尾
            swap(arr[0], arr[i]);
            // 对缩小后的堆（大小为 i）的根节点进行堆化
            heapify(arr, i, 0);
        }
    }
}

// 桶排序（Bucket Sort）：博采众长，将元素分配至若干个桶中，桶内排序算法自定义
// 平均/最好/最坏时间复杂度O(n + k)/O(n)/O(n^n)             空间复杂度O(n + k)  是否稳定取决于桶内排序算法稳定性
namespace s0_5_8
{   /*
    桶排序算法的核心思想分三步：
    1、将待排序数组中的元素使用映射函数分配到若干个「桶」中。
    2、对每个桶中的元素进行排序。
    3、最后将这些排好序的桶进行合并，得到排序结果。
    映射函数没有固定，桶的数量是若干的，每个桶内的排序方法没有固定，合并方法没有固定。
    桶排序只是提供一个分治的框架，一般都是结合其他排序方法一起使用（比如归并排序就是一种特殊的桶排序）

    平均时间复杂度：O(n + k)，当k与n成正比，且桶内元素均匀分布时，每个桶的大小约为O(1)，内部排序开销小，整体接近线性时间
    最坏时间复杂度：O(n^2)，如果所有元素都落入同一个桶中（例如数据高度集中），桶内部排序可能退化为O(n^2)
    最佳时间复杂度：O(n)，当数据完美均匀分布，且k选择合适时
    桶排序的性能很大程度上取决于桶的数目和数据的分布。如果k选择得当，桶排序可以比O(n log n)的排序算法（如快速排序）更快

    桶的内部用什么排序最好呢？
    桶内部排序算法应选择适合小数据集的排序方法，因为每个桶通常只包含少量元素，最常见的是使用插入排序
    
    又如何决定桶的数目k？ 
    关键在于数据要均匀分布，使每个桶的大小大致相等，排序效率高，在实际应用中，k常取n或略大于n（如k = 2n）
    这样每个桶平均有1个元素，内部排序开销最小，但需要处理空桶，如果数据范围大，可能浪费空间
    */
    void bucketSort(vector<int>& arr) {
        if (arr.empty()) return;

        int n = arr.size();
        int max_val = *max_element(arr.begin(), arr.end());
        int min_val = *min_element(arr.begin(), arr.end());
        int range = max_val - min_val + 1;

        // 创建桶，桶的数量设为n或根据范围调整，这里使用n个桶
        int bucket_count = min(n, range);// 与range取最小值，避免由重复值过多导致的大量空桶出现
        vector<vector<int>> buckets(bucket_count);

        // 将元素分配到桶中
        for (int num : arr) {
            // 这里一定要先乘再除，要不然都会得到为0的bucket_index
            // 一般只需要用这种将待排序数组中的元素使用映射函数分配到若干个「桶」中的方式：
            int bucket_index = (num - min_val) * 1.0 * bucket_count / range;
            // 这里不能是num - min_val + 1，否则bucket_index会在num = max_val时变成bucket_count，下标访问越界
            // 乘以1.0转化成double浮点数避免溢出，但因为浮点位数会有精度问题，更优解为：
            /*
            int bucket_index = static_cast<int>(
                (static_cast<int64_t>(num - min_val) * bucket_count) / range
                );
            int64_t代表不同平台都严格限制为64位的整形，虽然大多数64位系统中int64_t和long long都是相同的，但用int64_t更精准
            */
            buckets[bucket_index].push_back(num);
        }

        // 对每个桶进行排序（使用稳定排序以保持稳定性）
        for (auto& bucket : buckets) {
            stable_sort(bucket.begin(), bucket.end()); 
            // 使用stable_sort确保稳定(底层主要为归并排序)
            // 或使用sort来获得更高的效率
        }

        // 合并桶
        int index = 0;
        for (auto& bucket : buckets) {
            for (int num : bucket) {
                arr[index++] = num;
            }
        }
    }
}

// 计数排序（Counting Sort）：非比较排序算法，统计每个元素出现的次数来完成排序（将输入数据转换为键值存储在额外数组空间中）
// 平均/最好/最坏时间复杂度O(n + k)/O(n + k)/O(n + k)	        空间复杂度O(k)	        稳定排序
namespace s0_5_9o1
{   // 在某种角度可以认为是桶排序的变形，也是少有的非比较排序
    // 时间复杂度中的k指max -min，是待排序数组的元素范围
    // 计数排序效率高，但是对于k特别大的数组，可能导致大量的空间浪费，空间复杂度过高
    // 特别适合小范围整数排序（如成绩排序、年龄排序），或是需要稳定排序且数据范围可控的场景
    // 非稳定排序版本见o2，面对大范围数组的策略见o3
    // 其思想和bitmap类似，详见extention

    // 通过累加count前缀和 + 从后往前遍历实现稳定排序（但是需要额外数组output当作输出，以及遍历count数组进行累加）
    vector<int> countingSortWithNegative(vector<int>& nums) {
        int minVal = *min_element(nums.begin(), nums.end());
        int maxVal = *max_element(nums.begin(), nums.end());
        int range = maxVal - minVal + 1;
        int offset = -minVal;

        vector<int> count(range, 0);
        vector<int> output(nums.size());

        // 计数（添加偏移量使所有值非负，某些场景offset可以省去）
        for (int num : nums) {
            count[num + offset]++;
        }

        // 累加计数
        for (int i = 1; i < range; ++i) {
            count[i] += count[i - 1];
        }

        // 输出（稳定排序）
        for (int i = nums.size() - 1; i >= 0; --i) {
            // 能实现稳定排序的关键在于output[key - 1]的值是直接来源于nums[i]的，其附带的其他属性也能共享
            // 同时由于从后往前遍历，且count[key]代表值<= key的元素有count[key]个，需要放在位置[0, key - 1]位置上
            // 可以保证后出现的nums[i]也放在输出数组的靠后位置
            int key = nums[i] + offset;
            output[count[key] - 1] = nums[i];
            --count[key];
        }

        return output;
    }
}
namespace s0_5_9o2
{   // 如果不要求稳定排序，可以省去累加前缀和，以及开辟额外数组output的空间占用，进一步提高性能
    class Solution {
    public:
        vector<int> sortArray(vector<int>& nums) {
            int max = *max_element(nums.begin(), nums.end());
            int min = *min_element(nums.begin(), nums.end());
            int range = max - min + 1;
            int offset = -min;

            vector<int> count(range, 0);
            for (int num : nums) {
                ++count[num + offset];
            }

            int idx = 0;
            for (int i = 0; i < range; ++i) {
                while (count[i]-- > 0) {
                    nums[idx++] = i - offset;
                }
            }
            return nums;
        }
    };
}
namespace s0_5_9o3
{   // 应对大范围数据的优化策略：
    // 1.桶排序结合计数排序,将大范围分成多个桶，每个桶内使用计数排序
    // 2.使用哈希映射代替数组，只存储实际出现的数值（如果用unordered_map会失去稳定性，用map会增加时间复杂度）
    // 3.对于大范围整数，基数排序更适合，按位进行排序
    // 这里对1和2进行拓展
    
    // 策略1：将大范围数据分成多个较小的桶，每个桶内使用计数排序。这样既减少了空间占用，又保持了计数排序的高效性
    // 空间复杂度从O(k)降低到O(桶数 + 桶内范围)，适合数据分布相对均匀的情况，但需要需要合理选择桶大小
    // 下面是一个比较进阶的写法，融合了桶排序和计数排序（稳定排序的版本），不过空间占用偏高，只能说是一种针对特定场景的特定排序手段
    vector<int> bucketSortWithCountingSort(vector<int>& nums, int bucketSize = 100) {
        if (nums.empty()) return nums;

        // 找到最小值和最大值
        int minVal = *min_element(nums.begin(), nums.end());
        int maxVal = *max_element(nums.begin(), nums.end());

        // 计算桶的数量
        int bucketCount = (maxVal - minVal) / bucketSize + 1;
        vector<vector<int>> buckets(bucketCount);

        // 将元素分配到桶中
        for (int num : nums) {
            int bucketIndex = (num - minVal) / bucketSize;
            buckets[bucketIndex].push_back(num);
        }

        // 对每个桶进行排序
        vector<int> result;
        for (auto& bucket : buckets) {
            if (bucket.empty()) continue;

            // 计算当前桶的范围
            int bucketMin = *min_element(bucket.begin(), bucket.end());
            int bucketMax = *max_element(bucket.begin(), bucket.end());
            int bucketRange = bucketMax - bucketMin + 1;

            // 如果桶内范围较小，使用计数排序；否则使用标准排序
            if (bucketRange <= 1000) {
                // 计数排序实现
                vector<int> count(bucketRange, 0);
                vector<int> output(bucket.size());

                // 计数
                for (int num : bucket) {
                    count[num - bucketMin]++;
                }

                // 累加计数（为了稳定性）
                for (int i = 1; i < bucketRange; i++) {
                    count[i] += count[i - 1];
                }

                // 从后往前填充（保持稳定性）
                for (int i = bucket.size() - 1; i >= 0; i--) {
                    output[count[bucket[i] - bucketMin] - 1] = bucket[i];
                    count[bucket[i] - bucketMin]--;
                }

                result.insert(result.end(), output.begin(), output.end());
            }
            else {
                // 桶内范围太大，使用标准排序
                sort(bucket.begin(), bucket.end());
                result.insert(result.end(), bucket.begin(), bucket.end());
            }
        }

        return result;
    }

    // 策略2：当数据范围很大但实际出现的不同数值较少时（稀疏数据），使用哈希映射只存储实际出现的数值及其计数，极大节省空间
    // 优点是空间复杂度为O(k)，此时k为实际出现的不同数值的个数，适合稀疏数据
    // 但需要额外的键排序步骤，哈希操作也有额外的常数时间开销，不如数组访问效率高，整体时间复杂度不一定占用（相对于平均的计数排序）
    vector<int> countingSortWithHashMap(vector<int>& nums) {
        if (nums.empty()) return nums;

        // 使用哈希映射统计频率（因为是哈希表计数，所以无论是浮点数，负数，都不需要特殊处理了，顶多将key的类型定义成double，修改起来很简单）
        unordered_map<int, int> countMap;
        for (int num : nums) {
            countMap[num]++;
        }

        // 提取所有键并排序(或者不用unordered_map，而直接使用map)
        vector<int> keys;
        for (const auto& pair : countMap) {
            keys.push_back(pair.first);
        }
        sort(keys.begin(), keys.end());

        // 根据排序后的键重建数组
        vector<int> result;
        for (int key : keys) {
            int count = countMap[key];
            result.insert(result.end(), count, key); // 插入count个key
        }

        return result;
    }
}
namespace s0_5_9_extention
{   // 与bitmap思想的关联，Bitmap是计数排序的特殊情况：
    /*
    Bitmap（位图）是一种使用位来表示某些元素是否存在的数据结构，每个位只能表示0或1（存在/不存在）。
    不能记录多个相同的元素，所以Bitmap实际上是一种特殊的计数排序（每个元素出现次数不能超过1）。
    计数排序的计数数组可看作"广义Bitmap"，每个位置可存储任意计数值，当只需要判断存在性时，Bitmap更节省空间

    // Bitmap示例 - 只能判断存在性
    vector<bool> bitmap(10000, false);
    // 计数排序 - 可统计出现次数
    vector<int> count(10000, 0);
    */

    // 比如面试题 01.01. 判定字符是否唯一：实现一个算法，确定一个字符串 s 的所有字符是否全都不同。
    // s[i]仅包含小写字母，如果你不使用额外的数据结构，会很加分。

    class Solution {
    public:
        bool isUnique(string astr) {
            int bitmap = 0;

            for (char c : astr) {
                int offset = c - 'a';

                if (bitmap & (1 << offset)) {
                    return false;
                }
                bitmap |= (1 << offset);
            }
            return true;
        }
    };
}

// 基数排序（Radix Sort）：
// 平均/最好/最坏时间复杂度O(d(n + k))/O(d(n + k))/O(d(n + k))	        空间复杂度O(n + k)	        稳定排序
namespace s0_5_10o1
{   // 基数排序本质上也是桶排序（也跟计数排序思想类似，同属非比较型排序算法），但使用数位来划分桶
    // 按数字的位数进行排序，从最低有效位（LSB, Least Significant Bit）到最高有效位（MSB, Most Significant Digit）
    // 每次使用一个稳定的排序算法（如计数排序）对当前位进行排序。它适用于多位数整数、字符串排序和大范围数据。

    /*
    平均情况：O(d * (n + k))，其中d是数字的最大位数，k是基数（如十进制为10）。
    最好情况：O(d * (n + k))，与平均情况相同。
    最坏情况：O(d * (n + k))，没有退化，但d较大时效率低。
    无论输入数据如何分布，都需要进行d轮排序，每轮都需要遍历n个元素并进行分配，所以基数排序的时间复杂度非常稳定

    空间复杂度：O(n + k)，用于计数数组和输出数组。
    稳定性：基数排序是稳定的，因为它每次使用稳定的子排序（如计数排序），保持了相等元素的顺序。
    */
    
    // 适用于非负整数排序，特别是当n远大于k时。
    // 如果包含负数，需要先分离处理，见o2。这里使用计数排序作为子排序。
    // 十进制时基数为10，但基数排序也可以使用八进制、二进制等数制，见o3。

    void radixSort(vector<int>& arr) {
        if (arr.empty()) return;

        int n = arr.size();
        int max_val = *max_element(arr.begin(), arr.end());

        // 从最低位到最高位进行排序
        for (int base = 1; base <= max_val; base *= 10) {
            // 使用计数排序对当前位进行排序（通常是计数排序，但也可以换成别的排序算法）
            vector<int> output(n);
            vector<int> count(10, 0); // 十进制，基数10

            // 统计当前位的出现次数
            for (int num : arr) {
                int digit = (num / base) % 10;
                ++count[digit];
            }

            // 累加计数
            for (int i = 1; i < 10; ++i) {
                count[i] += count[i - 1];
            }

            // 构建输出数组（从后往前以保持稳定性，前缀和+逆序遍历的思路和计数排序保持稳定性类似）
            for (int i = n - 1; i >= 0; --i) {
                int digit = (arr[i] / base) % 10;
                // 这里必须用临时数组，count仅仅是按照某个数位进行排序
                // 如果直接原地修改，可能会重复读取到之前已经处理过的数组                
                output[count[digit] - 1] = arr[i];
                --count[digit];
            }

            // 将结果复制回原数组
            arr = output;
        }
    } 
}
namespace s0_5_10o2
{   // 如果输入包含负数：
    /*
    1.将负数和非负数分离放在两个不同的数组里：实现简单，但需要额外空间，要两次排序
    2.增加偏移量，将nums中所有数变成非负的，排序结束后再遍历一次复原：只要一次排序，但可能导致数值溢出
    3.利用补码特性，单独处理符号位，对符号位进行分离处理：效率最高，但实现复杂，不容易理解（暂时不看）
    */
    
    // 以下仅展示方法1和方法2
    void radixSort(vector<int>& arr) {
        // 略，同o1
    }

    void radixSortWithNegatives1(vector<int>& arr) {
        vector<int> positives, negatives;

        // 1. 分离正负数
        for (int num : arr) {
            if (num >= 0) positives.push_back(num);
            else negatives.push_back(-num);  // 将负数转为正数处理
        }

        // 2. 分别排序
        radixSort(positives);  // 正常排序正数
        radixSort(negatives);  // 排序转换后的"正数"

        // 3. 合并结果
        arr.clear();
        // 先放排序后的负数（需要还原并逆序）
        for (int i = negatives.size() - 1; i >= 0; i--) {
            arr.push_back(-negatives[i]);
        }
        // 再放正数
        arr.insert(arr.end(), positives.begin(), positives.end());
    }

    void radixSortWithNegatives2(vector<int>& arr) {
        if (arr.empty()) return;

        // 1. 找到最小值（最负数）
        int minVal = *min_element(arr.begin(), arr.end());
        int offset = (minVal < 0) ? -minVal : 0;

        // 2. 将所有数变为非负
        for (int& num : arr) {
            num += offset;
        }

        // 3. 正常基数排序
        radixSort(arr);

        // 4. 还原数值
        for (int& num : arr) {
            num -= offset;
        }
    }
}
namespace s0_5_10o3
{   /*
    对非十进制基数进行讨论：
    1.如果使用二进制基数，k = 2：
    for (int shift = 0; shift < 32; shift++) {
        // 每次处理1位
    }
    轮数d增加，32位整数需要32轮，但每轮实现都更简单（只有0和1两个情况），适合硬件实现

    2.如果使用十六进制基数，k = 16：
    每轮处理4位，轮数减少到8轮

    大基数则轮数少，每轮空间开销增大；如果小基数则轮数多，每轮空间开销低
    基数排序的实践中一般选取2的幂次作为基数，因为可以利用位运算高效运算
    */

    // 注：对二进制，十六进制等形式的基数排序做法暂时跳过
}