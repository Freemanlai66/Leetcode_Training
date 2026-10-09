#pragma once
#include <vector>
#include <array>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
using namespace std;

// 问题待定：
/*
sxxx：xxx
*/

/*
模板题：
1.标准错位前缀和，s[i] = s[i - 1] + nums[i - 1]，规避边界特判 ：303
2.判断两个字符串的关系的模板，字符需对应，但个数可不同，如hello, hhhello：809
3.左右移动'L','R'字符使两个字符串相同，脑筋急转弯：2337
4.删除字符 + 判断子序列，o2预处理索引法适合超大字典场景：524
*/

// 双序列双指针：双指针 + 判断子序列

// 【4.1】双指针 (11)
// 需要掌握如何限制指针遍历时的边界
/*
2109.向字符串添加空格：给你一个下标从 0 开始的字符串 s ，以及一个下标从 0 开始的整数数组 spaces 。
数组 spaces 描述原字符串中需要添加空格的下标。每个空格都应该插入到给定索引处的字符值 之前 。
例如，s = "EnjoyYourCoffee" 且 spaces = [5, 9] ，那么我们需要在 'Y' 和 'C' 之前添加空格，
这两个字符分别位于下标 5 和下标 9 。因此，最终得到 "Enjoy Your Coffee" 。
请你添加空格，并返回修改后的字符串。

2540.最小公共值：给你两个整数数组 nums1 和 nums2 ，它们已经按非降序排序，请你返回两个数组的 最小公共整数 。
如果两个数组 nums1 和 nums2 没有公共整数，请你返回 -1 。
如果一个整数在两个数组中都 至少出现一次 ，那么这个整数是数组 nums1 和 nums2 公共 的。

88.合并两个有序数组：给你两个按 非递减顺序 排列的整数数组 nums1 和 nums2，另有两个整数 m 和 n ，
分别表示 nums1 和 nums2 中的元素数目。
请你 合并 nums2 到 nums1 中，使合并后的数组同样按 非递减顺序 排列。
注意：最终，合并后数组不应由函数返回，而是存储在数组 nums1 中。为了应对这种情况，nums1 的初始长度为 m + n，
其中前 m 个元素表示应合并的元素，后 n 个元素为 0 ，应忽略。nums2 的长度为 n 。
*/
// ---------------------
namespace s2109m1
{
    class Solution {
    public:
        string addSpaces(string s, vector<int>& spaces) {
            string ans = "";
            int p2 = 0, n = s.size(), m = spaces.size();
            for (int i = 0; i < n; ++i) {
                if (i == spaces[p2]) {
                    ans += " ";// 要处理字符串终止符，没有+= ' '好
                    ++p2;
                    if (p2 == m) {
                        return ans + string(s.begin() + i, s.end());// 这里完全可以加在if的判断条件里
                    }
                }
                ans += s[i];
            }
            return "";
        }
    };
}
namespace s2109m2
{   // 优化版
    class Solution {
    public:
        string addSpaces(string s, vector<int>& spaces) {
            string ans = "";
            int j = 0, n = s.size(), m = spaces.size();
            for (int i = 0; i < n; ++i) {
                if (j < m && i == spaces[j]) {
                    ans += ' ';
                    ++j;
                }
                ans += s[i];
            }
            return ans;
        }
    };
}

namespace s2540m1
{
    class Solution {
    public:
        int getCommon(vector<int>& nums1, vector<int>& nums2) {
            int p1 = 0, p2 = 0;
            int n1 = nums1.size(), n2 = nums2.size();
            while (p1 < n1 && p2 < n2) {
                if (nums1[p1] == nums2[p2]) {
                    return nums1[p1];
                }
                else if (nums1[p1] > nums2[p2]) {
                    ++p2;
                }
                else {
                    ++p1;
                }
            }
            return -1;
        }
    };
}

// 模板题1：双序列双指针的边界控制，应该要做出来的，逆序双指针，想到了逆序，但边界的处理没想到怎么做
namespace s88o1
{   // 正序双指针会发生元素覆盖，逆序填入元素不会产生覆盖
    // 归并排序中也有合并两个有序数组，但那个场景下有辅助数组，所以是正序遍历
    class Solution {
    public:
        void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
            int p1 = m - 1;
            int p2 = n - 1;
            int p = n + m - 1;

            while (p2 >= 0) {
                if (p1 >= 0 && nums1[p1] > nums2[p2]) {
                    nums1[p--] = nums1[p1--];
                }
                else {
                    nums1[p--] = nums2[p2--];
                }
            }

            /*// 相当于下面这个，o1的写法更简洁
            int i = m - 1, j = n - 1, k = m + n - 1;

            while (i >= 0 && j >= 0) {
                if (nums1[i] > nums2[j]) {
                    nums1[k--] = nums1[i--];
                } else {
                    nums1[k--] = nums2[j--];
                }
            }
            while (j >= 0) nums1[k--] = nums2[j--];
            */
        }
    };
}

// 模仿s88进行p1, p2的边界控制
namespace s2570m1
{
    class Solution {
    public:
        vector<vector<int>> mergeArrays(vector<vector<int>>& nums1, vector<vector<int>>& nums2) {
            vector<vector<int>> ans;
            int p1 = 0, p2 = 0, p = 0;
            int n = nums1.size(), m = nums2.size();
            while (p2 < m) {
                auto& x = nums1[p1];
                auto& y = nums2[p2];
                if (p1 < n && x[0] == y[0]) {
                    int sum = x[1] + y[1];
                    ans.push_back({ x[0], sum });
                    ++p1; ++p2;
                }
                else if (p1 < n && x[0] < y[0]) {
                    ans.push_back({ x[0], x[1] });
                    ++p1;
                }
                else {
                    ans.push_back({ y[0], y[1] });
                    ++p2;
                }
            }
            for (; p1 < n; ++p1) {// while循环退出后，nums1内可能还有元素没遍历过
                auto& x = nums1[p1];
                ans.push_back({ x[0], x[1] });
            }
            return ans;
        }
    };
}

// s349和s350类似，都可以用哈希表来做
// 考虑s350的进阶问题：假如两个序列都是排序过的，如何做？（排序 + 双指针，s350o1）
namespace s349m1
{
    class Solution {
    public:
        vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
            vector<int> ans;
            unordered_set<int> st1(nums1.begin(), nums1.end());
            unordered_set<int> st2;
            for (int x : nums2) {
                if (st1.find(x) != st1.end()) {
                    st2.insert(x);
                }
            }
            return vector<int>(st2.begin(), st2.end());
        }
    };
}
namespace s350m1
{   // 哈希表一次遍历
    class Solution {
    public:
        vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
            unordered_map<int, int> mp1;
            vector<int> ans;
            for (int x : nums1) {
                ++mp1[x];
            }
            for (int x : nums2) {
                if (mp1.find(x) != mp1.end()) {
                    ans.push_back(x);
                    if (--mp1[x] == 0) {
                        mp1.erase(x);
                    }
                }
            }
            return ans;
        }
    };
}
namespace s350o1
{   // 因为题干要求交集中答案以重复次数最低的为准，其实就意味着求两个序列的交集，反而更简单了
    class Solution {
    public:
        vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
            sort(nums1.begin(), nums1.end());
            sort(nums2.begin(), nums2.end());
            vector<int> ans;
            int i = 0, j = 0;
            int n = nums1.size(), m = nums2.size();
            while (i < n && j < m) {
                int x = nums1[i], y = nums2[j];
                if (x < y) {
                    ++i;
                }
                else if (x > y) {
                    ++j;
                }
                else {
                    ans.push_back(x);
                    ++i;
                    ++j;
                }
            }
            return ans;
        }
    };
}

// 该题用二分查找做更合适，o2的双指针做法只用来扩展思路，不用需要熟悉该方法
namespace s1385o2
{	// 双指针做法，时间复杂度和二分查找类似
    // 两次排序为nlogn + mlogm，内部while循环为常量级
    class Solution {
    public:
        int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
            sort(arr1.begin(), arr1.end());
            // 因为arr1也排序了，所以x - d只会单调增大，j只会变大，只会完整各遍历一次arr1和arr2
            sort(arr2.begin(), arr2.end());
            int cnt = 0, j = 0;// 将下标j放在循环外
            for (int& x : arr1) {
                while (j < arr2.size() && arr2[j] < x - d) {
                    ++j;
                }
                if (j == arr2.size() || arr2[j] > x + d) {
                    ++cnt;
                }
            }
            return cnt;
        }
    };
}

namespace s925m1
{
    class Solution {
    public:
        bool isLongPressedName(string name, string typed) {
            int i = 0, j = 0;
            int n = name.size(), m = typed.size();
            while (i < n) {
                if (j < m && name[i] == typed[j]) {
                    ++i;
                    ++j;
                }
                else if (j > 0 && j < m && typed[j] == typed[j - 1]) {
                    ++j;
                }
                else {
                    return false;// 如果typed字符长度不够，这里也包含了
                }
            }
            while (j < m && typed[j] == typed[j - 1]) {
                ++j;// 可能typed比name要长，需要判断多出的部分是长按的还是错误字符
            }
            return j == m;
        }
    };
}
namespace s925o1
{   // 官解做法是在while中判断j < m，比我的写法更好
    class Solution {
    public:
        bool isLongPressedName(string name, string typed) {
            int i = 0, j = 0;
            int n = name.size(), m = typed.size();
            while (j < m) {
                if (i < n && name[i] == typed[j]) {
                    i++;
                    j++;
                }
                else if (j > 0 && typed[j] == typed[j - 1]) {
                    j++;
                }
                else {
                    return false;
                }
            }
            return i == n;
        }
    };
}

// 模板题2：判断两个字符串的关系的模板，字符需对应，但个数可不同，如hello, hhhello
namespace s809o1
{
    class Solution {
    public:
        int expressiveWords(string s, vector<string>& words) {
            int count = 0;
            for (const auto& word : words) {
                if (isStretchy(s, word)) {
                    ++count;
                }
            }
            return count;
        }

    private:
        bool isStretchy(const string& s, const string& word) {
            int n = s.size(), m = word.size();
            int i = 0, j = 0;

            while (i < n && j < m) {
                if (s[i] != word[j]) return false;

                // 统计s中当前字符的连续长度
                char c = s[i];
                int s_len = 0;
                while (i < n && s[i] == c) {
                    ++i;
                    ++s_len;
                }

                // 统计word中当前字符的连续长度
                int w_len = 0;
                while (j < m && word[j] == c) {
                    ++j;
                    ++w_len;
                }

                // 检查长度关系
                if (w_len > s_len) return false;
                if (s_len < 3 && w_len != s_len) return false;
            }

            // 确保两个字符串都已遍历完
            return i == n && j == m;
        }
    };
}

// 模板题3：左右移动'L','R'字符使两个字符串相同，脑筋急转弯
namespace s2337m1
{   // 1.确保start和target中字符一一对应
    // 2.确保start中'L'下标大于target中'L'下标，'R'下标小于target中'R'下标
    class Solution {
    public:
        bool canChange(string start, string target) {
            string s = start, t = target;
            s.erase(remove(s.begin(), s.end(), '_'), s.end());
            t.erase(remove(t.begin(), t.end(), '_'), t.end());
            if (s != t) {
                return false;
            }
            int i = 0, j = 0, n = start.size();
            while (j < n) {
                if (target[j] == '_') {
                    ++j;
                    continue;
                }
                while (start[i] == '_') {
                    ++i;
                }
                if (start[i] == 'L') {
                    if (i < j) return false;
                }
                else {
                    if (i > j) return false;
                }
                ++i;
                ++j;
            }
            return true;
        }
    };
}
// 同s2337，如果不先做s2337，这道题是真一时半会看不懂...
namespace s777m1
{
    class Solution {
    public:
        bool canTransform(string start, string result) {
            string s = start, t = result;
            s.erase(remove(s.begin(), s.end(), 'X'), s.end());
            t.erase(remove(t.begin(), t.end(), 'X'), t.end());
            if (s != t) {
                return false;
            }
            int i = 0, j = 0, n = start.size();
            while (i < n) {
                if (result[j] == 'X') {
                    ++j;
                    continue;
                }
                while (start[i] == 'X') {
                    ++i;
                }
                if (start[i] == 'L') {
                    if (i < j) return false;
                }
                else {
                    if (i > j) return false;
                }
                ++i;
                ++j;
            }
            return true;
        }
    };
}

// o1：另外新建两个字符串，将字符串当作栈处理，得到退格之后的字符串再比较，思路简单但是空间复杂度高
// o2：因为退格操作追溯性太强，不好同时对两个序列处理，可以各自原地修改后再比较，O(1)空间复杂度
// o3：优先掌握o2，更简单，正向不好处理，选择逆序遍历，同时处理两个字符串，逐个比较字符，同样O(1)空间复杂度
namespace s844o1
{	// 对两个数组分别进行一次遍历得到去除空格的字符串，再对两个新字符串进行逐个比较
    // 需要考虑ret.empty()情况
    class Solution {
    private:
        string clean(string& s) {
            string ret;
            for (char c : s) {
                if (c != '#') {
                    ret += c;
                }
                else if (!ret.empty()) {
                    ret.pop_back();
                }
            }
            return ret;
        }
    public:
        bool backspaceCompare(string s, string t) {
            return clean(s) == clean(t);
        }
    };
}
namespace s844o2
{	//指针原地修改，先得到两个新字符串再比较
    class Solution {
    public:
        bool backspaceCompare(string s, string t) {
            int ps = 0, n = s.size();
            for (char c : s) {
                if (c != '#') {// 将字符串s当成栈
                    s[ps++] = c;// 无需额外字符串，直接原地修改
                }
                else if (ps > 0) {// 防止下标为负
                    --ps;
                }
            }
            int pt = 0, m = t.size();
            for (char c : t) {
                if (c != '#') {
                    t[pt++] = c;
                }
                else if (pt > 0) {
                    --pt;
                }
            }
            if (ps != pt) return false;// 退格后长度不一致
            for (int i = 0; i < ps; ++i) {
                if (s[i] != t[i]) {
                    return false;// 只比较前ps个元素的异同，剩下的元素不用比较
                }
            }
            return true;
        }
    };
}
namespace s844o3
{	// 逆序遍历字符串，用 skip_s 和 skip_t 来记录当前需要跳过的字符数（即退格的累积效果）
    class Solution {
    public:
        bool backspaceCompare(string S, string T) {
            int i = S.length() - 1, j = T.length() - 1;
            int skipS = 0, skipT = 0;
            while (i >= 0 || j >= 0) {
                // 处理s的退格
                while (i >= 0) {
                    if (S[i] == '#') {
                        ++skipS; --i;// 逆序判定需要删除的元素递减，规避正向遍历如"bxo#j##tw"的问题
                    }
                    else if (skipS > 0) {// 如果skipS > 0表示，后一个元素是#，当前元素需要删除
                        --skipS;  --i;
                    }
                    else {// 表示该元素不用删除，此时i指向的元素是需要留下的元素
                        break;
                    }
                }
                // 处理t的退格
                while (j >= 0) {
                    if (T[j] == '#') {
                        ++skipT; --j;
                    }
                    else if (skipT > 0) {
                        --skipT, --j;
                    }
                    else {
                        break;
                    }
                }
                if (i >= 0 && j >= 0) {// 可能i，j的下标因#过多，存在变成-1的情况
                    if (S[i] != T[j]) {// 每次找到一个不需要被删除的元素就进行一次比较
                        return false;
                    }
                }
                else if (i >= 0 || j >= 0) {// 进入这一分支的条件是一个字符串被删空了，而另一个字符串能取到未被删除的元素
                        return false;
                }
                i--, j--;
            }
            return true;
        }
    };
}
// ---------------------
// 【4.2】判断子序列 (2)
// 删除字符 + 判断子序列的大字典查询场景，掌握模板题4即可
/*
392.判断子序列：给定字符串 s 和 t ，判断 s 是否为 t 的子序列。
字符串的一个子序列是原始字符串删除一些（也可以不删除）字符而不改变剩余字符相对位置形成的新字符串。
（例如，"ace"是"abcde"的一个子序列，而"aec"不是）。
进阶：
如果有大量输入的 S，称作 S1, S2, ... , Sk 其中 k >= 10亿，你需要依次检查它们是否为 T 的子序列。
在这种情况下，你会怎样改变代码？

524. 通过删除字母匹配到字典里最长单词：给你一个字符串 s 和一个字符串数组 dictionary ，
找出并返回 dictionary 中最长的字符串，该字符串可以通过删除 s 中的某些字符得到。
如果答案不止一个，返回长度最长且字母序最小的字符串。如果答案不存在，则返回空字符串。
*/
// ---------------------

// o1为进阶问题，当字典非常大时如何处理，与模板4 o2解法相同（可以略过了）
// o2为灵神的解法，预处理使用了动态规划，比较难理解，但是更好
namespace s392m1
{
    class Solution {
    public:
        bool isSubsequence(string s, string t) {
            int n = s.size(), m = t.size();
            if (n == 0) return true;
            if (n > m) return false;

            int i = 0, j = 0;
            int match = 0;
            while (i < n && j < m) {
                if (s[i] == t[j]) {
                    ++i;
                    if (++match == n) {
                        return true;
                    }
                }
                ++j;
            }

            return false;
        }
    };
}
namespace s392o1
{   
    class Solution {
    private:
        vector<vector<int>> build(const string& s) {
            vector<vector<int>> index(26);
            for (int i = 0; i < s.size(); ++i) {
                index[s[i] - 'a'].push_back(i);
            }
            return index;
        }
    public:
        bool isSubsequence(string s, string t) {
            vector<vector<int>> index = build(t);
            int prePos = -1;
            for (char c : s) {
                const vector<int>& vec = index[c - 'a'];
                auto it = upper_bound(vec.begin(), vec.end(), prePos);
                if (it == vec.end()) return false;
                prePos = *it;
            }
            return true;
        }
    };
}
namespace s392o2
{
    /*
    如果 t[i] == c，根据定义，nxt[i][c]=i。
    如果 t[i] != c，问题变成 t 中下标 ≥i+1 的最近字母 c 的下标，即 nxt[i][c]=nxt[i+1][c]。
    初始值 t[n][c]=n，动态规划，逆序遍历递推

    非常适合同一个 t 需要被查询很多次的场景（比如后续还有大量的 s 要判断）

    预处理：O(26 * n)
    查询：O(m)，m是字符串s的平均长度
    */
    class Solution {
    public:
        bool isSubsequence(string s, string t) {
            int n = t.size();
            vector<array<int, 26>> nxt(n + 1);
            fill(nxt[n].begin(), nxt[n].end(), n);
            for (int i = n - 1; i >= 0; i--) {
                nxt[i] = nxt[i + 1];
                nxt[i][t[i] - 'a'] = i;
            }

            // 这个写法无论 s 为空还是 t 为空，都能算出正确答案
            int i = -1;
            for (char c : s) {
                i = nxt[i + 1][c - 'a'];
                if (i == n) { // c 不在 t 中，说明 s 不是 t 的子序列
                    return false;
                }
            }
            return true; // s 是 t 的子序列
        }
    };
}

// 模板题4：删除字符 + 判断子序列（双指针匹配），o2预处理索引法适合超大字典场景（暂时跳过不看）
namespace s524m1
{   // 判断一个单词 word 是否能通过删除 s 中的字符得到
    // 本质上就是看 word 是否是 s 的一个子序列（不要求连续，但顺序必须一致），也就相当于s392的多个s查询
    
    // 最简单暴力的做法是逐个双指针比较，每次都要完整遍历整个s
    // 时间复杂度为O(n * m)，其中 n 是字符串 s 的长度，m 是字典中所有单词的总字符数，比较慢
    class Solution {
    private:
        bool isSubStr(const string& s, const string& word) {
            int m = s.size(), n = word.size();
            // if (n == 0) return true; // 题干支持word长度至少为1
            if (n > m) return false;

            int i = 0, j = 0;
            int match = 0;
            while (i < n && j < m) {
                if (word[i] == s[j]) {
                    ++i;
                    if (++match == n) {
                        return true;
                    }
                }
                ++j;
            }

            return false;
        }

    public:
        string findLongestWord(string s, vector<string>& dictionary) {
            int ansIndex = -1;// 只记录索引，避免拷贝
            int mxLen = 0;
            for (int i = 0; i < dictionary.size(); ++i) {
                const string& word = dictionary[i];// 用引用避免拷贝
                if (word.size() < mxLen) continue;// 小优化，短的字符串可以跳过判断

                // word.size() >= mxLen
                if (isSubStr(s, word)) {
                    if (word.size() > mxLen || word < dictionary[ansIndex]) {// 短路求值，判断字典序时两者长度一定相等
                        ansIndex = i;
                        mxLen = word.size();
                    }
                }
            }
            return ansIndex == -1 ? "" : dictionary[ansIndex];
        }
    };
}
namespace s524o1
{   // 预处理 + 双指针，对字典进行处理：按照字符串长度降序、字典序升序排序
    // 找到的第一个符合条件的单词就是最优解，可以提前终止搜索
    // 预处理排序：O(mlogm)，查找过程仍然最坏是O(n*m)，但当较长单词优先检查时，可以更早找到解
    // 一般来说m1就够了，o1得看具体场景，不见得有m1快
    class Solution {
    private:
        bool isSubStr(const string& s, const string& word) {
            int m = s.size(), n = word.size();
            // if (n == 0) return true; // 题干支持word长度至少为1
            if (n > m) return false;

            int i = 0, j = 0;
            int match = 0;
            while (i < n && j < m) {
                if (word[i] == s[j]) {
                    ++i;
                    if (++match == n) {
                        return true;
                    }
                }
                ++j;
            }

            return false;
        }

    public:
        string findLongestWord(string s, vector<string>& dictionary) {
            sort(dictionary.begin(), dictionary.end(),
                [](const string& a, const string& b) {
                    return a.size() > b.size() || (a.size() == b.size() && a < b);
                });

            for (const string& word : dictionary) {
                if (isSubStr(s, word)) {
                    return word;
                }
            }
            return "";
        }
    };
}
namespace s524o2
{   // 对于非常大的 s 字符串，可以预先为 s 创建字符出现位置的索引，加速子序列匹配
    // 这种方法在 s 很长 或者 字典中单词很多 时更加高效
    class Solution {
    private:
        vector<vector<int>> buildIndex(const string& s) {
            vector<vector<int>> index(26);
            for (int i = 0; i < s.size(); ++i) {
                index[s[i] - 'a'].push_back(i);
            }
            return index;
        }

        bool isSubsequence(const vector<vector<int>>& index, const string& word) {
            int prev_pos = -1;
            for (char c : word) {
                const vector<int>& vec = index[c - 'a'];
                // 找第一个 > prev_pos 的位置
                auto it = upper_bound(vec.begin(), vec.end(), prev_pos);
                if (it == vec.end()) return false;
                prev_pos = *it;
            }
            return true;
        }

    public:
        string findLongestWord(string s, vector<string>& dictionary) {
            vector<vector<int>> index = buildIndex(s);

            int ansIndex = -1, mxLen = 0;// 只记录下标，避免多次字符串拷贝
            for (int i = 0; i < dictionary.size(); ++i) {
                const string& word = dictionary[i];
                if (word.size() < mxLen) continue;
                if (isSubsequence(index, word)) {
                    if (word.size() > mxLen || word < dictionary[ansIndex]) {
                        mxLen = word.size();
                        ansIndex = i;
                    }
                }
            }
            return ansIndex == -1 ? "" : dictionary[ansIndex];
        }
    };
}