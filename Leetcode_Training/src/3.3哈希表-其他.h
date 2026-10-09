#pragma once
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
using namespace std;

/*

202.快乐数：编写一个算法来判断一个数 n 是不是快乐数。「快乐数」 定义为：
            对于一个正整数，每一次将该数替换为它每个位置上的数字的平方和。
            然后重复这个过程直到这个数变为 1，也可能是 无限循环 但始终变不到 1。
            如果这个过程 结果为 1，那么这个数就是快乐数。
            如果 n 是 快乐数 就返回 true ；不是，则返回 false 。
            输出结果中的每个元素一定是 唯一 的。我们可以 不考虑输出结果的顺序 。
*/

namespace s202m1
{   // 取数值的个、十、百位的方法
    // 题干中明确指出如果不是快乐数，会存在【循环】，即意味着sum会重复出现，这是解题的关键
    
    // 哈希表做法，空间复杂度没有快慢指针法优秀（o1）
    class Solution {
    private:
        int getSum(int n) {
            int sum = 0;
            while (n > 0) {
                int cur = n % 10;
                sum += cur * cur;
                n /= 10;
            }
            return sum;
        }

    public:
        bool isHappy(int n) {
            unordered_set<int> st;
            st.insert(n);

            while (n != 1) {
                n = getSum(n);
                if (!st.insert(n).second) {
                    break;
                }
            }

            return n == 1;
        }
    };
}
namespace s202o1
{   // 更优秀的做法：采用循环快慢双指针做法，这种位数和替代原值的运算最终都会进入循环
    // n → getSum(n) → getSum(getSum(n)) → ...
    // 快乐数：序列最终会进入 1 → 1 → 1 → ... 这个自环。
    // 不快乐数：序列最终会进入一个不包含 1 的循环，永远无法到达 1。
    // 只需检测环的入口是不是1即可判断是否为快乐数
    // 快指针是以相对速度1在进入循环后追赶慢指针，一定不会错过，思想详见环形链表II

    class Solution {
    private:
        int getSum(int n) {
            int sum = 0;
            while (n > 0) {
                int cur = n % 10;
                sum += cur * cur;
                n /= 10;
            }
            return sum;
        }

    public:
        bool isHappy(int n) {
            int slow = n, fast = n;
            do {
                slow = getSum(slow);
                fast = getSum(fast);
                fast = getSum(fast);
            } while (slow != fast);// 很少用do while，记得while那行要加;
            return slow == 1;
        }
    };
}
