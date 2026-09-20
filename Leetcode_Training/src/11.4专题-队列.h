#pragma once
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

// 问题待定：
/*
sxxx：xxx
*/

/*
模板题：
1.用两个队列实现栈，或用一个队列实现栈，环形结构，纯考察数据结构理解，无实际应用价值，每次入队都是O(n)：225
2.使用一个输入栈，一个输出栈，输入栈的元素移动到输出栈后就变成了先进先出：232
3.单调队列模板，一边滑动定长窗口，一边更新单调栈，及时去掉无用数据：239
*/

// 队列：基础 + 设计 + 双端队列 + 单调队列
// 队列常用于BFS中（所以有很多队列相关的题目在图论和网格图题单中），栈常用于DFS中
// 本小节不涉及树形结构的队列相关题目

// 【4.1】基础 (2)
// FIFO的思想运用与熟悉
/*
933.最近的请求次数：写一个 RecentCounter 类来计算特定时间范围内最近的请求。
请你实现 RecentCounter 类：RecentCounter() 初始化计数器，请求数为 0 。
int ping(int t) 在时间 t 添加一个新请求，其中 t 表示以毫秒为单位的某个时间，
并返回过去 3000 毫秒内发生的所有请求数（包括新请求）。确切地说，返回在 [t-3000, t] 内发生的请求数。
保证 每次对 ping 的调用都使用比之前更大的 t 值。

*/
// ---------------------
// 利用队列性质的模拟题
namespace s933m1
{   
    class RecentCounter {
    private:
        queue<int> q;
    public:
        RecentCounter() {
        }

        int ping(int t) {
            q.push(t);
            while (q.front() < t - 3000) {
                q.pop();
            }
            return q.size();
        }
    };

}

// 纯模拟题，思考逆向的过程
namespace s950m1
{   // 这个题解是我自己写出来的
    // 题解：https://leetcode.cn/problems/reveal-cards-in-increasing-order/solutions/3775675/liang-chong-zhi-jie-ni-xiang-mo-ni-fa-pa-p29n/
    // 重点在于“逆向”模拟，举例子看明白怎么逆向操作，在草稿纸上写写画画就明白了
    /*
    正向：
    1.顶部拿一张牌加进答案
    2.然后拿一张顶部的牌放到末尾
    3.最后一张牌就直接加进答案

    逆向思路1：
    1.答案尾部的一张牌直接放入牌组（最上方）
    2.牌组末尾牌放在顶部；
    3.将答案尾部的牌放在牌组顶部；
    4.重复步骤2-3

    逆向思路2：
    1.答案最后一张牌直接放入牌堆
    2.把答案倒数第二张牌放到牌堆最上方
        把牌堆最后一张牌放在牌堆最上方
    3.把答案倒数第三张牌放在牌堆最上方
        把牌堆最后一张牌放在牌堆最上方
    4.重复...
    5.直到答案里只剩一张牌，直接放在牌堆最上方
    */
    // 逆向思路1
    class Solution {
    public:
        vector<int> deckRevealedIncreasing(vector<int>& deck) {
            // 首先对牌组进行升序排序得到答案牌组
            sort(deck.begin(), deck.end());
            int n = deck.size();

            // 使用双端队列来模拟逆向发牌过程
            deque<int> que;
            // 逆向步骤1 直接将答案尾部的牌放在牌堆最上方
            que.push_front(deck[n - 1]);

            for (int i = n - 2; i >= 0; --i) {
                // 步骤2 将牌组末尾牌移动到顶部
                int back = que.back();
                que.pop_back();
                que.push_front(back);
                // 步骤3 将答案尾部牌放在牌组顶部
                que.push_front(deck[i]);
            }

            // 转换为vector返回
            vector<int> ans(que.begin(), que.end());
            return ans;
        }
    };
}
namespace s950o1
{   // 进行逆向模拟，脑筋急转弯，但原理是什么我搞不懂
    class Solution {
    public:
        vector<int> deckRevealedIncreasing(vector<int>& deck) {
            // 将deck降序排列
            sort(deck.begin(), deck.end(), greater<int>());

            // 通过queue反向构造答案
            int n = deck.size();
            queue<int> que;
            for (int i = 0; i < n - 1; ++i) {
                que.push(deck[i]);
                int front = que.front();
                que.pop();
                que.push(front);
            }
            que.push(deck[n - 1]);// 最后一个元素特殊处理

            // 倒着输出，得到答案
            vector<int> ans(n);
            for (int i = n - 1; i >= 0; --i) {
                ans[i] = que.front();
                que.pop();
            }
            return ans;
        }
    };
}
// ---------------------
// 【4.2】设计 (2)
// 
/*
225.用队列实现栈：请你仅使用两个队列实现一个后入先出（LIFO）的栈，
并支持普通栈的全部四种操作（push、top、pop 和 empty）。

232.用栈实现队列：请你仅使用两个栈实现先入先出队列。队列应当支持一般队列支持的所有操作（push、pop、peek、empty）
*/
// ---------------------
// 模板题1：用两个队列实现栈，或用一个队列实现栈，环形结构，纯考察数据结构理解，无实际应用价值，每次入队都是O(n)
namespace s225o1
{   // 单队列即可模拟栈，环形结构
    // 每次push时，都将之前除了新元素之外的所有元素pop并push到队列的末端，整个队列的顺序自然而然就会颠倒过来。
    // 这时进行pop操作，就是先进后出的栈
    class MyStack {
    private:
        queue<int> q;
    public:
        MyStack() {
        }

        void push(int x) {
            int n = q.size();
            q.push(x);
            for (int i = 0; i < n; ++i) {
                q.push(q.front());
                q.pop();
            }
        }

        int pop() {
            int r = q.front();
            q.pop();
            return r;
        }

        int top() {
            return q.front();
        }

        bool empty() {
            return q.empty();
        }
    };
}
namespace s225o2
{   // 双队列实现
    // q1为存放元素的队列，q2为辅助队列
    // 每次push都先将元素放入q2，再将q1所有元素出栈再压入q2，最后交换q1和q2，即可实现元素顺序的颠倒
    class MyStack {
    private:
        queue<int> q1;
        queue<int> q2;
    public:
        MyStack() {
        }

        void push(int x) {
            q2.push(x);
            while (!q1.empty()) {
                q2.push(q1.front());
                q1.pop();
            }
            swap(q1, q2);// 关键点在于q1和q2的swap
        }

        int pop() {
            int ans = q1.front();
            q1.pop();
            return ans;
        }

        int top() {
            return q1.front();
        }

        bool empty() {
            return q1.empty();
        }
    };
}

// 模板题2：使用一个输入栈，一个输出栈，输入栈的元素移动到输出栈后就变成了先进先出
namespace s232o1
{   // push直接使用输入栈的push；pop先检查输出栈是否为空，如果为空那么将整个输入栈移动到输出栈；
    // peek和pop类似，弹出后再补一个回去；队列empty的条件为两个栈都为空
    class MyQueue {
    private:
        stack<int> stIn;
        stack<int> stOut;
    public:
        MyQueue() {}

        void push(int x) {
            stIn.push(x);
        }

        int pop() {
            if (stOut.empty()) {
                while (!stIn.empty()) {
                    stOut.push(stIn.top());
                    stIn.pop();
                }
            }
            int res = stOut.top();
            stOut.pop();
            return res;
        }
        // 反过来，写好peek，然后pop中用peek也是等价的
        int peek() {
            int res = this->pop();
            stOut.push(res);
            return res;
        }

        bool empty() {
            return stIn.empty() && stOut.empty();
        }
    };
}
// ---------------------
// 【4.3】双端队列 (0)
// 
/*


*/
// ---------------------


// ---------------------
// 【4.4】单调队列 (3)
/*
单调队列 = 滑动窗口 + 单调栈，思考入队、出队、更新答案的顺序
有两种情况。如果更新答案时，用到的数据包含当前元素，那么就需要先入队，再更新答案；
如果用到的数据不包含当前元素，那么就需要先更新答案，再入队。
至于出队，一般写在前面，每遍历到一个新的元素，就看看队首元素是否失效（不满足要求），失效则弹出队首。
*/
/*
239. 滑动窗口最大值：给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧。
你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。返回 滑动窗口中的最大值 。
*/
// ---------------------
// 模板题3：单调队列模板，一边滑动定长窗口，一边更新单调栈，及时去掉无用数据
namespace s239o1
{
    class Solution {
    public:
        vector<int> maxSlidingWindow(vector<int>& nums, int k) {
            int n = nums.size();
            // 栈内保存元素的下标
            deque<int> deq;
            vector<int> ans;
            ans.reserve(n - k + 1);

            for (int i = 0; i < n; ++i) {
                // 入队：只保存单调递减的元素，窗口最左边就是最大值
                while (!deq.empty() && nums[deq.back()] <= nums[i]) {
                    deq.pop_back();
                }
                deq.push_back(i);
                // 出队：每次循环都判断一次窗口是否超过给定长度
                if (i - deq.front() + 1 > k) {
                    deq.pop_front();
                }
                // 更新答案：在满足窗口长度的条件后更新
                if (i >= k - 1) {
                    ans.push_back(nums[deq.front()]);
                }
            }
            return ans;
        }
    };
}

// 这道题和滑动窗口没什么关系，基础的双向单调队列的性质应用
namespace ls184m1
{
    class Checkout {
    private:
        deque<int> deq;
        queue<int> que;

    public:
        Checkout() {

        }

        int get_max() {
            return deq.empty() ? -1 : deq.front();
        }

        void add(int value) {
            que.push(value);
            while (!deq.empty() && deq.back() < value) {
                deq.pop_back();
            }
            deq.push_back(value);
        }

        int remove() {
            if (que.empty()) return -1;
            int val = que.front();
            if (val == deq.front()) {
                deq.pop_front();
            }
            que.pop();
            return val;
        }
    };
}

// 单调队列模板练习，最大+最小单调队列(O(n)时间复杂度)，也可以用哈希表 + 不定长滑动窗口来做(O(nlogn)时间复杂度)
namespace s1438o1
{	// 滑动窗口 + 单调队列做法，真正做到O(n)，回头重新刷
    class Solution {
    public:
        int longestSubarray(vector<int>& nums, int limit) {
            deque<int> min_q, max_q;
            int ans = 0, left = 0;

            for (int i = 0; i < nums.size(); i++) {
                int x = nums[i];

                // 1. 右边入(这里的判定条件可以改成 < ，也是正确的，只是这样在左边出环节可能就会多些pop操作
                while (!min_q.empty() && x <= nums[min_q.back()]) {
                    min_q.pop_back();
                }
                min_q.push_back(i);

                while (!max_q.empty() && x >= nums[max_q.back()]) {
                    max_q.pop_back();
                }
                max_q.push_back(i);

                // 2. 左边出
                while (nums[max_q.front()] - nums[min_q.front()] > limit) {
                    left++;
                    if (min_q.front() < left) { // 队首不在窗口中
                        min_q.pop_front();
                    }
                    if (max_q.front() < left) { // 队首不在窗口中
                        max_q.pop_front();
                    }
                }

                // 3. 更新答案
                ans = max(ans, i - left + 1);
            }
            return ans;
        }
    };
}
// 同s1438
namespace s2762m1
{

}