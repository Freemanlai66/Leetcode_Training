#pragma once
#include <vector>
#include <string>

using namespace std;

// 本专题中的题目大多来自于TOP150中的位运算板块，时间有限，并未系统参考灵神的位运算题单

namespace s67o1
{   // 与链表节点求和题目的框架非常像
    class Solution {
    public:
        string addBinary(string a, string b) {
            string ans = "";
            int m = a.size(), n = b.size();
            int i = m - 1, j = n - 1;
            int carry = 0;

            while (i >= 0 || j >= 0 || carry) {
                int num = carry;
                if (i >= 0) num += a[i--] - '0';
                if (j >= 0) num += b[j--] - '0';
                ans.push_back(num % 2 + '0');
                carry = num / 2;
            }

            reverse(ans.begin(), ans.end());
            return ans;
        }
    };
}

// 归并 + 通过位运算实现并行运算
namespace s190o1
{   // O(1)时间复杂度
    class Solution {
    private:
        uint32_t reverseBits32(uint32_t n) {
            static constexpr uint32_t m1 = 0x55555555;
            static constexpr uint32_t m2 = 0x33333333;
            static constexpr uint32_t m3 = 0x0f0f0f0f;
            static constexpr uint32_t m4 = 0x00ff00ff;

            n = (n >> 1 & m1) | ((n & m1) << 1);
            n = (n >> 2 & m2) | ((n & m2) << 2);
            n = (n >> 4 & m3) | ((n & m3) << 4);
            n = (n >> 8 & m4) | ((n & m4) << 8);
            return (n >> 16) | (n << 16);
        }

    public:
        int reverseBits(int n) {
            // 必须把n转成uint32_t才行，中间值n可能为负数！！
            return reverseBits32(n);
        }
    };
}

// 计算1的个数，位运算的基础，也是s137的基础之一
namespace s191o1
{
    class Solution {
    public:
        int hammingWeight(int n) {
            unsigned int x = n;
            int ans = 0;
            // 因为下面while循环采用的是x 与 0判断是否相等的做法
            // 如果n是负数，右移运算高位会补1，此时会进入死循环
            // 所以必须转成unsigned int
            while (x != 0) {
                ans += x & 1;
                x >>= 1;
            }

            return ans;
        }
    };
}

namespace s136o1
{   // a ^ a = 0, a ^ 0 = 1，位运算最基础的运用
    // 异或运算的本质是按比特位的模2加法，让每一个比特位在0, 1之间轮转
    // 异或运算的本质也可以理解成无进位按位相加
    // 本题思路：如果将所有数字逐个按照比特位相加，并把每个比特位上的结果（也就是1的个数）模2，即为答案
    // 因为解题思路和异或运算恰好符合，所以可以用异或来做
    // 如果重复次数不是2，而是改成3，比如s137题，那么就要按原有的思路设计新做法
    class Solution {
    public:
        int singleNumber(vector<int>& nums) {
            int ans = 0;
            for (int x : nums) {
                ans ^= x;
            }
            return ans;
        }
    };
}
namespace s137o1
{   // 本质是要实现按比特位的模3加法
    // o1是暴力做法，时间复杂度O(n)，可以继续优化
    class Solution {
    public:
        int singleNumber(vector<int>& nums) {
            int ans = 0;

            for (int i = 0; i < 32; ++i) {
                // cnt: 第 i 个比特位上，所有数字的1的个数
                int cnt = 0;
                for (int x : nums) {
                    cnt += (x >> i) & 1;
                }
                // 因为除了答案，其他数都恰好出现3次，cnt1 % 3的结果只可能是0或1
                // 或运算将对应位填入答案
                ans |= (cnt % 3) << i;
            }

            return ans;
        }
    };
}
namespace s137o2
{   // 按照00->01->10->00轮转的思路，将两个比特位拆分开成a和b
    // 公式是按照单个比特位，以及x == 1的情况推出的
    // 恰好x == 0时也成立
    // 同时异或运算可以推广到32位上，所以可以并行加速
    // o1是按比特位逐个运算的，串行计算32次

    // 时间复杂度也是O(1)，但是常数因子比o1小
    class Solution {
    public:
        int singleNumber(vector<int>& nums) {
            int a = 0, b = 0;
            for (int x : nums) {
                int tmp_a = a;
                a = (a ^ x) & (a | b);
                b = (b ^ x) & ~tmp_a;
            }
            return b;
        }
    };
}
namespace s137o3
{   // o2基础上进一步优化，但是我看不太懂（暂时跳过，掌握到o2就满意了）
    class Solution {
    public:
        int singleNumber(vector<int>& nums) {
            int a = 0, b = 0;
            for (int x : nums) {
                b = (b ^ x) & ~a;
                a = (a ^ x) & ~b;
            }
            return b;
        }
    };
}