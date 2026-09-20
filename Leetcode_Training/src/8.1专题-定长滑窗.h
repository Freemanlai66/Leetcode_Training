#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <unordered_map>
#include <numeric>
using namespace std;

// 滑动窗口：定长滑窗 + 不定长滑窗

/*
模板题：
1.定长滑窗基础模板：1456
2.循环数组的处理：取模 %，for循环判断条件不能局限于端点，而是直接按照移动次数设置：1652
3.当窗口判断条件比较复杂时，可以先只遍历k - 1个元素来简化代码（按模板1也行，但代码比较冗余）：2653
*/

// 定长滑窗(18)
// 【1.1】基础（此阶段，套模板解题，多熟悉流程）(10)
/*
1456.定长子串中元音的最大数目：给你字符串 s 和整数 k 。
			请返回字符串 s 中长度为 k 的单个子字符串中可能包含的最大元音字母数。
			英文中的元音字母为（a, e, i, o, u）。

643.子数组最大平均数 I：给你一个由 n 个元素组成的整数数组 nums 和一个整数 k 。
            请你找出平均数最大且 长度为 k 的连续子数组，并输出该最大平均数。
            任何误差小于 10-5 的答案都将被视为正确答案。

1343.大小为 K 且平均值大于等于阈值的子数组数目：给你一个整数数组 arr 和两个整数 k 和 threshold 。
            请你返回长度为 k 且平均值大于等于 threshold 的子数组数目。

2090.半径为k的子数组平均值：给你一个下标从 0 开始的数组 nums ，数组中有 n 个整数，另给你一个整数 k 。
            半径为 k 的子数组平均值 是指：nums 中一个以下标 i 为 中心 且 半径 为 k 的子数组中所有元素的平均值。
            即下标在 i - k 和 i + k 范围（含 i - k 和 i + k）内所有元素的平均值。
            如果在下标 i 前或后不足 k 个元素，那么 半径为 k 的子数组平均值 是 -1 。
            构建并返回一个长度为 n 的数组 avgs ，其中 avgs[i] 是以下标 i 为中心的子数组的 半径为 k 的子数组平均值 。

2379.得到 K 个黑块的最少涂色次数：给你一个长度为 n 下标从 0 开始的字符串 blocks ，
            blocks[i] 要么是 'W' 要么是 'B' ，表示第 i 块的颜色。字符 'W' 和 'B' 分别表示白色和黑色。
            给你一个整数 k ，表示想要 连续 黑色块的数目。
            每一次操作中，你可以选择一个白色块将它 涂成 黑色块。
            请你返回至少出现 一次 连续 k 个黑色块的 最少 操作次数。

1052.爱生气的书店老板：有一个书店老板，他的书店开了 n 分钟。每分钟都有一些顾客进入这家商店。
            给定一个长度为 n 的整数数组 customers ，其中 customers[i] 是在第 i 分钟开始时进入商店的顾客数量，所有这些顾客在第 i 分钟结束后离开。
            在某些分钟内，书店老板会生气。 如果书店老板在第 i 分钟生气，那么 grumpy[i] = 1，否则 grumpy[i] = 0。
            当书店老板生气时，那一分钟的顾客就会不满意，若老板不生气则顾客是满意的。
            书店老板知道一个秘密技巧，能抑制自己的情绪，可以让自己连续 minutes 分钟不生气，但却只能使用一次。
            请你返回 这一天营业下来，最多有多少客户能够感到满意 。

2841.几乎唯一子数组的最大和：给你一个整数数组 nums 和两个正整数 m 和 k 。
            请你返回 nums 中长度为 k 的 几乎唯一 子数组的 最大和 ，如果不存在几乎唯一子数组，请你返回 0 。
            如果 nums 的一个子数组有至少 m 个互不相同的元素，我们称它是 几乎唯一 子数组。

2461.长度为 K 子数组中的最大和：给你一个整数数组 nums 和一个整数 k 。请你从 nums 中满足下述条件的全部子数组中找出最大子数组和：
            子数组的长度是 k，且子数组中的所有元素 各不相同。
            返回满足题面要求的最大子数组和。如果不存在子数组满足这些条件，返回 0 。

1423.可获得的最大点数：几张卡牌 排成一行，每张卡牌都有一个对应的点数。点数由整数数组 cardPoints 给出。
            每次行动，你可以从行的开头或者末尾拿一张卡牌，最终你必须正好拿 k 张卡牌。
            你的点数就是你拿到手中的所有卡牌的点数之和。
            给你一个整数数组 cardPoints 和整数 k，请你返回可以获得的最大点数。

1652.拆炸弹（O(n)时间复杂度）：你有一个炸弹需要拆除，时间紧迫！你的情报员会给你一个长度为 n 的 循环 数组 code 以及一个密钥 k 。
            为了获得正确的密码，你需要替换掉每一个数字。所有数字会 同时 被替换。
            如果 k > 0 ，将第 i 个数字用 接下来 k 个数字之和替换。
            如果 k < 0 ，将第 i 个数字用 之前 k 个数字之和替换。
            如果 k == 0 ，将第 i 个数字用 0 替换。
            由于 code 是循环的， code[n-1] 下一个元素是 code[0] ，且 code[0] 前一个元素是 code[n-1] 。
            给你 循环 数组 code 和整数密钥 k ，请你返回解密后的结果来拆除炸弹！
*/
// ---------------------
// 模板题1
namespace s1456
{   // 模板题：步骤为先滑动k-1个范围，然后另开一个循环，判断右边进，更新数值，判断左边出，left，right移动。
    class Solution {
    public:
        int maxVowels(string s, int k) {
            int lens = s.size();
            unordered_set<char> vowels = { 'a', 'e', 'i', 'o', 'u' };
            int curVowel = 0;
            int left = 0, right = 0;
            for (; right < k; ++right) {
                if (vowels.find(s[right]) != vowels.end()) {
                    ++curVowel;
                }// 第一次遍历走完第一个窗口, 走完后right正好指向要滑动的第一个位置
            }
            int ans = curVowel;// 更新一次返回值（如果k正好等于lens，那么这次返回值就是最后的值
            for (; right < lens; ++left, ++right) {// 注意两个for循环的循环变量left, right都要定义在循环外
                if (vowels.find(s[right]) != vowels.end()) {
                    ++curVowel;
                }
                if (vowels.find(s[left]) != vowels.end()) {
                    --curVowel;
                }// 更新完新的窗口
                ans = max(ans, curVowel);// 更新返回值
            }
            return ans;
        }
    };
}

namespace s643
{   // 套用模板解题即可，注意下直接在滑动时改变sum，不要先除开来
    class Solution {
    public:
        double findMaxAverage(vector<int>& nums, int k) {
            int left = 0, right = 0;
            int n = nums.size();
            double sum = 0;
            for (; right < k; ++right) {
                sum += nums[right];
            }
            double ans = sum / k;
            for (; right < n; ++left, ++right) {
                sum += nums[right];
                sum -= nums[left];
                ans = max(ans, sum / k);
            }
            return ans;
        }
    };
}

namespace s1343
{   // 套用模板，和s643几乎一样
    int numOfSubarrays(vector<int>& arr, int k, int threshold) {
        int left = 0, right = 0;
        int n = arr.size();
        int sum = 0;
        int target = threshold * k;
        for (; right < k; ++right) {
            sum += arr[right];
        }
        int ans = sum >= target ? 1 : 0;
        for (; right < n; ++left, ++right) {
            sum += arr[right];
            sum -= arr[left];
            if (sum >= target) {
                ++ans;
            }
        }
        return ans;
    }
}

namespace s2090
{   // 还是类似的套用模板，但是窗口长度变为了2*k+1，同时通过预先初始化一个全为-1的数组，来避免后半段的遍历
    class Solution {
    public:
        vector<int> getAverages(vector<int>& nums, int k) {
            int left = 0, right = 0;
            int span = 2 * k + 1;
            int n = nums.size();
            vector<int> ans(n, -1);
            if (n < span) return ans;
            long long sum = 0;// 唯一坑点，需要改用long long，多个sum相加可能会超出int上限
            for (; right < span; ++right) {
                sum += nums[right];
            }
            ans[k] = sum / span;
            for (; right < n; ++left, ++right) {
                sum += nums[right];
                sum -= nums[left];
                ans[left + k + 1] = sum / span;
            }
            return ans;
        }
    };
}

// 仍是模板题，但是需要对问题进行抽象转化
namespace s2379
{   // 难点在于将题目要求抽象成：长度为k的滑动窗口内，最少的'W'数量为多少？
    class Solution {
    public:
        int minimumRecolors(string blocks, int k) {
            int left = 0, right = 0;
            int n = blocks.size();
            int wcount = 0;
            for (; right < k; ++right) {
                if (blocks[right] == 'W') {
                    ++wcount;
                }
            }
            int ans = wcount;
            for (; right < n; ++left, ++right) {
                if (blocks[left] == 'W') {
                    --wcount;
                }
                if (blocks[right] == 'W') {
                    ++wcount;
                }
                ans = min(ans, wcount);
            }
            return ans;
        }
    };
}

// 和哈希表结合的模板题
namespace s2841m
{   // 用额外空间O(n)的哈希表存储
    class Solution {
    public:
        long long maxSum(vector<int>& nums, int m, int k) {
            int left = 0, right = 0;
            int n = nums.size();
            long long sum = 0;
            unordered_map<int, int> numMap;
            for (; right < k; ++right) {
                ++numMap[nums[right]];
                sum += nums[right];
            }
            long long maxSum = numMap.size() >= m ? sum : 0;
            for (; right < n; ++left, ++right) {
                sum -= nums[left];  // 先处理左端点并没有特殊意义，先处理右端点也是对的
                --numMap[nums[left]];// 左端点已经在初始化第一个窗口处经过遍历，numMap对应value至少为1
                if (numMap[nums[left]] == 0) // 如果左端点对应value降为0，说明是个只出现一次的数字，窗口移动后要消除掉
                    numMap.erase(nums[left]);
                sum += nums[right];
                ++numMap[nums[right]];
                if (numMap.size() >= m) {
                    maxSum = max(maxSum, sum);
                }
            }
            return maxSum;
        }
    };
}

// 和s2841类似
namespace s2461
{   // 和2841几乎一样
    class Solution {
    public:
        long long maximumSubarraySum(vector<int>& nums, int k) {
            int left = 0, right = 0;
            int n = nums.size();
            unordered_map<int, int> numMap;
            long long sum = 0;
            for (; right < k; ++right) {
                sum += nums[right];
                ++numMap[nums[right]];
            }
            long long maxSum = numMap.size() == k ? sum : 0;
            for (; right < n; ++left, ++right) {
                sum -= nums[left];
                --numMap[nums[left]];
                if (numMap[nums[left]] == 0) {
                    numMap.erase(nums[left]);
                }
                sum += nums[right];
                ++numMap[nums[right]];
                if (numMap.size() == k) {
                    maxSum = max(maxSum, sum);
                }
            }
            return maxSum;
        }
    };
}

// 本质上还是模板题，但是端点的设置稍微有所不同
namespace s1423
{   // 
    class Solution {
    public:
        int maxScore(vector<int>& cardPoints, int k) {
            int n = cardPoints.size();
            int left = n - 1, right = 0;
            int sum = 0;
            for (; right < k; ++right) {
                sum += cardPoints[right];
            }
            int ans = sum;
            --right;// 注意要移动right，本体的right相当于模板里的left
            for (; right >= 0; --left, --right) {
                sum += cardPoints[left];// 左端点在数组最右边
                sum -= cardPoints[right];
                ans = max(ans, sum);
            }
            return ans;
        }
    };
}

namespace s1052
{   // 难点仍在于问题的抽象剥离：有点类似前缀和，先遍历一遍第一个窗口下的客人总量，就可以很方便的写出滑动窗口
    class Solution {
    public:
        int maxSatisfied(vector<int>& customers, vector<int>& grumpy, int minutes) {
            int left = 0, right = 0;
            int n = customers.size();
            int cusNum = 0;
            // 重点是要先完整遍历一遍整个customers和grumpy数组，确定第一个窗口下的客人总量
            for (; right < minutes; ++right) {
                cusNum += customers[right];
            }
            for (int i = right; i < n; ++i) {
                if (grumpy[i] == 0) {
                    cusNum += customers[i];
                }
            }
            int ans = cusNum;
            // 开始滑动
            for (; right < n; ++left, ++right) {
                if (grumpy[left] == 1) {// left处是生气的，窗口移走前忍住不生气有客人，窗口移动后客人失去
                    cusNum -= customers[left];
                }
                if (grumpy[right] == 1) {// right处是生气的，窗口移动后才能增加额外的客人，如果是不生气的，窗口移不移都不影响原来的客人总量
                    cusNum += customers[right];
                }
                ans = max(ans, cusNum);
            }
            return ans;
        }
    };
}

// 模板题2：循环数组的处理：取模 %，for循环判断条件不能局限于端点，而是直接按照移动次数设置
namespace s1652
{   // 整体结构还是和模板类似，但是由于循环数组的出现，for循环判断条件不能局限于端点，而是直接按照移动次数设置
    class Solution {
    public:
        vector<int> decrypt(vector<int>& code, int k) {
            int n = code.size();
            vector<int> ans(n, 0);
            if (k == 0) return ans;
            int left = k > 0 ? 1 : n + k;// 两种k对应不同的情况，但是窗口都是向右移动
            int right = left;
            int sum = 0;
            k = abs(k);
            for (int i = 0; i < k; ++i, ++right) {
                sum += code[right % n];// 取模运算是关键
            }
            ans[0] = sum;
            for (int i = 1; i < n; ++i, ++left, ++right) {
                sum += code[right % n];
                sum -= code[left % n];
                ans[i] = sum;
            }
            return ans;
        }
    };
}
// ---------------------

// 【1.2】进阶，选做(5)
/*
3439.重新安排会议得到最多空余时间 I：给你一个整数 eventTime 表示一个活动的总时长，
    这个活动开始于 t = 0 ，结束于 t = eventTime 。
    同时给你两个长度为 n 的整数数组 startTime 和 endTime 。它们表示这次活动中 n 个时间 没有重叠 的会议，
    其中第 i 个会议的时间为 [startTime[i], endTime[i]] 。
    你可以重新安排 至多 k 个会议，安排的规则是将会议时间平移，且保持原来的 会议时长 ，
    你的目的是移动会议后 最大化 相邻两个会议之间的 最长 连续空余时间。
    移动前后所有会议之间的 相对 顺序需要保持不变，而且会议时间也需要保持互不重叠。
    请你返回重新安排会议以后，可以得到的 最大 空余时间。
    注意，会议 不能 安排到整个活动的时间以外。

2134.最少交换次数来组合所有的 1 II：交换 定义为选中一个数组中的两个 互不相同 的位置并交换二者的值。
    环形 数组是一个数组，可以认为 第一个 元素和 最后一个 元素 相邻 。
    给你一个 二进制环形 数组 nums ，返回在 任意位置 将数组中的所有 1 聚集在一起需要的最少交换次数。

1297.子串的最大出现次数：给你一个字符串 s ，请你返回满足以下条件且出现次数最大的 任意 子串的出现次数：
    子串中不同字母的数目必须小于等于 maxLetters 。
    子串的长度必须大于等于 minSize 且小于等于 maxSize 。

2653.滑动子数组的美丽值：给你一个长度为 n 的整数数组 nums ，请你求出每个长度为 k 的子数组的 美丽值 。
    一个子数组的 美丽值 定义为：如果子数组中第 x 小整数 是 负数 ，那么美丽值为第 x 小的数，否则美丽值为 0 。
    请你返回一个包含 n - k + 1 个整数的数组，依次 表示数组中从第一个下标开始，每个长度为 k 的子数组的 美丽值 。
    子数组指的是数组中一段连续 非空 的元素序列。

438.找到字符串中所有字母异位词：给定两个字符串 s 和 p，找到 s 中所有 p 的 异位词 的子串，返回这些子串的起始索引。
    不考虑答案输出的顺序。
*/
// ---------------------
// 难点在于问题的转化
namespace s3439
{   // n个会议，之间的间隙共有n + 1个（包含两侧），k次重新安排会议，共能合并k + 1个空隙
    // 空隙长度可以是0
    // 问题转化为：给定一个大小为n + 1的数组，其长度为k + 1的滑动窗口中子数组之和的最大值
    class Solution {
    public:
        int maxFreeTime(int eventTime, int k, vector<int>& startTime, vector<int>& endTime) {
            int n = startTime.size();
            vector<int> freeTime(n + 1);
            freeTime[0] = startTime[0];
            freeTime[n] = eventTime - endTime[n - 1];
            for (int i = 1; i < n; ++i) {
                freeTime[i] = startTime[i] - endTime[i - 1];
            }// 先生成大小为n + 1的间隙时间数组，之后就是套模板了
            int left = 0, right = 0;
            int sum = 0;
            for (; right < k + 1; ++right) {
                sum += freeTime[right];
            }
            int ans = sum;
            for (; right < n + 1; ++left, ++right) {
                sum += freeTime[right];
                sum -= freeTime[left];
                ans = max(ans, sum);
            }
            return ans;
        }
    };
}

// 问题转化 + 循环数组的处理：取模 %，for循环判断条件不能局限于端点，而是直接按照移动次数设置
namespace s2134
{
    class Solution {
    public:
        int minSwaps(vector<int>& nums) {
            int n = nums.size();
            int k = accumulate(nums.begin(), nums.end(), 0);// 先计算出有多少个1
            if (k == 0 || k == n) return 0;//如果全为0或全为1，则不需要交换
            int left = 0, right = 0;
            int count = 0;// 统计窗口内0的数量
            for (; right < k; ++right) {// 将1的数量当作滑动窗口的长度
                if (nums[right] == 0) {
                    ++count;
                }
            }
            int ans = count;
            for (int i = 0; i < n; ++i, ++left, ++right) {// 这里i < n - 1才更准确
                if (nums[right % n] == 0) {// 对下标进行取模
                    ++count;
                }
                if (nums[left % n] == 0) {
                    --count;
                }
                ans = min(ans, count);
            }
            return ans;
        }
    };
}

// 熟悉用迭代器取子串
namespace s1297
{
    class Solution {
    public:
        int maxFreq(string s, int maxLetters, int minSize, int maxSize) {
            int n = s.size();
            unordered_map<char, int> windowChar;
            unordered_map<string, int> ansMap;
            int left = 0, right = 0;
            for (; right < minSize; ++right) {// 当k = minSize时，答案才会最大，maxSize没有意义
                ++windowChar[s[right]];
            }
            if (windowChar.size() <= maxLetters) {
                string sub = string(s.begin(), s.begin() + right);
                ++ansMap[sub];
            }
            for (; right < n; ++left, ++right) {
                ++windowChar[s[right]];
                --windowChar[s[left]];
                if (windowChar[s[left]] == 0) {
                    windowChar.erase(s[left]);
                }
                if (windowChar.size() <= maxLetters) {
                    string sub = string(s.begin() + left + 1, s.begin() + right + 1);
                    ++ansMap[sub];
                }
            }
            int ans = 0;
            for (auto& p : ansMap) {
                ans = max(p.second, ans);
            }
            return ans;
        }
    };
}

// 模板题3：计数排序，暴力枚举，当窗口判断条件比较复杂时，可以先只遍历k-1个元素来简化代码（按原模板1也行，但代码比较冗余）
// 以后碰到类似的题目都可以按这种思路来简化代码
namespace s2653m
{   // 模板1写法
    class Solution {
    public:
        vector<int> getSubarrayBeauty(vector<int>& nums, int k, int x) {
            int n = nums.size();
            vector<int> cnt(101);// 题干中指出nums的数值范围是[-50, 50]，所以可以暴力求解，记录每个值出现的次数
            vector<int> ans(n - k + 1);
            int left = 0, right = 0;
            for (; right < k; ++right) {
                ++cnt[nums[right] + 50];// 下标要有偏移量50
            }
            int find = x;
            for (int i = 0; i < 50; ++i) {// 这一段代码比较冗余，重复了一遍
                find -= cnt[i];
                if (find <= 0) {
                    ans[0] = -50 + i;
                    break;
                }
            }
            for (; right < n; ++left, ++right) {
                ++cnt[nums[right] + 50];
                --cnt[nums[left] + 50];
                int find = x;
                for (int i = 0; i < 50; ++i) {
                    find -= cnt[i];
                    if (find <= 0) {
                        ans[left + 1] = i - 50;
                        break;
                    }
                }
            }
            return ans;
        }
    };
}
namespace s2653o
{   // 写法相较于模板1更加简洁
    class Solution {
    public:
        vector<int> getSubarrayBeauty(vector<int>& nums, int k, int x) {
            int n = nums.size();
            vector<int> cnt(101);
            vector<int> ans(n - k + 1);
            int left = 0, right = 0;
            for (; right < k - 1; ++right) {
                ++cnt[nums[right] + 50];
            }
            for (; right < n; ++left, ++right) {
                ++cnt[nums[right] + 50];// 满足第一个窗口的大小
                int find = x;
                for (int i = 0; i < 50; ++i) {// 将窗口判断条件放在移进移出的中间
                    find -= cnt[i];
                    if (find <= 0) {
                        ans[left] = i - 50;
                        break;
                    }
                }
                --cnt[nums[left] + 50];// 将左端点移出窗口
            }
            return ans;
        }
    };
}

// m1,m2,m3为模板3写法（定长写法里m3最好），o1为不定长滑窗解法（见8.2专题），不定长写法比定长写法更好
namespace s438m1
{
    class Solution {
    public:
        vector<int> findAnagrams(string s, string p) {
            vector<int> ans;
            int n = s.size();
            int k = p.size();
            if (k > n) return ans;
            int left = 0, right = 0;
            vector<int> cnt(26);
            for (char c : p) {
                ++cnt[c - 'a'];
            }
            for (; right < k - 1; ++right) {
                --cnt[s[right] - 'a'];
            }
            for (; right < n; ++left, ++right) {
                --cnt[s[right] - 'a'];
                int count = 0;
                for (int i : cnt) {
                    if (i == 0) {
                        ++count;
                    }
                }
                if (count == 26) {
                    ans.push_back(left);
                }
                ++cnt[s[left] - 'a'];
            }
            return ans;
        }
    };
}
namespace s438m2 {
    // 相较于m1，窗口判定方法不同，可以通过break来提前结束判断，稍微快一点点
    class Solution {
    public:
        vector<int> findAnagrams(string s, string p) {
            vector<int> ans;
            int n = s.size();
            int m = p.size();
            if (m > n) return ans;
            int ref[26]{};
            for (int i = 0; i < m; ++i) {
                ++ref[p[i] - 'a'];
            }
            int letters[26]{};
            int left = 0, right = 0;
            for (; right < m - 1; ++right) {
                ++letters[s[right] - 'a'];
            }
            for (; right < n; ++right, ++left) {
                ++letters[s[right] - 'a'];
                for (int i = 0; i < 26; ++i) {
                    if (ref[i] != letters[i]) {
                        break;
                    }
                    if (i == 25) {
                        ans.push_back(left);
                    }
                }
                --letters[s[left] - 'a'];
            }
            return ans;
        }
    };
}
namespace s438m3
{   // 用k表示p中字符数，省去逐个比较26个字母的过程，大幅提速
    class Solution {
    public:
        vector<int> findAnagrams(string s, string p) {
            int n = s.size(), m = p.size();
            if (n < m) return {};

            int cnt[26]{};
            for (char c : p) {
                ++cnt[c - 'a'];
            }

            vector<int> ans;
            int k = m, left = 0, right = 0;
            for (; right < m - 1; ++right) {
                if (--cnt[s[right] - 'a'] >= 0) {
                    --k;
                };
            }

            for (; right < n; ++right) {
                if (--cnt[s[right] - 'a'] >= 0) {
                    --k;
                }
                if (k == 0) {
                    ans.push_back(left);
                }
                if (++cnt[s[left++] - 'a'] > 0) {
                    ++k;
                }
            }
            return ans;
        }
    };
}
namespace s438m4
{   // 很久以前写的代码，比较难懂，不够规划和套路化，不用看，仅作纪念...
    // 自己想的方法，有两层优化的滑动窗口，先初始一个窗口，然后左右端点同步移动，通过头尾端点的信息改变判断是否为异位词
    // 一是使用数组来代替哈希表
    // 二是只需检测头尾两个字母的频次就能判断是否为异位词（通过count记录频率错误字母的种类）
    // 在移动窗口时，容易遗漏部分情况（详见注释），要时刻注意，滑动窗口题目中，每次判断的对象是新【未改动前】的窗口
    class Solution {
    public:
        vector<int> findAnagrams(string s, string p) {
            int lenS = s.size(), lenP = p.size();
            vector<int> ans;
            // make sure there's at least one window
            if (lenP > lenS) return ans;

            // make frequency dictionary
            int record[26] = { 0 };
            int count = 0; // 记录频率错误字母的种类
            for (char c : p) {
                ++record[c - 'a'];
            }
            // initialize the first window
            int slow = 0, fast = 0;
            for (; fast < lenP; ++fast) {
                char temp = s[fast];
                --record[temp - 'a'];
                // record中元素不为0的可能性
                // s中出现p中元素种类之外的元素，从0减到了负数
                // s中出现p中元素种类之内的元素，从正数减到了非零值（更小或负数）
            }
            --fast; // 要记得把fast改回来，要不然下面移动时，区间长度就不对了
            for (int item : record) {
                if (item != 0) ++count;
            }
            if (count == 0) ans.push_back(0);

            // move window & update the start and end of record    
            // 端点移动前后的影响都要考虑到
            // 对新的【未改动前】的窗口进行判断
            while (fast < lenS - 1) {
                // 因为循环中是先移动再判断，若条件为< lenS, 则会出现越界，故改为lenS - 1  
                // move slow index
                if (record[s[slow] - 'a'] == 0) {// 将上一个【旧窗口对应的词典】的左端点影响消除
                    ++count;    // 若移动前的左端点对应的字母原本为正确频率，在递增复原后一定是变为不正确的，所以count增加
                }               // 比如"baa"和"aa"
                ++record[s[slow] - 'a'];
                if (record[s[slow] - 'a'] == 0) {
                    --count;    // 判断新窗口的端点：左端点字母若在递增后变为正确频率，要及时将count减少
                }
                ++slow;

                // move fast index
                ++fast;
                if (record[s[fast] - 'a'] == 0) {// 将上一个【旧窗口对应的词典】的右端点影响消除
                    ++count;    // 若移动后的新右端点对应的字母原本为正确频率，在递减后一定是变为不正确的，所以count增加
                }               // 比如"cbaebabacd"和"abc"，fast从cba移至bae时，原本旧窗口下，e是正确的，新窗口下e是错误的
                --record[s[fast] - 'a'];
                if (record[s[fast] - 'a'] == 0) {
                    --count;    // 判断新窗口的端点：右端点字母若在递减后变为正确频率，要及时将count减少
                }
                // check if current window is a match
                if (count == 0) ans.push_back(slow);
            }
            return ans;
        }
    };
}

// 与s438和s76类似，但是单个字母变成了单词，难度进一步上升
namespace s30m1
{
    class Solution {
    public:
        vector<int> findSubstring(string s, vector<string>& words) {
            vector<int> ans;
            int k = words.size();
            int wordLen = words[0].size();
            int windowLen = k * wordLen;
            int n = s.size();
            if (n < windowLen) return ans;

            unordered_map<string, int> wordMp;
            for (const auto& w : words) {
                ++wordMp[w];
            }

            for (int start = 0; start < wordLen; ++start) {
                // 剪枝
                if (start + windowLen > n) break;

                unordered_map<string, int> windowMp;
                int left = start;
                for (int right = start; right + wordLen <= n; right += wordLen) {
                    // in
                    string r = s.substr(right, wordLen);
                    // 如果r不在words内，那么以当前right为起点的整个窗口都不用判断了
                    if (!wordMp.count(r)) {
                        left = right + wordLen;
                        windowMp.clear();
                        continue;
                    }
                    ++windowMp[r];
                    // 当前点为right，加上单词的长度后右端点为right + wordLen - 1
                    // 窗口的左端点为left，两个左右端点的长度必须与windowLen相同
                    if (right + wordLen - left < windowLen) continue;

                    // update
                    if (windowMp == wordMp) {
                        ans.push_back(left);
                    }

                    // out
                    string l = s.substr(left, wordLen);
                    --windowMp[l];
                    left += wordLen;
                }
            }

            return ans;
        }
    };
}
namespace s30m2
{   // AI加的注释，怕之后再看自己代码就看不懂了
    class Solution {
    public:
        vector<int> findSubstring(string s, vector<string>& words) {
            vector<int> ans;                          // 存放所有合法起点

            int k = words.size();                     // 单词个数
            int wordLen = words[0].size();            // 每个单词长度（题目保证都相同）
            int windowLen = k * wordLen;              // 一个合法窗口的总长度
            int n = s.size();

            // 字符串本身都装不下一个窗口，直接返回空
            if (n < windowLen) return ans;

            // wordMp: 记录 words 里每种单词需要出现的次数
            //（用 map 而不是 set，因为单词可能重复，如 words = ["a","a"] 需要 a 出现 2 次）
            unordered_map<string, int> wordMp;
            for (const auto& w : words) {
                ++wordMp[w];
            }

            /*
             * 核心思想：真正的窗口起点不一定是 0 的整数倍。
             * 我们把 s 按 wordLen 切分成 wordLen 条"轨道"：
             *    轨道 0: s[0], s[wordLen], s[2*wordLen]...
             *    轨道 1: s[1], s[1+wordLen]...
             *        ...
             *    轨道 wordLen-1: ...
             * 任何合法的窗口，其起点只可能落在这某一条轨道上。
             * 于是在每条轨道上分别做滑动窗口。
             */
            for (int start = 0; start < wordLen; ++start) {
                // 剪枝：start 已经是本轨道最小的窗口起点；
                // 若 start 起的最小窗口都放不进 s，那后面更大的 start 更不可能，直接停。
                if (start + windowLen > n) break;

                // windowMp: 当前窗口 [left, right+wordLen) 内每种单词实际出现的次数
                unordered_map<string, int> windowMp;
                int left = start;                     // 窗口左边界（单词起点）

                // right 是"当前要加入窗口的单词"的起点；条件要放下一个单词即可
                for (int right = start; right + wordLen <= n; right += wordLen) {

                    // ---------- 1. 入窗（in）----------
                    string r = s.substr(right, wordLen);

                    // 遇到"坏词"（不在 words 里）：任何答案都不可能包含它，
                    // 所以把这个词之前的整个窗口全部作废。
                    if (!wordMp.count(r)) {
                        left = right + wordLen;   // 窗口左边界跳到坏词之后
                        windowMp.clear();         // 关键！清掉旧窗口的计数，避免脏数据累加
                        continue;
                    }

                    ++windowMp[r];                // 好词，加入窗口计数

                    // ---------- 2. 窗口是否已满 ----------
                    // 当前窗口区间是 [left, right + wordLen)，
                    // 长度 = right + wordLen - left，不够 windowLen 就继续加词
                    if (right + wordLen - left < windowLen) continue;

                    // ---------- 3. 窗口满了，判断是否合法 ----------
                    // 窗口内每种词的出现次数 == 目标次数 => 起点 left 是一个答案
                    if (windowMp == wordMp) {
                        ans.push_back(left);
                    }

                    // ---------- 4. 出窗（out）----------
                    // 把最左边的词弹出窗口，为下一个位置腾出空间
                    string l = s.substr(left, wordLen);
                    --windowMp[l];
                    if (windowMp[l] == 0) windowMp.erase(l);   // 可选：清掉 0，让比较更干净
                    left += wordLen;
                }
            }

            return ans;
        }
    };
}
// ---------------------

// 【1.3】其他，选做(3)
/*
2269. 找到一个数字的 K 美丽值：一个整数 num 的 k 美丽值定义为 num 中符合以下条件的 子字符串 数目：
子字符串长度为 k 。子字符串能整除 num 。给你整数 num 和 k ，请你返回 num 的 k 美丽值。
注意：允许有 前缀 0 。0 不能整除任何值。

1984. 学生分数的最小差值：给你一个 下标从 0 开始 的整数数组 nums ，其中 nums[i] 表示第 i 名学生的分数。
另给你一个整数 k 。从数组中选出任意 k 名学生的分数，使这 k 个分数间 最高分 和 最低分 的 差值 达到 最小化 。
返回可能的 最小差值 。

1461. 检查一个字符串是否包含所有长度为 K 的二进制子串：给你一个二进制字符串 s 和一个整数 k 。
如果所有长度为 k 的二进制字符串都是 s 的子串，请返回 true ，否则请返回 false 。
*/
// ---------------------
// 考验字符串和数字的互相转换，滑动窗口的算法倒是不怎么涉及
namespace s2269
{
    class Solution {
    public:
        int divisorSubstrings(int num, int k) {
            string s = to_string(num);
            int n = s.size();
            int count = 0;
            for (int left = 0; left < n - k + 1; ++left) {
                string sub(s.begin() + left, s.begin() + left + k);
                // string sub = s.substr(left, k);
                // 这里取子字符串也可以用substr
                int subNum = stoi(sub);
                if (subNum == 0) continue;
                if (num % subNum == 0) ++count;
            }
            return count;
        }
    };
}

// 简单题
namespace s1984
{
    class Solution {
    public:
        int minimumDifference(vector<int>& nums, int k) {
            sort(nums.begin(), nums.end());
            int n = nums.size();
            int ans = nums[n - 1] - nums[0];
            for (int left = 0; left < n - k + 1; ++left) {
                int diff = nums[left + k - 1] - nums[left];
                ans = min(ans, diff);
            }
            return ans;
        }
    };
}

// 熟悉位运算乘法 + 最高位清除，整形，字符串哈希表提速
namespace s1461o1
{
    // 最容易想到的做法是用unordered_set存储子串，最后检查哈希表的大小，但这样每次insert都是O(k)的时间复杂度
    // 可以用位运算利用二进制字符串的性质，将哈希表中存储的字符串换成整形
    // 而换成整形后，可以进一步更换为数组，继续提速
    class Solution {
    public:
        bool hasAllCodes(string s, int k) {
            int n = s.size();
            if (k > n) return 0;
            int right = 0;
            int num = 1 << k;
            vector<bool> used(num, false);
            int cur = 0;
            for (; right < k - 1; ++right) {
                cur = 2 * cur + (s[right] == '1');
            }
            for (; right < n; ++right) {
                cur = 2 * cur + (s[right] == '1');// 计算当前窗口下的数值
                used[cur] = true;
                cur &= ~(1 << (k - 1));
                // 最高一位空出，1 << k - 1 为0111...，取反后为1000...，再经过与运算&，可以清空最高位
            }
            for (bool i : used) {
                if (!i) return false;
            }
            return true;
        }
    };

}
// ---------------------