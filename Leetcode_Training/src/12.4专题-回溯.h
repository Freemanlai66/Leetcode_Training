#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
using namespace std;

// 问题待定：
/*
1.回溯类型题目的时间复杂度，空间复杂度分析，目前还未完全掌握
*/

/*
模板题：
1.static constexpr静态变量的定义 + 回溯入门级题目，理解回溯思想:17
2.子集型回溯模板， m1为选或不选（输入的视角），o1为枚举选哪个（答案的视角）:78
3.将字符串中子串的分割处（逗号）当作子集中元素，本质是子集型回溯:131
4.总体还是和子集型回溯比较类似，但是多出了剪枝的步骤 : 77
5.这道题需要一点思维转换，可以多看看, 将括号匹配转换成了选与不选 : 22
6.用布尔数组标记元素是否在集合中，用“枚举选哪个”的模板 : 46
7.N皇后，本质是枚举列号的全排列 : 51
8.与s78子集型回溯有所区别，重点在于只有不选相同的数才会导致结果重复，或是在枚举选哪个时不枚举相同元素（跳过）:90
9.排列型回溯的去重，与子集型的枚举选哪个去重比较类似:47
10.网格图 + 回溯:79
*/

// 回溯：入门回溯 + 子集型回溯 + 组合型回溯 + 排列型回溯 + 重复元素回溯 + 搜索 + 折半枚举

// 【4.1】入门回溯 (1)
// 回溯其实是二叉树、N叉树的递归衍生题，每个步骤选什么等效于对应节点有几个孩子
// 核心思维是将问题抽象成一颗树，思考如何拆解问题或是遍历这颗树
/*
17.电话号码的字母组合：给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。答案可以按 任意顺序 返回。
给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。
*/
// ---------------------
// 模板题1：static constexpr静态变量的定义 + 回溯入门级题目，理解回溯思想
namespace s17o1
{
    class Solution {
    private:
        static const string mapping[10];
        // static const变量，如果比较复杂（非整形）不能在类内直接定义，需要在类外（通常是在.cpp文件中，避免多次定义）
        // static constexpr可以放宽限制（浮点数等，但字符串等更复杂的类型依旧不支持）since C++14
        // static inline constexpr则没有类型限制 since C++17

        // 总时间复杂度是O(4^n * n)
    public:
        vector<string> letterCombinations(string digits) {
            int n = digits.length();
            if (n == 0) {
                return {};
            }

            vector<string> ans;
            // 初始化一个长度为n的空字符串，每个位置都是'\0'
            // string path(n, 0)会隐式的转换成path(n, '\0')，这里0对应ASCII码0的'\0'
            // 注意string path(n)是不合法的，string没有这种构造函数形式
            string path(n, 0);

            auto dfs = [&](auto&& self, int i)->void {
                if (i == n) {
                    ans.push_back(path);// 复制字符串需要O(n)的时间
                    return;
                }
                for (char c : mapping[digits[i] - '0']) {
                    // 直接覆盖，不需要恢复现场，因为path长度是固定的
                    // （在之后的子集型回溯里因为往答案里添加的东西长度不定，所以要定期恢复现场）
                    path[i] = c;
                    self(self, i + 1);
                }
                };

            dfs(dfs, 0);
            return ans;
        }
    };
    const string Solution::mapping[10] = {
            "", "", "abc", "def", "ghi", "jkl",
            "mno", "pqrs", "tuv", "wxyz"
    };
}
// ---------------------
// 【4.2】子集型回溯 (2)
// 有「选或不选」(每个元素可以选或不选)和「枚举选哪个」两种写法，各自有适合（更容易理解）的场景
/*
78.子集：给你一个整数数组 nums ，数组中的元素 互不相同 。返回该数组所有可能的子集（幂集）。
解集 不能 包含重复的子集。你可以按 任意顺序 返回解集。

39.组合总和：给你一个 无重复元素 的整数数组 candidates 和一个目标整数 target ，
找出 candidates 中可以使数字和为目标数 target 的 所有 不同组合 ，并以列表形式返回。你可以按 任意顺序 返回这些组合。
candidates 中的 同一个 数字可以 无限制重复被选取 。如果至少一个数字的被选数量不同，则两种组合是不同的。 
对于给定的输入，保证和为 target 的不同组合数少于 150 个。
*/
// ---------------------
// 模板题2：子集型回溯模板， m1为选或不选（输入的视角），o1为枚举选哪个（答案的视角）
// 选或不选s78已经很清晰了，如果还是看不懂，可以结合模板题3 s131o2 的解加深理解
namespace s78m1
{   // 选或不选，从输入的视角来看，每次可能选到空的元素
    class Solution {
    public:
        vector<vector<int>> subsets(vector<int>& nums) {
            int n = nums.size();
            if (n == 0) return {};

            vector<vector<int>> ans;
            vector<int> element;

            auto dfs = [&](auto&& self, int i) -> void {
                if (i == n) {
                    ans.push_back(element);
                    return;
                }
                self(self, i + 1);// 不选
                element.push_back(nums[i]);
                self(self, i + 1);// 选，先加到路径再递归
                element.pop_back();
                };

            dfs(dfs, 0);
            return ans;
        }
    };
}
namespace s78o1
{   // 枚举选哪个（答案的视角）,我个人觉得m1的思路比较好理解，因为就是二叉树递归直接变化而来
    // 枚举选哪个的做法就是不规则的树，比如数组长度为4
    // 在先把空集加入答案后(也就是根节点)，深度1有4个选择（因为不能选重复的数），深度2有3个选择，以此类推
    class Solution {
    public:
        vector<vector<int>> subsets(vector<int>& nums) {
            int n = nums.size();
            if (n == 0) return {};

            vector<vector<int>> ans;
            vector<int> element;

            auto dfs = [&](auto&& self, int i) -> void {
                // 本题答案的长度可以不固定（只选所有元素的一个都行），所以每次都要更新
                // 但也有很多情况下需要i == n时才可以向ans中更新答案
                ans.push_back(element);

                // 每次其实都有n个选择，但是因为不能重复选取，所以下一个数应该大于当前选择的数（也即形如下方的遍历）
                for (int j = i; j < n; ++j) {
                    element.push_back(nums[j]);
                    self(self, j + 1);// 注意这里是j + 1而不是i + 1，要时刻留意下一步递归的变量到底是什么
                    element.pop_back();
                }
                };

            dfs(dfs, 0);
            return ans;
        }
    };
}

// s78稍微变化了一下，元素变得可以重复选择
namespace s39m1
{   // 选或不选
    class Solution {
    public:
        vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
            vector<vector<int>> ans;
            vector<int> path;
            int n = candidates.size();

            auto dfs = [&](auto&& self, int sum, int i) {
                if (sum == target) {
                    ans.push_back(path);
                    return;
                }
                if (sum > target || i == n) {
                    return;
                }
                // 不选
                self(self, sum, i + 1);
                // 选
                path.push_back(candidates[i]);
                sum += candidates[i];
                self(self, sum, i);// 重点：这是i而不是i + 1，因为当前元素可以重复选，所以选择后还可以继续决定选/不选
                path.pop_back();
                };
            dfs(dfs, 0, 0);
            return ans;
        }
    };
}
namespace s39o1
{   // 选或不选 + 剪枝优化
    class Solution {
    public:
        vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
            int n = candidates.size();
            vector<int> path;
            vector<vector<int>> ans;

            // 只增加了一个排序都会更快
            sort(candidates.begin(), candidates.end());

            auto dfs = [&](auto&& self, int sum, int i) {
                if (sum == 0) {
                    ans.push_back(path);
                    return;
                }
                // 新的边界条件更严格，排序后，后面的candidates[i]会更大，如果当前就超了，后面也没必要递归
                // 注意，这里i == n写前面，要不然后面的candidates[i]可能会访问越界
                if (i == n || sum < candidates[i]) {
                    return;
                }

                self(self, sum, i + 1);

                path.push_back(candidates[i]);
                self(self, sum - candidates[i], i);
                path.pop_back();
                };

            dfs(dfs, 0, target);
            return ans;
        }
    };
}
namespace s39o2
{   // 枚举选哪个 + 剪枝优化
    class Solution {
    public:
        vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
            vector<vector<int>> ans;
            vector<int> path;
            sort(candidates.begin(), candidates.end());

            auto dfs = [&](auto&& self, int sum, int start) {
                if (sum == target) {
                    ans.push_back(path);
                    return;
                }

                // 剪枝优化（利用有序性）
                for (int i = start; i < candidates.size(); ++i) {
                    if (sum + candidates[i] > target) {  // 提前终止
                        break;
                    }
                    // 注：这里不能直接用sum += candidates[i]，因为从i到n - 1的所有遍历下sum是共用的

                    path.push_back(candidates[i]);
                    self(self, sum + candidates[i], i);  // 允许重复选
                    path.pop_back();
                }
                };

            dfs(dfs, 0, 0);
            return ans;
        }
    };
}
// ---------------------
// 【4.3】划分型回溯 (2)
// 把分割线（逗号）看成是可以「选或不选」的东西，本质是子集型回溯
/*
131.分割回文串：给你一个字符串 s，请你将 s 分割成一些 子串，使每个子串都是 回文串 。返回 s 所有可能的分割方案。

93.复原IP地址：有效 IP 地址 正好由四个整数（每个整数位于 0 到 255 之间组成，且不能含有前导 0），整数之间用 '.' 分隔。
例如："0.1.2.201" 和 "192.168.1.1" 是 有效 IP 地址，
但是 "0.011.255.245"、"192.168.1.312" 和 "192.168@1.1" 是 无效 IP 地址。
给定一个只包含数字的字符串 s ，用以表示一个 IP 地址，
返回所有可能的有效 IP 地址，这些地址可以通过在 s 中插入 '.' 来形成。
你 不能 重新排序或删除 s 中的任何数字。你可以按 任何 顺序返回答案。
*/
// ---------------------
// 模板题3：将字符串中子串的分割处（逗号）当作子集中元素，本质是子集型回溯
namespace s131o1
{   // 选或不选，"a,a,b,"，对i = 0, i = 1, i = 2三处的逗号，判断选或不选
    class Solution {
    private:
        // 双指针判断是否为回文串
        bool isPalindrome(const string& s, int left, int right) {
            while (left < right) {
                if (s[left++] != s[right--]) {
                    return false;
                }
            }
            return true;
        }

    public:
        vector<vector<string>> partition(string s) {
            int n = s.size();
            vector<vector<string>> ans;
            vector<string> path; // 题目要求返回的是分割的“方案”，每个方案即vector<string>

            // start表示这段回文子串的开始位置
            auto dfs = [&](auto&& self, int start, int end) -> void {
                if (end == n) {
                    ans.push_back(path);
                    return;
                }

                // i这个位置不进行分割
                if (end < n - 1) {
                    // 当i = n - 1，也即指向字符串最后一个字符右边时，是必须进行分割的
                    // 在i < n - 1时，可以一直随意的跳过分割点，不进行分割
                    self(self, start, end + 1);
                }

                // 在i处分割，如果[start, i]的部分不是回文串，那也就没有后续了
                if (isPalindrome(s, start, end)) {
                    path.emplace_back(s.begin() + start, s.begin() + end + 1);
                    self(self, end + 1, end + 1);
                    path.pop_back();// 恢复现场
                }
                };

            dfs(dfs, 0, 0);
            return ans;
        }
    };
}
namespace s131o2
{   // 枚举选哪个
    // 这里逗号的"位置"就不是固定的了，而是逗号的"数量"是固定的，与o1选与不选不同
    // 对例子"aab"，假设有3个逗号，后面的逗号必须在之前逗号的右边
    // 第一个逗号可以在：第一个a后面/第二个a后面/b后面
    // 展开就是：
    /*
            a,ab            aa,b           aab,
           / \                |           
      a,a,b  a,ab,          aa,b,
        |
      a,a,b,
    */
    class Solution {
    private:
        bool isPalindrome(const string& s, int left, int right) {
            while (left < right) {
                if (s[left++] != s[right--]) {
                    return false;
                }
            }
            return true;
        }

    public:
        vector<vector<string>> partition(string s) {
            vector<vector<string>> ans;
            vector<string> path;
            int n = s.size();

            auto dfs = [&](auto&& self, int i) -> void {
                if (i == n) {
                    ans.push_back(path);
                    return;
                }
                // 因为
                for (int j = i; j < n; ++j) {
                    // 这个for loop中的每个j都对应一个子树，当j = n - 1的时候path = s
                    // 只有当前[i, j]的这部分子串是回文的才会继续向下递归，所以当j == n时一定整体都是回文的
                    if (isPalindrome(s, i, j)) {// 闭区间
                        path.emplace_back(s.begin() + i, s.begin() + j + 1);
                        self(self, j + 1);
                        // 这里恢复现场也就相当于在每个j对应的大子树里面返回靠近叶子的根节点
                        // e.g. a,a,b -> a, ab -> a,ab,
                        path.pop_back();
                    }
                }
                };

            dfs(dfs, 0);
            return ans;
        }
    };
}

// 算是第一次自己做出来回溯题，与s131本质上是一道题
namespace s93m1
{   // 枚举选哪个
    class Solution {
    private:
        bool isIPAddress(const string& s, int left, int right) {
            // 不能有前导0
            if (s[left] == '0' && right > left) {
                return false;
            }
            // 超过4位数提前返回
            if (right - left + 1 > 3) {
                return false;
            }
            int num = 0;
            for (int i = left; i <= right; ++i) {
                num = num * 10 + (s[i] - '0');
            }
            // 1至3位数字，但需要在0-255范围内（num >= 0无需判断）
            return num <= 255;
            // return stoi(s.substr(left, right - left + 1)) <= 255; // 也可以用库函数
        }

    public:
        vector<string> restoreIpAddresses(string s) {
            int n = s.size();
            // 小剪枝
            if (n < 4 || n > 12) return {};
            vector<string> ans;
            vector<string> path;

            auto dfs = [&](auto&& self, int start) {
                // path必须刚好有4个子段
                if (start == n && path.size() == 4) {
                    // 找到符合要求的IP地址，开始拼接+添加"."并更新答案
                    string temp = "";
                    for (auto& ele : path) {
                        temp += ele;
                        temp += ".";
                    }
                    // string temp = path[0] + "." + path[1] + "." + path[2] + "." + path[3]; 直接展开也可以
                    temp.pop_back(); // 把末尾多出来的"."去除
                    ans.push_back(temp);
                    return;
                }
                // 当i = 4但path.size() != 4时，下面for循环初始的j < n判断就过不去，相当于return了

                for (int end = start; end < n; ++end) {
                    // 当前子段数字必须满足要求 && 不能超过4个子段
                    if (isIPAddress(s, start, end) && path.size() < 4) {
                        path.emplace_back(s.begin() + start, s.begin() + end + 1);
                        self(self, end + 1);
                        path.pop_back();
                    }
                }
                };

            dfs(dfs, 0);
            return ans;
        }
    };
}
namespace s93o1
{   // 选或不选
    class Solution {
    private:
        bool isIPAddress(const string& s, int left, int right) {
            // 不能有前导0 || 超过4位数提前返回
            int len = right - left + 1;
            if ((s[left] == '0' && len > 1) || len > 3) {
                return false;
            }
            // 1至3位数字，但需要在0-255范围内
            return stoi(s.substr(left, len)) <= 255;
        }

    public:
        vector<string> restoreIpAddresses(string s) {
            int n = s.size();
            // 小剪枝
            if (n < 4 || n > 12) return {};
            vector<string> ans;
            vector<string> path;

            auto dfs = [&](auto&& self, int start, int end)->void {
                // path必须刚好有4个子段
                if (end == n) {
                    if (path.size() == 4) {
                        // 找到符合要求的IP地址，开始拼接+添加"."并更新答案
                        string temp = path[0] + "." + path[1] + "." + path[2] + "." + path[3];
                        ans.push_back(move(temp));
                    }
                    // end == n的时候必须手动返回，否则会在下面isIPAddress函数中访问end == n，导致字符串访问越界
                    return;
                }

                // m2中AI提供的剪枝可以加进来，纯良性改动
                
                // 不选：但如果当前[start, end]范围内都不符合0-255要求，那也没必要继续递归了
                // 选：当前子段数字必须满足要求 && 不能超过4个子段
                // 这一段的写法最终优化版见m2
                if (isIPAddress(s, start, end) && path.size() < 4) {
                    self(self, start, end + 1);

                    path.emplace_back(s.begin() + start, s.begin() + end + 1);
                    self(self, end + 1, end + 1);
                    path.pop_back();
                }
                };

            dfs(dfs, 0, 0);
            return ans;
        }
    };
}
namespace s93m2
{   // 模仿s131的选或不选重新写的一版
    class Solution {
    private:
        // isValid还是o1的版本写的简洁
        bool isValid(const string& s, int left, int right) {
            int len = right - left + 1;
            if (len > 3) return false;
            if (len > 1 && s[left] == '0') return false;

            int num = 0;
            for (int i = left; i <= right; ++i) {
                num = num * 10 + (s[i] - '0');
            }
            return num <= 255;
        }

    public:
        vector<string> restoreIpAddresses(string s) {
            int n = s.size();
            // 小剪枝
            if (n < 4 || n > 12) return {};
            vector<string> ans;
            vector<string> path;

            auto dfs = [&](auto&& dfs, int start, int end)->void {
                // AI给出的剪枝：剩余字符太多或太少
                int remainingChars = n - start;
                int remainingSegments = 4 - path.size();

                // 剩余字符数不在合理范围，提前返回
                if (remainingChars < remainingSegments
                    || remainingChars > remainingSegments * 3) {
                    return;
                }

                if (path.size() == 4) {// 只要path.size() == 4了，无论是否选到末尾，都应该立刻返回
                    if (end == n) {
                        string ip = path[0] + '.' + path[1]
                            + '.' + path[2] + '.' + path[3];
                        ans.push_back(move(ip));
                    }
                    return;
                }

                // 选或不选小段最好的写法
                if (isValid(s, start, end)) {
                    // 不选
                    int len = end - start + 1;
                    if (len < 3 && end < n - 1) {
                        dfs(dfs, start, end + 1);
                    }
                    // 选
                    if (path.size() < 4) {
                        path.emplace_back(s.begin() + start, s.begin() + end + 1);
                        dfs(dfs, end + 1, end + 1);
                        path.pop_back();
                    }
                }
                };

            dfs(dfs, 0, 0);
            return ans;
        }
    };
}
namespace s93m3 {
    // 整合版：o1 + m2
    class Solution {
    private:
        // 闭区间
        bool isValid(string& s, int start, int end) {
            int len = end - start + 1;
            if (len > 3 || (len > 1 && s[start] == '0')) {
                return false;
            }
            return stoi(s.substr(start, len)) <= 255;
        }

    public:
        vector<string> restoreIpAddresses(string s) {
            int n = s.size();
            if (n < 4 || n > 12) return {};

            vector<string> path;
            vector<string> ans;

            auto dfs = [&](auto&& dfs, int start, int end)->void {
                int remainingChars = n - start;
                int remainingSegs = 4 - path.size();
                if (remainingChars < remainingSegs || remainingChars > remainingSegs * 3) {
                    return;
                }

                if (end == n) {
                    if (path.size() == 4) {
                        string ip = path[0] + '.' + path[1] + '.' + path[2] + '.' + path[3];
                        ans.push_back(move(ip));
                    }
                    return;
                }

                if (isValid(s, start, end)) {
                    // 不选
                    int len = end - start + 1;
                    if (len < 3 && end < n - 1) {
                        dfs(dfs, start, end + 1);
                    }

                    // 选
                    if (path.size() < 4) {
                        path.emplace_back(s.begin() + start, s.begin() + end + 1);
                        dfs(dfs, end + 1, end + 1);
                        path.pop_back();
                    }
                }
                };

            dfs(dfs, 0, 0);
            return ans;
        }
    };
}
// ---------------------
// 【4.4】组合型回溯 (4)
// 组合型回溯相比子集型回溯，不同之处在于“组合”的长度是固定的，比如从n个数里选固定长为k的组合
// 其他过程与子集型是类似的，同时由于长度固定，可以在递归路径中进行剪枝优化
/*
77.组合：给定两个整数 n 和 k，返回范围 [1, n] 中所有可能的 k 个数的组合。你可以按 任何顺序 返回答案。

216.组合总和 III：找出所有相加之和为 n 的 k 个数的组合，且满足下列条件：
只使用数字1到9；每个数字 最多使用一次 。
返回 所有可能的有效组合的列表 。该列表不能包含相同的组合两次，组合可以以任何顺序返回。

22.括号生成：数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合。

301.删除无效的括号：给你一个由若干括号和字母组成的字符串 s ，删除最小数量的无效括号，使得输入的字符串有效。
返回所有可能的结果。答案可以按 任意顺序 返回。
*/
// ---------------------
// 模板题4：总体还是和子集型回溯比较类似，但是多出了剪枝的步骤
namespace s77o1
{   // 枚举选哪个
    // 剪枝优化1：当已经选了k个时，可以结束递归（感觉这个勉强算是剪枝吧，本来就要结束的）
    // 剪枝优化2：当剩余的选择不足以支撑选出k个元素时，就可以结束递归
    //            设path长度为m，则每次递归还需选k - m个数，如果剩下的元素个数小于 k - m，则可以结束递推
    class Solution {
    public:
        vector<vector<int>> combine(int n, int k) {
            vector<vector<int>> ans;
            vector<int> path;

            auto dfs = [&](auto&& self, int i) {
                if (path.size() == k) {
                    ans.push_back(path);
                    return;
                }
                // 逆序遍历，让这个剪枝优化的判断条件更简洁
                // 如果正序，那此处判断条件为：n - i + 1 < k - path.size()
                if (i < k - path.size()) {
                    return;
                }
                for (int j = i; j >= 1; --j) {
                    path.push_back(j);
                    self(self, j - 1);
                    path.pop_back();
                }

                };
            
            dfs(dfs, n);
            return ans;
        }
    };

}
namespace s77o2
{   // 选或不选
    class Solution {
    public:
        vector<vector<int>> combine(int n, int k) {
            vector<vector<int>> ans;
            vector<int> path;

            auto dfs = [&](auto&& self, int i) {
                if (path.size() == k) {
                    ans.push_back(path);
                    return;
                }
                // 这里的剪枝可以优化，见下方：
                if (i < k - path.size()) {
                    return;
                }
                self(self, i - 1); // 不选
                /*
                * 只有剩下i > k - path.size()时，才有资本在这里不选继续递归，不能是>=，因为当前这个是不选的
                if (i > k - path.size() {
                    self(self, i - 1);
                }
                // 下方的“选”并不会受影响，因为连“不选”都有资本浪费挥霍，那“选”一定是满足的（path的size更大）
                */
                path.push_back(i);
                self(self, i - 1);
                path.pop_back();
                };
            dfs(dfs, n);
            return ans;
        }
    };

}
namespace s77m1
{   // 将path改为固定长度，无需pop_back()
    class Solution {
    public:
        vector<vector<int>> combine(int n, int k) {
            vector<int> path(k);
            vector<vector<int>> ans;

            auto dfs = [&](auto&& dfs, int i, int j)->void {
                if (j == k) {
                    ans.push_back(path);
                    return;
                }

                int remainingNum = n - i + 1;
                int remainingSpace = k - j;
                if (remainingSpace > remainingNum) {
                    return;
                }

                // 不选
                dfs(dfs, i + 1, j);

                // 选
                path[j] = i;
                dfs(dfs, i + 1, j + 1);
                };

            dfs(dfs, 1, 0);
            return ans;
        }
    };
}

// 跟s77大体类似，多了一点剪枝和判断
namespace s216m1
{   // 选或不选
    class Solution {
    public:
        vector<vector<int>> combinationSum3(int k, int n) {
            vector<vector<int>> ans;
            vector<int> path;
            int sum = 0;

            auto dfs = [&](auto&& self, int num) {
                if (path.size() == k) {
                    if (sum == n) {
                        ans.push_back(path);
                    }
                    return;
                }
                int rest = k - path.size();
                // 如果1-9已经全部用完 || 剩下的数不够了 || 剩下全选1都超出 || 剩下全选9都不够
                // 后两种情况的剪枝只是粗略的，因为每个数字只能选一个，不能都选1或9
                // 更精确的剪枝可以用逆序遍历 + 等差数列求和
                if (num > 9 || num < rest || rest + sum > n || rest * 9 + sum < n) {
                    return;
                }
                self(self, num + 1);
                sum += num;
                path.push_back(num);
                self(self, num + 1);
                sum -= num;
                path.pop_back();
                };
            dfs(dfs, 1);
            return ans;
        }
    };
}
namespace s216m2
{   // 枚举选哪个
    class Solution {
    public:
        vector<vector<int>> combinationSum3(int k, int n) {
            vector<vector<int>> ans;
            vector<int> path;

            auto dfs = [&](auto&& self, int num, int sum) {
                if (path.size() == k) {
                    if (sum == n) {
                        ans.push_back(path);
                    }
                    return;
                }
                int rest = k - path.size();
                // 如果1-9已经全部用完（这个剪枝for循环能做到） || 剩下的数不够了 || 剩下全选1都超出 || 剩下全选9都不够
                // 后两种情况的剪枝只是粗略的，因为每个数字只能选一个，不能都选1或9
                // 更精确的剪枝可以用逆序遍历 + 等差数列求和
                if (num < rest || rest + sum > n || rest * 9 + sum < n) {
                    return;
                }
                for (int i = num; i <= 9; ++i) {
                    path.push_back(i);
                    self(self, i + 1, sum + i);
                    path.pop_back();
                }
                };
            dfs(dfs, 1, 0);
            return ans;
        }
    };
}
namespace s216m3
{   // 枚举选哪个的简单写法，没管那么多剪枝
    class Solution {
    public:
        vector<vector<int>> combinationSum3(int k, int n) {
            vector<vector<int>> ans;
            vector<int> path;

            auto dfs = [&](auto&& dfs, int i, int sum)->void {
                if (path.size() == k && sum == n) {
                    ans.push_back(path);
                    return;
                }

                for (int j = i; j <= 9; ++j) {
                    if (path.size() < k && sum + j <= n) {
                        path.push_back(j);
                        dfs(dfs, j + 1, sum + j);
                        path.pop_back();
                    }
                }
                };
            dfs(dfs, 1, 0);
            return ans;
        }
    };
}
namespace s216m4
{   // 将path改为固定长度，无需pop_back() + 规范命名
    class Solution {
    public:
        vector<vector<int>> combinationSum3(int k, int n) {
            vector<int> path(k);
            vector<vector<int>> ans;

            auto dfs = [&](auto&& dfs, int i, int j, int sum)->void {
                if (j == k) {
                    if (sum == n) {
                        ans.push_back(path);
                    }
                    return;
                }

                int remainingNum = 9 - i + 1;
                int remainingSpace = k - j;
                if (remainingNum < remainingSpace || sum + i > n
                    || sum + remainingSpace > n || sum + 9 * remainingSpace < n) {
                    return;
                }

                dfs(dfs, i + 1, j, sum);
                path[j] = i;
                dfs(dfs, i + 1, j + 1, sum + i);
                };

            dfs(dfs, 1, 0, 0);
            return ans;
        }
    };
}

// 模板题5：这道题需要一点思维转换，可以多看看
namespace s22o1
{   // 选或不选（这道题枚举选哪个稍微麻烦点就不做了）
    // 相当于在2n个位置里选n个位置放“左括号”，不选就放“右括号”
    // 同时遵从以下原则：
    // 1.左括号数目一旦达到n个，则剩下全部放右括号
    // 2.右括号数目不能超过左括号数目，也即一旦两者数目相等，则只能放左左括号
    class Solution {
    public:
        vector<string> generateParenthesis(int n) {
            vector<string> ans;
            string path;
            // 如果path(n, 0)定义为定长的字符串，那就不需要pop_back，但递归参数要变成i（表示2n个位置里当前位置）

            auto dfs = [&](auto&& self, int left, int right) {
                if (left == n && right == n) {// 这里可以改成right == n，因为递归中right一定小于等于left
                    ans.push_back(path);
                    return;
                }
                // 不选：放右括号，但right数量不能超过left
                if (right < left) {
                    path += ")";
                    self(self, left, right + 1);
                    path.pop_back();
                }
                // 选：放左括号，但左括号数目一旦达到n个，则剩下全部放右括号
                if (left < n) {
                    path += "(";
                    self(self, left + 1, right);
                    path.pop_back();
                }
                };

            dfs(dfs, 0, 0);
            return ans;
        }
    };
}
namespace s22o2
{   // o1的改版，选或不选，优化版
    class Solution {
    public:
        vector<string> generateParenthesis(int n) {
            vector<string> ans;
            string path(2 * n, 0);// 0代表'/n'，直接预分配固定大小内存，避免o1的自动扩容

            auto dfs = [&](auto&& self, int left, int right) {
                if (right == n) {   // 少一个判断条件，其实这里只需要判断right即可，o1同样可以直接改成这样
                    ans.push_back(path);
                    return;
                }
                if (right < left) {
                    path[left + right] = ')';// 使用下标，直接省去pop_back，微型提速
                    self(self, left, right + 1);
                }
                if (left < n) {
                    path[left + right] = '(';
                    self(self, left + 1, right);
                }
                };

            dfs(dfs, 0, 0);
            return ans;
        }
    };
}

// 回溯里的第一道困难题，o1常规做法虽然效率不算最优慢，但容易理解，o2做法有额外时间再研究
namespace s301o1
{   // 选或不选
    // 至多20个括号，且字符串长度最多只有25，除去括号外，字符串中只有小写字母
    class Solution {
    public:
        vector<string> removeInvalidParentheses(string s) {
            int lremove = 0, rremove = 0;
            // Calculate minimum removals 这里算出的一定是需要移除的括号的最小值，lremove只有在配对后才能减少
            for (char c : s) {
                if (c == '(') {
                    lremove++;
                }
                else if (c == ')') {
                    if (lremove > 0) lremove--;
                    else rremove++;
                }
            }
            
            // 用set是因为部分删除方法得到的字符串是相同的，比如())，删除第一个右括号和第二个右括号得到的字符串相同
            unordered_set<string> ansSet;
            string path;
            // Optimized: use reference to avoid copying
            // open表示尚未被匹配的左括号数量（即已出现但尚未被右括号闭合的左括号数量）
            auto dfs = [&](auto&& self, int index, int lremove, int rremove, int open) -> void {
                // Prune: insufficient remaining characters to delete
                if (s.size() - index < lremove + rremove) return;
                // Prune: insufficient remaining characters to balance 'open'
                if (s.size() - index < open) return;

                // Termination: reached end of string
                if (index == s.size()) {
                    if (lremove == 0 && rremove == 0) {
                        // 选则保留时的open > 0确保了括号正确匹配，而lremove和rremove都为0说明没有多余括号，可以放心加入答案
                        ansSet.insert(path);
                    }
                    return;
                }

                char c = s[index];
                // Option 1: Delete current character (if applicable)（也即不选当前符号，lremove rremove相应减小）
                if (c == '(' && lremove > 0) {
                    self(self, index + 1, lremove - 1, rremove, open);
                }
                else if (c == ')' && rremove > 0) {
                    self(self, index + 1, lremove, rremove - 1, open);
                }

                // Option 2: Keep current character（也即选当前符号，open相应进行改变）
                path.push_back(c);
                if (c == '(') {
                    self(self, index + 1, lremove, rremove, open + 1);
                }
                else if (c == ')') {
                    // Only keep if valid
                    if (open > 0) {
                        self(self, index + 1, lremove, rremove, open - 1);
                    }
                }
                else {
                    // Non-parenthesis
                    self(self, index + 1, lremove, rremove, open);
                }
                path.pop_back(); // Explicit backtracking
                };
            dfs(dfs, 0, lremove, rremove, 0);

            return vector<string>(ansSet.begin(), ansSet.end());
        }
    };
}
namespace s301o2
{   // 正向反向两遍扫描法，不需要提前统计多余的左右括号数量，并且剪枝更厉害
    // 也不需要用哈希表进行去重，更优秀的做法，但是这个做法非常规，仅做了解
    // 技巧：通过用open和close分别代表'(',')'，此时只要调换open和close，就能实现从右扫描的代码复用

    // 我在原始题解的评论区中记录下了当时的思考
    // https://leetcode.cn/problems/remove-invalid-parentheses/solutions/3754927/liang-bian-sao-miao-fa-bu-xu-yao-ti-qian-c3kl/
    class Solution {
    public:
        vector<string> removeInvalidParentheses(string s) {
            vector<string> res;
            solve(s, res, 0, 0, '(', ')');
            return res;
        }

    private:
        void solve(const string& s, vector<string>& res, int last_i, int last_j, char open, char close) {
            int balance = 0;

            // last_i 是扫描起点，i是当前扫描指针
            for (int i = last_i; i < s.size(); ++i) {
                if (s[i] == open) balance++;
                else if (s[i] == close) balance--;

                // 如果balance < 0，说明在[last_i, i]区间内出现了不匹配的右括号
                if (balance < 0) {
                    // 我们需要在[last_j, i]区间内寻找一个右括号来删除
                    // last_j 是删除操作的起点（第一次删除last_j就是0，后面更新为j）
                    for (int j = last_j; j <= i; ++j) {
                        // 1. 必须是需要删除的括号类型
                        // 2. 去重：如果是连续的多个右括号，只删除第一个
                        /*
                        首先一旦balance<0就要进入操作分支，并之后一定会return退出solve函数。
                        所以进入balance<0时balance一定为-1，也即只多出一个右括号，此时有两类可能:
                        1....())，删除前后右括号结果一致，需要去重
                        2....()a)，删除前后右括号结果不一致，不需要去重
                        也即，只有两个右括号连在一起的才会造成重复，所以只需要检查相连的字符是否同为右括号就能实现去重。
                        */
                        if ((s[j] == close) && ((j == last_j) || (s[j - 1] != close))) {
                            // 构建新字符串并递归(0开始，共j个，j为从0开始的下标，所以已经删除了一个)
                            // substr(j + 1)则代码下标j + 1处及后面的字符，拼接在一起
                            string ss = s.substr(0, j) + s.substr(j + 1);
                            // 每次深入一层递归solve都代表删除掉了一个多余的字符串
                            solve(ss, res, i, j, open, close);
                        }
                    }
                    // 只要找到了一个不合法的 balance < 0，当前函数的使命就结束了
                    // 因为所有的可能性都已经通过新的递归分支去探索了
                    return; // 重要
                }
            }

            // 如果循环结束，balance >= 0，说明从左到右方向是合法的，反转字符串准备从右往左
            string reversed = s;
            reverse(reversed.begin(), reversed.end());

            if (open == '(') {
                // 如果是第一次（正向）扫描，现在反转字符串，处理左括号
                solve(reversed, res, 0, 0, close, open);
            }
            else {
                // 如果是第二次（反向）扫描完成，说明 reversed 现在是完全合法的
                // 将其反转回原始顺序，加入结果集
                res.push_back(reversed);
            }
        }
    };
}
namespace s3o1o3
{   // 优化命名提升可读性后的o2解法
    class Solution {
    private:
        // 注：这里s加上const是个小细节
        void dfsRemove(const string& s, vector<string>& ans,
            int scanStart, int deleteStart,
            char openBracket, char closeBracket) {
            int n = s.size();
            int balance = 0;

            for (int i = scanStart; i < n; ++i) {
                // 注意有除括号外的字母字符
                if (s[i] == openBracket) {
                    ++balance;
                }
                else if (s[i] == closeBracket) {
                    --balance;
                }

                if (balance < 0) {
                    for (int j = deleteStart; j <= i; ++j) {
                        if (s[j] == closeBracket &&
                            (j == deleteStart || s[j - 1] != closeBracket)) {
                            string newStr = s.substr(0, j) + s.substr(j + 1);
                            // 因为newStr的长度比s小一，删除了一个元素，所以i和j并不会加一，而是维持不变
                            // 仍然是在闭区间上处理，区间不会有重叠
                            dfsRemove(newStr, ans, i, j, openBracket, closeBracket);
                        }
                    }
                    return; // 一次只处理一个多余的括号
                }
            }

            string reversedStr = s;
            reverse(reversedStr.begin(), reversedStr.end());
            if (openBracket == '(') {
                dfsRemove(reversedStr, ans, 0, 0, closeBracket, openBracket);
            }
            else {
                ans.push_back(reversedStr);
            }
        }

    public:
        vector<string> removeInvalidParentheses(string s) {
            vector<string> ans;
            // 还能拓展成[]{}等字符的配对
            dfsRemove(s, ans, 0, 0, '(', ')');
            return ans;
        }
    };
}
// ---------------------
// 【4.5】排列型回溯 (2)
// 不用考虑元素之间的前后关系，只需要考虑是否访问过，回溯的过程形成的树更加规整
// 相比起组合问题，之前没选过的元素都是备选，所以哈希很重要
/*
46.全排列：给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序 返回答案。

51.N皇后：按照国际象棋的规则，皇后可以攻击与之处在同一行或同一列或同一斜线上的棋子。
n 皇后问题 研究的是如何将 n 个皇后放置在 n×n 的棋盘上，并且使皇后彼此之间不能相互攻击。
给你一个整数 n ，返回所有不同的 n 皇后问题 的解决方案。
每一种解法包含一个不同的 n 皇后问题 的棋子放置方案，该方案中 'Q' 和 '.' 分别代表了皇后和空位。

52.N皇后II：n 皇后问题 研究的是如何将 n 个皇后放置在 n × n 的棋盘上，并且使皇后彼此之间不能相互攻击。
给你一个整数 n ，返回 n 皇后问题 不同的解决方案的数量。
*/
// ---------------------
// 模板题6：用布尔数组标记元素是否在集合中，用“枚举选哪个”的模板
namespace s46o1
{   // 时间复杂度，总共有n!个叶子，路径长度是nums的长度n，所以时间复杂度是O(n * n!)
    class Solution {
    public:
        vector<vector<int>> permute(vector<int>& nums) {
            int n = nums.size();
            vector<int> path(n);
            vector<int8_t> visited(n, false);
            vector<vector<int>> ans;

            auto dfs = [&](auto&& self, int index) {
                if (index == n) {
                    ans.push_back(path);
                    return;
                }
                for (int j = 0; j < n; ++j) {
                    if (!visited[j]) {
                        path[index] = nums[j];
                        visited[j] = true;
                        self(self, index + 1);
                        visited[j] = false;
                    }
                }
                };
            dfs(dfs, 0);
            return ans;
        }
    };
}
namespace s46m1
{   // 时隔数月后自己写出来的，题干中指出元素取值为[-10, 10]，所以可以直接用数组当哈希表，比unordered_map快
    // 哈希表的设定与o1相比各有优劣，当值域比较小的时候适合m1，当n比较小的时候适合o1
    class Solution {
    public:
        vector<vector<int>> permute(vector<int>& nums) {
            int cnt[21]{}; // 元素值 + 10当作哈希表的下标，利用了题干的信息
            vector<vector<int>> ans;
            vector<int> path;// 这里也可以改成固定长度的path，那后面就不用pop_back了
            int n = nums.size();

            auto dfs = [&](auto&& dfs, int i) -> void {
                if (path.size() == n) {
                    ans.push_back(path);
                    return;
                }

                for (int j = 0; j < n; ++j) {
                    int x = nums[j];
                    if (cnt[x + 10]) continue;
                    ++cnt[x + 10];
                    path.push_back(x);
                    dfs(dfs, i + 1);
                    path.pop_back();
                    --cnt[x + 10];
                }
                };
            dfs(dfs, 0);
            return ans;
        }
    };
}
namespace s46m2
{   // 在m1的基础上改为下标path和布尔数组visited，更规范
    class Solution {
    public:
        vector<vector<int>> permute(vector<int>& nums) {
            int n = nums.size();
            vector<bool> visited(21, false);
            vector<int> path(n);
            vector<vector<int>> ans;

            auto dfs = [&](auto&& dfs, int i)->void {
                if (i == n) {
                    ans.push_back(path);
                    return;
                }

                for (int j = 0; j < n; ++j) {
                    int x = nums[j];
                    if (visited[x + 10]) continue;
                    visited[x + 10] = true;
                    path[i] = x;
                    dfs(dfs, i + 1);
                    visited[x + 10] = false;
                }
                };

            dfs(dfs, 0);
            return ans;
        }
    };
}

// 模板题7：本质是枚举列号的全排列
namespace s51o1
{   // o1写法最清晰最容易懂，但是可以优化
    class Solution {
    private:
        // 因为枚举过程是从上到下的，所以只需要检查当前位置的上面半区（左上和右上）
        // 检查斜向（左上，右上）是否已经放过皇后，当前isvalid是O(n)复杂度，可以优化成O(1)，详见o2解法
        bool isValid(const vector<int>& path, int row, int col) {
            int n = path.size();
            int diag1 = row + col;
            int diag2 = row - col;
            for (int r = 0; r < n; ++r) {
                int c = path[r];
                int d1 = r + c;
                int d2 = r - c;
                if (diag1 == d1 || diag2 == d2) {
                    return false;
                }
            }
            return true;
        }
    public:
        vector<vector<string>> solveNQueens(int n) {
            vector<vector<string>> ans;
            vector<int> path;
            // colVisted就相当于对列号进行全排列，可以确保每行每列只有一个皇后
            vector<bool> colVisited(n, false);

            auto dfs = [&](auto&& self, int row)->void {
                if (row == n) {
                    vector<string> scheme;
                    // 比较笨的向ans更新答案的方法，其实可以先预设好一个全是'.'的棋盘，然后按照path放好Q后再更新，详见o2
                    for (int col : path) {
                        string s = "";
                        for (int i = 0; i < col; ++i) {
                            s += '.';
                        }
                        s += 'Q';
                        for (int j = col + 1; j < n; ++j) {
                            s += '.';
                        }
                        scheme.push_back(s);
                    }
                    ans.push_back(scheme);
                    return;
                }

                for (int col = 0; col < n; ++col) {
                    if (!colVisited[col] && isValid(path, row, col)) {
                        colVisited[col] = true;
                        path.push_back(col);
                        self(self, row + 1);
                        path.pop_back();
                        colVisited[col] = false;
                    }
                }
                };
            dfs(dfs, 0);

            return ans;
        }
    };
}
namespace s51o2
{   // 改进1：用diag1和diag2（相当于visited）布尔数组，加快isValid查询过程，将isValid的时间复杂度降至O(1)
    // 改进2：预定义空棋盘，修改时定点放入皇后棋子，更新答案更高效
    // 改进3（可选）：将布尔数组替换为uint8_t，加快运行速度
    class Solution {
    public:
        vector<vector<string>> solveNQueens(int n) {
            vector<vector<string>> ans;
            vector<string> board(n, string(n, '.'));
            vector<bool> colVisited(n, false);
            // 与colVisited相似，用布尔数组代替O(n)的检索过程，注意对角线布尔数组大小预设为2 * n - 1
            // diag1储存row + col，那么最小为0，最大为2 * n - 2，共2 * n - 1个数
            // diag2储存row - col，但因存在负数（最小为0 - (n - 1)），所以检索时将下标加上n - 1，最大为n - 1，共2 * n - 1个数
            vector<bool> diag1(2 * n - 1, false);
            vector<bool> diag2(2 * n - 1, false);
            // vector<uint8_t> diag1(2 * n - 1, 0); 
            // 可以用unsigned 8 bit type(无符号8位整数，相当于unsigned char)来提高效率
            // uint8_t在只将bool当作0/1处理的场景下运行速度比bool更快，但占用的空间比bool大，
            // 在需要储存极大的位图，或是对内存敏感的场景可以用bool，
            // 毕竟只占用一个字节，但是uint8_t的速度是bool的几倍，需要自行取舍
            // bool经过位压缩，可能访问时涉及不同CPU缓存行，而uint8_t是连续的字节储存，缓存命中率更高
            // 同时uint8_t的访问修改都是直接的CPU指令，bool需要额外的位运算，CPU指令更多更复杂

            auto dfs = [&](auto&& self, int row)->void {
                if (row == n) {
                    ans.push_back(board);
                    return;
                }

                for (int col = 0; col < n; ++col) {
                    int rc = row - col + n - 1;
                    if (!colVisited[col] && !diag1[row + col] && !diag2[rc]) {
                        board[row][col] = 'Q';// 直接修改空棋盘
                        colVisited[col] = diag1[row + col] = diag2[rc] = true;// 连等号简化代码
                        self(self, row + 1);
                        colVisited[col] = diag1[row + col] = diag2[rc] = false;
                        board[row][col] = '.';// 恢复现场
                    }
                }
                };
            dfs(dfs, 0);

            return ans;
        }
    };
}
// ---------------------
// 【4.6】有重复元素的回溯 (3)
// 与子集型、组合型、排列型回溯的区别在于需要进行额外的一步去重（一般是在不选阶段+单词枚举开始前），一般需要进行排序
// 子集型和组合型回溯两种方法都可以，但排列型必须用枚举选哪个的去重方式，最好两种去重的回溯方法都要掌握
// 这三道题基本就是前面几种类型回溯的综合，对比着思考思考
// 没道题是dfs(i + 1)还是dfs(j + 1)，终止条件加入答案的条件，去重方式都多少有些不同，好好体会
// （还有一种类型是元素可以重复使用的回溯，但那种比较简单）
/*
90.子集II：给你一个整数数组 nums ，其中可能包含重复元素，请你返回该数组所有可能的 子集（幂集）。
解集 不能 包含重复的子集。返回的解集中，子集可以按 任意顺序 排列。

40.组合总和II：给定一个候选人编号的集合 candidates 和一个目标数 target ，
找出 candidates 中所有可以使数字和为 target 的组合。
candidates 中的每个数字在每个组合中只能使用 一次 。
注意：解集不能包含重复的组合。

47.全排列II：给定一个可包含重复数字的序列 nums ，按任意顺序 返回所有不重复的全排列。
*/
// ---------------------
// 模板题8：与s78子集型回溯有所区别，重点在于只有不选相同的数才会导致结果重复，或是在枚举选哪个时不枚举相同元素（跳过）
namespace s90o1
{   // 选或不选
    class Solution {
        // 与s78有所区别，重点在于只有不选相同的数才会导致结果重复
        // 所以只需要在进行不选的递归前进行判断即可

        // 比如[1, 2, 2]，可能造成重复的原因在于：
        // 不选第一个2，以及不选第二个2，都能得到相同的子集[1, 2]
    public:
        vector<vector<int>> subsetsWithDup(vector<int>& nums) {
            int n = nums.size();
            if (n == 0) return {};

            vector<vector<int>> ans;
            vector<int> path;

            sort(nums.begin(), nums.end());

            auto dfs = [&](auto&& self, int i) -> void {
                if (i == n) {
                    ans.push_back(path);
                    return;
                }
                path.push_back(nums[i]);
                self(self, i + 1);
                path.pop_back();

                // 允许重复的情况下一般选或不选都是先写不选再写选
                // 但不允许重复时因为要跳过元素修改i，所以先写选再写不选
                while (i + 1 < n && nums[i + 1] == nums[i]) {
                    ++i;
                }
                self(self, i + 1);
                };
            dfs(dfs, 0);
            return ans;
        }
    };
}
namespace s90o2
{   // 枚举选哪个
    class Solution {
    public:
        vector<vector<int>> subsetsWithDup(vector<int>& nums) {
            int n = nums.size();
            if (n == 0) return {};

            vector<vector<int>> ans;
            vector<int> path;

            sort(nums.begin(), nums.end());

            auto dfs = [&](auto&& self, int i) -> void {
                ans.push_back(path);

                for (int j = i; j < n; ++j) {
                    // 去重逻辑
                    if (j > i && nums[j] == nums[j - 1]) {
                        // 这里的去重写法和s47不同，s47是j > 0
                        // 每轮选择的多叉树中，只要左右相邻的元素相同，就跳过，比如[1, 1, 2, 2]
                        // 在深度为1时，i = 0, j = i = 0，有4个可选项
                        // 在第二个1和第二个2时进行跳过，只要这一层多叉树中没有出现重复的可选项，答案就不会重复
                        // 所以这里条件是j > i而不是j > 0
                        continue;
                    }
                    path.push_back(nums[j]);
                    self(self, j + 1);
                    path.pop_back();
                }
                };
            dfs(dfs, 0);
            return ans;
        }
    };
}

// 和s90中跳过重复元素的方法一致
namespace s40m1
{
    class Solution {
    public:
        vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
            sort(candidates.begin(), candidates.end());
            vector<vector<int>> ans;
            vector<int> path;
            int n = candidates.size();

            auto dfs = [&](auto&& self, int i, int sum) {
                if (sum == target) {
                    ans.push_back(path);
                    return;
                }
                if (i == n || sum + candidates[i] > target) {
                    return;
                }
                path.push_back(candidates[i]);
                self(self, i + 1, sum + candidates[i]);
                path.pop_back();

                while (i + 1 < n && candidates[i] == candidates[i + 1]) {
                    ++i;
                }
                self(self, i + 1, sum);
                };
            dfs(dfs, 0, 0);
            return ans;
        }
    };
}
namespace s40m2
{   // 枚举选哪个
    class Solution {
    public:
        vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
            sort(candidates.begin(), candidates.end());
            vector<vector<int>> ans;
            vector<int> path;
            int n = candidates.size();

            auto dfs = [&](auto&& self, int start, int sum) {
                if (sum == target) {
                    ans.push_back(path);
                    return;
                }

                for (int i = start; i < n; ++i) {
                    if (sum + candidates[i] > target) {
                        break;
                    }
                    if (i > start && candidates[i] == candidates[i - 1]) {
                        continue;
                    }
                    path.push_back(candidates[i]);
                    self(self, i + 1, sum + candidates[i]);
                    path.pop_back();
                }
                };
            dfs(dfs, 0, 0);
            return ans;
        }
    };
}

// 模板题9：排列型回溯的去重，与子集型的枚举选哪个去重比较类似
namespace s47o1
{   // 这个的去重没有那么好理解
    // 比如[1, 1, 2]，当第一个位置选到第二个1时，如果此时第一个1还没使用，是不能在第一个位置填第二个1的，会导致重复
    // 产生两个[1, 1, 2]，去重条件是：前一个枚举到的元素和当前元素相等，同时前一个元素还处于空闲状态
    class Solution {
    public:
        vector<vector<int>> permuteUnique(vector<int>& nums) {
            int n = nums.size();
            if (n == 0) return {};

            vector<vector<int>> ans;
            vector<int> path(n, 0);
            vector<bool> visited(n, false);
            sort(nums.begin(), nums.end());// 排序

            auto dfs = [&](auto&& self, int index)->void {
                // 这里用index和i，不用i和j的原因是可读性更高
                if (index == n) {
                    ans.push_back(path);
                    return;
                }

                for (int i = 0; i < n; ++i) {
                    if (!visited[i]) {
                        // 下面这个多的if判断就是去重
                        if (i > 0 && nums[i] == nums[i - 1] && !visited[i - 1]) {
                            // 简单理解为：只要是有相同的元素，只要将其放入排列的path中
                            // 放入path的顺序必须是从左到右的，比如[1A, 1B, 1C, 2]
                            // 只有1A放入了path，1B才能放入path
                            // 只有1B放入了path，1C才能放入path
                            // 这样可以确保不会出现重复的path
                            // 总结：大哥都还没入队，小弟就别上
                            continue;
                        }
                        // 也可以整体写为为：
                        // if (visited[i] || (i > 0 && nums[i] == nums[i - 1] && !visited[i - 1])) continue;
                        visited[i] = true;
                        path[index] = nums[i];
                        self(self, index + 1);
                        visited[i] = false;
                    }
                }
                };
            dfs(dfs, 0);
            return ans;
        }
    };
}
// ---------------------
// 【4.7】搜索 (1)
// 网格图 + 回溯
/*
79.单词搜索：给定一个 m x n 二维字符网格 board 和一个字符串单词 word 。
如果 word 存在于网格中，返回 true ；否则，返回 false 。
单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。
同一个单元格内的字母不允许被重复使用。
*/
// ---------------------
// 模板题10：岛屿类网格图DFS + 回溯
namespace s79o1
{   // o1为最粗暴的做法，整体框架和岛屿类网格图DFS很类似
    // 本题题干中m, n最大为6，如需处理m, n更大的棋盘，需要剪枝优化
    class Solution {
    public:
        bool exist(vector<vector<char>>& board, string word) {
            int m = board.size();
            int n = board[0].size();

            // 创建visited数组，记录访问状态
            vector<vector<bool>> visited(m, vector<bool>(n, false));

            auto dfs = [&](auto&& self, int r, int c, int i)->bool {
                // 越界检查放在最前面
                if (r < 0 || r >= m || c < 0 || c >= n) return false;

                // 检查是否已访问或字符不匹配
                if (visited[r][c] || board[r][c] != word[i]) {
                    return false;
                }

                // 找到完整单词
                if (i + 1 == word.length()) {
                    return true;
                }

                // 标记当前单元格已访问
                visited[r][c] = true;

                // 向4个方向递归搜索
                if (self(self, r - 1, c, i + 1) ||  // 上
                    self(self, r + 1, c, i + 1) ||  // 下
                    self(self, r, c - 1, i + 1) ||  // 左
                    self(self, r, c + 1, i + 1)) {  // 右
                    return true;
                }

                // 回溯：恢复访问状态
                visited[r][c] = false;
                return false;
                };

            // 遍历棋盘上的每个起始点
            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c) {
                    // 如果找到单词，立即返回true
                    if (dfs(dfs, r, c, 0)) {
                        return true;
                    }
                }
            }

            return false;
        }
    };
}
namespace s79o2
{   // 引入两个剪枝：
    // 1.可行性剪枝，如果word中的某个字符数量比棋盘中的对应字符数量还多，那么一定是无法在棋盘中搜到word的，利用O(n)的时间剪枝
    // 2.顺序剪枝，通过更改递归的顺序，来提高查找到答案的速度（可能性），也即比较word中第一个字母和最后一个字母在棋盘中的
    //   出现次数，如果最后一个字母出现次数更多，那么将word反转后再来搜会更好，更容易在一开始就满足 board[i][j] != word[k]，
    //   也就不会再往下递归了，递归次数更少。
    class Solution {
    public:
        bool exist(vector<vector<char>>& board, string word) {
            int m = board.size(), n = board[0].size();
            int len = word.size();
            // 0.可行性剪枝
            if (m * n < len) return false;

            // 1.可行性剪枝
            unordered_map<char, int> boardMp;
            unordered_map<char, int> wordMp;
            for (auto& row : board) {
                for (char c : row) {
                    ++boardMp[c];
                }
            }
            for (char c : word) {
                if (++wordMp[c] > boardMp[c]) {
                    return false;
                }
            }

            // 2.顺序剪枝
            if (boardMp[word.back()] < boardMp[word[0]]) {
                reverse(word.begin(), word.end());
            }

            // 余下部分和o1相同
            // 创建visited数组，记录访问状态
            vector<vector<bool>> visited(m, vector<bool>(n, false));

            auto dfs = [&](auto&& self, int r, int c, int i)->bool {
                // 越界检查放在最前面
                if (r < 0 || r >= m || c < 0 || c >= n) return false;

                // 检查是否已访问或字符不匹配
                if (visited[r][c] || board[r][c] != word[i]) {
                    return false;
                }

                // 找到完整单词
                if (i + 1 == word.length()) {
                    return true;
                }

                // 标记当前单元格已访问
                visited[r][c] = true;

                // 向4个方向递归搜索
                if (self(self, r - 1, c, i + 1) ||  // 上
                    self(self, r + 1, c, i + 1) ||  // 下
                    self(self, r, c - 1, i + 1) ||  // 左
                    self(self, r, c + 1, i + 1)) {  // 右
                    return true;
                }

                // 回溯：恢复访问状态
                visited[r][c] = false;
                return false;
                };

            // 遍历棋盘上的每个起始点
            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c) {
                    // 如果找到单词，立即返回true
                    if (dfs(dfs, r, c, 0)) {
                        return true;
                    }
                }
            }
            return false;
        }
    };
}
namespace s79m1
{   // 通过修改board来省掉visited数组，且不会影响原始数据board，结束遍历后会还原
    class Solution {
    public:
        bool exist(vector<vector<char>>& board, string word) {
            int m = board.size(), n = board[0].size();
            int len = word.size();
            // 可行性剪枝
            if (m * n < len) return false;

            unordered_map<char, int> boardMap;
            unordered_map<char, int> wordMap;
            for (const auto& row : board) {
                for (char c : row) {
                    ++boardMap[c];
                }
            }
            for (char c : word) {
                if (++wordMap[c] > boardMap[c]) {
                    return false;
                }
            }

            // 顺序剪枝
            if (boardMap[word.back()] < boardMap[word[0]]) {
                reverse(word.begin(), word.end());
            }

            auto dfs = [&](auto&& dfs, int r, int c, int i)->bool {
                if (r < 0 || r >= m || c < 0 || c >= n) {
                    return false;
                }
                if (!board[r][c] || board[r][c] != word[i]) {
                    return false;
                }
                // 细节：这里不能改成i == len，如果改了为了避免上一行越界，要把这行前移
                // 但这样仍然会是错误的，因为如果i == n - 1时匹配完成后，运行下面的四向判断时全部越界，那么就走不到
                // i == len这一步。比如word = "a", board = [['a']]，能提前结束递归就提前结束
                if (i + 1 == len) {
                    return true;
                }
                // 虽然dfs的途中修改了board，但结束搜索的时候会进行还原，可以把visited省掉
                board[r][c] = 0;
                if (dfs(dfs, r + 1, c, i + 1) || dfs(dfs, r - 1, c, i + 1) ||
                    dfs(dfs, r, c + 1, i + 1) || dfs(dfs, r, c - 1, i + 1)) {
                    board[r][c] = word[i];  // 这一行要加上，如果要求不能修改原始数据
                    return true;
                }
                // 还原board
                board[r][c] = word[i];
                return false;
                };

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dfs(dfs, i, j, 0)) {
                        return true;
                    }
                }
            }
            return false;
        }
    };
}
// ---------------------
// 【4.8】折半枚举 ()
/*

*/
// ---------------------
// 暂时略过

