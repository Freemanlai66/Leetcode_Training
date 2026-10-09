#pragma once
#include<stack>
#include<vector>
#include<string>
#include<numeric>
#include<sstream>
#include<unordered_map>

using namespace std;

// 问题待定：
/*
sxxx：xxx
*/

/*
模板题：
1.类似s20的括号匹配，将匹配过程用栈模拟：1003
2.栈重建，将计数器和字符都存储在栈中，不需要修改字符串，只需要根据栈中结果重建字符串：1209
3.本题本身没什么特别的，但是可以当作学习使用常量静态变量的练习：20
4.o1为常规O(n ^ 2)做法，o2为奇技淫巧了解即可，将多次反转转换成单次遍历：1190
5.和s1209类似，栈不存储容器元素，而是存储答案相关的指标，比如这题中是分数：856
6.没有括号的中缀表达式求值：1006
7.后缀表达式求值，拓展为如何将中缀表达式转化为后缀表达式：150
8.跟s856有些类似，但也有所不同，s856最终分数可以保存在stack.top()上，但这题不行：394
9.有括号（优先级）的中缀表达式（仅限 + / -），需要注意 - x可以作为一元运算符，还有空格：224
10.没有括号的中缀表达式（ + - * / ），和s1006是类似的，但更难：227
11.初识单调栈，及时去掉无用元素，保持栈内元素单调性：739
12.稍微复杂一点点的单调栈，和s503结合对照理解：496
13.单调栈问题的集大成，考察分析与转化，遍历每个高度，获取每个高度左边和右边的最近更小值下标：84
14.移除xx元素后，获得最小/大字典序排列的字符串模板：402
*/

// 栈：基础 + 进阶 + 邻项消除 + 合法括号字符串（RBS）+ 表达式解析 + 对顶栈 + 单调栈

// 【3.1-3.2】基础 + 进阶(8)
// FILO的思想运用，不一定用到了stack，可能是vector，主要是锻炼栈的思维
/*
1441.用栈操作构建数组：给你一个数组 target 和一个整数 n。给你一个空栈和两种操作：
"Push"：将一个整数加到栈顶。"Pop"：从栈顶删除一个整数。同时给定一个范围 [1, n] 中的整数流。
使用两个栈操作使栈中的数字（从底部到顶部）等于 target。你应该遵循以下规则：
如果整数流不为空，从流中选取下一个整数并将其推送到栈顶。如果栈不为空，弹出栈顶的整数。
如果，在任何时刻，栈中的元素（从底部到顶部）等于 target，则不要从流中读取新的整数，也不要对栈进行更多操作。
请返回遵循上述规则构建 target 所用的操作序列。如果存在多个合法答案，返回 任一 即可。


*/
// ---------------------
// m1和o1思路都可以，殊途同归
namespace s1441m1
{   // 遍历[1, n]，逐个与target中元素进行比较
    class Solution {
    public:
        vector<string> buildArray(vector<int>& target, int n) {
            vector<string> operations;
            int index = 0; // 指向target当前需要匹配的元素位置
            for (int i = 1; i <= n && index < target.size(); ++i) {
                operations.push_back("Push"); // 必须Push当前数字
                if (i == target[index]) {
                    index++; // 匹配成功,准备匹配下一个
                }
                else {
                    operations.push_back("Pop"); // 不匹配,需要Pop
                }
            }
            return operations;
        }
    };
}
namespace s1441o1
{   // 遍历target，不是与target中元素相等就跳过，与m1没有本质区别
    class Solution {
    public:
        vector<string> buildArray(vector<int>& target, int n) {
            int cur = 0;
            vector<string> ans;
            for (int num : target) {
                while (++cur < num) {
                    ans.push_back("Push");
                    ans.push_back("Pop");
                }
                ans.push_back("Push");
            }
            return ans;
        }
    };

}

// o1更规范，用switch和op[0]来加速
namespace s682m1
{
    class Solution {
    public:
        int calPoints(vector<string>& operations) {
            vector<int> pnt;
            for (const auto& op : operations) {
                int n = pnt.size();
                if (op == "+") {
                    pnt.push_back(pnt[n - 1] + pnt[n - 2]);
                }
                else if (op == "D") {
                    pnt.push_back(pnt[n - 1] * 2);
                }
                else if (op == "C") {
                    pnt.pop_back();
                }
                else {
                    pnt.push_back(stoi(op));
                }
            }
            return accumulate(pnt.begin(), pnt.end(), 0);
        }
    };
}
namespace s682o1
{
    class Solution {
    public:
        int calPoints(vector<string>& operations) {
            vector<int> st;
            for (const auto& op : operations) {
                switch (op[0]) {
                case '+':
                    st.push_back(st[st.size() - 2] + st.back());
                    break;
                case 'D':
                    st.push_back(st.back() * 2);
                    break;
                case 'C':
                    st.pop_back();
                    break;
                default:
                    st.push_back(stoi(op));
                }
            }
            return accumulate(st.begin(), st.end(), 0);
        }
    };
}

// 相当于退格字符串s844
namespace s2390m1
{
    class Solution {
    public:
        string removeStars(string s) {
            string ret;
            for (char c : s) {
                if (c != '*') {
                    ret += c;
                }
                else {
                    ret.pop_back();// 这里不用检查，因为题目保证了每个星号都有配对的元素可供删除
                }
            }
            return ret;
        }
    };
}

// o1用了初始化列表以及resize来实现vector尾部元素的删除
namespace s1472m1
{
    class BrowserHistory {
    private:
        vector<string> pages;
        int index;
    public:
        BrowserHistory(string homepage) {
            pages.push_back(homepage);
            index = 0;
        }

        void visit(string url) {
            int cur = pages.size() - 1;
            while (cur > index) {
                pages.pop_back();
                --cur;
            }
            pages.push_back(url);
            index = pages.size() - 1;
        }

        string back(int steps) {
            index = max(0, index - steps);
            return pages[index];
        }

        string forward(int steps) {
            index = min(int(pages.size() - 1), index + steps);
            return pages[index];
        }
    };
}
namespace s1472o1
{
    class BrowserHistory {
    private:
        vector<string> pages;
        int index = 0;
    public:
        BrowserHistory(string homepage) : pages({ homepage }) {}

        void visit(string url) {
            ++index;
            pages.resize(index);// resize只保留vector的前index个元素，其他的删除
            pages.push_back(url);
        }

        string back(int steps) {
            index = max(0, index - steps);
            return pages[index];
        }

        string forward(int steps) {
            index = min((int)pages.size() - 1, index + steps);
            return pages[index];
        }
    };
}

namespace s946m1
{
    class Solution {
    public:
        bool validateStackSequences(vector<int>& pushed, vector<int>& popped) {
            stack<int> st;
            int pp = 0, n = pushed.size();
            for (int num : pushed) {
                st.push(num);
                while (!st.empty() && st.top() == popped[pp]) {
                    st.pop();
                    ++pp;
                }
            }
            return st.empty();
        }
    };
}

// 用26个栈储存字母的最新下标
namespace s3412m1
{   // 用26个栈储存字母的最新下标
    class Solution {
    public:
        long long calculateScore(string s) {
            vector<stack<int>> dic(26);
            long long score = 0;
            int n = s.size();
            for (int i = 0; i < n; ++i) {
                char c = s[i];
                char sym = 'a' + 25 - (c - 'a');

                if (!dic[sym - 'a'].empty()) {
                    score += i - dic[sym - 'a'].top();
                    dic[sym - 'a'].pop();
                }
                else {
                    dic[c - 'a'].push(i);
                }
            }
            return score;
        }
    };
}

// ostringstream 是输出字符串流，专门用于高效构建字符串，头文件<sstream>
namespace s71m1
{
    class Solution {
    public:
        string simplifyPath(string path) {
            // 空串和'.' 直接跳过
         // ..和其左侧的字符串 删除（模拟返回上一级目录）
            vector<string> directories;
            istringstream ss(path);
            string s;

            while (getline(ss, s, '/')) {
                if (s.empty() || s == ".") {
                    continue;
                }
                if (s != "..") {
                    directories.push_back(s);
                }
                else if (!directories.empty()) {
                    directories.pop_back();
                }
            }

            if (directories.empty()) return "/";

            string ans;
            for (const auto& s : directories){
                ans += '/';
                ans += s;
            }
            return ans;
        }
    };
}

namespace s155m1
{   // 前缀最小值的栈
    class MinStack {
    private:
        stack<int> st;
        stack<int> minSt;
    public:
        MinStack() {
            minSt.push(INT_MAX);
        }

        void push(int val) {
            st.push(val);
            minSt.push(min(minSt.top(), val));
        }

        void pop() {
            st.pop();
            minSt.pop();
        }

        int top() {
            return st.top();
        }

        int getMin() {
            return minSt.top();
        }
    };
}
namespace s155o1
{   // m1是双栈，灵神写法是pair，两者思想是一致的
    class MinStack {
        stack<pair<int, int>> st;

    public:
        MinStack() {
            // 添加栈底哨兵 INT_MAX
            // 这里的 0 写成任意数都可以，反正用不到
            st.emplace(0, INT_MAX);
        }

        void push(int val) {
            st.emplace(val, min(getMin(), val));
        }

        void pop() {
            st.pop();
        }

        int top() {
            return st.top().first;
        }

        int getMin() {
            return st.top().second;
        }
    };
}
namespace s155o2
{   // 不使用辅助栈写法 ，虽然题干指出pop top getMin总是在非空栈上调用，但这里还是给出更健壮的方案，了解一下没坏处
    class MinStack {
    private:
        long long minVal;       // 当前最小值
        stack<long long> stk;   // 存储差值

    public:
        MinStack() {
            // minVal 在第一次 push 时初始化，这里不用特别处理
        }

        void push(int value) {
            // 拓宽转换 int -> long long
            long long x = value;
            if (stk.empty()) {
                stk.push(0);
                minVal = x;
            }
            else {
                long long diff = x - minVal;
                stk.push(diff);
                minVal = min(minVal, x);
            }
        }

        void pop() {
            long long topDiff = stk.top();
            if (topDiff < 0) {
                // 恢复上一个最小值
                minVal = minVal - topDiff;
            }
            stk.pop();
        }

        int top() {
            long long topDiff = stk.top();
            if (topDiff > 0) {
                // 缩窄转换 long long -> int，用static_int显式写出以表明意图，且避免编译器警告
                return static_cast<int>(topDiff + minVal);
            }
            else {
                // 栈顶就是当前最小值
                return static_cast<int>(minVal);
            }
        }

        int getMin() {
            return static_cast<int>(minVal);
        }
    };

}
// ---------------------
// 【3.3】邻项消除(4)
// 只要题干出现字符串邻项按照某种条件消除的描述，都可以往栈这块考虑
/*
1047.删除字符串中的所有相邻重复项：给出由小写字母组成的字符串 s，重复项删除操作会选择两个相邻且相同的字母，并删除它们。
在 s 上反复执行重复项删除操作，直到无法继续删除。在完成所有重复项删除操作后返回最终的字符串。答案保证唯一。
*/
// ---------------------
namespace s1047m1
{
    class Solution {
    public:
        string removeDuplicates(string s) {
            string ans;
            for (char c : s) {
                if (!ans.empty() && c == ans.back()) {
                    ans.pop_back();
                }
                else {
                    ans.push_back(c);
                }
            }
            return ans;
        }
    };
}

// 模板题1：类似s20的括号匹配，将匹配过程用栈模拟
// m1常规栈写法，空间复杂度O(n)
// o1写法将s本身当作栈，用i充当栈大小和下标，可读性差，但空间复杂度O(1)
namespace s1003m1
{
    class Solution {
    public:
        bool isValid(string s) {
            stack<char> st;
            for (char c : s) {
                if (c == 'c') {
                    // 检查栈顶是否依次是 'b' 和 'a'
                    if (st.size() < 2) return false;
                    char b = st.top(); st.pop();
                    char a = st.top(); st.pop();
                    if (b != 'b' || a != 'a') return false;
                }
                else {
                    st.push(c);
                }
            }
            return st.empty();
        }
    };
}
namespace s1003o1
{   // 实在太过简洁，可读性太差，不如m1，这种原地O(1)空间复杂度的技巧不会也无所谓
    class Solution {
    public:
        bool isValid(string s) { // s 同时作为栈
            int i = 0; // i-1 表示栈顶下标，i表示栈的大小
            for (char c : s) {
                if (c > 'a' && (i == 0 || c - s[--i] != 1))
                    return false;
                // 上两行代码等价于下面的代码：
                if (c > 'a') { // 当前字符为b或c
                    if (i == 0) return false;// 栈为空的前提下push b或c一定不满足条件
                    --i; // pop出之前的元素
                    if (c - s[i] != 1) {// s[i]实际上已经出栈了
                        return false;// c之前不是b，或者b之前不是a
                    }
                }

                // 在进入下行代码前，如果没有返回false：
                // 如果当前字符为b，那么已经将之前的a出栈，准备将b放进去（也即只要栈内有b，一定代表着b的原位置曾经有a）
                // 如果当前字符是c，那么已经将之前的b出栈，把abc一组元素消除干净了
                if (c < 'c') {// 当前字符为a或b
                    s[i++] = c; // 入栈
                }
                
                
                
            }
            return i == 0;
        }
    };

  
}

// 模板题2：m1, o1为栈重建方法，将计数器和字符都存储在栈中，不需要修改字符串，只需要根据栈中结果重建字符串
// 栈不仅可以存储题干中给出容器内部的部分元素，还可以存储一些答案相关的指标，比如这题中就是字符出现的频次
// o2用双指针在原地进行字符串修改，空间占用更低，另外可以借鉴o1和o2中对于栈空的条件判断写法(||)
namespace s1209m1
{   // 属于栈重建方法，先用栈保存字符串，再依照栈重建出字符串返回值，需要额外空间
    // 用栈记录每个字符和对应的出现次数
    class Solution {
    public:
        string removeDuplicates(string s, int k) {
            vector<pair<char, int>> st;
            for (char c : s) {
                if (st.empty()) {
                    st.emplace_back(c, 1);
                    continue;
                }

                if (c == st.back().first) {
                    if (st.back().second == k - 1) {
                        st.pop_back();
                    }
                    else {
                        ++st.back().second;// 重点：不是加一个pair进去，而是直接将栈顶元素+1
                    }                      // 这样一来从栈顶删除元素更容易，栈元素之间也将有连续性
                }
                else {
                    st.emplace_back(c, 1);
                }
            }
            string ans;
            for (auto& p : st) {
                while (p.second--) {
                    ans += p.first;
                }
            }
            return ans;
        }
    };
}
namespace s1209o1
{   //为m1方法的优化版
    class Solution {
    public:
        string removeDuplicates(string s, int k) {
            vector<pair<char, int>> counts;
            for (char c : s) {
                if (counts.empty() || c != counts.back().first) {
                    counts.emplace_back(c, 1);
                }// 优化了逻辑顺序，简化代码
                else if (++counts.back().second == k) {
                    counts.pop_back();
                }
            }
            string ans = "";
            for (auto& p : counts) {
                ans += string(p.second, p.first);// 拼接字符串更高效
            }
            return ans;
        }
    };
}
namespace s1209o2
{   // 原地修改字符串，空间复杂度比o1更低
    // 用双指针进行修改
    class Solution {
    public:
        string removeDuplicates(string s, int k) {
            stack<int> counts;
            int j = 0;
            for (int i = 0; i < s.size(); ++i, ++j) {
                // 把当前字符复制到结果位置
                s[j] = s[i];

                // 检查当前字符是否是新的字符序列的开始
                if (j == 0 || s[j] != s[j - 1]) {
                    counts.push(1);
                }
                // 否则增加计数器，并检查是否需要删除
                else if (++counts.top() == k) {
                    counts.pop();// 移除这个计数器
                    j -= k;     // 回退j指针，相当于删除这k个字符
                }
            }
            return s.substr(0, j);// 返回处理后的字符串
            // 注意有效字符最右侧为s[j],因为退出循环后有++j，所以这里直接count = j就好
        }
    };
}

namespace s735m1
{
    class Solution {
    public:
        vector<int> asteroidCollision(vector<int>& asteroids) {
            vector<int> ans;
            for (int x : asteroids) {
                if (ans.empty() || x > 0 || (x < 0 && ans.back() < 0)) {
                    ans.push_back(x);// 只有ans.back() > 0（向右），x < 0（向左）时才会发生碰撞
                }
                else {
                    while (!ans.empty() && ans.back() > 0 && ans.back() < -x) {
                        ans.pop_back();//尽可能的进行碰撞（行星质量相等的情况不能归在这里，因为此时撞一次就没了）
                    }
                    if (ans.empty() || ans.back() < 0) {
                        ans.push_back(x);// 只有在ans为空，或末尾为向左的行星，碰撞后才能把当前行星加进ans
                    }
                    else if (ans.back() == -x) {
                        ans.pop_back();
                    }
                }
            }
            return ans;
        }
    };
}
// ---------------------
// 【3.4】合法括号字符串（RBS）(5)
// 字符串中各式各样的括号配对题，只要题目中提了括号都可以往这里考虑
/*
20.有效的括号：给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s ，判断字符串是否有效。
有效字符串需满足：左括号必须用相同类型的右括号闭合。左括号必须以正确的顺序闭合。
每个右括号都有一个对应的相同类型的左括号。
*/
// ---------------------
// 模板题3：本题本身没什么特别的，但是可以当作学习使用常量静态变量的练习
namespace s20m1
{
    class Solution {
    public:
        bool isValid(string s) {
            int n = s.size();
            if (n % 2 == 1) return false;// 确保长度为偶数，同时n >= 2
            unordered_map<char, char> mp = { {'(', ')'}, {'[', ']'}, {'{', '}'} };
            stack<char> st;
            for (char c : s) {
                // 如果c为左半括号，则直接加c加入栈(入栈的永远是左半括号)
                if (mp.find(c) != mp.end()) {
                    st.push(c);
                    // 如果c为右半括号，开始判断是否匹配
                }
                else if (!st.empty() && mp[st.top()] == c) {
                    st.pop();//匹配成功，栈顶左半括号出栈
                }
                else {
                    return false;//匹配失败 或 当前栈内没有元素
                }
            }
            return st.empty();
        }
    };
}
namespace s20o1
{   // 如何将常量设为静态变量
    class Solution {
    private:
        static const unordered_map<char, char> mp;
        // 如果要将mp设置成常量静态成员变量，初始化必须放在类外
        // 如果mp是bool, int, char，因为同时是inline和const值，在编译时就可以确定，那么就可以在类内初始化（C++ 17）
        // 同时访问mp时也要注意了，不能再用operator[]，因为这个操作符重载默认如果找不到元素就加入一个新的，与const冲突
        // 如果是C++17及以上的版本，可以直接初始化为下列形式
        // static const inline unordered_map<char, char> mp = { {'(', ')'}, {'[', ']'}, {'{', '}'} };

    public:
        bool isValid(string s) {
            int n = s.size();
            if (n % 2 == 1) return false;// 确保长度为偶数，同时n >= 2
            stack<char> st;
            for (char c : s) {
                // 如果c为左半括号，则直接加c加入栈(入栈的永远是左半括号)
                if (mp.find(c) != mp.end()) {
                    st.push(c);
                    // 如果c为右半括号，开始判断是否匹配
                }
                else if (!st.empty() && mp.at(st.top()) == c) {
                    // 将operator[]改成at()，这样如果找不到会抛出异常，能通过编译（虽然这份代码运行过程中不可能抛出异常）
                    st.pop();//匹配成功，栈顶左半括号出栈
                }
                else {
                    return false;//匹配失败 或 当前栈内没有元素
                }
            }
            return st.empty();
        }
    };
    const unordered_map<char, char> Solution::mp = { {'(', ')'}, {'[', ']'}, {'{', '}'} };
    // 初始化时也要带上const关键字
}
namespace s20o2
{   // 用静态函数返回局部static变量，延迟初始化，降低启动开销，只有第一次被调用时才会初始化
    class Solution {
    private:
        // 用静态函数返回局部static变量，这样可以完全规避复杂的静态成员初始化问题
        static const unordered_map<char, char>& getMap() {
            static const unordered_map<char, char> mp = { {'(',')'}, {'[',']'}, {'{','}'} };
            return mp;
        }
    public:
        bool isValid(string s) {
            int n = s.size();
            if (n % 2 == 1) return false;// 确保长度为偶数，同时n >= 2
            const auto& mp = getMap();
            stack<char> st;
            for (const char c : s) {
                // 如果c为左半括号，则直接加c加入栈(入栈的永远是左半括号)
                if (mp.find(c) != mp.end()) {
                    st.push(c);
                    // 如果c为右半括号，开始判断是否匹配
                }
                else if (!st.empty() && mp.at(st.top()) == c) {
                    st.pop();//匹配成功，栈顶左半括号出栈
                }
                else {
                    return false;//匹配失败 或 当前栈内没有元素
                }
            }
            return st.empty();
        }
    };
}
namespace s20m2
{   // 自己想的简单写法，没有中途返回false的功能，必须全部遍历完整个s
    class Solution {
    public:
        bool isValid(string s) {
            unordered_map<char, char> mp = {
                {')', '('}, {']', '['}, {'}', '{'}
            };
            stack<char> stk;

            for (char c : s) {
                if (!stk.empty() && stk.top() == mp[c]) {
                    stk.pop();
                }
                else {
                    stk.push(c);
                }
            }
            return stk.empty();
        }
    };
}

// 类似s20括号配对，但是简单不少，理解题意即可
namespace s921m1
{
    class Solution {
    public:
        int minAddToMakeValid(string s) {
            stack<char> st;
            for (char c : s) {
                if (!st.empty() && st.top() == '(' && c == ')') {
                    st.pop();
                }
                else {
                    st.push(c);
                }
            }
            return st.size();
        }
    };
}

// 模板题4：o1为常规O(n^2)做法，o2为奇技淫巧了解即可，将多次反转转换成单次遍历
namespace s1190o1
{   // 从内到外反转，内部的字符串会被多次反转，时间复杂度O(n^2)
    class Solution {
    public:
        string reverseParentheses(string s) {
            string res;
            for (char c : s) {
                if (c != ')') {
                    res.push_back(c);
                }
                else {
                    string temp;
                    while (!res.empty() && res.back() != '(') {
                        temp.push_back(res.back());// 巧妙之处在于从res的尾部加入temp,相当于reverse
                        res.pop_back();
                    }
                    res.pop_back(); // pop出左半括号 (
                    res += temp;
                }
            }
            return res;
        }
    };
}
namespace s1190o2
{
    class Solution {
    public:
        string reverseParentheses(string s) {
            stack<int> stk;
            int n = s.size();
            vector<int> next(n);//i 会跳到其 next[i]的位置
            for (int i = 0; i < n; ++i) {
                if (s[i] == '(') {
                    stk.push(i);
                }
                else if (s[i] == ')') {
                    int j = stk.top();
                    stk.pop();
                    next[i] = j;
                    next[j] = i;
                }
            }
            string ans = "";
            int dir = 1;
            for (int i = 0; i < n; i += dir) {// 退出循环条件设为i < n即可，一定会在i == n时结束
                if (s[i] == '(' || s[i] == ')') {
                    i = next[i];
                    dir = -dir;//变换遍历方向
                }
                else {
                    ans += s[i];
                }
            }
            return ans;
        }
    };
}

// 模板题5：和s1209类似，栈不存储容器元素，而是存储答案相关的指标，比如这题中是分数
namespace s856o1
{   /*
    遇到 '('，压入一个 0，表示新开启一个层级。
    遇到 ')'，弹出栈顶元素。如果弹出的值是 0，代表当前的括号对是 ()，得 1 分；
    否则得分是弹出的值的两倍。然后将得分加到新的栈顶元素上。
    */
    class Solution {
    public:
        int scoreOfParentheses(string s) {
            stack<int> st;
            st.push(0);// 哨兵元素，既省去定义int ans = 0，也不需要进行特判
            for (char c : s) {
                if (c == '(') {
                    st.push(0);
                }
                else {
                    int tp = st.top();
                    st.pop();
                    st.top() += (tp == 0 ? 1 : tp * 2);
                }
            }
            return st.top();
        }
    };
}

// 和s1209类似，本题需要插入-1作为哨兵基准点，应对“()”这种情况，此时长度为1 - (-1) = 2，还有空间O(1)的版本
namespace s32o1
{   
    /* 
    栈可以不仅仅用来保存答案，也可以保存与答案有关的变量。

    栈中保存的是未配对的下标，那么从栈顶加一的位置到 i，就是已配对的连续括号，长度为 i 减去栈顶，更新答案的最大值。
    此时有两种情况需要特殊处理：
            1.更新时栈底元素即为最近的未配对的左括号，如s = "()"，此时更新答案需要stk.top()，可以
              初始化时在栈内加入-1作为哨兵，以此简化代码；
            2.遍历到右括号时，栈内除了哨兵节点外，不存在最近的未配对的左括号，那么此时不能更新答案，而是将
              哨兵更换为目前的右括号
    */
    class Solution {
    public:
        int longestValidParentheses(string s) {
            stack<int> stk;
            stk.push(-1);
            int ans = 0;

            for (int i = 0; i < s.size(); ++i) {
                if (s[i] == '(') {
                    stk.push(i);
                }
                else {
                    stk.pop();
                    if (stk.empty()) {// 基准点都没了
                        stk.push(i);// 将这个')'下标重新作为基准
                    }
                    else {
                        ans = max(ans, i - stk.top());
                    }
                }
            }
            return ans;
        }
    };
}
namespace s32o2
{   // o1是O(n)空间复杂度，但其实可以通过左右两遍扫描实现O(1)空间复杂度
    // 与s301删除无效的括号有点类似
    // 从左到右扫描时，left < right和left == right的情况都能正确判断， 但是left > right的情况不行
    // 比如 "()(()" 和 "((())" ，遍历到最后一个字符时，left = 3, right = 2，但一个有效长度是4，一个是2
    // 这个时候反过来，从右往左遍历，把')'当成是左括号，此时left和right的相对关系又变回了left < right，就不会漏掉答案了

    class Solution {
    public:
        int longestValidParentheses(string s) {
            int ans = 0;
            int left = 0, right = 0;

            for (char c : s) {
                c == '(' ? ++left : ++right;
                if (right > left) {
                    left = right = 0;
                }
                else if (left == right) {
                    ans = max(ans, 2 * right);
                }
            }

            int n = s.size();
            left = right = 0;
            for (int i = n - 1; i >= 0; --i) {
                s[i] == ')' ? ++left : ++right;
                if (right > left) {
                    left = right = 0;
                }
                else if (left == right) {
                    ans = max(ans, 2 * right);
                }
            }

            return ans;
        }
    };
}
// ---------------------
// 【3.5】表达式解析/求值(5)
/*
各种类型的表达式求值，跟前缀/中缀/后缀相关，基本都需要用到栈
1.没有括号的中缀表达式：遇到数字就入栈；
    遇遇到乘除计算栈顶两个元素和再入栈，遇到加号直接跳过，遇到减号将栈顶元素取反；
    最后将栈内元素累加即为答案；
2.后缀表达式（将运算符写在操作数之后）：遇到数字就入栈，遇到符号就取栈顶两个元素计算后将结果入栈；
*/
// ---------------------
/*
1006.笨阶乘：通常，正整数 n 的阶乘是所有小于或等于 n 的正整数的乘积。
例如，factorial(10) = 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1。
相反，我们设计了一个笨阶乘 clumsy：在整数的递减序列中，我们以一个固定顺序的操作符序列来依次替换原有的乘法操作符：
乘法(*)，除法(/)，加法(+)和减法(-)。例如，clumsy(10) = 10 * 9 / 8 + 7 - 6 * 5 / 4 + 3 - 2 * 1。
然而，这些运算仍然使用通常的算术运算顺序：我们在任何加、减步骤之前执行所有的乘法和除法步骤，
并且按从左到右处理乘法和除法步骤。另外，我们使用的除法是地板除法（floor division），
所以 10 * 9 / 8 等于 11。这保证结果是一个整数。实现上面定义的笨函数：给定一个整数 N，它返回 N 的笨阶乘。

150.逆波兰表达式求值：给你一个字符串数组 tokens ，表示一个根据 逆波兰表示法 表示的算术表达式。
请你计算该表达式。返回一个表示表达式值的整数。注意：有效的算符为 '+'、'-'、'*' 和 '/' 。
每个操作数（运算对象）都可以是一个整数或者另一个表达式。两个整数之间的除法总是 向零截断 。表达式中不含除零运算。
输入是一个根据逆波兰表示法表示的算术表达式。答案及所有中间计算结果可以用 32 位 整数表示。

394.字符串解码：给定一个经过编码的字符串，返回它解码后的字符串。
编码规则为: k[encoded_string]，表示其中方括号内部的 encoded_string 正好重复 k 次。注意 k 保证为正整数。
你可以认为输入字符串总是有效的；输入字符串中没有额外的空格，且输入的方括号总是符合格式要求的。
此外，你可以认为原始数据不包含数字，所有的数字只表示重复的次数 k ，例如不会出现像 3a 或 2[4] 的输入。
测试用例保证输出的长度不会超过 105。

224.基本计算器：给你一个字符串表达式 s ，请你实现一个基本计算器来计算并返回它的值。
注意:不允许使用任何将字符串作为数学表达式计算的内置函数，比如 eval() 。

227.基本计算器 II：给你一个字符串表达式 s ，请你实现一个基本计算器来计算并返回它的值。
整数除法仅保留整数部分。你可以假设给定的表达式总是有效的。所有中间结果将在 [-231, 231 - 1] 的范围内。
注意：不允许使用任何将字符串作为数学表达式计算的内置函数，比如 eval() 。
*/

// 模板题6：没有括号的中缀表达式求值
namespace s1006o1
{   // 乘除就与栈顶元素计算， 加减就入栈
    class Solution {
    public:
        int clumsy(int n) {
            stack<int> stk;
            stk.push(n--);
            int p = 0;
            while (n > 0) {
                switch (p % 4) {
                case 0:
                    stk.top() *= n;
                    break;
                case 1:
                    stk.top() /= n;
                    break;
                case 2:
                    stk.push(n);
                    break;
                case 3:
                    stk.push(-n);
                    break;
                }
                --n;
                ++p;
            }
            int ans = 0;
            while (!stk.empty()) {
                ans += stk.top();
                stk.pop();
            }
            return ans;
        }
    };
}

// 模板题7：后缀表达式（逆波兰表达式）求值，拓展为如何将中缀表达式转化为后缀表达式
namespace s150o1
{   // 逆波兰（后缀）序列式转中缀表达式，栈的应用之一
    // 遇到数字就push进栈，遇到操作符就将之前进栈的两个数字取出进行运算，并之后将运算结果入栈
    /* 逆波兰表达式主要有以下两个优点：
    去掉括号后表达式无歧义，上式即便写成 1 2 + 3 4 + * 也可以依据次序计算出正确结果。
    适合用栈操作运算：遇到数字则入栈；遇到算符则取出栈顶两个数字进行计算，并将结果压入栈中
    */
    class Solution {
    public:
        int evalRPN(vector<string>& tokens) {
            stack<int> st;
            for (const string& temp : tokens) {
                if (temp == "+" || temp == "-" || temp == "*" || temp == "/") {
                    int num2 = st.top();// 判定的是temp和"+"的比较结果，不是'+'
                    st.pop();
                    int num1 = st.top();
                    st.pop();
                    switch (temp[0]) {// 用switch语句的代码比四个if好一些
                    case '+':          // 要注意，switch语句只能判断整数，所以要曲temp[0]
                        st.push(num1 + num2);
                        break;
                    case '-':
                        st.push(num1 - num2);// 记得num1 operator num2的顺序，不要颠倒
                        break;              // 逆波兰序列中数字的使用顺序跟中序排列一样是从左到右
                    case '*':               // 所以先入栈的num1才是第一个操作数
                        st.push(num1 * num2);
                        break;
                    case '/':
                        st.push(num1 / num2);
                        break;
                    }
                }
                else {
                    st.push(stoi(temp));
                }
            }
            return st.top();
        }
    };
}
namespace s150m1
{   // 时隔数月后自己写出来的版本
    class Solution {
    public:
        int evalRPN(vector<string>& tokens) {
            stack<int> stk;
            for (const auto& s : tokens) {
                if (s == "+" || s == "-" || s == "*" || s == "/") {
                // 常见的判断字符串是否为数字（包含负数）的方法如下，没有上面的方法好，容易遗漏
                //  if(isdigit(s.back()) || (s[0] == '-' && s.size() > 1)) 
                    int num2 = stk.top();
                    stk.pop();
                    switch (s[0]) {
                    case '+':
                        stk.top() += num2;
                        break;
                    case '-':
                        stk.top() -= num2;
                        break;
                    case '*':
                        stk.top() *= num2;
                        break;
                    case '/':
                        stk.top() /= num2;
                        break;
                    }
                }
                else {
                    stk.push(stoi(s));
                }
            }
            return stk.top();
        }
    };
}
namespace s150extension
{   // 中缀表达式转后缀表达式
    // 还是属于stack的应用，但是比后缀转前缀的逻辑复杂许多
    // 计算机进行运算时，需要将我们的中缀转成后缀，然后使用s150中的方法得到最后的值
    // 以下做法的样例：9+(3-1)*3+10/2  ----> 931-3*+102
    // 注意，以下做法是按字符串来做的，只能处理0-9的四则运算
    // 我在加上额外条件“所有数字和运算符都单独使用一个string来表达，输入为vector<string> tokens”后，改写了一版
    /*
    *  a+b   ab+
    *  a*b-c  ab*c-
    *  a+b*c   abc*+
    * 1.遇到非字符串,直接拼串
    * 2.遇到运算符  判断优先级，
    *   如果优先级高，则直接入栈，
    *   否则，>= 将栈内元素出栈，拼接 再入栈
    * 左括号直接入栈, 右括号把遇到左括号前的所有符号都出栈
    * 遍历完成,栈里剩余运算符 依次出栈拼接
    */
    class Solution
    {
    private:
        int priority(char& c) {// 区分运算符优先级
            switch (c) {
            case '(':// 只有左括号 ( 可能在栈中保存一段时间，将其优先级设为最低，那么+-*/都将直接入栈
                return 0;
            case '+':
            case '-':
                return 1;
            case '*':
            case '/':
                return 2;
            default:
                return -1;
            }
        }
    public:
        string reverseRPN(vector<string> tokens) {
            string ans;
            stack<char> st;
            for (string& c : tokens) {
                if (c.size() > 1) {
                    ans += c;
                    ans += ' ';
                }
                else {
                    switch (c[0]) {
                    case '+':// 使用switch语句，快速合并运算符的四种可能性
                    case '-':
                    case '*':
                    case '/':
                        if (st.empty()) {//若栈为空，直接入栈，无需判断是什么运算符
                            st.push(c[0]);
                            break;
                        }
                        else if (priority(c[0]) > priority(st.top())) {//栈顶运算符优先级低
                            st.push(c[0]);                     // 可能的情况：*、/ 高于 ( 、+ 、-
                            break;                          // 可能的情况：+、 - 高于 (
                        }
                        else {
                            while (!st.empty() && priority(c[0]) <= priority(st.top())) {//栈顶运算符优先级高或相等
                                ans += st.top();            // 可能的情况：+、- 小于等于 + 、- 、 *、 /
                                st.pop();                   // 左括号处会得到保留，因为其优先级为0
                                ans += ' ';
                            }
                            st.push(c[0]);//通过前面对比,把优先级高或相等的先拼接,之后再把优先级低的运算符入栈
                            break;

                        }
                    case '(':
                        st.push(c[0]);
                        break;
                    case ')':
                        while (!st.empty() && st.top() != '(') {
                            ans += st.top();
                            st.pop();
                            ans += ' ';
                        }
                        st.pop();//左括号弹出
                        break;
                    default:
                        ans += c;
                        ans += ' ';
                        break;
                    }
                }
            }
            while (!st.empty()) {// 弹出剩余运算符
                ans += st.top();
                st.pop();
                ans += ' ';
            }
            return ans;
        }
    };
}

// 模板题8：跟s856有些类似，但也有所不同，s856最终分数可以保存在stack.top()上，但这题不行，直接看o4解法最简单易懂
namespace s394m1
{   // 自己写 + ai帮助写的垃圾做法，太多冗余操作了，还是看看o1o2吧
    class Solution {
    public:
        string decodeString(string s) {
            stack<int> numStk;
            stack<string> chStk;

            int num = 0;
            for (char c : s) {
                if (isdigit(c)) {
                    num = 10 * num + (c - '0');
                }
                else {
                    if (num > 0) {
                        numStk.push(num);
                        num = 0;
                    }
                    if (c != ']') {
                        chStk.push(string(1, c));
                    }
                    else {
                        stack<string> strs;
                        while (chStk.top() != "[") {
                            strs.push(chStk.top());
                            chStk.pop();
                        }
                        chStk.pop(); // pop '['
                        string temp = "";
                        while (!strs.empty()) {
                            temp += strs.top();
                            strs.pop();
                        }
                        int cnt = numStk.top();
                        numStk.pop();

                        string str = "";
                        for (int i = 0; i < cnt; ++i) {
                            str += temp;
                        }
                        chStk.push(str);
                    }
                }
            }
            string result = "";
            while (!chStk.empty()) {
                result = chStk.top() + result;// 放在chStk里的子串顺序是反的，如果一开始定义chStk为deque可以更快
                chStk.pop();
            }
            return result;
        }
    };
}
namespace s394o1
{   // o1和o2写法本质上是相同的，只是细节处略有不同
    class Solution {
    public:
        string decodeString(string s) {
            stack<string> strStk;
            stack<int> codeStk;
            // 也可以变成stack<pair<string, int>>反正两个栈的出栈入栈时机是相同的
            int num = 0;
            string cur = "";
            for (char c : s) {
                if (isdigit(c)) {
                    num = 10 * num + (c - '0');
                }
                else if (islower(c)) {
                    cur += c;
                }
                else if (c == '[') {// 当碰到'[’时将数字和之前保存到的字符串入栈
                    strStk.push(cur);// 用cur代表返回值，可以确保k1[..]k2[...]str中最后的str也能加入到答案cur中
                    // 也可以优化成strStk.push(move(cur));// 这样就不用加cur.clear()
                    codeStk.push(num);
                    num = 0;
                    cur.clear();
                }
                else {  // c == ']'
                    int k = codeStk.top();
                    codeStk.pop();
                    string prev = strStk.top();
                    strStk.pop();
                    string temp = "";
                    temp.reserve(k * cur.size());
                    while (k--) {
                        temp += cur;
                    }
                    cur = prev + temp;
                }
            }

            return cur;
        }
    };
}
namespace s394o2
{
    class Solution {
    public:
        string decodeString(string s) {
            stack<pair<string, int>> stk; // 用于模拟计算机的递归
            string res = "";
            int k = 0;

            for (char c : s) {
                if (isalpha(c)) {
                    res += c;
                    // res为当前拼接出的字符串，比如当s = "abc3[cd]xyz"时，最开始res = "abc"，
                    // 然后move之后res = "cd"，重复3次后加到prev，也即"abc"上，之后以此类推
                }
                else if (isdigit(c)) {
                    k = 10 * k + (c - '0');
                }
                else if (c == '[') {
                    // 模拟递归
                    // 在递归之前，把当前递归函数中的局部变量 res 和 k 保存到栈中
                    stk.emplace(move(res), k);
                    // stk.emplace(res, k);
                    // res.clear();
                    // 递归，初始化 res 和 k（由于 move 了，res 此时为空）
                    // move之后cur相当于空字符串，处于有效但未指定的状态
                    // 这里也可以换成stk.push_back({move(res), k});
                    // 但emplace的意图更明显，明确表示“原地构造”，在C++17之后两者等价
                    // 但在更旧的标准下, push_back的版本可能生成多余的临时pair
                    k = 0;
                }
                else { // ']'
                    // 递归结束，从栈中恢复递归之前保存的局部变量
                    // 这里不能用引用auto&，因为之后将栈顶元素对弹出销毁了，后面的行为都是未定义的
                    auto [pre_res, pre_k] = stk.top();
                    stk.pop();
                    // 此时 res 是下层递归的返回值，将其重复 pre_k 次，拼接到递归前的 pre_res 之后
                    while (pre_k--) {
                        pre_res += res;
                    }
                    res = move(pre_res);
                    /* 相当于
                    string temp = "";
                    while (k--) {
                        temp += res;
                    }
                    prev_res += temp;
                    res = move(pre_res);
                    */
                }
            }
            return res;
        }
    };
}
namespace s394o3
{   // o2为o1思路的递归写法
    class Solution {
    private:
        int i;  // 需要在成员函数间共享的index

        string decode(string& s) {
            string res;
            int k = 0;
            while (i < s.size()) {
                char c = s[i];
                i++;
                if (isalpha(c)) {
                    res += c;
                }
                else if (isdigit(c)) {
                    k = k * 10 + (c - '0');
                }
                else if (c == '[') { // 进入递归调用：递
                    string t = decode(s);
                    for (; k > 0; k--) {
                        res += t; // 把括号内的字符串重复k次
                    }
                }
                else { // ']' 返回：归
                    break;
                }
            }
            return res;
        }

    public:
        string decodeString(string s) {
            i = 0;  // 初始化index
            return decode(s);
        }
    };

}   
namespace s394o4
{   // 思路最容易懂的解法，核心在于"["和"]"的匹配
    class Solution {
    public:
        string decodeString(string s) {
            stack<string> stk;

            for (char ch : s) {
                // 不是']'就将字符转为字符串后入栈
                if (ch != ']') {
                    stk.emplace(1, ch);
                    continue;
                }

                // 解码步骤1：提取本段字符串
                string sub = "";
                while (stk.top() != "[") {
                    sub = stk.top() + sub;  // 注意拼接顺序
                    stk.pop();
                }
                stk.pop();  // 移除"["

                // 解码步骤2：提取本段数字
                int k = 0, base = 1;
                while (!stk.empty() && isdigit(stk.top()[0])) {
                    k += (stk.top()[0] - '0') * base;
                    base *= 10;
                    stk.pop();
                }

                // 本段解码后再次入栈
                string repeated = "";
                for (int i = 0; i < k; i++) {
                    repeated += sub;
                }
                stk.push(move(repeated));
            }

            // 将栈中所有元素按顺序拼接
            string result = "";
            while (!stk.empty()) {
                result = stk.top() + result; // 注意拼接顺序
                stk.pop();
            }
            return result;
        }
    };
}

// 模板题9：有括号（优先级）的中缀表达式（仅限+/-），需要注意-x可以作为一元运算符，还有空格
// 巧妙之处：将'+'和'-'符号对数字的影响变成当前符号sign，搭配result = 0解决一元运算的问题
namespace s224o1
{   // 用int来表达stk、curNum、result可能导致中间值溢出，需用long long
    class Solution {
    public:
        int calculate(string s) {
            stack<long long> stk;
            long long curNum = 0; // 记录当前遍历到的数字
            long long result = 0; // 当前计算结果（必须要预设为0，解决诸如((a + b) + c)或-(a + b)的问题
            int sign = 1;   // 当前符号，1为正，-1为负

            for (char c : s) {
                if (isdigit(c)) {
                    curNum = curNum * 10 + (c - '0');
                }
                else if (c == '+') {
                    result += sign * curNum;// 把之前的数字加到结果
                    curNum = 0;             // 重置数字
                    sign = 1;               // 设置遇到的下个数字符号为正
                }
                else if (c == '-') {
                    result += sign * curNum;// 把之前的数字加到结果
                    curNum = 0;             // 重置数字
                    sign = -1;              // 设置遇到的下个数字符号为负
                }
                else if (c == '(') {
                    // 遇到左括号，把当前result和sign压栈，也即：保存左括号前的运算结果
                    stk.push(result);
                    stk.push(sign);
                    result = 0;             // 重置计算状态，准备计算括号内的表达式
                    sign = 1;               // curNum不用重置，因为'('前如果出现过运算，必定跟着加减符，已经重置过了
                }
                else if (c == ')') {
                    result += sign * curNum;// 先计算括号内的最后一个数字
                    curNum = 0;
                    // 此时栈顶是外层的sign，次顶是外层的result
                    sign = stk.top(); stk.pop();   // 取出外层符号
                    result *= sign;                // 括号整体乘外层符号（处理 "-(...)"）
                    result += stk.top(); stk.pop();// 合并外层 result
                }// 其他情况（如空格）直接跳过
            }

            result += sign * curNum;// 别忘了处理最后一个数字
            return static_cast<int>(result);
        }
    };
}

// 模板题10：没有括号的中缀表达式（+-*/），和s1006是类似的，但更难
// 拓展为包含括号的中缀表达式（+-*/）,代码基本和本题o1相同，但是加上了递归的进入和退出
namespace s227o1
{
    class Solution {
    public:
        int calculate(string s) {
            stack<int> stk;
            int curNum = 0;
            int n = s.size();
            char sign = '+';
            for (int i = 0; i < n; ++i) {
                char c = s[i];
                if (isdigit(c)) {
                    curNum = curNum * 10 + (c - '0');
                }// 这里更新数字的if不和下面的连成if-else是为了处理边界i == n - 1的情况
                // 一般来说只有当前c为运算符号时，才会将之前的结果根据sign处理（前一个运算符号）
                // 当i == n - 1时，要么是空格，要么是数字，所以需要将if隔开
                if (c == '+' || c == '-' || c == '*' || c == '/' || i == n - 1) {
                    if (sign == '+') {
                        stk.push(curNum);
                    }
                    else if (sign == '-') {
                        stk.push(-curNum);
                    }
                    else if (sign == '*') {
                        stk.top() *= curNum;
                    }
                    else if (sign == '/') {
                        stk.top() /= curNum;
                    }
                    curNum = 0;
                    sign = c;// 重点：将前一个符号记录下来
                }
            }
            int ans = 0;
            while (!stk.empty()) {
                ans += stk.top();
                stk.pop();
            }
            return ans;
        }
    };
}
namespace s227extension
{   // 使用递归
    int calculate(string& s, int& i) {  // i 是当前解析位置，要用引用，共用一份副本，或者将i设为成员变量
        stack<int> st;
        int num = 0;
        char sign = '+';

        for (; i < s.size(); ++i) {
            char ch = s[i];
            if (isdigit(ch)) {
                num = num * 10 + (ch - '0');
            }
            // 可以用例子 num * (....)为例进行思考，此处num计算出括号内的值
            if (ch == '(') {// 和s394o2类似，在左半括号处进入下一层递归
                num = calculate(s, ++i); // 递归计算括号内
                i++; // 跳过 ')'
            }
            // 与if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || i == n - 1) 等价
            if ((!isdigit(ch) && ch != ' ') || i == s.size() - 1) {
                if (sign == '+') st.push(num);
                else if (sign == '-') st.push(-num);
                else if (sign == '*') st.top() *= num;
                else if (sign == '/') st.top() /= num;
                sign = ch;
                num = 0;
            }

            if (ch == ')') break; // 返回上一层递归，在右半括号处退出递归
        }

        int res = 0;
        while (!st.empty()) {
            res += st.top();
            st.pop();
        }
        return res;
    }
}
// ---------------------
// 【3.5.1】单调栈-基础/进阶（6）
// 单调栈是什么？栈内元素的大小具备单调性，如果更新答案时需要计算上一个/下一个更大/更小的元素，可以考虑单调栈
// 体会从左至右/从右至左两种遍历方式的区别，还有注意往往stack存储的是下标（因为可以随时用下标获取数据）
// 我个人更喜欢从左往右遍历，比较符合直觉，易于理解
/*
739.每日温度：给定一个整数数组 temperatures ，表示每天的温度，返回一个数组 answer ，其中 answer[i] 是指对于第 i 天，
下一个更高温度出现在几天后。如果气温在这之后都不会升高，请在该位置用 0 来代替。
*/
// ---------------------
// 模板题11：初识单调栈，及时去掉无用元素，保持栈内元素单调性
namespace s739m1
{   // 从右至左遍历，m1为自己看了思路后写出来的
    class Solution {
    public:
        vector<int> dailyTemperatures(vector<int>& temperatures) {
            int n = temperatures.size();
            stack<pair<int, int>> stk;// 用pair储存温度和下标对，但其实没有必要，只需要储存下标，可以随时用下标获取温度
            vector<int> ans(n);
            for (int i = n - 1; i >= 0; --i) {
                int t = temperatures[i];
                while (!stk.empty() && t >= stk.top().first) {
                    stk.pop();
                }
                if (stk.empty()) {
                    ans[i] = 0;// 这里也可以省去，默认值就是0，不赋值就行了
                }
                else {
                    ans[i] = stk.top().second - i;
                }
                stk.emplace(t, i);
            }
            return ans;
        }
    };
}
namespace s739o1
{   //m1的优化版，栈内只存储下标
    // 从右往左：for循环内部每次都更新答案，每次更新答案前通过while + pop满足更新答案的条件
    class Solution {
    public:
        vector<int> dailyTemperatures(vector<int>& temperatures) {
            int n = temperatures.size();
            stack<int> stk;
            vector<int> ans(n);

            for (int i = n - 1; i >= 0; --i) {
                int t = temperatures[i];
                while (!stk.empty() && t >= temperatures[stk.top()]) {
                    stk.pop();
                }
                if (!stk.empty()) {
                    ans[i] = stk.top() - i;
                }
                stk.push(i);
            }
            return ans;
        }
    };
}
namespace s739o2
{   // 从左到右遍历，每次只在while内部更新答案，一次更新一批
    class Solution {
    public:
        vector<int> dailyTemperatures(vector<int>& temperatures) {
            int n = temperatures.size();
            stack<int> stk;
            vector<int> ans(n);

            for (int i = 0; i < n; ++i) {
                int t = temperatures[i];
                while (!stk.empty() && t > temperatures[stk.top()]) {
                    ans[stk.top()] = i - stk.top();
                    stk.pop();
                }
                stk.push(i);
            }
            return ans;
        }
    };
}
   
// 用来进一步熟悉单调栈
namespace s1475m1
{   // 从左到右遍历
    class Solution {
    public:
        vector<int> finalPrices(vector<int>& prices) {
            stack<int> stk;
            int n = prices.size();
            for (int i = 0; i < n; ++i) {
                int p = prices[i];
                while (!stk.empty() && p <= prices[stk.top()]) {
                    prices[stk.top()] -= p;
                    stk.pop();
                }
                stk.push(i);
            }
            return prices;
        }
    };
}
namespace s1475m2
{   // 从右往左遍历
    class Solution {
    public:
        vector<int> finalPrices(vector<int>& prices) {
            stack<int> stk;
            int n = prices.size();
            vector<int> ans = prices;
            for (int i = n - 1; i >= 0; --i) {
                int p = prices[i];
                while (!stk.empty() && p < prices[stk.top()]) {
                    stk.pop();
                }
                if (!stk.empty()) {
                    ans[i] -= prices[stk.top()];
                }
                stk.push(i);
            }
            return ans;
        }
    };
}

// 模板题12：稍微复杂一点点的单调栈，和s503结合对照理解
namespace s496m1
{   // 从左往右遍历
    class Solution {
    public:
        vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
            int n1 = nums1.size();
            int n2 = nums2.size();
            unordered_map<int, int> idx;
            for (int i = 0; i < n1; ++i) {
                idx[nums1[i]] = i;
            }
            stack<int> stk;
            vector<int> ans(n1, -1);
            for (int i = 0; i < n2; ++i) {
                int num = nums2[i];
                while (!stk.empty() && num > stk.top()) {
                    if (idx.count(stk.top())) {
                        ans[idx[stk.top()]] = num;
                    }
                    stk.pop();
                }
                stk.push(num);
            }
            return ans;
        }
    };
}
namespace s496o1
{   // 重点是先用哈希表idx记录num1中元素与对应下标，然后从右往左遍历num2即可，标准单调栈应用
    // 从右往左遍历，栈中记录下一个更大元素的「候选项」
    class Solution {
    public:
        vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
            // 各自内部没有重复元素，num1是nums2的子集
            int n1 = nums1.size();
            int n2 = nums2.size();
            unordered_map<int, int> idx;
            for (int i = 0; i < n1; ++i) {
                idx[nums1[i]] = i;
            }
            stack<int> stk;
            vector<int> ans(n1, -1);
            for (int i = n2 - 1; i >= 0; --i) {
                int num = nums2[i];
                while (!stk.empty() && num > stk.top()) {
                    stk.pop();
                }
                if (idx.count(num) && !stk.empty()) {
                    ans[idx[num]] = stk.top();
                }
                stk.push(num);
            }
            return ans;
        }
    };
}

// 循环数组 + 单调栈，可以将数组复制一份拼接在尾端（用取模%就行，不用真的复制），就变成了简单的单调栈
namespace s503m1
{   // 从左往右遍历，栈中记录还没算出「下一个更大元素」的那些数的下标
    class Solution {
    public:
        vector<int> nextGreaterElements(vector<int>& nums) {
            int n = nums.size();
            int m = 2 * n;
            vector<int> ans(n, -1);
            stack<int> stk;
            for (int i = 0; i < m; ++i) {
                int num = nums[i % n];
                while (!stk.empty() && num > nums[stk.top()]) {
                    ans[stk.top()] = num;
                    stk.pop();
                }
                if (i < n) {
                    stk.push(i);// 不加if i < n也正确，但是不必要的while循环会多很多
                }                        // 只要第一轮遍历中还在栈里的下标都是没找到下一个最大值的，不用再遍历n - m
            }
            return ans;
        }
    };
}
namespace s503m2
{ // 从右往左遍历，栈中记录下一个更大元素的「候选项」（这里存下标也没问题，但没必要）
    class Solution {
    public:
        vector<int> nextGreaterElements(vector<int>& nums) {
            int n = nums.size();
            int m = 2 * n;
            vector<int> ans(n, -1);
            stack<int> stk;
            for (int i = m - 1; i >= 0; --i) {
                int num = nums[i % n];
                while (!stk.empty() && num >= stk.top()) {
                    stk.pop();
                }
                if (!stk.empty() && i < n) {// 注意这里是i < n时才更新答案
                    ans[i] = stk.top();
                }
                stk.push(num);
            }
            return ans;
        }
    };
}
// ---------------------
// 【3.5.2】单调栈-矩形（3）
/*
84.柱状图中最大的矩形：给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为 1 。
求在该柱状图中，能够勾勒出来的矩形的最大面积。
*/
// ---------------------
// 模板题13：单调栈问题的集大成，考察分析与转化，遍历每个高度，获取每个高度左边和右边的最近更小值下标
// 保证能做出m3，能写出o2算超常发挥，o3不要求，太复杂太灵活和模板不搭
namespace s84m1
{   // 最大矩形的高度一定是heights中的元素
    // 看完思路后自己写的版本，写完后是感觉至少prefix和suffix两个数组的计算是可以放在一个遍历下的，还能优化
    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            stack<int> lstk;
            stack<int> rstk;// 首先，prefix和suffix数组可以共用同一个stack，可以节省空间
            int n = heights.size();
            vector<int> prefix(n, -1);// prefix和suffix这两个数组的名字取得不是很贴切（但暂时不管了
            vector<int> suffix(n, -1);// 将suffix初始化为n后，更新答案时就不需要特别判断了
            int ans = 0;
            for (int i = n - 1; i >= 0; --i) {
                int h = heights[i];
                while (!lstk.empty() && h < heights[lstk.top()]) {
                    prefix[lstk.top()] = i;
                    lstk.pop();
                }
                lstk.push(i);
            }
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                while (!rstk.empty() && h < heights[rstk.top()]) {
                    suffix[rstk.top()] = i;
                    rstk.pop();
                }
                rstk.push(i);
            }
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                int width = 1;
                // 如果suffix初始化为n，那么下面这些if-eles判断全部可以合并
                if (suffix[i] == -1 && prefix[i] == -1) {
                    width = n;
                }
                else if (suffix[i] == -1) {
                    width = n - 1 - prefix[i];
                }
                else if (prefix[i] == -1) {
                    width = suffix[i];
                }
                else {
                    width = suffix[i] - prefix[i] - 1;
                }
                ans = max(ans, h * width);
            }
            return ans;
        }
    };
}
namespace s84m2
{   // 基于m1优化后的三次遍历写法，也是最容易理解的三次遍历版本
    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            stack<int> stk;
            int n = heights.size();
            // -1和n的由来可以从prefix和suffix的定义来看，比如：
            // prefix代表i下标处，左边最近的，小于h的下标
            // 在计算面积时只要将prefix[i] + 1就可以得到当前计算矩形的最左边下标
            // 代入实际情况，极端时最左边下标应该为0，也即左边不存在最近的，小于h的下标
            // 反推可得prefix初始值应该为-1，之后+1就会变成0，suffix初始化为n同理
            vector<int> prefix(n, -1);
            vector<int> suffix(n, n);
            int ans = 0;
            for (int i = n - 1; i >= 0; --i) {
                int h = heights[i];
                while (!stk.empty() && h < heights[stk.top()]) {
                    prefix[stk.top()] = i;
                    stk.pop();
                }
                stk.push(i);
            }
            stk = stack<int>();// 快速清空栈内元素
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                while (!stk.empty() && h < heights[stk.top()]) {
                    suffix[stk.top()] = i;
                    stk.pop();
                }
                stk.push(i);
            }
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                int width = suffix[i] - prefix[i] - 1;
                ans = max(ans, h * width);
            }
            return ans;
        }
    };
}
namespace s84m3
{   // 虽然是写在同一个循环内部，但是其实本质上还是三次遍历... 不如m2
    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            stack<int> lstk;
            stack<int> rstk;
            int n = heights.size();
            vector<int> prefix(n, -1);
            vector<int> suffix(n, n);
            int ans = 0;
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                while (!lstk.empty() && h <= heights[lstk.top()]) {
                    lstk.pop();
                }
                if (!lstk.empty()) {
                    prefix[i] = lstk.top();
                }
                lstk.push(i);
                while (!rstk.empty() && h < heights[rstk.top()]) {
                    suffix[rstk.top()] = i;
                    rstk.pop();
                }
                rstk.push(i);

            }
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                int width = suffix[i] - prefix[i] - 1;
                ans = max(ans, h * width);
            }
            return ans;
        }
    };
}
namespace s84o2
{   // 两次遍历写法，属于高阶技巧了...
    // 将right(也就是前面的suffix）的定义改为：在i右侧的小于或等于h的最近元素下标
    // 比如[1,3,4,3,2]中的第一个3虽然面积算不对，但是右边那个3面积是正确的，如果没有重复的高度，那么改变定义更没关系
    // 巧妙之处在于将向左遍历/向右遍历两种写法的判断条件变成了统一的
    // 详细的推导过程见o2extention
    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            int n = heights.size();
            vector<int> left(n, -1);// 在i左侧的小于h的最近元素下标
            vector<int> right(n, n);// 在i右侧的小于或等于h的最近元素下标
            stack<int> stk;
            for (int i = 0; i < n; i++) {
                int h = heights[i];
                while (!stk.empty() && h <= heights[stk.top()]) {
                    right[stk.top()] = i;
                    stk.pop();
                }// left和right数组更新完全不冲突
                if (!stk.empty()) {
                    left[i] = stk.top();
                }
                stk.push(i);
            }
            int ans = 0;
            for (int i = 0; i < n; i++) {
                int h = heights[i];
                int len = right[i] - left[i] - 1;
                ans = max(ans, h * len);
            }
            return ans;
        }
    };
}
namespace s84o2extention {
    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            int n = heights.size();
            stack<int> stk;

            // left数组：储存下标i位置的左边第一个小于heights[i]的元素的下标
            vector<int> left(n, -1);
            // 遍历1：从左到右遍历生成left，stk内存储单调增的元素
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                while (!stk.empty() && h <= heights[stk.top()]) {
                    stk.pop();
                }
                if (!stk.empty()) {
                    left[i] = stk.top();

                }
                stk.push(i);
            }
            // 遍历2：从右到左遍历生成left，stk内存储单调非减的元素
            for (int i = n - 1; i >= 0; --i) {
                int h = heights[i];
                while (!stk.empty() && h < heights[stk.top()]) {
                    left[stk.top()] = i;
                    stk.pop();
                }
                stk.push(i);
            }

            stk = stack<int>();
            // right数组：储存下标i位置的右边第一个小于heights[i]的元素的下标
            vector<int> right(n, n);
            // 遍历3：从左到右遍历生成right，stk内存储单调非减的元素
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                while (!stk.empty() && h < heights[stk.top()]) {
                    right[stk.top()] = i;
                    stk.pop();
                }
                stk.push(i);
            }
            // 遍历4：从右到左遍历生成right，stk内存储单调增的元素
            for (int i = n - 1; i >= 0; --i) {
                int h = heights[i];
                while (!stk.empty() && h <= heights[stk.top()]) {
                    stk.pop();
                }
                if (!stk.empty()) {
                    right[i] = stk.top();
                }
                stk.push(i);
            }

            // 一次遍历生成left和right的条件：
            // 1.需要满足栈内元素单调性的一致（也即栈内元素可以共用）
            // 2.遍历时while的循环条件要相同
            // 3.遍历左右顺序需要一致
            // 可以得出，原始定义下的left，right数组是无法合并的，比如遍历1和遍历3的栈内元素单调性不同
            // 因此需要改变right或left数组的定义

            // 假如想通过一次从左到右的遍历同时生成left和right数组
            // 由上述代码可知生成left的代码（遍历1）的循环判断条件和stk单调性更严格
            // 因此想要修改定义，最好还是修改right的定义，将其遍历代码改为更严格的<=和单调增
            // 尝试把right数组的定义修改成：在i右侧的小于或等于heights[i]的最近元素的下标
            // 此时如果出现重复的元素，如heights = [1,3,4,3,2]，左边的3对应的right[1]变小的，但右边的3对应的right[3]不变
            // 但我们是通过遍历每个高度来更新答案的，既然两个高度都是3，那么“只要有一个”3能正确更新答案即可

            // 那么此时从左到右遍历生成right数组的代码如下：
            // 遍历5：从左到右遍历生成right，stk内存储单调增的元素
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                while (!stk.empty() && h <= heights[stk.top()]) {
                    right[stk.top()] = i;
                    stk.pop();
                }
                stk.push(i);
            }
            /*  再看遍历1，两者遍历顺序一致，栈内元素单调性一致（也即可以共用），循环条件相同，进行合并得到遍历6
            // 遍历1：从左到右遍历生成left，stk内存储单调增的元素
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                while (!stk.empty() && h <= heights[stk.top()]) {
                    stk.pop();
                }
                if (!stk.empty()) {
                    left[i] = stk.top();

                }
                stk.push(i);
            }
            */
            // 遍历6：从左到右遍历生成left和right，stk内存储单调增的元素
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                while (!stk.empty() && h <= heights[stk.top()]) {
                    right[stk.top()] = i;
                    stk.pop();
                }
                if (!stk.empty()) {
                    left[i] = stk.top();

                }
                stk.push(i);
            }
            // 若想通过一次从右到左的遍历同时生成left和right数组
            // 同理，可将left的定义改为：在i左侧的小于或等于heights[i]的最近元素的下标，此时代码如下：
            // 遍历7：从右到左遍历生成left，stk内存储单调增的元素
            for (int i = n - 1; i >= 0; --i) {
                int h = heights[i];
                while (!stk.empty() && h <= heights[stk.top()]) {
                    left[stk.top()] = i;
                    stk.pop();
                }
                stk.push(i);
            }
            // 与遍历4合并后的代码如下：
            // 遍历8：从右到左遍历生成left和right，stk内存储单调增的元素
            for (int i = n - 1; i >= 0; --i) {
                int h = heights[i];
                while (!stk.empty() && h <= heights[stk.top()]) {
                    left[stk.top()] = i;
                    stk.pop();
                }
                if (!stk.empty()) {
                    right[i] = stk.top();
                }
                stk.push(i);
            }
        }
    };
}
namespace s84o3
{   // 一次遍历，配合哨兵实现每次for循环内都更新left和right
    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            heights.push_back(-1); 
            // 最后大火收汁，用 -1 把栈清空，这里用0也行，只要<=高度数组的最小值
            // 循环结束的时候，栈中可能还有数据，可以再写一个循环单独处理，但更简单的办法是加哨兵
            // 哨兵必须恒定满足heights[st.top()] >= heights[i]，也即<=高度数组的最小值
            // 这样遍历到哨兵时，一定能清空掉栈内所有元素

            stack<int> st;
            st.push(-1); 
            // 在栈中只有一个数的时候，栈顶的「下面那个数」是 -1，对应 left[i] = -1 的情况
            // 这样做可以简化代码，不用判断if (!stk.empty())
            // 这个技巧没有什么可迁移性，只对这道题有用

            // 注：如果提前用int n = height.size()保存了长度，push_back(-1)之后，下边遍历要遍历到n，而不是n - 1
            // 可以在push_back(-1)之后取int n，也可以改成right <= n

            int ans = 0;
            for (int right = 0; right < heights.size(); right++) {
                int h = heights[right];
                while (st.size() > 1 && heights[st.top()] >= h) {
                    int i = st.top(); // 矩形的高（的下标）
                    st.pop();
                    int left = st.top(); // 栈顶下面那个数就是 left
                    ans = max(ans, heights[i] * (right - left - 1));
                }
                st.push(right);
            }
            return ans;
        }
    };
}

// 84的变体进阶，需要脑洞，进行m次柱状图s84运算，动态改变高度数组
namespace s85o1
{
    class Solution {
    private:
        int largestRectangle(vector<int>& heights) {
            stack<int> stk;
            int n = heights.size();
            vector<int> left(n, -1);
            vector<int> right(n, n);// 右边的小于等于当前高度的柱子的下标
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                while (!stk.empty() && h <= heights[stk.top()]) {
                    right[stk.top()] = i;
                    stk.pop();
                }
                if (!stk.empty()) {
                    left[i] = stk.top();
                }
                stk.push(i);
            }
            int ans = 0;
            for (int i = 0; i < n; ++i) {
                int h = heights[i];
                int len = right[i] - left[i] - 1;
                ans = max(ans, h * len);
            }
            return ans;
        }
    public:
        int maximalRectangle(vector<vector<char>>& matrix) {
            // 枚举矩阵的每一行，将枚举到的行上方当成s84来做，总共做m次84题
            // 如何算每行中的每列的矩形高度？
            // 如果 matrix[i][j]=0，那么没有柱子，高度等于0，否则，在上一行柱子基础上把柱子高度加1
            int m = matrix.size(), n = matrix[0].size();
            vector<int> heights(n);
            int ans = 0;
            for (int i = 0; i < m; ++i) {
                vector<char>& row = matrix[i];
                for (int j = 0; j < n; ++j) {
                    if (row[j] == '0') {
                        heights[j] = 0;
                    }
                    else {
                        heights[j] += 1;
                    }
                }
                ans = max(ans, largestRectangle(heights));
            }
            return ans;
        }
    };
}
namespace s85o2
{   // 如果用单次遍历的s84版本，那么heights的大小要改成n + 1，其他不变
    class Solution {
    private:
        int largestRectangle(vector<int>& heights) {
            stack<int> st;
            st.push(-1); // 在栈中只有一个数的时候，栈顶的「下面那个数」是 -1，对应 left[i] = -1 的情况
            int ans = 0;
            for (int right = 0; right < heights.size(); right++) {
                int h = heights[right];
                while (st.size() > 1 && heights[st.top()] >= h) {
                    int i = st.top(); // 矩形的高（的下标）
                    st.pop();
                    int left = st.top(); // 栈顶下面那个数就是 left
                    ans = max(ans, heights[i] * (right - left - 1));
                }
                st.push(right);
            }
            return ans;
        }

    public:
        int maximalRectangle(vector<vector<char>>& matrix) {
            int n = matrix[0].size();
            int ans = 0;
            vector<int> heights(n + 1, 0);

            for (auto& row : matrix) {
                for (int j = 0; j < n; ++j) {
                    if (row[j] == '0') {
                        heights[j] = 0;
                    }
                    else {
                        ++heights[j];
                    }
                }
                ans = max(ans, largestRectangle(heights));
            }

            return ans;
        }
    };
}

// 横着做，把每个柱子当作一个水桶，逐渐填坑（不要求掌握，仅开拓视野，竖着做的前后缀数组或是双指针做法已经足矣）
namespace s42o3
{
    class Solution {
    public:
        int trap(vector<int>& height) {
            int ans = 0;
            stack<int> st;
            for (int i = 0; i < height.size(); i++) {
                int h = height[i];
                while (!st.empty() && height[st.top()] <= h) {
                    int bottom_h = height[st.top()];
                    st.pop();
                    if (st.empty()) {
                        break;
                    }
                    int left = st.top();
                    int dh = min(height[left], height[i]) - bottom_h; // 面积的高
                    ans += dh * (i - left - 1);
                }
                st.push(i);
            }
            return ans;
        }
    };
}

// ---------------------
// 【3.5.3】单调栈-贡献法（0）(907暂时没做）
/*

*/
// ---------------------
namespace s907
{

}
// ---------------------
// 【3.5.4】单调栈-最小字典序（4）
//四合一https://leetcode.cn/problems/remove-k-digits/solutions/290203/yi-zhao-chi-bian-li-kou-si-dao-ti-ma-ma-zai-ye-b-5/
/*
402.移掉 K 位数字：给你一个以字符串表示的非负整数 num 和一个整数 k ，
移除这个数中的 k 位数字，使得剩下的数字最小。请你以字符串形式返回这个最小的数字。
*/
// ---------------------
// 模板题14：移除xx元素后，获得最小/大字典序排列的字符串模板
namespace s402o1
{
    class Solution {
    public:
        string removeKdigits(string num, int k) {
            // 移除之后可能存在前导0，输出中要去除
            // 可以全部移空，最少要输出一个0
            // 字符串非负，除了 0 本身之外，num 不含任何前导零
            vector<char> stk;
            int remain = num.size() - k;
            for (char c : num) {
                while (k > 0 && !stk.empty() && c < stk.back()) {
                    stk.pop_back();// 从左到右遍历，维持栈内数字单调非减（前提是k > 0)
                    --k;
                }
                stk.push_back(c);
            }
            string ans(stk.begin(), stk.begin() + remain);
            ans.erase(0, ans.find_first_not_of('0'));// 移除前导0的方法值得借鉴
            return ans.empty() ? "0" : ans;
        }
    };
}

// 与s402不同，没有全局的k值，而是变成每个字符出现次数-1, 先用哈希表遍历一次生成字典即可，很好解决
namespace s316o1
{
    class Solution {
    public:
        string removeDuplicateLetters(string s) {
            string stk;
            int count[26]{};
            bool inStack[26]{};

            // 统计每个字符出现的次数
            for (char c : s) {
                ++count[c - 'a'];
            }

            for (char c : s) {
                // 减少当前字符的剩余计数，这步是关键，从左到右遍历，已经遍历过的元素不能再用，都需要--count
                --count[c - 'a'];

                // 如果已经在栈中，跳过 
                if (inStack[c - 'a']) continue;
                // if (stk.find(c) != string::npos) continue; // 也可用find查询，省掉inStack数组

                // 当栈不空，当前字符小于栈顶字符，且栈顶字符后面还会出现
                // 就可以安全地弹出栈顶字符
                while (!stk.empty() && count[stk.back() - 'a'] > 0 && c < stk.back()) {
                    inStack[stk.back() - 'a'] = false;
                    stk.pop_back();
                }

                // 当前字符入栈
                stk.push_back(c);
                inStack[c - 'a'] = true;
            }
            return stk;
        }
    };
}
// 同s316
namespace s1081m1
{
    class Solution {
    public:
        string smallestSubsequence(string s) {
            string stk;
            int count[26]{};
            for (char c : s) {
                ++count[c - 'a'];
            }
            for (char c : s) {
                --count[c - 'a'];
                if (stk.find(c) != string::npos) continue;
                while (!stk.empty() && c < stk.back() && count[stk.back() - 'a'] > 0) {
                    stk.pop_back();
                }
                stk.push_back(c);
            }
            return stk;
        }
    };
}

// s402的升级版，单个数组的最大最小变成了两个数组的最大最小，采用分而治之的思路求解
// 对一个数组取i个，另一个数组取k-i个，然后合并出最大数列，同时每次更新答案还需要比较两个数组大小，整体难度较高
// 如果面试出了这道题，说明面试官对你不满意
namespace s321o1
{   // 为什么merge时不能只简单的比较单个当前元素而是要比较字典序？
    // num1 = [6,7], nums2 = [6, 0, 4]，只单个比较num1[i] > nums2[j]判断会得到[6,6,7,0,4]
    // 而通过字典序判断可以得到正确答案[6,7,6,0,4]
    // 同时比较字典序的compare也在更新最终答案时可以进行复用
    class Solution {
    private:
        // 从一个数组中选择k个数字构成最大子序列
        vector<int> pickMax(vector<int>& nums, int k) {
            vector<int> stk;
            if (k == 0) return stk;

            int drop = nums.size() - k;// 去除的元素数量
            for (int num : nums) {
                while (!stk.empty() && drop > 0 && stk.back() < num) {
                    stk.pop_back();
                    drop--;
                }
                stk.push_back(num);
            }
            // 可能数字个数超过k，所以截取前k个
            stk.resize(k);
            return stk;
        }

        // 合并两个子序列形成最大序列
        vector<int> merge(vector<int>& nums1, vector<int>& nums2) {
            int m = nums1.size(), n = nums2.size();
            vector<int> res;
            res.reserve(m + n);
            int i = 0, j = 0;
            while (i < m || j < n) {
                if (compare(nums1, i, nums2, j)) {
                    res.push_back(nums1[i++]);
                }
                else {
                    res.push_back(nums2[j++]);
                }
            }
            return res;
        }

        // 比较两个序列从指定位置开始的字典序
        bool compare(vector<int>& nums1, int i, vector<int>& nums2, int j) {
            // 当两个数组都还有剩余元素时
            while (i < nums1.size() && j < nums2.size()) {
                if (nums1[i] != nums2[j]) {
                    return nums1[i] > nums2[j];// 发现不同元素，直接比较大小
                }
                // 相同则继续比较下一个
                i++;
                j++;
            }
            // 如果前面比较都相同，看哪个数组还有剩余元素
            return (nums1.size() - i) > (nums2.size() - j);
        }

    public:
        vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
            vector<int> res;
            // 遍历所有可能的分配方式
            for (int i = 0; i <= k; ++i) {
                if (i > nums1.size() || k - i > nums2.size()) continue;
                // 从nums1取i个数字构成最大子序列
                vector<int> sub1 = pickMax(nums1, i);
                // 从nums2取k-i个数字构成最大子序列
                vector<int> sub2 = pickMax(nums2, k - i);
                // 合并两个子序列
                vector<int> merged = merge(sub1, sub2);
                // 比较并保留最大的结果
                if (res.empty() || compare(merged, 0, res, 0)) {
                    res = merged;
                }
            }
            return res;

        }
    };
}