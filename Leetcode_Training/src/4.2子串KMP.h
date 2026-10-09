#include <vector>
#include <string>
using namespace std;

/*
28.找出字符串中第一个匹配项的下标：给你两个字符串 haystack 和 needle。
   请你在 haystack 字符串中找出 needle 字符串的第一个匹配项的下标（下标从 0 开始）。
   如果 needle 不是 haystack 的一部分，则返回  -1 。(等效于实现substr)

459.重复的子字符串：给定一个非空的字符串 s ，检查是否可以通过由它的一个子串重复多次构成。
*/

// 模板题1：最基础的KMP，直接看o4解法
namespace s28o1
{
    //暴力解法，最坏可能要把haystack中所有needle长度的子串都遍历一遍，时间复杂度O(m*n)
    class Solution {
    public:
        int strStr(string haystack, string needle) {
            int n = haystack.size(), m = needle.size();
            for (int i = 0; i + m <= n; i++) {
                bool flag = true;
                for (int j = 0; j < m; j++) {
                    if (haystack[i + j] != needle[j]) {
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    return i;
                }
            }
            return -1;
        }
    };
}
namespace s28o2
{
    //直接调用stl库函数
    class Solution {
    public:
        int strStr(string haystack, string needle) {
            return haystack.find(needle);
        }
    };
}
namespace s28o3
{
    //KMP解法（常见字符串匹配算法），暂不学习，解析在收藏夹里（时间复杂度O（m+n），但占用的空间会更大
    // 空间复杂度O(m)
    class Solution {
    public:
        int strStr(string haystack, string needle) {
            int n = haystack.size(), m = needle.size();
            if (m == 0) {
                return 0;
            }
            vector<int> pi(m);
            for (int i = 1, j = 0; i < m; i++) {
                while (j > 0 && needle[i] != needle[j]) {
                    j = pi[j - 1];
                }
                if (needle[i] == needle[j]) {
                    j++;
                }
                pi[i] = j;
            }
            for (int i = 0, j = 0; i < n; i++) {
                while (j > 0 && haystack[i] != needle[j]) {
                    j = pi[j - 1];
                }
                if (haystack[i] == needle[j]) {
                    j++;
                }
                if (j == m) {
                    return i - m + 1;
                }
            }
            return -1;
        }
    };
}
namespace s28o4
{
    // https://www.bilibili.com/video/BV1Er421K7kF
    // 从前缀函数一步步推导KMP，方便记忆，且从原理出发，建议优先记忆o4
    // 虽然容易记忆且代码简洁，但缺点在于拼接了一次字符串，时间复杂度从O(m)变成了O(m + n)

    class Solution {
    public:
        int strStr(string haystack, string needle) {
            int m = needle.size();
            // 将模式串和主串进行拼接
            string s = needle + '#' + haystack;
            // pi为前缀函数，表示下标为i位置的真前后缀匹配的最大值（不包含本身）
            // 如ATAATA的pi[i]就是0，0，1，1，2，3
            vector<int> pi(s.size());

            for (int i = 1; i < s.size(); ++i) {
                int len = pi[i - 1];

                // 逐步寻找最大匹配的len值
                while (len > 0 && s[i] != s[len]) {
                    len = pi[len - 1];
                }
                // 用i - 1位置上的pi值更新pi[i]
                if (s[i] == s[len]) {
                    pi[i] = len + 1;
                    if (pi[i] == m) {
                        // m + 1 + x + m = i + 1
                        // x = i - 2 * m
                        return i - 2 * m;
                    }
                }
            }

            return -1;
        }
    };
}
namespace s28o5
{   // 如果沿着o4的思路，将for循环拆成两个，就可以省去拼接的过程

    class Solution {
    public:
        int strStr(string haystack, string needle) {
            int n = haystack.size(), m = needle.size();
            vector<int> pi(m);

            // 阶段1：对needle计算前缀函数pi, 对应拼接写法中s的[1, m - 1]部分
            for (int i = 1; i < m; ++i) {
                int len = pi[i - 1];
                while (len > 0 && needle[i] != needle[len]) {
                    len = pi[len - 1];
                }
                if (needle[i] == needle[len]) {
                    pi[i] = len + 1;
                }
            }

            // 阶段2：用pi去匹配haystack，对应拼接写法中s的[m + 1, ...]部分
            int j = 0;  // 当前已匹配的字符数（等价于拼接写法中haystack部分的pi[i]）
            for (int k = 0; k < n; ++k) {
                while (j > 0 && haystack[k] != needle[j]) {
                    j = pi[j - 1]; // 回退，和拼接写法完全一样的逻辑
                    // pi[i - 1]代表上一轮匹配的长度，而j正好就是上一轮的值，不用每次都重新定义j
                    // while (len > 0 && s[i] != s[len])	
                    // while (j > 0 && haystack[k] != needle[j])
                    // 上面两行是一一对应的
                }
                if (haystack[k] == needle[j]) {
                    ++j;                  // 相当于拼接写法中的 pi[i] = len + 1
                    if (j == m) {         // 完全匹配
                        return k - m + 1; // 起始位置
                    }
                }
            }

            return -1;
        }
    };
}

namespace s459o1
{   // 子串至少重复两次，所以长度为 1 的字符串（如 "a"）应返回 false
    /*
    将原字符串 s 复制一遍得到 s + s，然后去掉第一个和最后一个字符（避免出现原串本身），
    再检查这个新字符串是否包含 s。如果 s 是由重复子串构成的，那么 s + s 去掉首尾后必然还包含 s。

    原理：设 s = t*t*...*t（k 次），那么 s+s = t...t t...t（2k 次），去掉首尾各一个字符后，
    中间仍然有 (2k-2) 个 t，而 s 有 k 个 t，只要 k ≥ 2，s 必然会在中间出现（因为 2k-2 ≥ k 当 k ≥ 2）。
    
    时空复杂度都是O(n)
    */
    class Solution {
    public:
        bool repeatedSubstringPattern(string s) {
            string t = s + s;
            // 去掉 t 的第一个和最后一个字符
            t = t.substr(1, t.size() - 2);
            return t.find(s) != string::npos;
        }
    };
}
namespace s459o2
{   // 基于s28o4的KMP写法，时空复杂度都为O(n)
    /*
    如果 s 是由某个子串 t 重复 k 次构成的（k ≥ 2），那么：
        s 的长度 n = k * |t|。
        s 的最长相等前后缀的长度一定是 (k-1) * |t|。

    反过来：记 p = pi[n-1]（整个字符串的最长相等前后缀长度）。
    如果 p > 0 且 n % (n - p) == 0，则 s 一定能由长度为 n - p 的子串重复构成。

    所以我们要做的就是求p（也即求解整个字符串的前缀函数值），并判断其与n的关系
        要求：p > 0 && n % (n - p) == 0
    */
    class Solution {
    public:
        bool repeatedSubstringPattern(string s) {
            int n = s.size();
            vector<int> pi(n);

            // 1. 计算整个字符串 s 的前缀函数
            for (int i = 1; i < n; ++i) {
                int len = pi[i - 1];
                while (len > 0 && s[i] != s[len]) {
                    len = pi[len - 1];
                }
                if (s[i] == s[len]) {
                    pi[i] = len + 1;
                }
            }

            // 2. 利用 pi[n-1] 判断
            int p = pi[n - 1];
            return p > 0 && n % (n - p) == 0;
        }
    };
}
namespace s459o3
{
    // o3写法是常规的KMP写法，其实就是把len换成了j，不每次重新定义，本质是相同的
    class Solution {
    public:
        bool repeatedSubstringPattern(string s) {
            int n = s.size();
            // 计算 next 数组（部分匹配表）
            vector<int> next(n, 0);
            for (int i = 1, j = 0; i < n; ++i) {
                while (j > 0 && s[i] != s[j]) {
                    j = next[j - 1];
                }
                if (s[i] == s[j]) {
                    ++j;
                }
                next[i] = j;
            }
            // 检查是否能由子串重复构成
            int len = n - next[n - 1];
            return next[n - 1] > 0 && n % len == 0;
        }
    };
}
