#pragma once

#include <vector>// 可能包含<list>，但最好还是显式include <list>
#include <list>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <ctime>       // time()

using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// 问题待定：
/*
21 && 23 && 148：递归解法
*/

/*
模板题：
1.两种思路，常规思路/整体移动：19
2.考察插入排序 + 链表操作：147
3.两两反转，和s92类似，但是更新prev和cur稍微有所不同：24
4.s24 + s92的进阶，两个一组变成k个一组，因为每组个数不确定，所以需要先统计链表长度，不足k个的部分不反转：25
5.快慢指针初级：876
6.从快慢指针相遇点开始，让head与slow同步移动，相遇点即为环的入口：142
后面的待补充，题做了，但是没选模板题（也可能是没有能当模板的）
*/

// 链表：遍历链表 + 删除节点 + 插入节点 + 前后指针 + 快慢指针 + 双指针 + 合并链表 + 分治 + 综合 + 其他

// 【1.1】遍历链表 (4)
// 考察链表的基本结构、主要为模拟题，不涉及算法
/*
1290.二进制链表转整数：给你一个单链表的引用结点 head。链表中每个结点的值不是 0 就是 1。
已知此链表是一个整数数字的二进制表示形式。请你返回该链表所表示数字的 十进制值 。最高位 在链表的头部。

*/
// ---------------------
// 简单链表遍历，o1为位运算优化
namespace s1290m1
{
    class Solution {
    public:
        int getDecimalValue(ListNode* head) {
            int ans = 0;
            while (head != nullptr) {
                ans = 2 * ans + head->val;
                head = head->next;
            }
            return ans;
        }
    };
}
namespace s1290o1
{
    class Solution {
    public:
        int getDecimalValue(ListNode* head) {
            int ans = 0;
            while (head != nullptr) {
                ans = (ans << 1) | head->val;// 就相当于ans * 2 + head->val(当然只限于这道题，因为val要么是0要么是1
                head = head->next;
            }
            return ans;
        }
    };
}

// 还是不熟悉链表，写了二十多分钟才写出来这么道简单题
namespace s2181m1
{   // 将链表中的非零元素的值加到零节点上，最后一个零节点删去
    // 这个做法虽然能通过，但是删除多余节点不是很方便（假如传入的head是动态变量）
    // o1的思路更优秀，m1写法中没有对多余节点进行删除
    class Solution {
    public:
        ListNode* mergeNodes(ListNode* head) {

            ListNode* cur = head;
            int sum = 0;

            while (cur->next != nullptr) {
                if (cur->next->val != 0) {
                    sum += cur->next->val;
                    cur->next = cur->next->next;// 一边遍历，一边收缩链表
                }
                else {
                    cur->val = sum;// 非零节点遍历完了，给将值赋给当前的零节点
                    sum = 0;
                    if (cur->next != nullptr && cur->next->next != nullptr) {
                        cur = cur->next;// 并将当前零节点的next指针指向下一个用来累加值的零节点
                    }
                    else {
                        cur->next = nullptr;// 最后一个零节点特殊处理
                    }
                }
            }
            return head;
        }
    };
}
namespace s2181o1
{   // 灵神做法，思路有点像快慢双指针，慢指针逐个储存零节点之间的数据，快指针遍历元素
    // 最后只要把慢指针的next指向nullptr即可，也方便删除多余的节点
    class Solution {
    public:
        ListNode* mergeNodes(ListNode* head) {
            ListNode* tail = head;
            for (ListNode* cur = head->next; cur->next != nullptr; cur = cur->next) {
                if (cur->val != 0) {
                    tail->val += cur->val;
                }
                else {
                    tail = tail->next;
                    tail->val = 0;// 这里可以放心覆盖，因为tail是慢的，此处的数据早就用过了
                }
            }

            // 保存待删除的剩余节点链表的头
            ListNode* remaining = tail->next;
            // 遍历并删除剩余节点
            while (remaining != nullptr) {
                ListNode* temp = remaining->next;
                delete remaining;
                remaining = temp;
            }
            
            tail->next = nullptr;  // 断开合并后的链表
            return head;
        }
    };
}

// 返回值是vector<ListNode*>，不要求所有节点全部放在数组里，只要放各个子链的头节点
namespace s725o1
{
    class Solution {
    public:
        vector<ListNode*> splitListToParts(ListNode* head, int k) {
            int len = 0;
            ListNode* cur = head;

            // 链表总长度
            while (cur != nullptr) {
                cur = cur->next;
                len++;
            }

            // 前面链表的长度是 cnt + 1 
            int rmn = len % k;
            // 剩下的链表长度是 cnt
            int cnt = len / k;

            cur = head;
            // pre指向cur的前一个节点
            ListNode* pre = nullptr;
            vector<ListNode*> ans;

            while (cur != nullptr) {
                ans.push_back(cur);

                for (int i = 0; i < cnt; ++i) {
                    pre = cur;
                    cur = cur->next;// 循环结束后cur指向下一段子链的头部
                }
                if (rmn > 0) {
                    --rmn;
                    pre = cur;
                    cur = cur->next;
                }
                pre->next = nullptr;// 切断子链
            }
            int empty = k - ans.size();
            for (int i = 0; i < empty; ++i) {
                ans.push_back(nullptr);
            }
            return ans;
        }
    };
}

// 简单链表遍历模拟题，结合哈希表
namespace s817m1
{
    class Solution {
    public:
        int numComponents(ListNode* head, vector<int>& nums) {
            int ans = 0;
            bool isComponent = false;
            unordered_set<int> st(nums.begin(), nums.end());
            ListNode* cur = head;
            while (cur != nullptr) {
                if (st.count(cur->val)) {
                    if (!isComponent) ++ans;
                    isComponent = true;
                }
                else {
                    isComponent = false;
                }
                cur = cur->next;
            }
            return ans;
        }
    };
}
// ---------------------
// 【1.2】删除节点(5)
// 思考一个问题：如果答案中头节点可能会被删除，那么就考虑设置哨兵，否则不设置
/*

*/
// ---------------------
// 纯脑筋急转弯，因为题目要求的“删除”是给定节点的值不存在即可，并不是常规意义上的“删除节点”
namespace s237o1
{   // 难度在于题干的误导，实际为简单题
    class Solution {
    public:
        void deleteNode(ListNode* node) {
            node->val = node->next->val;
            node->next = node->next->next;
        }
    };
}

// 最简单的删除节点题
namespace s203m1
{
    class Solution {
    public:
        ListNode* removeElements(ListNode* head, int val) {
            ListNode dummy;
            dummy.next = head;
            ListNode* cur = &dummy;
            while (cur->next != nullptr) {
                if (cur->next->val == val) {
                    ListNode* temp = cur->next;
                    cur->next = cur->next->next;
                    delete temp;
                }
                else {
                    cur = cur->next;
                }
            }
            return dummy.next;
        }
    };
}

// 模板题1：两种思路，常规思路/整体移动
namespace s19m1
{   // 常规做法：先遍历一遍得到链表长度，然后遍历至len - n处，删除节点
    class Solution {
    public:
        ListNode* removeNthFromEnd(ListNode* head, int n) {
            // 计算链表长度
            int len = 0;
            ListNode* cur = head;
            while (cur != nullptr) {
                cur = cur->next;
                ++len;
            }
            // 定义哨兵头节点
            ListNode dummy;
            dummy.next = head;
            cur = &dummy;
            // 将cur移动至需要删除的节点的前一位
            for (int i = 0; i < len - n; ++i) {
                cur = cur->next;
            }
            // 删除节点
            ListNode* nxt = cur->next;
            cur->next = cur->next->next;
            delete nxt;

            return dummy.next;
        }
    };
}
namespace s19o1
{	// 右指针先走n个距离，然后左右指针一起走，保持右指针一直在左指针右边n个节点处
    // 当右指针走到尾时，左指针的next就是要删除的节点
    class Solution {
    public:
        ListNode* removeNthFromEnd(ListNode* head, int n) {
            // 由于可能会删除链表头部，用哨兵节点简化代码
            ListNode dummy;
            dummy.next = head;

            ListNode* right = &dummy;
            while (n--) {
                right = right->next;// 右指针先向右走 n 步
            }

            ListNode* left = &dummy;
            while (right->next != nullptr) {
                right = right->next;
                left = left->next;// 左右指针一起走
            }
            // 左指针的下一个节点就是倒数第 n 个节点
            ListNode* nxt = left->next;
            left->next = left->next->next;
            delete nxt;

            return dummy.next;
        }
    };
}

// 删除重复节点，重复节点需要保留一个，头节点可以保留，不需要哨兵
namespace s83m1
{
    class Solution {
    public:
        ListNode* deleteDuplicates(ListNode* head) {
            if (head == nullptr) return nullptr;
            ListNode* cur = head;
            while (cur->next != nullptr) {
                if (cur->val == cur->next->val) {
                    ListNode* temp = cur->next;
                    cur->next = cur->next->next;
                    delete temp;
                }
                else {
                    cur = cur->next;
                }
            }
            return head;
        }
    };
}

// 83题进阶，所有重复过的节点都删除，需要哨兵
namespace s82m1
{
    class Solution {
    public:
        ListNode* deleteDuplicates(ListNode* head) {
            ListNode dummy;
            dummy.next = head;
            ListNode* p0 = &dummy;// p0指向需要判断是否要删除的节点子链的前一个节点

            ListNode* cur = head;
            while (cur && cur->next) {
                if (cur->val == cur->next->val) {
                    // 删除重复节点
                    int num = cur->val;
                    while (cur && cur->val == num) {
                        ListNode* nxt = cur;
                        cur = cur->next;
                        delete nxt;
                    }   
                    // 此时cur指向删除后的重复节点的下一个节点处
                    p0->next = cur;
                }
                else {
                    // 更新p0与cur，继续判断是否为重复节点
                    p0 = cur;
                    cur = cur->next;
                }
            }
            return dummy.next;
        }
    };
}
namespace s82o1
{   // 跟m1写法大差不差
    // o1写法出自灵神，但是没有delete，o2是自己写的，所有删除的节点全部都delete过
    class Solution {
    public:
        ListNode* deleteDuplicates(ListNode* head) {
            ListNode dummy;
            dummy.next = head;
            ListNode* cur = &dummy;

            while (cur->next != nullptr) {
                ListNode* node = cur->next;
                // 检查是否有重复
                if (node->next != nullptr && node->val == node->next->val) {
                    int num = node->val;
                    // 跳过所有相同值的节点
                    while (node != nullptr && node->val == num) {
                        ListNode* temp = node;
                        node = node->next;
                        delete temp;// 值等于 val 的节点全部删除
                    }
                    cur->next = node;  // 跳过这些节点
                }
                else {
                    cur = cur->next;   // 只有确认不是重复节点才前移
                }
            }

            return dummy.next;  // 注意返回dummy.next而不是head
        }
    };

}
namespace s82o2
{
    class Solution {
    public:
        ListNode* deleteDuplicates(ListNode* head) {
            ListNode dummy(0, head);
            ListNode* cur = &dummy;

            while (cur->next && cur->next->next) {
                ListNode* node = cur->next;
                if (node->val == node->next->val) {
                    while (node->next->next && node->next->val == node->next->next->val) {
                        ListNode* temp = node->next;
                        node->next = node->next->next;
                        delete temp;
                    }
                    ListNode* nxt = node->next->next;
                    delete node->next;
                    delete node;
                    cur->next = nxt;
                }
                else {
                    cur = cur->next;
                }
            }

            return dummy.next;
        }
    };
}
// ---------------------
// 【1.3】插入节点 (2)
// 用来锻炼链表的代码能力，不涉及算法
/*

*/
// ---------------------
// 模板题2：考察插入排序 + 链表操作，时间复杂度O(n * n)
namespace s147m1
{   // 自己写的，比较杂乱，看o1解法
    class Solution {
    public:
        ListNode* insertionSortList(ListNode* head) {
            if (!head || !head->next) return head;  // 处理空链表或单节点情况（不加这条也正确）

            ListNode* current = head->next;   // 当前待插入节点，从第二个开始
            ListNode* prevNode = head;        // current的前驱节点
            while (current != nullptr) {
                // 寻找插入位置
                ListNode* searchNode = head;  // 从头开始查找插入位置
                ListNode* preInsertNode = head;  // 插入位置的前驱节点

                // 找到第一个大于current值的节点
                while (current->val > searchNode->val && searchNode != current) {
                    preInsertNode = searchNode;
                    searchNode = searchNode->next;
                }
                // 如果current正好在正确位置，直接跳过
                if (searchNode == current) {
                    prevNode = current;
                    current = current->next;
                    continue;
                }
                // 将current从原位置移除
                prevNode->next = current->next;

                // 插入current到正确位置
                if (searchNode == head) {  // 需要插入到链表头部
                    current->next = head;
                    head = current;  // 更新头指针
                }
                else {  // 插入到中间位置
                    current->next = preInsertNode->next;// 也即current->next = searchNode
                    preInsertNode->next = current;
                }
                // 移动current到下一个待处理节点
                current = prevNode->next;
            }
            return head;
        }
    };
}
namespace s147o1
{   // 这份插入排序的代码非常不错，可以通过sorted的尾端与cur比较来跳过一些本来就有序的段落
    // 能够实现原始数据有序性越高效率越高的特性，是链表中插入排序的完美解答
    class Solution {
        // 用虚拟头节点，可以简化插入的逻辑，进一步提速
    public:
        ListNode* insertionSortList(ListNode* head) {
            if (!head || !head->next) return head;

            ListNode dummy(0, head);  // 使用dummy节点简化处理
            ListNode* cur = head->next;
            ListNode* sorted = head;

            while (cur) {
                if (cur->val >= sorted->val) {  // 已经是有序的，直接跳过
                    sorted = cur;
                    cur = cur->next;
                    continue;
                }

                ListNode* insertPos = &dummy;
                // while循环的判断条件也更简练，优于m1
                while (insertPos->next->val <= cur->val) {
                    insertPos = insertPos->next;
                }

                sorted->next = cur->next;
                cur->next = insertPos->next;
                insertPos->next = cur;
                cur = sorted->next;
            }
            return dummy.next;
        }
    };
}

namespace s708m1
{

}
// ---------------------
// 【1.4】反转链表 (4)
// 考察反转后的子链如何与原始链表重新链接的过程
/*

*/
// ---------------------
// 也可以用递归做
namespace s206m1
{
    class Solution {
    public:
        ListNode* reverseList(ListNode* head) {
            ListNode* prev = nullptr;
            ListNode* cur = head;
            while (cur != nullptr) {
                ListNode* nxt = cur->next;
                cur->next = prev;
                prev = cur;
                cur = nxt;
            }
            return prev;// 记住，反转链表的头是prev，这点在分段的反转链表题中也是一致的，如s92，s24
        }
    };
}
namespace s206o1
{	// 递归法，空间复杂度为O(n)，这题比较简单，可以通过这个解法尝试熟悉递归法
    class Solution {
    public:
        ListNode* reverse(ListNode* pre, ListNode* cur) {
            if (cur == nullptr) return pre;
            ListNode* temp = cur->next;
            cur->next = pre;
            // 可以和双指针法的代码进行对比，如下递归的写法，其实就是做了这两步
            // pre = cur;
            // cur = temp;
            return reverse(cur, temp);
        }
        ListNode* reverseList(ListNode* head) {
            // 和双指针法初始化是一样的逻辑
            // ListNode* cur = head;
            // ListNode* pre = nullptr;
            return reverse(nullptr, head);
        }
    };
}

// s206的进阶，加深局部链表翻转的理解
namespace s92o1
{   // 反转整个链表n个节点，最后pre为原始链表末尾
    // 想要p0为left, right子链的左边第一个元素，那么要先移动left - 1次
    class Solution {
    public:
        ListNode* reverseBetween(ListNode* head, int left, int right) {
            ListNode dummy;
            dummy.next = head;
            ListNode* p0 = &dummy;

            for (int i = 0; i < left - 1; ++i) {
                p0 = p0->next;
            }
            // 此时p0为[left, right]子链的左边第一个节点

            ListNode* pre = nullptr;
            ListNode* cur = p0->next;

            for (int i = 0; i < right - left + 1; ++i) {
                ListNode* nxt = cur->next;
                cur->next = pre;
                pre = cur;
                cur = nxt;
            }
            p0->next->next = cur;
            p0->next = pre;
            return dummy.next;
        }
    };
}
namespace s92m1
{   // 自己的写法，比较直接，没o1巧妙，o1是从left到right遍历的过程中就顺便完成反转了，而我要再遍历一次
    class Solution {
    private:
        ListNode* reverseLN(ListNode* head) {
            ListNode* pre = nullptr;
            ListNode* cur = head;
            while (cur) {
                ListNode* nxt = cur->next;
                cur->next = pre;
                pre = cur;
                cur = nxt;
            }
            return pre;
        }

    public:
        ListNode* reverseBetween(ListNode* head, int left, int right) {
            ListNode dummy(0, head);

            // 移动left - 1次到left左边
            ListNode* cur = &dummy;
            for (int i = 0; i < left - 1; ++i) {
                cur = cur->next;
            }
            ListNode* p0 = cur;// left左边为p0
            ListNode* newHead = cur->next;
            // 继续移动到right处
            for (int i = left - 1; i < right; ++i) {
                cur = cur->next;
            }
            ListNode* p1 = cur->next;// right右边为p1
            cur->next = nullptr;

            newHead = reverseLN(newHead);
            p0->next->next = p1;
            p0->next = newHead;

            return dummy.next;
        }
    };
}

// 模板题3：两两反转，和s92类似，但是更新prev和cur稍微有所不同，也可以认为是s25的特殊形式
namespace s24m1
{
    class Solution {
    public:
        ListNode* swapPairs(ListNode* head) {
            // 常见的一种预处理手段
            if (!head || !head->next) return head;

            ListNode dummy(0, head);

            ListNode* prev = &dummy;
            ListNode* cur = head;

            while (cur != nullptr && cur->next != nullptr) {
                // 两两更换
                ListNode* temp = cur->next->next;
                prev->next = cur->next;
                cur->next->next = cur;
                cur->next = temp;
                // 向右移动prev和cur
                prev = cur;
                cur = temp;
            }
            
            return dummy.next;
        }
    };
}
namespace s24m2
{   // m1写法比m1更规整
    class Solution {
    public:
        ListNode* swapPairs(ListNode* head) {
            ListNode dummy(0, head);
            ListNode* prev = &dummy;
            ListNode* cur = head;
               
            // 循环更改结点，非常规整
            while (cur && cur->next) {
                prev->next = cur->next;
                cur->next = prev->next->next;
                prev->next->next = cur;

                prev = cur;
                cur = cur->next;
            }

            return dummy.next;
        }
    };
}

// 模板题4：s24 + s92的进阶，两个一组变成k个一组，因为每组个数不确定，所以需要先统计链表长度，不足k个的部分不反转
namespace s25o1
{
    class Solution {
    public:
        ListNode* reverseKGroup(ListNode* head, int k) {
            // 先计算链表长度
            int len = 0;
            ListNode* cur = head;
            while (cur) {
                cur = cur->next;
                ++len;
            }

            ListNode dummy(0, head);
            ListNode* p0 = &dummy;
            // 分段反转子链
            while (len >= k) {
                len -= k;

                // pre和cur的定义也可以放在while循环外
                ListNode* pre = nullptr;
                ListNode* cur = p0->next;
                for (int i = 0; i < k; ++i) {
                    ListNode* tmp = cur->next;
                    cur->next = pre;
                    pre = cur;
                    cur = tmp;
                }

                ListNode* nxt = p0->next;// 链接断开的链表前先保存下一段子链的p0
                p0->next->next = cur;
                p0->next = pre;
                p0 = nxt;
            }

            return dummy.next;
        }
    };
}
namespace s25m1
{   // 自己想的凌乱写法，时空复杂度和o1一致
    class Solution {
    public:
        ListNode* reverseKGroup(ListNode* head, int k) {
            if (!head || k == 1) return head;

            // 计算链表长度len
            int len = 0;
            ListNode* node = head;
            while (node) {
                ++len;
                node = node->next;
            }
            // 计算需要翻转的链表组数
            int group = len / k;
            if (group == 0) return head;

            // 分组进行翻转
            ListNode* nxt = head;// 下一组链表的头
            ListNode* cur = head;// 当前组链表的头
            ListNode* ans = head;// 整个链表的新表头
            ListNode* tail = nullptr;// 当前组翻转之后的尾节点
            ListNode* curHead = nullptr;// 当前组翻转之前的头
            for (int i = 0; i < group; ++i) {
                ListNode* prev = nullptr;
                // 保存之前的尾节点
                for (int i = 0; i < k; ++i) {
                    nxt = nxt->next;
                }
                // 翻转
                curHead = cur;
                for (int i = 0; i < k; ++i) {
                    ListNode* temp = cur->next;
                    cur->next = prev;
                    prev = cur;
                    cur = temp;
                }

                // 将之前组与当前组翻转之后的头节点相连接
                if (!tail) {
                    tail = curHead;
                }
                else {
                    tail->next = prev;
                    tail = curHead;
                }
                // 保存结果头节点
                if (i == 0) {
                    ans = prev;
                }
                // 更新到下一组链表
                cur = nxt;
            }
            // 将没有翻转的部分链接上
            if (curHead) {
                curHead->next = nxt;
            }

            return ans;
        }
    };
}
// ---------------------
// 【1.5】前后指针 (2)
// 分前后指针，整体同步移动
/*

*/
// ---------------------
// o1：分前后指针，整体同步移动，前后指针模板
namespace s19m1
{   // m1是最普通的做法，但几乎要遍历两次链表，没o1方法简洁和巧妙
    class Solution {
    public:
        ListNode* removeNthFromEnd(ListNode* head, int n) {
            // 计算链表长度
            int len = 0;
            ListNode* cur = head;
            while (cur) {
                cur = cur->next;
                ++len;
            }

            // 定义哨兵 
            ListNode dummy;
            dummy.next = head;
            cur = &dummy;

            // 移动到待删除的前一个节点
            int k = len - n;
            while (k--) {
                cur = cur->next;
            }
            ListNode* nxt = cur->next;
            cur->next = cur->next->next;
            delete nxt;

            return dummy.next;
        }
    };
}
namespace s19o1
{	// 右指针先走n个距离，然后左右指针一起走，保持右指针一直在左指针右边n个节点处
    // 当右指针走到尾时，左指针的next就是要删除的节点
    class Solution {
    public:
        ListNode* removeNthFromEnd(ListNode* head, int n) {
            // 由于可能会删除链表头部，用哨兵节点简化代码
            ListNode dummy(0, head);
            ListNode* slow = &dummy;
            ListNode* fast = &dummy;
            
            // fast指针先向右走 n 步
            for (int i = 0; i < n; ++i) {
                fast = fast->next;
            }

            // 快慢指针一起走
            while (fast->next) {
                slow = slow->next;
                fast = fast->next;
            }

            // slow指针的下一个节点就是倒数第 n 个节点
            ListNode* nxt = slow->next;
            slow->next = slow->next->next;
            delete nxt;
            return dummy.next;
        }
    };
}

namespace s61m1
{
    class Solution {
    public:
        ListNode* rotateRight(ListNode* head, int k) {
            if (!head) return nullptr;// 如果len = 0，那么k % 0会报错

            int len = 0;
            ListNode* cur = head;
            while (cur) {
                cur = cur->next;
                ++len;
            }
            k %= len;
            // 这里必须剪枝，因为后续默认m < len，也即k > 0
            if (k == 0) return head;// 如果k为0，则不需要旋转
            
            // cur指向被旋转的子链的前一个节点
            int m = len - k;
            ListNode dummy;
            dummy.next = head;// 这里cur = head, m = len - k - 1也可以，不需要用到哨兵
            cur = &dummy;
            while (m--) {
                cur = cur->next;
            }

            ListNode* newHead = cur->next;// 保存新链表头
            cur->next = nullptr;          // 切断链表

            cur = newHead;
            while (cur->next) {
                cur = cur->next;
            }
            cur->next = head;             // 连接两段链表

            return newHead;
        }
    };
}
// ---------------------
// 【1.6】快慢指针/环形链表 (5)
// 慢指针一次走一步，快指针一次走两步
/*

*/
// ---------------------
// 模板题5：快慢指针初级
namespace s876o1
{   // slow和fast都在head上，将链表看成头结点 + 剩余的链表两部分
    // 如果链表长度为偶数，那么head右边链表长度为奇数，结束移动时slow在右边链表的中间，也即整个链表的中间右边
    // 如果链表长度为奇数，那么head右边链表长度为偶数，结束移动时slow在右边链表的中间左边，也即整个链表的中间
    // 因为这道题要求的中间节点为偏右的那个，所以无需哨兵
    // 如果要偏左的中间节点，那么需要哨兵
    class Solution {
    public:
        ListNode* middleNode(ListNode* head) {
            ListNode* slow = head;
            ListNode* fast = head;
            while (fast && fast->next) {
                fast = fast->next->next;
                slow = slow->next;
            }
            return slow;
        }
    };
}

// 最简单的判断是否有环
namespace s141m1
{   // s876代码稍微改改就行
    class Solution {
    public:
        bool hasCycle(ListNode* head) {
            ListNode* slow = head;
            ListNode* fast = head;
            while (fast && fast->next) {
                fast = fast->next->next;
                slow = slow->next;
                if (fast == slow) {
                    return true;
                }
            }
            return false;
        }
    };
}

// 数学推导 2 * ( a + b ) = a + b + k * ( b + c ) ---> a = c + k * ( b + c )
// 模板题6：从快慢指针相遇点开始，让head与slow同步移动，相遇点即为环的入口
namespace s142o1
{   // 继续在s141的代码上进行修改即可
    class Solution {
    public:
        ListNode* detectCycle(ListNode* head) {
            ListNode* slow = head;
            ListNode* fast = head;
            while (fast && fast->next) {
                slow = slow->next;
                fast = fast->next->next;
                if (slow == fast) {
                    while (slow != head) {
                        slow = slow->next;
                        head = head->next;
                    }
                    return slow;
                }
            }
            return nullptr;
        }
    };
}
namespace s142o2
{
    // 简单做法，用哈希表保存访问过的节点，空间复杂度O(N)稍高，时间复杂度O(N)
    class Solution {
    public:
        ListNode* detectCycle(ListNode* head) {
            unordered_set<ListNode*> visited;
            while (head) {
                if (visited.count(head)) {
                    return head;
                }
                visited.insert(head);
                head = head->next;
            }
            return nullptr;
        }
    };
}

// 结合s876找中间偏右的节点 + s206反转链表得到反转后的右边子链，将问题转化为合并两条链表
// 模板题：找中间节点 + 反转链表 + 合并链表的综合运用
namespace s143o1
{   // 易错点在while的循环判断条件
    class Solution {
    public:
        void reorderList(ListNode* head) {
            // s876：得到中间节点
            ListNode* slow = head;
            ListNode* fast = head;
            while (fast && fast->next) {
                slow = slow->next;
                fast = fast->next->next;
            }
            // 此时slow为中间节点

            // s206：反转链表
            ListNode* pre = nullptr;
            ListNode* cur = slow;// 从slow->next开始反转的话，下面while循环条件就是head2，见m1
            while (cur) {
                ListNode* nxt = cur->next;
                cur->next = pre;
                pre = cur;
                cur = nxt;
            }
            // 此时pre为右边子链的头节点

            // 合并链表（左边链表的最后一个节点还连着右边链表的最后一个节点）
            // o1做法中因为是从slow开始反转的，所以不能像m1一样，直接slow->next = nullptr断开两段链表
            ListNode* head2 = pre;
            while (head2->next) {// 重点在这里的判断条件：head2->next != nullptr
                // 如果写head2，可能导致自己指向自己
                // 比如1，2，3，4，分割完后会变成1-2-3和4-3，最后3会指向自己，导致1->4->2->3->3
                // 重点是前半段的链表其实并不会是1-2，其尾端与中间偏右的节点仍然连在一起
                ListNode* nxt = head->next;
                ListNode* nxt2 = head2->next;
                head->next = head2;
                head2->next = nxt;
                head = nxt;
                head2 = nxt2;
            }
        }
    };
}
namespace s143m1
{   // 第二步反转链表的起点和o1不一样，我感觉m1是比o1更好理解，更容易记的
    class Solution {
    public:
        void reorderList(ListNode* head) {
            ListNode* slow = head;
            ListNode* fast = head;
            while (fast && fast->next) {
                fast = fast->next->next;
                slow = slow->next;
            }

            ListNode* pre = nullptr;
            ListNode* cur = slow->next;
            while (cur) {
                ListNode* temp = cur->next;
                cur->next = pre;
                pre = cur;
                cur = temp;
            }
            slow->next = nullptr;// 断开前半段与后半段的链接

            ListNode* head2 = pre;
            cur = head;
            while (head2) {
                ListNode* temp = cur->next;
                cur->next = head2;
                ListNode* temp2 = head2->next;
                head2->next = temp;
                cur = temp;
                head2 = temp2;
            }
        }
    };
}

// 与s143o1类似，这里注意不要用s143m1的截断思路了，必须从slow开始反转，直接基于o1做法来做更简单
namespace s234m1
{   // 可以用数组做，但是空间复杂度O(n)
    class Solution {
    public:
        bool isPalindrome(ListNode* head) {
            vector<int> nums;
            ListNode* cur = head;
            while (cur) {
                nums.push_back(cur->val);
                cur = cur->next;
            }
            int n = nums.size();
            int left = 0, right = n - 1;
            while (left < right) {
                if (nums[left] != nums[right]) {
                    return false;
                }
                ++left; --right;
            }
            return true;
        }
    };
}
namespace s234o1
{   // 解法和s143基本一样，但拆分链表 + 反转后不需要合并，只是逐个对值进行比较
    class Solution {
    private:
        ListNode* findMiddle(ListNode* head) {
            ListNode* fast = head;
            ListNode* slow = head;
            while (fast && fast->next) {
                fast = fast->next->next;
                slow = slow->next;
            }
            return slow;
        }

        ListNode* reverseLink(ListNode* head) {
            ListNode* pre = nullptr;
            ListNode* cur = head;
            while (cur) {
                ListNode* nxt = cur->next;
                cur->next = pre;
                pre = cur;
                cur = nxt;
            }
            return pre;
        }

    public:
        bool isPalindrome(ListNode* head) {
            ListNode* mid = findMiddle(head);
            ListNode* head2 = reverseLink(mid);

            while (head2) {
                if (head->val != head2->val) {
                    return false;
                }
                head = head->next;
                head2 = head2->next;
            }
            return true;
        }
    };
}

// 可以用原地修改来做（数字各自归位），o2为思维拓展，数组模拟链表，快慢指针找环的入口(s142)
namespace s287o2
{   // 思路有点像原地修改的循环追踪，如果有一个重复的数，那么代表着模拟的链表有一个环，问题转换为求环的入口
    // slow一次移动一步，操作为slow = nums[slow]
    // fast一次移动两步，操作为fast = nums[nums[fast]]
    class Solution {
    public:
        int findDuplicate(vector<int>& nums) {
            int slow = 0, fast = 0;
            slow = nums[slow];
            fast = nums[nums[fast]];

            while (slow != fast) {
                slow = nums[slow];
                fast = nums[nums[fast]];
            }

            int head = 0;
            while (head != slow) {
                head = nums[head];
                slow = nums[slow];
            }

            return slow;
        }
    };
}
// ---------------------
// 【1.7】双指针 (2)
/*

*/
// ---------------------
// 虽然可以把small和big两个子链都从原链中拆分出来，但我觉得没必要，单独把big拆出来再接回去就够了
namespace s86m1
{   // 思路为将大的节点移除出原链表，单独成链，最后再将两个链表首尾拼接在一起即可
    class Solution {
    public:
        ListNode* partition(ListNode* head, int x) {
            if (!head) return nullptr;

            ListNode* bigHead = nullptr;
            ListNode* bigCur = nullptr;

            ListNode dummy;
            dummy.next = head;
            ListNode* cur = &dummy;
            while (cur && cur->next) {
                if (cur->next->val >= x) {
                    if (!bigHead) {
                        bigHead = cur->next;
                        bigCur = bigHead;
                    }
                    else {
                        bigCur->next = cur->next;
                        bigCur = bigCur->next;
                    }
                    cur->next = cur->next->next;
                }
                else {
                    cur = cur->next;
                }
            }
            if (bigHead) bigCur->next = nullptr;// 注意大数子链最后一个节点的next要置空，否则可能成环

            cur->next = bigHead; // 连接两个链表

            return dummy.next;
        }
    };
}

// 与s86类似，将相邻结点分离的过程与s138类似
namespace s328m1
{   // 自己的写法，比较繁琐
    class Solution {
    public:
        ListNode* oddEvenList(ListNode* head) {
            if (!head) return nullptr;

            ListNode* evenHead = nullptr;
            ListNode* evenCur = nullptr;

            ListNode* cur = head;
            while (cur && cur->next) {
                if (!evenHead) {
                    evenHead = cur->next;
                    evenCur = evenHead;
                }
                else {
                    evenCur->next = cur->next;
                    evenCur = evenCur->next;
                }
                cur->next = cur->next->next;
                if (cur->next == nullptr) {
                    break;// 可能是1,2,3,4偶数长度的链表，如果不break，那么cur将变成nullptr，跟偶子链就接不上了
                }
                cur = cur->next;
            }

            if (evenHead) evenCur->next = nullptr;

            cur->next = evenHead;

            return head;
        }
    };
}
namespace s328o1
{
    class Solution {
    public:
        ListNode* oddEvenList(ListNode* head) {
            // 这里只写if (!head) return nullptr 也对
            if (!head || !head->next) return head;

            ListNode* cur = head;
            ListNode* evenHead = cur->next;
            while (cur->next && cur->next->next) {
                ListNode* even = cur->next;
                cur->next = even->next;
                even->next = even->next->next;

                cur = cur->next;
            }
            cur->next = evenHead;// 无论链表节点数是奇数还是偶数，将cur->next与evenHead相连都是正确的
            return head;
        }
    };
}

// 哈希表做法空间复杂度高，m1做法代码复杂，灵神o1解法秒了
namespace s160m1
{   // 先计算两个链表的长度，然后长链表移动diff个距离，再逐个比较节点
    class Solution {
    public:
        ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
            int lenA = 0, lenB = 0;
            ListNode* curA = headA;
            ListNode* curB = headB;
            while (curA) {
                ++lenA;
                curA = curA->next;
            }
            while (curB) {
                ++lenB;
                curB = curB->next;
            }
            // A的长度保持更长
            if (lenA < lenB) {
                swap(lenA, lenB);
                swap(headA, headB);
            }
            curA = headA;
            int diff = lenA - lenB;
            while (diff--) {
                curA = curA->next;
            }
            // 开始逐个比较
            curB = headB;
            while (curA) {
                if (curA == curB) {
                    return curA;
                }
                curA = curA->next;
                curB = curB->next;
            }

            return nullptr;
        }
    };
}
namespace s160o1
{
    // 看了m1解法就能懂o1解法，链A：x + z(共用段)，链B：y + z
    // 那么x + z + y = y + z + x
    // p 和 q指针把所有节点遍历完一次后的地方一定是交点
    class Solution {
    public:
        ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
            ListNode* p = headA;
            ListNode* q = headB;
            while (p != q) {
                p = (p != nullptr) ? p->next : headB;
                q = (q != nullptr) ? q->next : headA;
            }
            return p;
        }
    };
}

// ---------------------
// 【1.8】合并链表 (3)
/*

*/
// ---------------------
// 逐位相加法：这道题需要用到在堆上创建的动态变量用于返回
namespace s2m1
{   // 自己的写法，优化空间很大，详见o1
    class Solution {
    public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
            ListNode* dummy = new ListNode(0, nullptr);
            ListNode* cur = dummy;

            int carry = 0;
            while (l1 && l2) {
                // 如果这里sum对l1, l2, carry分开处理，代码就能简洁不少
                int sum = l1->val + l2->val + carry;
                carry = sum / 10;
                cur->next = new ListNode(sum % 10, nullptr);
                cur = cur->next;
                l1 = l1->next;
                l2 = l2->next;
            }

            if (l2) swap(l1, l2);

            while (l1 || carry) {
                int sum = l1 ? l1->val + carry : carry;
                carry = sum / 10;
                cur->next = new ListNode(sum % 10, nullptr);
                cur = cur->next;
                l1 = l1 ? l1->next : nullptr;;
            }
            return dummy->next;
        }
    };
}
namespace s2o1
{   
    class Solution {
    public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
            ListNode dummy; 
            ListNode* cur = &dummy;
            int carry = 0; // 进位

            while (l1 || l2 || carry) { // 有一个不是空节点，或者还有进位，就继续迭代
                int sum = carry;
                if (l1) {
                    carry += l1->val; 
                    l1 = l1->next; 
                }
                if (l2) {
                    carry += l2->val; 
                    l2 = l2->next; 
                }
                carry = sum / 10;// 新的进位
                cur->next = new ListNode(sum % 10); // 创建动态变量
                cur = cur->next;
            }

            return dummy.next; // 哨兵节点的下一个节点就是头节点
        }
    };
}

// 如果给定的两个链表是从高位数字开始给，各自进行一次链表反转即可
namespace s445m1
{
    class Solution {
    private:
        ListNode* reverseLN(ListNode* head) {
            ListNode* pre = nullptr;
            ListNode* cur = head;
            while (cur) {
                ListNode* nxt = cur->next;
                cur->next = pre;
                pre = cur;
                cur = nxt;
            }
            return pre;
        }

    public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
            ListNode* head1 = reverseLN(l1);
            ListNode* head2 = reverseLN(l2);

            ListNode dummy;
            ListNode* cur = &dummy;
            int carry = 0;

            while (head1 || head2 || carry) {
                int sum = carry;
                if (head1) {
                    sum += head1->val;
                    head1 = head1->next;
                }
                if (head2) {
                    sum += head2->val;
                    head2 = head2->next;
                }

                cur->next = new ListNode(sum % 10);
                carry = sum / 10;
                cur = cur->next;
            }
            // 注意生成的新链表也是低位在前，返回时需要再反转一次
            return reverseLN(dummy.next);
        }
    };
}

// 迭代写法很容易想到，o1为递归做法
namespace s21m1
{
    class Solution {
    public:
        ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
            ListNode dummy;
            ListNode* cur = &dummy;

            while (list1 && list2) {
                if (list1->val < list2->val) {
                    cur->next = list1;
                    list1 = list1->next;
                }
                else {
                    cur->next = list2;
                    list2 = list2->next;
                }
                cur = cur->next;
            }
            cur->next = list1 ? list1 : list2;
            return dummy.next;
        }
    };
}
namespace s21o1
{   // 递归做法
    class Solution {
    public:
        ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
            ListNode dummy; // 用哨兵节点简化代码逻辑
            ListNode* cur = &dummy; // cur 指向新链表的末尾
            while (list1 && list2) {
                if (list1->val < list2->val) {
                    cur->next = list1; // 把 list1 加到新链表中
                    list1 = list1->next;
                }
                else { // 注：相等的情况加哪个节点都是可以的
                    cur->next = list2; // 把 list2 加到新链表中
                    list2 = list2->next;
                }
                cur = cur->next;
            }
            cur->next = list1 ? list1 : list2; // 拼接剩余链表
            return dummy.next;
        }
    };
}
// ---------------------
// 【1.9】分治 (2)
// 改变迭代处理的顺序，两两分组处理，等价于后序遍历的平衡二叉树
/*

*/
// ---------------------
// 分治，o1为迭代写法，o2为递归写法，和归并排序148类似。o3为最小堆写法，最简单，也需要掌握，面试可能倾向于考察o1
namespace s23o1
{   // 迭代做法是最容易想到的
    // 时间复杂度O(nlogm),m为lists的长度，n为所有链表的长度之和，外层关于step的循环有logm次
    // 内层循环相当于把所有节点都遍历了一遍，是O(n)的，总时间复杂度为O(nlogm)
    // 空间复杂度为O(1)
    class Solution {
    private:
        // s21. 合并两个升序链表
        ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
            ListNode dummy;
            ListNode* cur = &dummy;

            while (list1 && list2) {
                if (list1->val < list2->val) {
                    cur->next = list1;
                    list1 = list1->next;
                }
                else {
                    cur->next = list2;
                    list2 = list2->next;
                }
                cur = cur->next;
            }

            cur->next = list1 ? list1 : list2;

            return dummy.next;
        }

    public:
        ListNode* mergeKLists(vector<ListNode*>& lists) {
            if (lists.empty()) return nullptr;// 列表为空时要特判，返回值lists[0]无意义，小陷阱
            int m = lists.size();

            // 两两合并，四四合并，八八合并... 合并结果储存左边链表中
            for (int step = 1; step < m; step *= 2) {
                for (int i = 0; i + step < m; i += step * 2) {
                    lists[i] = mergeTwoLists(lists[i], lists[i + step]);
                }
            }

            return lists[0];// 最终合并结果在0的位置
        }
    };
}
namespace s23o2
{   // 本质思想和m1相同，时间复杂度O(nlogm)，空间复杂度O(logm)栈空间，仅作拓展，只掌握o1的迭代法就够了
    class Solution {
        // 21. 合并两个有序链表
        ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
            ListNode dummy(0); 
            ListNode* cur = &dummy;

            while (list1 && list2) {
                if (list1->val < list2->val) {
                    cur->next = list1; 
                    list1 = list1->next;
                }
                else { 
                    cur->next = list2;
                    list2 = list2->next;
                }
                cur = cur->next;
            }
            cur->next = list1 ? list1 : list2; 
            return dummy.next;
        }

        // 合并从 lists[i] 到 lists[j-1] 的链表（leetcode给的原函数参数不够用，重载一个同名的）
        ListNode* mergeKLists(vector<ListNode*>& lists, int i, int j) {
            int len = j - i;// 左闭右开
            if (len == 0) {
                return nullptr; // 注意输入的 lists 可能是空的
            }
            if (len == 1) {
                return lists[i]; // 无需合并，直接返回
            }
            int mid = i + len / 2;
            ListNode* left = mergeKLists(lists, i, mid); // 合并左半部分
            ListNode* right = mergeKLists(lists, mid, j); // 合并右半部分
            return mergeTwoLists(left, right); // 最后把左半和右半合并
        }

    public:
        ListNode* mergeKLists(vector<ListNode*>& lists) {
            return mergeKLists(lists, 0, lists.size());
        }
    };
}
namespace s23o3
{   // 时间复杂度O(Llogm),m为lists的长度，L为总节点数，每个节点都经历一次入堆出堆，入堆出堆时间复杂度为O(logm)
    // 最小节点只可能是某个链表头，而下一个最小节点只可能是目前最小节点的下一个节点/另一个链表头
    // 把所有备选项都放进一个最小堆里，不断的push出top即可得到排序后的链表
    // 空间复杂度O(m)
    class Solution {
    public:
        ListNode* mergeKLists(vector<ListNode*>& lists) {
            // []默认按值捕获是优先选择，没有用到外部变量最好默认
            auto cmp = [](const ListNode* a, const ListNode* b) {
                return a->val > b->val;
                };
            // 注意用decltype捕获lambda cmp的类型作为模板参数
            priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);

            for (auto head : lists) {
                if (head) {
                    pq.push(head);// 把所有非空链表的头节点入堆
                }
            }

            ListNode dummy;
            ListNode* cur = &dummy;
            while (!pq.empty()) {
                ListNode* node = pq.top();
                pq.pop();
                if (node->next) {
                    pq.push(node->next); // 下一个节点有可能是最小节点，入堆
                }
                cur->next = node;
                cur = cur->next;
            }

            return dummy.next;
        }
    };
}

// s147限制为插入排序(时间复杂度O(n^2))，s148要求是排序，对排序方式不做限制，归并排序为O(nlogn)
// o1为s147插入排序copy过来，o2为归并排序(分治），o3为归并排序（迭代，空间复杂度降至O(1)，最优先掌握）
namespace s148o1
{   // 插入排序，时间复杂度O(n * n)
    class Solution {
    public:
        ListNode* sortList(ListNode* head) {
            if (!head) return head;

            ListNode dummy(0, head); // 使用dummy节点简化处理
            ListNode* cur = head->next;
            ListNode* prev = head;

            while (cur) {
                if (cur->val >= prev->val) { // 已经是有序的，直接跳过
                    prev = cur;
                    cur = cur->next;
                    continue;
                }

                ListNode* insertPos = &dummy;
                while (insertPos->next->val <= cur->val) {
                    insertPos = insertPos->next;
                }

                prev->next = cur->next;
                cur->next = insertPos->next;
                insertPos->next = cur;
                cur = prev->next;
            }

            return dummy.next;
        }
    };
}
namespace s148o2
{   // 最简单的思路是转成数组然后排序，但是O(n)空间复杂度，链表归并递归分治可以实现O(logn)空间复杂度，如果迭代可以O(1)
    // 时间复杂度O(nlogn)，空间复杂度O(logn)，空间复杂度源于递归栈开销
    // 整体代码比较简单，只要会s876 + s21就能很快写出来，但是空间复杂度较高
    // 将原始链表不断的二分，将问题转化成合并两个有序链表，在合并前递归调用自身实现排序
    // 递归到最下层时只有链表只有2个元素，分开再合并，然后跟另一段2个元素的合并成4个元素的链表，一层层回到最开始
    // 所以任务变成实现：
    // 1.合并两个有序链表
    // 2.找到当前链表的中间节点并分开
    // 也就是s876 + s21，剩下的就是递归调用自身即可
    class Solution {
    private:
        // s876：找链表的中间节点，并从该位置断开，返回后半段链表头
        ListNode* middleNode(ListNode* head) {
            ListNode* pre = head;// 中间节点的前一个节点
            ListNode* slow = head;
            ListNode* fast = head;

            while (fast && fast->next) {
                pre = slow;
                slow = slow->next;
                fast = fast->next->next;
            }
            // 断开前后两段链表
            pre->next = nullptr;
            return slow;
        }

        // s21：合并两个升序链表，并返回新链表的头
        ListNode* mergeLists(ListNode* list1, ListNode* list2) {
            ListNode dummy;
            ListNode* cur = &dummy;

            while (list1 && list2) {
                if (list1->val > list2->val) {
                    cur->next = list2;
                    list2 = list2->next;
                }
                else {
                    cur->next = list1;
                    list1 = list1->next;
                }
                cur = cur->next;
            }
            cur->next = list1 ? list1 : list2;
            return dummy.next;
        }

    public:
        ListNode* sortList(ListNode* head) {
            // 如果链表为空或者只有一个节点，无需排序
            if (!head || !head->next) {
                return head;
            }
            // 找到中间节点 head2，并断开 head2 与其前一个节点的连接
            // 比如 head=[4,2,1,3]，那么 middleNode 调用结束后 head=[4,2] head2=[1,3]
            ListNode* head2 = middleNode(head);
            // 分治
            head = sortList(head);
            head2 = sortList(head2);
            // 合并
            return mergeLists(head, head2);
        }
    };
}
namespace s148o3
{   // 时间复杂度O(nlogn)，空间复杂度O(1)
    // o1递归做法为自顶向下计算，o2迭代做法为自底向上计算
    // 迭代做法比较复杂，可以先看递归做法熟悉思想
    // 归并排序，先从一个节点开始归并（形成一个两节点的有序子链），然后2个2个一组归并，再4个4个直到所有节点都归并完
    /*
    迭代法归并
    步长为1时，按照顺序将链表中所有长为1的子链，两两合并，确保按照顺序排列的所有长为2的子链内部是有序的
    步长为2时，按照顺序将链表中所有长为2的子链，两两合并，确保按照顺序排列的所有长为4的子链内部是有序的
    ....
    */
    class Solution {
    private:
        // 获取总链表长度
        int getListLen(ListNode* head) {
            int len = 0;
            while (head) {
                ++len;
                head = head->next;
            }
            return len;
        }

        // 从链表中分割出size个节点，并返回剩下链表的头节点
        // 如果链表长度<=size，不做任何操作，返回空节点
        // 如果链表长度> size，把链表的前size个节点分割出来，并返回剩下链表的头节点
        ListNode* splitList(ListNode* head, int step) {
            ListNode dummy(0, head);
            ListNode* cur = &dummy;

            while (cur && step--) {
                cur = cur->next;
            }
            // 如果cur == nullptr，说明当前head为头节点下的链表长度 < step
            // 如果长度 = step，说明head头节点下的链表正好有一组，不需要再split，直接用来合并即可
            // 注：长度 = step的分支与长度 > step的分支也可以合并在一起，也即下面的!cur->next的判断也可以省去
            // 如果长度 < step，说明head头节点下的链表都够不成一组，也就更无需split了
            // 因为step是从1开始取的，所以当长度 <= step时，对应的head下的子链一定是有序的
            // 不需要通过merge函数合并，所以返回空节点，跳过合并过程
            if (!cur || !cur->next) {
                // 这里把!cur->next省掉也正确，无非在长度 = step时，下面的赋值是nullptr = nullptr
                return nullptr;
            }
            ListNode* nxt = cur->next;
            cur->next = nullptr;
            return nxt;
        }

        // s21:合并两个有序链表，返回合并后的链表的头节点和尾节点
        // 因为splitList之后链表被打散了，需要重新连起来，所以需要返回合两个升序链表合并后的首尾节点
        pair<ListNode*, ListNode*> mergeTwoLists(ListNode* head1, ListNode* head2) {
            ListNode dummy;
            ListNode* cur = &dummy;

            while (head1 && head2) {
                if (head1->val < head2->val) {
                    cur->next = head1;
                    head1 = head1->next;
                }
                else {
                    cur->next = head2;
                    head2 = head2->next;
                }
                cur = cur->next;
            }
            cur->next = head1 ? head1 : head2;

            while (cur->next) {
                cur = cur->next;// 找到尾节点
            }
            return { dummy.next, cur };
        }

    public:
        ListNode* sortList(ListNode* head) {
            if (!head || !head->next) return head;

            int len = getListLen(head);
            ListNode dummy(0, head);

            for (int step = 1; step < len; step *= 2) {
                ListNode* cur = dummy.next; // 每轮循环/两两合并的起点
                ListNode* newListTail = &dummy; // 新链表的末尾
                //newList是split + merge后的新链表，分段有序的子链集合逐个加到newList末尾

                while (cur) {
                    ListNode* head1 = cur;
                    ListNode* head2 = splitList(head1, step);// 断开head1子链，并得到head2子链头
                    cur = splitList(head2, step);// 断开head2子链，并得到下一个待处理的链表头

                    // 合并两段长为step的链表(后一段不一定有step的长度)
                    auto p = mergeTwoLists(head1, head2);
                    // auto [head, tail] = mergeTwoLists(head1, head2); // 结构化绑定
                    // newListTail->next = head;
                    // newListTail = tail;
                    newListTail->next = p.first;
                    newListTail = p.second;
                }
            }
            return dummy.next;
        }
    };
}
namespace s148o4
{   // 面试也可能要求使用快速排序，以下是快速排序的递归解答，由于快排的分区特性，在链表上无法避免保存待处理子链表
    // 所以空间复杂度不像归并那样可以做到O(1)，时间复杂度在原链表升序/降序情况下可能退化为O(n*n)，所以还要换成随机pivot

    // 链表节点定义（和 LeetCode 一致）
    struct ListNode {
        int val;
        ListNode* next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode* next) : val(x), next(next) {}
    };

    class Solution {
    public:
        ListNode* sortList(ListNode* head) {
            if (!head || !head->next) return head;

            // 计算链表总长度
            int len = 0;
            for (ListNode* cur = head; cur; cur = cur->next) ++len;

            // 调用支持长度的快排函数
            return quickSort(head, len);
        }

    private:
        // 当子链表长度 ≤ 这个值时，改用插入排序
        static constexpr int INSERTION_THRESHOLD = 16;

        // 随机数生成种子（只需初始化一次）
        static bool rand_seeded;// 初始化放在类外，兼容旧环境
        // inline static bool rand_seeded = false; // C++17
        // seedRand()函数定义为静态符合“函数级单例”思想，不依赖任何非静态变量与this指针，是个独立的工具函数
        static void seedRand() {
            // srand() 意为 seed random，即给随机数生成器一颗种子，函数参数中的整形值会成为随机数列的起点
            // time() 可以返回从1970年1月1日以来到现在经过的秒数，参数写nullptr 表示“不需要把时间存到别的地方，直接返回这个整数就行”
            if (!rand_seeded) {
                srand(time(nullptr));
                rand_seeded = true;
            }
        }

        // 带长度的快排
        ListNode* quickSort(ListNode* head, int length) {
            // 边界情况
            if (length <= 1) return head;

            // 小数组用插入排序，避免深层递归
            if (length <= INSERTION_THRESHOLD) {
                return insertionSortList(head);
            }

            // 随机选 pivot：生成 [0, length-1] 的随机索引
            seedRand();
            int pivotIdx = rand() % length;

            // 遍历到 pivot 节点，拿到值
            ListNode* pivotNode = head;
            for (int i = 0; i < pivotIdx; ++i) {
                pivotNode = pivotNode->next;
            }
            int pivotVal = pivotNode->val;

            // 分区：用三个虚拟头节点，分别建立三个临时链表
            ListNode lessDummy(0), equalDummy(0), greaterDummy(0);
            ListNode* lessTail = &lessDummy, * equalTail = &equalDummy, * greaterTail = &greaterDummy;
            int lessLen = 0, greaterLen = 0;  // 同时统计各段长度

            ListNode* cur = head;
            while (cur) {
                ListNode* nextNode = cur->next; // 预先保存下一个节点
                cur->next = nullptr;            // 将当前节点从原链表“拆下”

                if (cur->val < pivotVal) {
                    lessTail->next = cur;
                    lessTail = cur;
                    ++lessLen;
                }
                else if (cur->val == pivotVal) {
                    equalTail->next = cur;
                    equalTail = cur;
                }
                else {
                    greaterTail->next = cur;
                    greaterTail = cur;
                    ++greaterLen;
                }

                cur = nextNode;
            }

            // 递归排序 less 和 greater（equal 已经有序）
            ListNode* sortedLess = quickSort(lessDummy.next, lessLen);
            ListNode* sortedGreater = quickSort(greaterDummy.next, greaterLen);

            // 拼接：less -> equal -> greater
            ListNode dummy(0);
            ListNode* tail = &dummy;

            // 连接 less 部分
            tail->next = sortedLess;
            while (tail->next) tail = tail->next;  // 走到 less 尾部

            // 连接 equal 部分
            tail->next = equalDummy.next;
            while (tail->next) tail = tail->next;  // 走到 equal 尾部

            // 连接 greater 部分
            tail->next = sortedGreater;

            return dummy.next;
        }

        // 插入排序（当子链表很短时使用），解法来自s147o1
        ListNode* insertionSortList(ListNode* head) {
            if (!head) return nullptr;

            ListNode dummy(0, head);
            ListNode* sorted = head;
            ListNode* cur = head->next;
            while (cur) {
                if (cur->val >= sorted->val) {
                    sorted = sorted->next;
                    cur = cur->next;
                    continue;
                }

                ListNode* insertPos = &dummy;
                while (cur->val >= insertPos->next->val) {
                    insertPos = insertPos->next;
                }
                sorted->next = cur->next;
                cur->next = insertPos->next;
                insertPos->next = cur;

                cur = sorted->next;
            }
            return dummy.next;
        }
    };

    bool Solution::rand_seeded = false;
}
// ---------------------
// 【1.10】综合应用(3)
// 综合应用题一般是设计一个链表类，包含多个需要考察的子功能，拆分开都是常见的链表题
/*

*/
// ---------------------
// m1:单链做法，相比原题，对深拷贝构造、移动构造、拷贝赋值、移动赋值、析构进行了拓展
// o1:双链做法，设置返回index处节点的辅助函数，使用 midIndex = size / 2 来判断是离尾巴近还是头近，加速查询
namespace s707m1
{
    class MyLinkedList {
    public:
        struct MyLinkedNode {
            int val;
            MyLinkedNode* next;
            MyLinkedNode(int x) : val(x), next(nullptr) {}
        };

    private:
        int mSize;
        MyLinkedNode* dummyHead;

    public:
        MyLinkedList() {
            mSize = 0;
            dummyHead = new MyLinkedNode(0);
        }

        // 拷贝构造函数（深拷贝）
        // 拷贝语义本质上是承诺不修改源对象，所以要加上const，更规范
        MyLinkedList(const MyLinkedList& other)
            : mSize(0), dummyHead(new MyLinkedNode(0)) {
            MyLinkedNode* cur = other.dummyHead->next;
            while (cur) {
                addAtTail(cur->val);
                cur = cur->next;
            }
        }

        // 移动构造函数，noexcept对编译器承诺"这个函数不会抛出异常"，默许其进行优化与优先进行移动操作
        // 移动语义本质决定必须修改源对象，所以不能加const，这里调用的场景可能是MyLinkedList a(std::move(b));
        // 之后MyLinkedList b就不应该再去使用了
        MyLinkedList(MyLinkedList&& other) noexcept
            : mSize(other.mSize), dummyHead(other.dummyHead) {
            other.mSize = 0;
            other.dummyHead = nullptr;
        }

        // 需要设定析构函数，否则在使用=运算符赋值时资源不会正确释放
        ~MyLinkedList() {
            MyLinkedNode* cur = dummyHead;
            while (cur) {
                MyLinkedNode* temp = cur;
                cur = cur->next;
                delete temp;
            }
        }

        MyLinkedList& operator=(const MyLinkedList& other) {
            if (this != &other) {
                // 创建临时副本，本函数运行结束后，原来链表里的资源会随着temp的析构正确自动释放
                MyLinkedList temp(other);
                swap(mSize, temp.mSize);
                swap(dummyHead, temp.dummyHead);
            }
            return *this;
        }

        // 同理，这里不加const,但加上noexcept
        // 调用时场景一般是 MyLinkedList a = std::move(b); 将原来的左值b转成右值b，之后也不应该再次访问b
        MyLinkedList& operator=(MyLinkedList&& other) noexcept {
            if (this != &other) {
                std::swap(mSize, other.mSize);
                std::swap(dummyHead, other.dummyHead);
            }
            // 当前链表资源会随着临时变量（右值）other的生命周期结束而自动释放
            return *this;
        }

        // get不会改变值，加上const关键字
        int get(int index) const {
            // 题干为index >= 0，一般这里是index < 0 || index >= mSize
            // 下面的add, delete同理
            if (index >= mSize)
                return -1;

            MyLinkedNode* cur = dummyHead;
            for (int i = 0; i < index + 1; ++i) {
                cur = cur->next;
            }
            return cur->val;
        }

        // addAtHead和addAtTail都可以复用addAtIndex的代码
        void addAtHead(int val) { addAtIndex(0, val); }
        void addAtTail(int val) { addAtIndex(mSize, val); }

        void addAtIndex(int index, int val) {
            if (index > mSize)
                return;

            MyLinkedNode* cur = dummyHead;
            for (int i = 0; i < index; ++i) {
                cur = cur->next;
            }

            MyLinkedNode* nxt = cur->next;
            MyLinkedNode* node = new MyLinkedNode(val);
            cur->next = node;
            node->next = nxt;

            ++mSize;
        }

        void deleteAtIndex(int index) {
            if (index >= mSize)
                return;

            MyLinkedNode* cur = dummyHead;
            for (int i = 0; i < index; ++i) {
                cur = cur->next;
            }

            MyLinkedNode* nxt = cur->next;
            cur->next = cur->next->next;
            delete nxt;

            --mSize;
        }
    };
}
namespace s707o1
{
    class MyLinkedList {
    public:
        struct MyNode {
            int val;
            MyNode* next;
            MyNode* prev;
            MyNode(int val, MyNode* n = nullptr, MyNode* p = nullptr)
                : val(val), next(n), prev(p) {
            }
        };

    private:
        int mSize;
        MyNode* dummyNode;

        // 辅助函数一般为private，getNode用于返回index处的节点指针
        MyNode* getNode(int index) {
            if (index < 0 || index >= mSize) return nullptr;

            MyNode* cur = dummyNode;
            if (index < mSize / 2) {
                for (int i = 0; i <= index; ++i) {
                    cur = cur->next;
                }
            }
            else {
                for (int i = 0; i < mSize - index; ++i) {
                    cur = cur->prev;
                }
            }

            return cur;
        }

    public:
        MyLinkedList() : mSize(0) {
            dummyNode = new MyNode(0);
            dummyNode->next = dummyNode;
            dummyNode->prev = dummyNode;
        }

        ~MyLinkedList() {
            MyNode* cur = dummyNode->next;

            while (cur != dummyNode) {
                MyNode* temp = cur;
                cur = cur->next;
                delete temp;
            }

            delete dummyNode;
        }

        int get(int index) {
            MyNode* node = getNode(index);
            return node ? node->val : -1;
        }

        void addAtHead(int val) {
            addAtIndex(0, val);
        }

        void addAtTail(int val) {
            addAtIndex(mSize, val);
        }

        void addAtIndex(int index, int val) {
            if (index > mSize) return;

            index = max(0, index);// 处理负数下标，全部当作插入头部

            MyNode* succ = index == mSize ? dummyNode : getNode(index);
            MyNode* pred = succ->prev;

            MyNode* newNode = new MyNode(val, succ, pred);
            pred->next = newNode;
            succ->prev = newNode;

            ++mSize;
        }

        void deleteAtIndex(int index) {
            if (index < 0 || index >= mSize) return;

            MyNode* toDelete = getNode(index);
            toDelete->prev->next = toDelete->next;
            toDelete->next->prev = toDelete->prev;

            delete toDelete;
            --mSize;
        }
    };
}

// LRU: 当某个块被使用的时候，它将被排到第一，当缓存满了的时候，会替换/驱逐排在最后的块
// o1:标准库链表写法，o2:自行实现双向循环链表，标准库做法用来熟悉思想，面试大概率是全部手撕o2
namespace s146o1
{
    class LRUCache {
    private:
        int mCapacity;
        list<pair<int, int>> cacheList;
        unordered_map<int, list<pair<int, int>>::iterator> key2IterMap;

    public:
        LRUCache(int capacity) : mCapacity(capacity) {}

        int get(int key) {
            auto p = key2IterMap.find(key);
            if (p == key2IterMap.end()) return -1;
            // 当前key在链表中
            auto iter = p->second;
            // 把当前key-value从当前链表中删除并放在最前面
            cacheList.splice(cacheList.begin(), cacheList, iter);

            return iter->second;
        }

        void put(int key, int value) {
            auto p = key2IterMap.find(key);

            // 当前key-value已经存在于链表中
            if (p != key2IterMap.end()) {
                auto iter = p->second;
                // 更新value
                iter->second = value;
                // 把新的key-value从当前链表中删除并放在最前面
                cacheList.splice(cacheList.begin(), cacheList, iter);
                return;
            }
            // 当前key-value不存在与链表中，插入链表最前端
            cacheList.emplace_front(key, value);
            key2IterMap[key] = cacheList.begin();

            // 判断是否key-value对数是否超出capacity
            if (key2IterMap.size() > mCapacity) {
                // 去掉链表末尾key-value对
                key2IterMap.erase(cacheList.back().first);
                cacheList.pop_back();
            }
        }
    };
}
namespace s146o2
{
    class LRUCache {
    public:
        struct Node {
            int key;
            int value;
            Node* prev;
            Node* next;

            Node(int k = 0, int v = 0, Node* p = nullptr, Node* n = nullptr)
                : key(k), value(v), prev(p), next(n) {
            }
        };

    private:
        int capacity;
        Node* dummy;
        unordered_map<int, Node*> key2Node;

        void remove(Node* toRemove) {
            toRemove->prev->next = toRemove->next;
            toRemove->next->prev = toRemove->prev;
            // delete toRemove;  
            // delete不能放在这里，因为getNode中需要再次push_front
        }

        void push_front(Node* x) {
            x->prev = dummy;
            x->next = dummy->next;
            x->prev->next = x;
            x->next->prev = x;
        }

        Node* getNode(int key) {
            auto it = key2Node.find(key);

            if (it == key2Node.end()) {
                return nullptr;
            }

            Node* node = it->second;
            remove(node);
            push_front(node);
            return node;
        }

    public:
        LRUCache(int capacity) : capacity(capacity), dummy(new Node) {
            dummy->prev = dummy;
            dummy->next = dummy;
        }

        // 析构函数是必要的，否则会有内存泄漏，注意哈希表和节点都要删除
        ~LRUCache() {
            // 如果不清空哈希表，在删除节点后，key_to_node 中的指针就变成了悬空指针（dangling pointer）
            // 力扣平台的调试模式或内存检查工具会检测到这种悬空指针而报错
            key2Node.clear();
            
            Node* cur = dummy->next;
            // 注意是cur != dummy
            while (cur != dummy) {
                Node* temp = cur;
                cur = cur->next;
                delete temp;
            }

            delete dummy;
        }

        int get(int key) {
            // getNode也会把对应节点移动到链表头部
            Node* node = getNode(key);
            return node ? node->value : -1;
        }

        void put(int key, int value) {
            Node* node = getNode(key);
            // 如果当前节点在链表中
            if (node) {
                // 更新value
                node->value = value;
                return;
            }
            // 如果当前节点不在链表中
            node = new Node(key, value);
            key2Node[key] = node;
            push_front(node);
            // 判断是否超过容量
            if (key2Node.size() > capacity) {
                Node* back = dummy->prev;
                remove(back);
                key2Node.erase(back->key);
                delete back;
            }
        }
    };
}

// LFU：当某个块被使用的时候，它的“频率”被+1，当缓存满了的时候，会替换/驱逐排在最后的块，算是LRU进阶，更难
// 所有频率的链表的节点都在key2Node里，freq2dummy对应不同频率的链表，“最后的块”为频率最小的链表的尾节点
// o1:标准库链表写法，o2:自行实现双向循环链表
namespace s460o1
{
    class LFUCache {
    public:
        struct Entry {
            int key;
            int value;
            int freq;

            Entry(int k, int v, int f)
                : key(k), value(v), freq(f) {
            }
        };

    private:
        int mCapacity;
        int minFreq = 1;
        unordered_map<int, list<Entry>::iterator> key2Iter;
        unordered_map<int, list<Entry>> freq2List;

        void move(list<Entry>::iterator it) {
            Entry e = *it;
            auto& oldList = freq2List[e.freq];
            oldList.erase(it);
            if (oldList.empty()) {
                freq2List.erase(e.freq);
                if (minFreq == e.freq) {
                    ++minFreq;
                }
            }

            ++(e.freq);
            freq2List[e.freq].emplace_front(e);
            key2Iter[e.key] = freq2List[e.freq].begin();
        }

    public:
        LFUCache(int capacity) : mCapacity(capacity) {}

        int get(int key) {
            auto it = key2Iter.find(key);
            if (it == key2Iter.end()) {
                return -1;
            }

            int value = it->second->value;
            move(it->second);
            return value;
        }

        void put(int key, int value) {
            auto it = key2Iter.find(key);
            if (it != key2Iter.end()) {
                it->second->value = value;
                move(it->second);
                return;
            }

            if (key2Iter.size() == mCapacity) {
                auto& lst = freq2List[minFreq];
                key2Iter.erase(lst.back().key);
                lst.pop_back();
                if (lst.empty()) {
                    freq2List.erase(minFreq);
                }
            }

            freq2List[1].emplace_front(key, value, 1);
            key2Iter[key] = freq2List[1].begin();
            minFreq = 1;
        }
    };
}
namespace s460o2
{
    class LFUCache {
    public:
        struct Node {
            int key;
            int value;
            int freq = 1;// 代表每个节点被访问过的次数，访问次数相同的节点放在同一个链表中
            Node* prev;
            Node* next;

            Node(int k = 0, int v = 0, Node* p = nullptr, Node* n = nullptr)
                : key(k), value(v), prev(p), next(n) {
            }
        };

    private:
        int mCapacity;
        int minFreq = 1;// 记录链表最小频率，不进行初始化也正确，但是明确是1更易读
        unordered_map<int, Node*> key2Node;
        unordered_map<int, Node*> freq2dummy;

        Node* getNode(int key) {
            auto it = key2Node.find(key);
            if (it == key2Node.end()) {
                return nullptr;
            }

            Node* node = it->second;
            remove(node);
            Node* dummy = freq2dummy[node->freq];
            if (dummy->prev == dummy) {
                freq2dummy.erase(node->freq);
                delete dummy;

                if (minFreq == node->freq) {
                    ++minFreq;
                }
            }

            ++(node->freq);
            push_front(node->freq, node);
            return node;
        }

        Node* newList() {
            Node* dummy = new Node;
            dummy->prev = dummy;
            dummy->next = dummy;
            return dummy;
        }

        void remove(Node* toRemove) {
            toRemove->prev->next = toRemove->next;
            toRemove->next->prev = toRemove->prev;
        }

        void push_front(int freq, Node* node) {
            auto it = freq2dummy.find(freq);
            if (it == freq2dummy.end()) {
                it = freq2dummy.emplace(freq, newList()).first;
            }

            Node* dummy = it->second;
            node->prev = dummy;
            node->next = dummy->next;
            node->prev->next = node;
            node->next->prev = node;
        }

    public:
        LFUCache(int capacity) : mCapacity(capacity) {}

        ~LFUCache() {
            for (auto p : freq2dummy) {
                Node* dummy = p.second;
                Node* cur = dummy->next;
                while (cur != dummy) {
                    Node* temp = cur;
                    cur = cur->next;
                    delete temp;
                }
                delete dummy;
            }
        }

        int get(int key) {
            Node* node = getNode(key);
            return node ? node->value : -1;
        }

        void put(int key, int value) {
            Node* node = getNode(key);
            if (node) {
                node->value = value;
                return;
            }

            if (key2Node.size() == mCapacity) {
                Node* dummy = freq2dummy[minFreq];
                Node* back = dummy->prev;
                key2Node.erase(back->key);
                remove(back);
                delete back;

                if (dummy->prev == dummy) {
                    freq2dummy.erase(minFreq);
                    delete dummy;
                }
            }
            node = new Node(key, value);
            key2Node[key] = node;
            push_front(1, node);
            minFreq = 1;
        }
    };
}
// ---------------------
// 【1.11】其他 (1)
// 
/*

*/
// ---------------------
// o1,o2为哈希表做法，空间复杂度O(n)，简单易懂；o3为新旧链表拼接+拆分做法，空间复杂度O(1)
namespace s138o1
{   // 哈希表里最简单易懂的写法
    class Node {
    public:
        int val;
        Node* next;
        Node* random;

        Node(int _val) {
            val = _val;
            next = NULL;
            random = NULL;
        }
    };

    class Solution {
    public:
        Node* copyRandomList(Node* head) {
            unordered_map<Node*, Node*> nodeMap;
            // 因为Node*是指针，所以当作哈希表的Key也没问题
            // 创建所有新的节点，并同时建立新老节点之间的映射
            Node* cur = head;
            while (cur) {
                nodeMap[cur] = new Node(cur->val);
                cur = cur->next;
            }

            // 根据映射对新节点的连接信息进行补全
            cur = head;
            while (cur) {
                nodeMap[cur]->next = nodeMap[cur->next];
                nodeMap[cur]->random = nodeMap[cur->random];
                cur = cur->next;
            }

            return nodeMap[head];
        }
    };
}
namespace s138o2
{   // 和o2本质相同，只不过深拷贝的顺序有所区别
    class Node {
    public:
        int val;
        Node* next;
        Node* random;

        Node(int _val) {
            val = _val;
            next = NULL;
            random = NULL;
        }
    };

    class Solution {
    public:
        Node* copyRandomList(Node* head) {
            unordered_map<Node*, Node*> nodeMap;
            // 先深拷贝val和next，同时建立新老链表对应节点的映射
            Node dummy(0);
            dummy.next = head;
            Node* pre = &dummy;
            Node* cur = head;
            while (cur) {
                Node* node = new Node(cur->val);
                nodeMap[cur] = node;
                pre->next = node;
                pre = node;
                cur = cur->next;
            }
            // 根据映射对random进行深拷贝
            Node* curOld = head;
            Node* curNew = dummy.next;
            while (curOld) {
                curNew->random = nodeMap[curOld->random];
                curNew = curNew->next;
                curOld = curOld->next;
            }

            return dummy.next;
        }
    };
}
namespace s138o3
{   // 不能只是删除合并链表中的旧节点，因为题目要求原链表的 next 不能修改，拆分时还需要还原原链表。
    class Node {
    public:
        int val;
        Node* next;
        Node* random;

        Node(int _val) {
            val = _val;
            next = NULL;
            random = NULL;
        }
    };

    class Solution {
    public:
        Node* copyRandomList(Node* head) {
            // 如果原链表为空，必须提前返回，因为第一个while循环中就用到了cur->next
            // 下面拆分也是建立在至少有一个节点前提下
            if (!head) return nullptr;
            // 深拷贝新节点，并将原链表、新链表交错拼接在一起
            Node* cur = head;
            while (cur) {
                Node* tmp = new Node(cur->val);
                tmp->next = cur->next;
                cur->next = tmp;
                cur = tmp->next;
            }
            //构建新节点的random指向
            cur = head;
            while (cur) {
                if (cur->random != nullptr) {
                    // 关键
                    cur->next->random = cur->random->next;
                }
                cur = cur->next->next;
            }
            // 拆分链表
            cur = head->next;
            Node* pre = head;
            Node* newHead = head->next;
            while (cur->next) {
                pre->next = cur->next;
                cur->next = cur->next->next;

                pre = pre->next;
                cur = cur->next;
            }
            pre->next = nullptr; // 单独处理原链表尾节点
            // 返回新链表头节点
            return newHead;
        }
    };
}
namespace s138o4
{   // o3思路的灵神写法，代码更简洁一点
    class Node {
    public:
        int val;
        Node* next;
        Node* random;

        Node(int _val, Node* nxt, Node* rdm) :  val(_val), next(nxt), random(rdm) {}
    };

    class Solution {
    public:
        Node* copyRandomList(Node* head) {
            if (!head) return nullptr;

            for (Node* cur = head; cur; cur = cur->next->next) {
                // 这行能通过说明leetcode的类定义其实比给出的要更复杂，原始的版本是没有这种构造函数的
                cur->next = new Node(cur->val, cur->next, nullptr);
            }

            for (Node* cur = head; cur; cur = cur->next->next) {
                if (cur->random) {
                    cur->next->random = cur->random->next;
                }
            }

            Node* newHead = head->next;
            Node* cur = head;
            for (; cur->next->next; cur = cur->next) {
                Node* copy = cur->next;
                cur->next = copy->next;
                copy->next = copy->next->next;
            }
            /* for循环等价于这段
            while (cur->next->next) {
                Node* copy = cur->next;
                cur->next = copy->next;
                copy->next = copy->next->next;

                cur = cur->next;
            }
            */
            cur->next = nullptr;// 也需要单独处理尾部节点
            return newHead;
        }
    };
}