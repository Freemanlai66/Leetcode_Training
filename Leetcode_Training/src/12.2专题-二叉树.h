#pragma once
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() : val(0), left(NULL), right(NULL), next(NULL) {}

    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}

    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {
    }
};

// 问题待定：
/*
1.小节2.2中的大部分题目都只提供了递归解法，对应BFS迭代写法在小节2.13中
104, 111, 112, 129, 199, 100
2.小节2.6为DP相关，暂时跳过
*/

/*
模板题：
1.二叉树前序遍历，o1为递归法，o3为前序遍历迭代法模板:144
2.二叉树后序遍历，m1为递归法，o1,o2为迭代法，o2为后序遍历迭代法模板:145
3.二叉树中序遍历，m1为递归法，o1为迭代法，o2为统一迭代法（可以用在前中后三种遍历里面）:94
4.两种递归模式：自顶向下 / 自底向上:104
5.两种递归模式：自顶向下 / 自底向上的对比，这道题更典型 : 1026
6.不用回溯也能做，o3为回溯做法，恢复现场和递归中的if - else需要仔细体会 : 257
7.m1为常规路径记录 + 回溯写法，o1为标准 & 最好的公共祖先递归写法（比较难理解），o2为栈迭代写法（面试变态的话可能会需要):236
8.利用二叉搜索树的性质，问题更加简单，o1为迭代法，更好理解也很简单，o2为递归法 : 235
9.o1为前序遍历（不断更新区间），o2为中序遍历（最常见，利用二叉搜索树性质），o3为后序遍历 : 98
10.o1为逆天写法，务必要看懂，二叉树 + 哨兵，对递归的理解，递归与迭代的转化帮助很大 : 897
11.创建BST，将创建二叉树的过程分解为一个个子问题，用递归来解决 : 108
11.5从前序/中序数组生成树，热门题的漏网之鱼，初见自己最多能做出O(n * n)做法：105
12.加深BST插入的理解 : 701
13.锻炼分类讨论能力，题目比较难，虽然有取巧的方法，但是会让树退化成链表，增加树的高度 : 450
14.继续强化分类讨论能力，比s450难度稍低一些 : 669
15.o1为双数组法，o2为队列迭代法（标准模板），o3为递归法 : 102
16.双队列求解，路径总和的BFS解，一般不这么做，DFS更适合这道题，本题用于加深BFS迭代的理解 : 112
17.套模板题16 + 二叉树性质，根节点编号为index，则左子节点编号为2 * index, 右子节点为2* index + 1:662
18.二叉树转成链表，o2o3后序遍历的逆序，反向修改二叉树:114
19.链表转成平衡BST，s108是转换有序数组，这题是链表，难度更高:109
20.BFS解法每层从右到左更新，链表特化解法o2空间复杂度O(1)只能在这道题用（s117和本题解法相同，故不列出）:116

额外模板：
1.lambda函数如何递归调用自身的3种写法：auto&&(C++14), std::function, this auto&&(C++23)：872
    可以用c++23的this auto&&就用，是最完美的实现，否则用auto&&（Y Combinator），不要用function
    std::function开销很大，要堆分配+类型擦除，而且内联优化弱，动态分配不稳定，完全不推荐用
    碰到返回值是TreeNode*生成树的场景，还可能报错，非头铁要用也只适合返回值是void之类的简单场景
    递归这种高频调用的场景下，overhead累积明显，面试如果用function会扣分
    极端场景下，std::function比lambda慢10-100倍，游戏引擎内部就禁止使用（《原神》游戏客户端框架就禁用 std::function）
    只有类似void setCallback(std::function<void(int)> cb);之类的回调接口才必须要用，或是
    std::queue<std::function<void()>> tasks;
    tasks.push(...);之类需要延迟执行的场景，例如事件系统、任务队列，因为存储的是“动作本身”，类型必须被擦除。
*/

// 二叉树：遍历二叉树 + 先序遍历 + 后序遍历 + 二叉树直径 + 回溯 + 最近公共祖先 + 二叉搜索树 +
// 创建二叉树 + 插入/删除节点 + 树形DP + BFS + 链表二叉树 + N叉树

// 【2.1】遍历二叉树 (7)
// 144，145，94对应二叉树最简单的三种遍历，其他题为随便选一种遍历方式遍历所有节点
// ---------------------
// 模板题1：二叉树前序遍历，o1为递归法，o3为前序遍历迭代法模板
namespace s144o1
{   // 递归法，后序和中序只需要改一下traversal函数内的顺序
    class Solution {
    private:
        void traversal(TreeNode* cur, vector<int>& vec) {// 递归的传递参数
            // 递归的退出条件            
            if (cur == nullptr) {
                return;
            }
            // 递归的逻辑
            vec.push_back(cur->val);
            traversal(cur->left, vec);
            traversal(cur->right, vec);
        }
    public:
        vector<int> preorderTraversal(TreeNode* root) {
            vector<int> ans;
            traversal(root, ans);
            return ans;
        }
    };
}
namespace s144o2
{   // 迭代法，严格按照前序遍历的逻辑流程来写迭代过程，先一路遍历完所有的左孩子，然后再pop出来遍历右孩子
    // 本写法可以用来熟悉迭代遍历的逻辑，但不是模板写法，迭代法模板见o3
    class Solution {
    public:
        vector<int> preorderTraversal(TreeNode* root) {
            vector<int> ans;// 这种写法下不需要进行空树的判断
            stack<TreeNode*> stk;// 先进后出，符合遍历特点，选用栈
            TreeNode* node = root;
            // 判断条件是关键，两个都要写，初次调用时栈为空但root非空/过程中某节点的右子树可能是空的
            while (!stk.empty() || node != nullptr) {
                while (node != nullptr) {
                    ans.push_back(node->val);
                    stk.push(node);
                    node = node->left;
                }
                // 因为在内部while循环内，node为叶子结点的左孩子（空指针）处，需要利用栈的top回到上一层
                node = stk.top();
                stk.pop();
                node = node->right;
            }
            return ans;
        }
    };
}
namespace s144o3
{   // DFS迭代遍历模板
    class Solution {
    public:
        vector<int> preorderTraversal(TreeNode* root) {
            if (!root) return {};

            vector<int> ans;
            stack<TreeNode*> stk;
            stk.push(root);
            while (!stk.empty()) {
                TreeNode* node = stk.top();
                stk.pop();
                ans.push_back(node->val);
                // 因为栈是先进后出，所以前序遍历中，先让右孩子进栈
                if (node->right) stk.push(node->right);
                if (node->left) stk.push(node->left);
            }
            return ans;
        }
    };
}

// 模板题2：二叉树后序遍历，m1为递归法，o1,o2为迭代法，o1为后序遍历迭代法模板
namespace s145m1
{
    class Solution {
    private:
        void traversal(TreeNode* node, vector<int>& vec) {
            if (!node) return;

            traversal(node->left, vec);
            traversal(node->right, vec);
            vec.push_back(node->val);
        }

    public:
        vector<int> postorderTraversal(TreeNode* root) {
            vector<int> ans;
            traversal(root, ans);
            return ans;
        }
    };
}
namespace s145o1
{  
    class Solution {    
    public:
        vector<int> postorderTraversal(TreeNode* root) {
            vector<int> ans;
            stack<TreeNode*> stk;

            TreeNode* prev = nullptr; // 记录上一个访问的节点
            TreeNode* node = root;    // 当前节点
            // 外层循环控制整个遍历过程，内层循环向左子树深入
            while (!stk.empty() || node != nullptr) {
                // 1.遍历到最左子节点（最初的遍历 + 判断之后碰到的右节点是否有深入的必要）
                while (node != nullptr) {
                    stk.push(node);
                    node = node->left;
                }

                node = stk.top();
                stk.pop();

                // 2.检查是否需要处理右子树(右子树存在 && 未访问过)
                if (node->right != nullptr && node->right != prev) {
                    stk.push(node); // 重复压栈以记录当前路径分叉节点
                    node = node->right;
                }
                else {
                    // 3.访问当前节点
                    ans.push_back(node->val);
                    prev = node;// 避免重复访问右子树，右子树加入答案后，对应根节点会再次经历if-else判断，用来标记重复
                    node = nullptr;// 避免重复访问左子树[设空节点](如果不设空，内层的while循环会一直重复)
                }
            }
            return ans;
        }
    };
}
namespace s145o2
{   // 和s144o2相呼应
    class Solution {
    public:
        vector<int> postorderTraversal(TreeNode* root) {
            if (!root) return {}; 

            vector<int> ans;
            stack<TreeNode*> stk;
            stk.push(root);
            while (!stk.empty()) {
                TreeNode* node = stk.top();
                stk.pop();
                ans.push_back(node->val);
                // ans遍历结果为中-右-左
                if (node->left) stk.push(node->left);
                if (node->right) stk.push(node->right);
            }
            // reverse后为左-右-中，恰好是后序遍历结果
            reverse(ans.begin(), ans.end());
            return ans;
        }
    };
}

// 模板题3：二叉树中序遍历，m1为递归法，o1为迭代法，o2为统一迭代法（可以用在前中后三种遍历里面）
namespace s94m1
{
    class Solution {
    private:
        void traversal(TreeNode* node, vector<int>& vec) {
            if (!node) return;

            traversal(node->left, vec);
            vec.push_back(node->val);
            traversal(node->right, vec);
        }

    public:
        vector<int> inorderTraversal(TreeNode* root) {
            vector<int> ans;
            traversal(root, ans);
            return ans;
        }
    };
}
namespace s94o1
{
    class Solution {
    public:
        vector<int> inorderTraversal(TreeNode* root) {
            vector<int> ans;// 这个写法下，不用特地去加判定空树的代码
            stack<TreeNode*> stk;
            TreeNode* node = root;
            while (node != nullptr || !stk.empty()) {// 逐步访问到最底层的最左边的叶子处
                while (node != nullptr) {
                    stk.push(node);
                    node = node->left;
                }
                // 因为在内部while循环内，node为叶子结点的左孩子（空指针）处，需要利用栈的top回到上一层
                node = stk.top();
                stk.pop();
                ans.push_back(node->val);

                node = node->right;
            }
            return ans;
        }
    };
}
namespace s94o2
{   // 统一迭代法（空指针法，在中间结点后面加上一个空指针，只有读到空指针时才将结点val放入返回数组
    // 其他两种遍历只需要稍微改点语句顺序
    /*
    以中序遍历为例理解统一迭代法，每次对于当前结点都弹出，然后根据遍历的顺序要求再对该结点、该节点的孩子依次入栈
    但是非统一迭代法的空间复杂度平均O(longn)，在树呈链状时最坏为O(n)
    而统一迭代法，因为为每个结点都加入空指针标记，空间复杂度为O(n) + O(logn) = O(n)
    如果不是面试真的记不起来非统一迭代法，就不要写统一迭代法
    */
    class Solution {
    public:
        vector<int> inorderTraversal(TreeNode* root) {
            if (!root) return {};

            vector<int> result;
            stack<TreeNode*> stk;
            stk.push(root);
            while (!stk.empty()) {
                TreeNode* node = stk.top();
                if (node != nullptr) {
                    stk.pop(); // 将该节点弹出，避免重复操作，下面再将右中左节点添加到栈中
                    if (node->right != nullptr) stk.push(node->right);  // 添加右节点（空节点不入栈）

                    stk.push(node);                          // 添加中节点
                    stk.push(nullptr); // 中节点访问过，但是还没有处理，加入空节点做为标记。

                    if (node->left != nullptr) stk.push(node->left);    // 添加左节点（空节点不入栈）
                    /*
                    *   如果是前序的，那么改为：
                    if (node->right != nullptr) st.push(node->right);
                    if (node->left != nullptr) st.push(node->left);
                    st.push(node);
                    st.push(nullptr);
                    *   如果是后序的，那么改为：
                    * st.push(node);
                    st.push(nullptr);
                    if (node->right != nullptr) st.push(node->right);
                    if (node->left != nullptr) st.push(node->left);
                    */
                } 
                else { // 只有遇到空节点的时候，才将下一个节点放进结果集
                    stk.pop();           // 将空节点弹出
                    node = stk.top();    // 重新取出栈中元素
                    stk.pop();
                    result.push_back(node->val); // 加入到结果集
                }
            }
            return result;
        }
    };
}

// o1中对递归的lambda写法进行了讨论
namespace s872m1
{   // 写法基于前序遍历，但增加判断是否为叶子节点
    class Solution {
    private:
        vector<int> leafSequence(TreeNode* root) {
            if (!root) return {};

            vector<int> ans;
            stack<TreeNode*> stk;
            stk.push(root);
            while (!stk.empty()) {
                TreeNode* node = stk.top();
                stk.pop();
                if (node->right) stk.push(node->right);
                if (node->left) stk.push(node->left);
                if (!node->left && !node->right) ans.push_back(node->val);
            }
            return ans;
        }
    public:
        bool leafSimilar(TreeNode* root1, TreeNode* root2) {
            return leafSequence(root1) == leafSequence(root2);
        }
    };
}
namespace s872m1
{   // 递归法
    class Solution {
    private:
        void dfs(TreeNode* node, vector<int>& vec) {
            if (!node) return;
            if (!node->left && !node->right) {
                vec.push_back(node->val);
                return;
            }
            dfs(node->left, vec);
            dfs(node->right, vec);
        }
    public:
        bool leafSimilar(TreeNode* root1, TreeNode* root2) {
            vector<int> vec1;
            vector<int> vec2;
            dfs(root1, vec1);
            dfs(root2, vec2);
            return vec1 == vec2;
        }
    };
}
namespace s872o1
{   // 灵神递归写法，比较高级（用到了C++23,这里我将其换成了C++14的具名lambda）
    // lambda写法高级虽然高级，但创建独立的函数效果相同，所以不要求掌握
    class Solution {
    private:
        vector<int> leafValues(TreeNode* root) {
            vector<int> ans;
            // lambda不同于普通函数，如果在内部直接写dfs(node->left)，递归调用处lambda还不完整，所以会报错
            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;
                if (!node->left && !node->right) {
                    ans.push_back(node->val);
                    return;
                }
                self(self, node->left);
                self(self, node->right);
                };
            dfs(dfs, root);
            // 或者写成function函数模板形式
            /*
            function<void(TreeNode*)> dfs = [&](TreeNode* node) {
                if (!node) return;
                if (!node->left && !node->right) {
                    ans.push_back(node->val);
                    return;
                }
                dfs(node->left);
                dfs(node->right);
            };
            dfs(root);            
            */
            // 下面是灵神的原始代码，用到了C++23的特性
            /*
            auto dfs = [&](this auto&& dfs, TreeNode* node) -> void {
                if (node == nullptr) { // 空节点
                    return;
                }
                if (node->left == nullptr && node->right == nullptr) { // 叶子
                    res.push_back(node->val);
                    return;
                }
                dfs(node->left);
                dfs(node->right);
                };
            dfs(root);
            */
            return ans;
        }
    public:
        bool leafSimilar(TreeNode* root1, TreeNode* root2) {
            return leafValues(root1) == leafValues(root2);
        }
    };

}

// 中序遍历的拆分化处理，另一个角度理解中序遍历的迭代写法
namespace s173m1
{   // 在构造函数中中序遍历一次，用数组存储节点元素
    // 空间复杂度O(n)，可以继续优化
    class BSTIterator {
    private:
        vector<int> nums;
        int index = -1;

    public:
        BSTIterator(TreeNode* root) {
            stack<TreeNode*> stk;
            TreeNode* node = root;

            while (node || !stk.empty()) {
                while (node) {
                    if (node) {
                        stk.push(node);
                    }
                    node = node->left;
                }

                node = stk.top();
                stk.pop();
                nums.push_back(node->val);

                node = node->right;
            }
        }

        int next() {
            return nums[++index];
        }

        bool hasNext() {
            if (index + 1 <= nums.size() - 1) {
                return true;
            }
            else {
                return false;
            }
        }
    };
}
namespace s173o1
{   // 将中序遍历的栈遍历过程拆分开来模拟
    // 空间复杂度O(h)，h为树的最大高度
    class BSTIterator {
    private:
        stack<TreeNode*> st;  // 显式栈，保存待处理的节点
        // 辅助函数：将当前节点及其所有左子节点压入栈
        void pushLeft(TreeNode* node) {
            while (node) {
                st.push(node);
                node = node->left;
            }
        }

    public:
        BSTIterator(TreeNode* root) {
            // 初始化：将根节点及其所有左子节点入栈
            pushLeft(root);
        }

        int next() {
            // 弹出栈顶节点（当前最小值）
            TreeNode* cur = st.top();
            st.pop();
            int result = cur->val;
            // 如果该节点有右子节点，则将其右子节点及其所有左子节点入栈
            if (cur->right) {
                pushLeft(cur->right);
            }
            // pushLeft(cur->right); // 不判断if(cur->right)直接调用pushLeft也是正确的

            return result;
        }

        bool hasNext() {
            // 栈非空说明还有节点未访问
            return !st.empty();
        }
    };
}

namespace ls44m1
{
    class Solution {
    public:
        int numColor(TreeNode* root) {
            if (!root) return 0;

            unordered_set<int> colors;
            stack<TreeNode*> stk;
            stk.push(root);
            while (!stk.empty()) {
                TreeNode* node = stk.top();
                stk.pop();
                colors.insert(node->val);
                if (node->left) stk.push(node->left);
                if (node->right) stk.push(node->right);
            }
            return colors.size();
        }
    };
}

namespace s404m1
{
    class Solution {
    private:
        int ans = 0;
        void dfs(TreeNode* node) {
            if (!node) return;

            if (node->left && !node->left->left && !node->left->right) {
                ans += node->left->val;
            }
            dfs(node->left);
            dfs(node->right);
        }

    public:
        int sumOfLeftLeaves(TreeNode* root) {
            dfs(root);
            return ans;
        }
    };
}

namespace s671m1
{   // 根节点一定是最小的值，第二小的值也即：所有比根节点大的值里最小值
    class Solution {
    public:
        int findSecondMinimumValue(TreeNode* root) {
            int minVal = root->val;
            int ans = -1;
            stack<TreeNode*> stk;
            stk.push(root);
            while (!stk.empty()) {
                TreeNode* node = stk.top();
                stk.pop();
                if (node->val > minVal) {
                    ans = (ans == -1) ? node->val : min(ans, node->val);
                }
                if (node->left) stk.push(node->left);
                if (node->right) stk.push(node->right);
            }
            return ans;
        }
    };
}
// ---------------------
// 【2.2】自顶向下DFS（先序遍历/前序遍历） (8)
// 主要考察递归的简单应用，本小节很多题目也可以用层序遍历求解，对应题解补充在2.13小节
// 自顶向上类型的DFS递归题目，一般递归函数dfs返回值是void，求解用到的数值设为形式参数一层层传递下去
/*

*/
// ---------------------
// 模板题4：两种递归模式：自顶向下/自底向上
namespace s104m1
{
    // 自顶向下传递数值
    class Solution {
    private:
        int dfs(TreeNode* node, int depth) {
            if (!node) return depth;
            return max(dfs(node->left, depth + 1), dfs(node->right, depth + 1));
        }
    public:
        int maxDepth(TreeNode* root) {
            return dfs(root, 0);
        }
    };
}
namespace s104o1
{
    // 自底向上传递数值
    class Solution {
    public:
        int maxDepth(TreeNode* root) {
            if (!root) return 0;
            return max(maxDepth(root->left), maxDepth(root->right)) + 1;
        }
    };
}

namespace s111m1
{   // 自顶向下，递 + 最优性剪枝（如果不剪枝，可能会遍历超过最小深度层的节点）
    class Solution {
    private:
        int minDep = INT_MAX;
        void dfs(TreeNode* node, int depth) {
            // 最优性剪枝：如果递归中发现 cnt≥ans，由于继续向下递归也不会让 ans 变小，直接返回
            if (!node || ++depth >= minDep) return;
            if (!node->left && !node->right) {
                minDep = depth; // 如果没有剪枝，那么这里变成ans = min(ans, cnt);
                return;
            }
            dfs(node->left, depth);
            dfs(node->right, depth);
        }
    public:
        int minDepth(TreeNode* root) {
            if (!root) return 0;
            dfs(root, 0);
            return minDep;
        }
    };
}
namespace s111o1
{   // 自底向上，归
    class Solution {
    public:
        int minDepth(TreeNode* root) {
            if (!root) return 0;

            // 如果root没有右儿子，那么深度就是左子树的深度加一
            if (!root->right) {
                return minDepth(root->left) + 1;
            }
            // 如果root没有左儿子，那么深度就是右子树的深度加一
            if (!root->left) {
                return minDepth(root->right) + 1;
            }
            // 如果root左右儿子都有，那么分别递归计算左子树的深度，以及右子树的深度，二者取最小值再加一
            return min(minDepth(root->left), minDepth(root->right)) + 1;
        }
    };
}
namespace s111o2
{   // 自底向上，归，写法2
    class Solution {
    public:
        int minDepth(TreeNode* root) {
            if (!root) return 0;

            if (!root->left && !root->right) {
                return 1;
            }

            int leftDepth = root->left ? minDepth(root->left) : INT_MAX;
            int rightDepth = root->right ? minDepth(root->right) : INT_MAX;
            return min(leftDepth, rightDepth) + 1;
        }
    };
}

// 有BFS和迭代的做法，o1为最简单的递归
namespace s112o1
{   /* 递归的逻辑：将targetSum用每层节点的值减去，如果是0说明找到了路径；
    1.如果当前节点是空的，则无法当作减数，返回false（都遍历到空节点了还没return true，说明已经找不到了）；
    2.用目标总和减去当前节点值；
    3.如果当前节点是叶子节点，那么判断是否当前目标值是否为0；
    判断左子树是否能满足要求；
    判断右子树是否能满足要求。
    */
    class Solution {
    public:
        bool hasPathSum(TreeNode* root, int targetSum) {
            if (root == nullptr) {
                return false;
            }
            targetSum -= root->val;
            if (root->left == nullptr && root->right == nullptr) { // root 是叶子
                return targetSum == 0;
            }
            return hasPathSum(root->left, targetSum) || hasPathSum(root->right, targetSum);
        }
    };
}

namespace s129m1
{
    class Solution {
    private:
        int ans = 0;
        void dfs(TreeNode* node, int num) {
            if (!node) return;
            num = num * 10 + node->val;
            if (!node->left && !node->right) {
                ans += num;
            }
            dfs(node->left, num);
            dfs(node->right, num);
        }
    public:
        int sumNumbers(TreeNode* root) {
            dfs(root, 0);
            return ans;
        }
    };
}

// 这道题BFS更容易理解和想到，但DFS递归也能做
namespace s199o1
{
    class Solution {
    private:
        void dfs(TreeNode* node, vector<int>& vec, int depth) {
            if (!node) return;
            // 这个深度首次遇到，用ans.size()来动态判断，很巧妙
            if (vec.size() == depth) {
                vec.push_back(node->val);
            }
            // 先递归右子树，保证首次遇到的一定是最右边的节点
            dfs(node->right, vec, depth + 1);
            dfs(node->left, vec, depth + 1);
        }
    public:
        vector<int> rightSideView(TreeNode* root) {
            vector<int> ans;
            dfs(root, ans, 0);
            return ans;
        }
    };
}

namespace s1448m1
{   // 自顶向下
    class Solution {
    private:
        int ans = 0;
        void dfs(TreeNode* node, int mx) {
            if (!node) return;
            if (mx <= node->val) {
                mx = node->val;
                ++ans;
            }
            dfs(node->left, mx);
            dfs(node->right, mx);
        }
    public:
        int goodNodes(TreeNode* root) {
            dfs(root, root->val);
            return ans;
        }
    };
}
namespace s1448o1
{   // 自底向上写法，我个人还是倾向于m1自顶向下
    class Solution {
    public:
        int goodNodes(TreeNode* root, int mx = INT_MIN) {
            if (!root) return 0;
            int left = goodNodes(root->left, max(mx, root->val));
            int right = goodNodes(root->right, max(mx, root->val));
            return left + right + (mx <= root->val);
        }
    };
}

namespace s988m1
{
    class Solution {
    private:
        string ans = "";
        void dfs(TreeNode* node, string s) {
            if (!node) return;
            s += ('a' + node->val);
            if (!node->left && !node->right) {
                reverse(s.begin(), s.end());
                ans = ans.empty() ? s : min(ans, s);
                return;
            }
            dfs(node->left, s);
            dfs(node->right, s);
        }
    public:
        string smallestFromLeaf(TreeNode* root) {
            dfs(root, "");
            return ans;
        }
    };
}

// 模板题5：两种递归模式：自顶向下/自底向上的对比，这道题更典型
namespace s1026m1
{
    class Solution {
    private:
        int ans = 0;
        void dfs(TreeNode* node, int mx, int mn) {
            if (!node) return;
            ans = max(ans, max(abs(node->val - mx), abs(node->val - mn)));
            mx = max(mx, node->val);
            mn = min(mn, node->val);
            /* 改成下面更好，因为因为mx和mn一定是路径上的极值
            mx = max(mx, node->val);
            mn = min(mn, node->val);
            ans = max(ans, mx - mn);
            
            */
            dfs(node->left, mx, mn);
            dfs(node->right, mx, mn);
        }
    public:
        int maxAncestorDiff(TreeNode* root) {
            dfs(root, root->val, root->val);
            return ans;
        }
    };
}
namespace s1026o1
{   // 自底向上：后序遍历，根据左子树的情况和右子树的情况，得到本树的情况。
    /*
    * 思路是维护B的祖先节点中的最小值和最大值，我们还可以站在祖先A的视角，维护A子孙节点中的最小值mn和最大值mx。
    * 换句话说，最小值和最大值不再作为入参，而是作为返回值，意思是以A为根的子树中的最小值mn和最大值mx。
    */
    class Solution {
        int ans = 0;
        pair<int, int> dfs(TreeNode* node) {
            if (node == nullptr) {
                return{ INT_MAX, INT_MIN };
            }
            pair<int, int> left = dfs(node->left);
            pair<int, int> right = dfs(node->right);
            int mn = min({ left.first, right.first, node->val });
            int mx = max({ left.second, right.second, node->val });
            ans = max({ ans, mx - node->val, node->val - mn });
            return { mn, mx };
        }
    public:
        int maxAncestorDiff(TreeNode* root) {
            dfs(root);
            return ans;
        }
    };
}
// 【2.3】自底向上DFS (后序遍历) （10）
// 自顶向上类型的DFS递归题目，一般递归函数dfs返回值是需要用到的参数，编写递归函数时默认底层传上来的数值是正确的
/*

*/
// ---------------------
namespace s965m1
{
    class Solution {
    private:
        bool dfs(TreeNode* node, int x) {
            if (!node) return true;
            if (node->val != x) return false;
            return dfs(node->left, x) && dfs(node->right, x);
        }
    public:
        bool isUnivalTree(TreeNode* root) {
            return dfs(root, root->val);
        }
    };
}

namespace s100o1
{
    // 判断两个树是否相同 = 两个树的根节点相同 + 根结点的左右两个子树相同
    class Solution {
    public:
        bool isSameTree(TreeNode* p, TreeNode* q) {
            if (!p || !q) {
                return !p && !q;
            }

            if (p->val != q->val) {
                return false;
            }

            return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
        }
    };
}

namespace s101m1
{
    class Solution {
    private:
        bool dfs(TreeNode* p, TreeNode* q) {
            if (!p || !q) {
                return p == q;// p = q = nullptr时才可能相等
            }

            if (p->val != q->val) {
                return false;
            }
            /* 也可以像下面这么写：
            if (p == q) {
                return true;// p = q = nullptr
            }
            if (!q || !p || p->val != q->val) return false;
            */

            return dfs(p->left, q->right) && dfs(p->right, q->left);
        }
    public:
        bool isSymmetric(TreeNode* root) {
            return dfs(root->left, root->right);
        }
    };
}

// 结合s100 + s101
namespace s951m1
{
    class Solution {
    public:
        bool flipEquiv(TreeNode* root1, TreeNode* root2) {
            if (!root1 || !root2) {
                return root1 == root2;
            }
            if (root1->val != root2->val) {
                return false;
            }
            return (flipEquiv(root1->left, root2->right) && flipEquiv(root1->right, root2->left))
                || (flipEquiv(root1->left, root2->left) && flipEquiv(root1->right, root2->right));
        }
    };
}

namespace s110o1
{   // DFS递归做法，和二叉树的最大深度s104类似
    // 返回的深度都是正数，负数用不到，那么就用负数来表示当前子树是不平衡的
    // 这道题用BFS不好做
    class Solution {
    private:
        int getHeight(TreeNode* node) {
            if (!node) return 0;
            int leftHeight = getHeight(node->left);
            if (leftHeight == -1) return -1;
            int rightHeight = getHeight(node->right);
            if (rightHeight == -1 || abs(leftHeight - rightHeight) > 1) {
                return -1;
            }
            return max(leftHeight, rightHeight) + 1;
        }

    public:
        bool isBalanced(TreeNode* root) { 
            return getHeight(root) != -1; 
        }
    };
}

namespace s226m1
{
    class Solution {
    private:
        void invert(TreeNode* node) {
            if (!node) return;
            swap(node->left, node->right);
            invert(node->left);
            invert(node->right);
        }
    public:
        TreeNode* invertTree(TreeNode* root) {
            invert(root);
            return root;
        }
    };
}
namespace s226o1
{   // DFS迭代法 前序遍历，这里把stack换成queue，进行层序遍历也没任何问题
    class Solution {
    public:
        TreeNode* invertTree(TreeNode* root) {
            if (!root) return root;
            stack<TreeNode*> stk;
            stk.push(root);
            while (!stk.empty()) {
                TreeNode* node = stk.top();
                stk.pop();
                swap(node->left, node->right);
                // 这里node->right和node->left不分先后
                if (node->right) stk.push(node->right);
                if (node->left) stk.push(node->left);
            }
            return root;
        }
    };
}
namespace s226o2
{   // 递归做法熟练之后可以直接这样写，中途过程的返回值没有意义无所谓，根节点返回了正确答案就可以，没必要新开函数
    class Solution {
    public:
        TreeNode* invertTree(TreeNode* root) {
            if (!root) return nullptr;

            swap(root->left, root->right);
            invertTree(root->left);
            invertTree(root->right);
            return root;
        }
    };
}

// 返回类型是TreeNode*递归
namespace s617o1
{   // 以往这种TreeNode*类型返回值一直觉得没什么作用，这次派上用场了
    class Solution {
    public:
        TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
            if (!root1) return root2;
            if (!root2) return root1;
            root1->val += root2->val;
            root1->left = mergeTrees(root1->left, root2->left);
            root1->right = mergeTrees(root1->right, root2->right);
            return root1;
        }
    };
}

namespace s2331m1
{
    class Solution {
    public:
        bool evaluateTree(TreeNode* root) {
            if (root->val == 0 || root->val == 1) {
                return root->val;
            }
            if (root->val == 2) {
                return evaluateTree(root->left) || evaluateTree(root->right);
            }
            else {
                return evaluateTree(root->left) && evaluateTree(root->right);
            }
        }
    };
}

namespace s508m1
{
    class Solution {
    private:
        unordered_map<int, int> freqMap;
        int mxCnt = 0;
        int dfs(TreeNode* node) {
            if (!node) return 0;
            int sum = node->val + dfs(node->left) + dfs(node->right);
            mxCnt = max(mxCnt, ++freqMap[sum]);
            return sum;
        }
    public:
        vector<int> findFrequentTreeSum(TreeNode* root) {
            vector<int> ans;
            dfs(root);
            for (const auto& p : freqMap) {
                if (p.second == mxCnt) {
                    ans.push_back(p.first);
                }
            }
            return ans;
        }
    };
}

namespace s606m1
{
    class Solution {
    private:
        string ans = "";
        void dfs(TreeNode* node) {
            if (!node) return;
            ans += to_string(node->val);
            // 左孩子为空，但右孩子不为空的情况可以并入左孩子不为空的分支
            if (node->left || (!node->left && node->right)) {
                ans += '(';
                dfs(node->left);
                ans += ')';
            }
            if (node->right) {
                ans += '(';
                dfs(node->right);
                ans += ')';
            }
        }
    public:
        string tree2str(TreeNode* root) {
            dfs(root);
            return ans;
        }
    };
}
// ---------------------
// 【2.4】自底向上DFS：删点 (1)
/*

*/
// ---------------------
// 返回类型是TreeNode*，和s617类似
namespace s814o1
{
    class Solution {
    public:
        TreeNode* pruneTree(TreeNode* root) {
            if (!root) return nullptr;

            root->left = pruneTree(root->left);
            root->right = pruneTree(root->right);

            if (!root->left && !root->right && root->val == 0) {
                return nullptr;
            }

            return root;
        }
    };
}
// ---------------------
// 【2.5】有递有归 (1)
/*

*/
// ---------------------
// 不带返回值，但是又是自底向上
namespace s538o1
{   // 遍历顺序为右 - 中 - 左 + 顺便复习lambda递归写法（规避全局变量）
    class Solution {
    public:
        TreeNode* convertBST(TreeNode* root) {
            int s = 0;
            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;
                self(self, node->right);
                s += node->val;
                node->val = s;
                self(self, node->left);
                };
            dfs(dfs, root);
            return root;
        }
    };
}
// ---------------------
// 【2.6】二叉树的直径 ()
/*

*/
// ---------------------
// 暂时略过，部分题目详见6.6专题-树形DP
// ---------------------
// 【2.7】回溯 (3)
// 回溯题相比常规递归，多了个恢复现场的过程（pop_back），能重复利用某个变量降低空间复杂度，但时间复杂度不变
// 特点为判断是否为叶子节点和是否向下递归是二选一的if-else
/*

*/
// ---------------------
// 模板题6：不用回溯也能做，比如o1，但是需要多次复制字符串，o3为回溯做法，恢复现场和递归中的if-else需要仔细体会
namespace s257m1
{   // 无回溯做法，期间需要多次复制字符串，自顶向下DFS
    class Solution {
    private:
        string s = "";
        vector<string> path;
    public:
        vector<string> binaryTreePaths(TreeNode* root) {
            vector<string> path;

            auto dfs = [&](auto&& self, TreeNode* node, string s) -> void {
                if (!node) return;
                if (!s.empty()) {
                    s += "->";
                }
                s += to_string(node->val);
                if (!node->left && !node->right) {
                    path.push_back(s);
                    return;
                }
                self(self, node->left, s);
                self(self, node->right, s);
                };

            dfs(dfs, root, "");
            return path;
        }
    };
}
namespace s257m2
{   // 无回溯做法，但是把根节点的特判放在递归的外面
    namespace s257m
    {
        class Solution {
        private:
            vector<string> result;
            void dfs(TreeNode* node, string path) {
                if (node == nullptr) {
                    return;
                }
                path += "->";
                path += to_string(node->val);
                if (!node->left && !node->right) {
                    result.push_back(path);
                    return;
                }
                dfs(node->left, path);
                dfs(node->right, path);
            }
        public:
            vector<string> binaryTreePaths(TreeNode* root) {
                string rootVal = to_string(root->val);
                if (!root->left && !root->right) {
                    return vector<string>({ rootVal });// 因为递归中“->val”是连在一起的，所以root要单独讨论处理
                }
                dfs(root->left, rootVal);// 传入的并不是空路径
                dfs(root->right, rootVal);
                return result;
            }
        };
    }
}
namespace s257o1
{   // 灵神写法，跟我的很类似，但是更好
    class Solution {
        vector<string> result;
        void dfs(TreeNode* node, string path) {
            if (!node) return;
            path += to_string(node->val);
            if (!node->left && !node->right) {
                result.push_back(path);
                return;
            }
            path += "->";//把加入箭头这一步单独放在这，如此一来root就不用单独处理了
            dfs(node->left, path);
            dfs(node->right, path);
        }
    public:
        vector<string> binaryTreePaths(TreeNode* root) {
            dfs(root, "");//初始化为空路径
            return result;
        }
    };
}
namespace s257o2
{   // 另一种无回溯做法，递归停止条件变成：当前节点是否为叶子节点
    class Solution {
        vector<string> res;
        void backtrack(TreeNode* node, string path) {
            if (!node->left && !node->right) {
                res.push_back(path);
                return;
            }
            if (node->left) {
                backtrack(node->left, path + "->" + to_string(node->left->val));
            }
            if (node->right) {
                backtrack(node->right, path + "->" + to_string(node->right->val));
            }
        }
    public:
        vector<string> binaryTreePaths(TreeNode* root) {
            backtrack(root, to_string(root->val));
            return res;
        }
    };
}
namespace s257o3
{   /* 回溯：与普通递归的区别在于会反复恢复某个变量值，删除垃圾数据，比如这个写法里的path
    本体思路：（对于本题其实还是o1普通递归做法更好）
    如果没有递归到叶子节点，我们会先递归左子树，然后递归右子树。
    递归完了左子树，就要倒回去，递归右子树。
    倒回去的过程中，之前加到 path 中的数据（在左子树中）是垃圾数据，要及时清除掉（恢复现场）。
    */
    class Solution {
    public:
        vector<string> binaryTreePaths(TreeNode* root) {
            // 只保存节点的值，而不保存"->"
            vector<string> path;
            vector<string> ans;

            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;

                path.push_back(to_string(node->val));

                if (!node->left && !node->right) {
                    string jointed = path[0];
                    for (int i = 1; i < path.size(); ++i) {
                        jointed += "->";
                        jointed += path[i];
                    }
                    // 移动语义提速, jointed是之后不会用到的局部变量
                    ans.push_back(move(jointed));
                }
                else {// 这两个回溯必须写成if else形式
                    self(self, node->left);
                    self(self, node->right);
                }
                // 恢复现场
                path.pop_back();
                };

            dfs(dfs, root);
            return ans;
        }
    };
}

namespace s113m1
{   // 无回溯做法
    class Solution {
    public:
        vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
            vector<vector<int>> ans;

            auto dfs = [&](auto&& self, TreeNode* node, vector<int> path, int sum) ->void {
                if (!node) return;
                sum += node->val;
                path.push_back(node->val);

                if (!node->left && !node->right) {
                    if (sum == targetSum) {
                        ans.push_back(path);
                    }
                    return;
                }

                self(self, node->left, path, sum);
                self(self, node->right, path, sum);
                };

            dfs(dfs, root, {}, 0);
            return ans;
        }
    };
}
namespace s113o1
{   // 回溯做法
    class Solution {
    public:
        vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
            vector<int> path;
            vector<vector<int>> ans;

            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;

                path.push_back(node->val);
                targetSum -= node->val;

                if (!node->left && !node->right) {
                    if (targetSum == 0) {
                        ans.push_back(path);
                    }
                }   // 叶子/向下递归是二选一的
                else {
                    self(self, node->left);
                    self(self, node->right);
                }

                targetSum += node->val;
                path.pop_back();
                };

            dfs(dfs, root);
            return ans;
        }
    };
}

// 属于稍微难点的题，回溯 + 枚举 + 前缀和结合才能做出来，基础题为s560
namespace s437o1
{   // 题目要求返回满足总和的路径的数目，可以联想至s560的满足target的子数组
    // 路径和子树组是类似的，路径总和与前缀和的计算也是相似的
    // 一个用固定的sum，线性遍历数组记录，一个用递归参数s就可以
    // 记录之前的sum同样用哈希表，只需要每次递归结束恢复现场即可
    
    // DFS 遍历这棵树，遍历到节点 node 时，假设 node 是路径的终点，
    // 那么有多少个起点，满足起点到终点 node 的路径总和恰好等于 targetSum ?
    class Solution {
    public:
        int pathSum(TreeNode* root, int targetSum) {
            unordered_map<long long, int> freqMap;
            int ans = 0;
            freqMap[0LL] = 1;// 记住要额外加入{0, 1}

            auto dfs = [&](auto&& self, TreeNode* node, long long s) {
                if (!node) return;

                s += node->val;
                auto it = freqMap.find(s - targetSum);
                if (it != freqMap.end()) {
                    ans += it->second;
                }
                ++freqMap[s];

                self(self, node->left, s);
                self(self, node->right, s);
                // 子树对freqMap的影响需要撤销
                --freqMap[s];
                };

            dfs(dfs, root, 0LL);
            return ans;
        }
    };
}
// ---------------------
// 【2.8】最近公共祖先 (2)
// 不用想那么多，记下来就行，这种类型的题目就3道常见的，目前时间有限只做两道
/*

*/
// ---------------------
// 模板题7：m1为常规路径记录 + 回溯写法，o1为标准&最好的公共祖先递归写法（比较难理解），o2为栈迭代写法（面试变态的话可能会需要）
namespace s236m1
{   // 最笨的做法，记录达到p和q的路径，再比较两个路径找到最近公共祖先
    // 必须要用回溯降低空间复杂度才能通过，要不然会超过内存限制
    class Solution {
    public:
        TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
            vector<vector<TreeNode*>> paths;
            vector<TreeNode*> path;
            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;
                path.push_back(node);
                if (node == p || node == q) {
                    paths.push_back(path);
                }
                else {
                    self(self, node->left);
                    self(self, node->right);
                }
                path.pop_back();
                };

            dfs(dfs, root);

            // 题目中指出最少有两个节点，所以下面这段if可以整段删掉
            if (paths.size() == 1) {
                return p ? p : q;
            }

            vector<TreeNode*>& vec1 = paths[0];
            vector<TreeNode*>& vec2 = paths[1];

            unordered_set<TreeNode*> st(vec1.begin(), vec1.end());
            for (int i = vec2.size() - 1; i >= 0; --i) {
                if (st.find(vec2[i]) != st.end()) {
                    return vec2[i];
                }
            }
            return nullptr;
        }
    };
}
namespace s236o1
{   /*
    递归函数的返回值的意义为：最近公共祖先可能的候选项，为空说明该分支下不存在候选项

    总共分以下情况：
    1.当前节点为空
    2.当前节点为p或q
    3.当前节点非空且不是p或q：
        3.1.p和q分别在当前节点的左右子树里
        3.2.p或q都在当前节点的左子树里
        3.3.p或q都在当前节点的右子树里
        3.4.p或q都不在当前节点的子树里

    情况1：返回空指针
    情况2：不需要继续递归下去，直接返回当前节点即可，因为答案只可能在当前节点及当前节点更高的位置
    情况3.1：说明当前节点就是最近公共祖先，返回当前节点
    情况3.2：返回递归左子树的结果
    情况3.3：返回递归右子树的结果
    情况3.4：返回空节点
    */

    class Solution {
    public:
        TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
            // 情况1和2合并
            if (!root || root == p || root == q) {
                return root;
            }

            // left和right代表左子树和右子树里是否能找到公共祖先的候选项
            TreeNode* left = lowestCommonAncestor(root->left, p, q);
            TreeNode* right = lowestCommonAncestor(root->right, p, q);

            if (!right) return left;// 情况3.2 + 情况3.4
            if (!left) return right;// 情况3.3

            // 情况3.1 left和right都不为空，说明root为最近公共祖先
            return root;

            /*  也可以继续将上式简化：
            if (left && right) return root;
            return left ? left : right;
            */
        }
    };
}
namespace s236o2
{   // 栈写法
    class Solution {
    public:
        TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
            stack<TreeNode*> stk;
            stk.push(root);

            // 哈希表记录父节点 son -> parent
            unordered_map<TreeNode*, TreeNode*> parent;
            parent[root] = nullptr; // 根节点没有父节点
            // 这点很重要，否则后面的while(p)会变成死循环

            // 遍历直到找到p和q两个节点
            while (parent.find(p) == parent.end() || parent.find(q) == parent.end()) {
                TreeNode* node = stk.top();
                stk.pop();

                if (node->left) {
                    parent[node->left] = node;
                    stk.push(node->left);
                }
                if (node->right) {
                    parent[node->right] = node;
                    stk.push(node->right);
                }
            }

            // 找到p的所有祖先节点（利用哈希表父子映射关系循环追踪）
            unordered_set<TreeNode*> ancestors;
            while (p) {
                ancestors.insert(p);
                p = parent[p];// 自动一层层向上回溯，太妙了
            }

            // 寻找q的祖先中第一个也是p的祖先的节点
            while (ancestors.find(q) == ancestors.end()) {
                q = parent[q];
            }

            return q;
        }
    };
}

// 模板题8：利用二叉搜索树的性质，问题更加简单，o1为迭代法，更好理解也很简单，o2为递归法
namespace s235o1
{   // 空间复杂度O(1)，递归则是O(n)
    class Solution {
    public:
        TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
            while (root) {
                int x = root->val;
                if (x < p->val && x < q->val) {
                    root = root->right;
                }
                else if (x > p->val && x > q->val) {
                    root = root->left;
                }
                else {
                    break;
                }
            }
            /* 还可继续优化：
            if (p->val > q->val) {
                swap(p, q);
            }
            while (root) {
                if (root->val < p->val) {
                    root = root->right;
                }
                else if (root->val > q->val) {
                    root = root->left;
                }
                else {
                    break;
                }
            }
            */
            return root;
        }
    };
}
namespace s235o2
{
    class Solution {
    public:
        TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
            int x = root->val;
            if (x < q->val && x < p->val) {
                return lowestCommonAncestor(root->right, p, q);
            }
            if (x > q->val && x > p->val) {
                return lowestCommonAncestor(root->left, p, q);
            }
            return root;
        }
    };
}
// ---------------------
// 【2.9】二叉搜索树 (10) 性质如下：
/*
1.查找快，就像查字典一样，每次比较都能排除一半的可能性
2.中序遍历可以得到有序数列
*/
// ---------------------
// 模板题9：o1为前序遍历（不断更新区间），o2为中序遍历（最常见，利用二叉搜索树性质），o3为后序遍历
namespace s98o1
{   // 注意需要用long long，因为节点值范围是INT_MIN - INT_MAX，“无穷大”区间边界需要超过它们
    // 前序遍历：先判断，后递归（把节点值的范围区间往下传）
    // 核心是传递区间
    /*
    1.前序遍历在某些数据下不需要递归到叶子节点就能返回（比如根节点左儿子的值大于根节点的值，左儿子就不会继续往下递归了），
        而中序遍历和后序遍历至少要递归到一个叶子节点。从这个角度上来说，前序遍历是最快的。
    2.中序遍历很好地利用了二叉搜索树的性质，使用到的变量最少。
    3.后序遍历的思想是最通用的，即自底向上计算子问题的过程。想要学好动态规划的话，请务必掌握自底向上的思想。
    */
    class Solution {
    private:
        bool dfs(TreeNode* node, long long l, long long r) {
            if (!node) return true;
            long long x = node->val;
            if (x >= r || x <= l) {
                return false;
            }
            return dfs(node->left, l, x) &&
                dfs(node->right, x, r);
        }
    public:
        bool isValidBST(TreeNode* root) {
            return dfs(root, LLONG_MIN, LLONG_MAX);
        }
    };
}
namespace s98o2
{   // 中序遍历：大于上一个节点，因为二叉搜索树的性质，对树进行中序遍历得到的结果一定是严格递增的
    // 这里用迭代法遍历个人感觉更好，见s98m1
    class Solution {
    public:
        bool isValidBST(TreeNode* root) {
            // 用prev来记录遍历到的数字，按照中序遍历所有节点并不断更新prev，只要更新prev时比之前小，那么return false
            long long prev = LLONG_MIN;
            auto dfs = [&](auto&& self, TreeNode* node) {
                if (!node) return true;

                if (!self(self, node->left)) {
                    return false;
                }
                if (node->val <= prev) {
                    return false;
                }
                prev = node->val;
                return self(self, node->right);
                };
            return dfs(dfs, root);
        }
    };
}
namespace s98o3
{   // 后序遍历：先递归，再判断（把节点值的范围区间往上传）
    class Solution {
    private:
        pair<long long, long long> dfs(TreeNode* node) {
            if (!node) {
                return { LLONG_MAX, LLONG_MIN };
            }

            auto left = dfs(node->left);
            auto right = dfs(node->right);

            long long x = node->val;
            if (x <= left.second || x >= right.first) {
                return { LLONG_MIN, LLONG_MAX };// 可以确保在返回之后上一层也返回{ LLONG_MIN, LLONG_MAX }
            }
            // x <= left.second和x >= right.first也可以在dfs(node->left)和dfs(node->right)后分别判断，提前返回 

            // 因为左右子节点可能是空节点，所以需要再取一次min和max把inf和-inf去除掉
            return { min(left.first, x), max(right.second, x) };
        }
    public:
        bool isValidBST(TreeNode* root) {
            // 本质上返回{ LLONG_MIN, LLONG_MAX }和返回false是等价的，因为中后序遍历都需要一路将答案返回到根节点判断
            return dfs(root).first != LLONG_MIN;
        }
    };
}
namespace s98m1
{   // 迭代法中序遍历
    class Solution {
    public:
        bool isValidBST(TreeNode* root) {
            long long prev = LLONG_MIN;
            stack<TreeNode*> stk;
            TreeNode* node = root;
            while (node || !stk.empty()) {
                while (node) {
                    stk.push(node);
                    node = node->left;
                }
                node = stk.top();
                stk.pop();
                if (node->val <= prev) {
                    return false;
                }
                prev = node->val;
                node = node->right;
            }
            return true;
        }
    };
}

// 中序遍历简单题，m1为迭代法, o1为更简单的迭代法，o2为递归法
namespace s700m1
{   // 迭代法（其实完全用不到栈）
    class Solution {
    public:
        TreeNode* searchBST(TreeNode* root, int val) {
            stack<TreeNode*> stk;
            stk.push(root);
            while (!stk.empty()) {
                TreeNode* node = stk.top();
                stk.pop();
                if (node->val == val) {
                    return node;
                }
                if (node->left && node->val > val) {
                    stk.push(node->left);
                }
                else if (node->right && node->val < val) {
                    stk.push(node->right);
                }
                else {
                    break;
                }
            }
            return nullptr;
        }
    };
}
namespace s700o1
{
    class Solution {
    public:
        TreeNode* searchBST(TreeNode* root, int val) {
            while (root) {
                if (root->val == val) {
                    return root;
                }
                root = (root->val > val) ? root->left : root->right;
            }
            return nullptr;
        }
    };
}
namespace s700o2
{   
    class Solution {
    public:
        TreeNode* searchBST(TreeNode* root, int val) {
            if (!root || root->val == val) {
                return root;
            }
            return root->val < val ? searchBST(root->right, val) : searchBST(root->left, val);
        }
    };
}

// 依旧中序遍历
namespace s530m1
{
    class Solution {
    private:
        int ans = INT_MAX;
        int prev = -1e5;// 防止减法溢出
        void dfs(TreeNode* node) {
            if (!node) return;

            dfs(node->left);

            ans = min(ans, node->val - prev);
            prev = node->val;

            dfs(node->right);
        }
    public:
        int getMinimumDifference(TreeNode* root) {
            dfs(root);
            return ans;
        }
    };
}

// 同s530，但代码写的更规范
namespace s783m1
{
    class Solution {
    public:
        int minDiffInBST(TreeNode* root) {
            int prev = -1e5;
            int ans = INT_MAX;

            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;
                self(self, node->left);
                ans = min(ans, node->val - prev);
                prev = node->val;
                self(self, node->right);
                };

            dfs(dfs, root);
            return ans;
        }
    };
}

// m1没有用到二叉搜索树的性质，只进行了基本的遍历，o1,o2为更好的答案
namespace s938m1
{
    class Solution {
    public:
        int rangeSumBST(TreeNode* root, int low, int high) {
            int ans = 0;

            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;

                self(self, node->left);
                if (node->val >= low && node->val <= high) {
                    ans += node->val;
                }
                self(self, node->right);
                };

            dfs(dfs, root);
            return ans;
        }
    };
}
namespace s938o1
{   // 跟中序遍历没什么关系，有点像自底向上的后序遍历
    class Solution {
    public:
        int rangeSumBST(TreeNode* root, int low, int high) {
            if (!root) return 0;

            if (root->val > high) {
                return rangeSumBST(root->left, low, high);
            }
            if (root->val < low) {
                return rangeSumBST(root->right, low, high);
            }
            return root->val + rangeSumBST(root->left, low, high) + rangeSumBST(root->right, low, high);
        }
    };
}
namespace s938o2
{   // 前序遍历写法
    class Solution {
    public:
        int rangeSumBST(TreeNode* root, int low, int high) {
            if (!root) return 0;

            int sum = 0;
            // 前序：先处理当前节点
            if (root->val >= low && root->val <= high) {
                sum += root->val;
            }

            // 只有当前值 > low 时，左子树才可能有有效值（否则无需递归）
            if (root->val > low) {
                sum += rangeSumBST(root->left, low, high);
            }
            // 只有当前值 < high 时，右子树才可能有有效值（否则无需递归）
            if (root->val < high) {
                sum += rangeSumBST(root->right, low, high);
            }

            return sum;
        }
    };
}

// 最简单的做法是整个遍历一遍用哈希表，但是如果想要用常数空间（不计算递归栈调用占用），那就需要用到中序递归遍历
namespace s501o1
{
    class Solution {
    public:
        vector<int> findMode(TreeNode* root) {
            int mxCnt = 0;
            int cnt = 0;
            int prev = INT_MIN;
            vector<int> ans;

            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;

                self(self, node->left);

                cnt = prev == node->val ? cnt + 1 : 1;
                prev = node->val;

                if (cnt == mxCnt) {
                    ans.push_back(node->val);
                }
                if (cnt > mxCnt) {
                    mxCnt = cnt;
                    ans.clear();
                    ans.push_back(node->val);
                }

                self(self, node->right);
                };

            dfs(dfs, root);
            return ans;
        }
    };
}

namespace s230m1
{
    class Solution {
    public:
        int kthSmallest(TreeNode* root, int k) {
            int ans = 0;

            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;

                self(self, node->left);

                if (--k == 0) {
                    ans = node->val;
                }

                self(self, node->right);
                };

            dfs(dfs, root);
            return ans;
        }
    };
}
namespace s230m2
{   // 迭代法中序遍历，更好
    // 如果是求BST中的第K大元素，那么遍历顺序改成右中左即可
    class Solution {
    public:
        int kthSmallest(TreeNode* root, int k) {
            int i = 0;
            stack<TreeNode*> stk;
            TreeNode* node = root;
            while (node || !stk.empty()) {
                while (node) {
                    stk.push(node);
                    node = node->left;
                }

                node = stk.top();
                stk.pop();
                if (++i == k) {
                    return node->val;
                }
                node = node->right;
            }
            return -1;
        }
    };
}

// o1为迭代法，o2为递归法
namespace s99o1
{   // 迭代法
    /*          为什么选第一次的前一个节点和第二次的后一个节点？
    假设正常递增序列为 '......a1、a2、a3、......、b1、b2、b3......'；此时规律是 a1 < a2 < a3
    当a1和b1交换时 '......a1、b2、a3、......、b1、a2、b3......'；a1和b2间的关系还是 a1 < b2，
    但改变的关系是 b2 > a3，即交换后，第一次出现冲突，是在 交换结点和其后结点 上发生的，
    所以选第一次冲突的 "前一个结点"，第二次冲突选后一个结点同理
    */
    class Solution {
    public:
        void recoverTree(TreeNode* root) {
            stack<TreeNode*> stk;
            TreeNode* node = root;

            TreeNode* firstError = nullptr;
            TreeNode* secondError = nullptr;
            TreeNode* prev = nullptr;

            while (!stk.empty() || node) {
                while (node) {
                    stk.push(node);
                    node = node->left;
                }
                node = stk.top();
                stk.pop();

                // 中序遍历框架：此处为节点处理
                if (prev && !firstError && node->val < prev->val) {
                    firstError = prev;
                }
                // 需要连着两个if，因为可能两个错误节点可能正好相邻
                if (firstError && node->val < prev->val) {
                    secondError = node;
                    // 不能在这里立刻交换node和firstError并提前返回，不能默认错误节点相邻，
                    // 如果不相邻，secondError会写入两次，后面那次才是正确的
                }
                prev = node;

                node = node->right;
            }
            swap(firstError->val, secondError->val);
        }
    };
}
namespace s99o2
{
    class Solution {
    public:
        void recoverTree(TreeNode* root) {
            TreeNode* firstError = nullptr;
            TreeNode* secondError = nullptr;
            TreeNode* prev = nullptr;

            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;
                self(self, node->left);

                if (prev && !firstError && node->val < prev->val) {
                    firstError = prev;
                }
                if (firstError && node->val < prev->val) {
                    secondError = node;
                }
                prev = node;

                self(self, node->right);
                };

            dfs(dfs, root);
            swap(firstError->val, secondError->val);
        }
    };
}

// 模板题10：o1为逆天写法，务必要看懂，二叉树+哨兵，对递归的理解，递归与迭代的转化帮助很大
namespace s897m1
{   // 有了o1写法，那m1就不用再看了
    class Solution {
    public:
        TreeNode* increasingBST(TreeNode* root) {
            vector<TreeNode*> nodes;

            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;
                self(self, node->left);
                nodes.push_back(node);
                self(self, node->right);
                };

            dfs(dfs, root);

            TreeNode* prev = nodes[0];
            for (int i = 1; i < nodes.size(); ++i) {
                prev->left = nullptr;
                prev->right = nodes[i];
                prev = nodes[i];
            }
            // 单独处理最后一个节点
            nodes[nodes.size() - 1]->left = nullptr;
            // nodes[nodes.size() - 1]->right = nullptr; 右孩子不用处理，因为最后一个节点右孩子一定是nullptr
            return nodes[0];
        }
    };
}
namespace s897o1
{   // Update：时隔几个月后第一次写就写出了o1写法，应该不会再忘了
    class Solution {
    public:
        TreeNode* increasingBST(TreeNode* root) {
            // 二叉树也可以用哨兵，规避prev初始为空的问题，也返回方便答案
            TreeNode dummy;
            TreeNode* cur = &dummy;

            // 可以这样想，中序遍历的中间节点处理部分，一定是按照中序遍历的顺序依次处理节点的
            // 只要处理的时候不要改后面的节点，就不会影响中序遍历的顺序
            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;

                self(self, node->left);

                node->left = nullptr;// 没问题，因为当前节点的左子树已经递归完了
                cur->right = node;  // 没问题，因为prev的处理已经结束了，想怎么改怎么改
                cur = node;

                self(self, node->right);
                };

            dfs(dfs, root);
            return dummy.right;
        }
    };
}
namespace s897o2
{   // 理解迭代和递归的区别后，迭代和递归两者转化非常简单，只要记住两者的基本框架即可
    class Solution {
    public:
        TreeNode* increasingBST(TreeNode* root) {
            TreeNode dummy;
            TreeNode* prev = &dummy;

            stack<TreeNode*> stk;
            TreeNode* node = root;
            while (!stk.empty() || node) {
                while (node) {
                    stk.push(node);
                    node = node->left;
                }
                node = stk.top();
                stk.pop();
                
                node->left = nullptr;
                prev->right = node;
                prev = node;

                node = node->right;
            }

            return dummy.right;
        }
    };
}

namespace s653m1
{   // 这个解法完全没有用到二叉搜索树的性质，但也没有什么更优的解法，空间复杂度怎么样都要O(n)
    class Solution {
    public:
        bool findTarget(TreeNode* root, int k) {
            unordered_set<int> st;
            stack<TreeNode*> stk;
            TreeNode* node = root;
            while (!stk.empty() || node) {
                while (node) {
                    stk.push(node);
                    node = node->left;
                }
                node = stk.top();
                stk.pop();

                if (st.count(k - node->val)) {
                    return true;
                }
                st.insert(node->val);

                node = node->right;
            }
            return false;
        }
    };
}
// ---------------------
// 【2.10】创建二叉树 (2)
// 一般是给定一个数组，根据题干规则生成一颗二叉树（如BST），初见确实做不出来，多看题解吧
/*

*/
// ---------------------
// 模板题11：创建BST，将创建二叉树的过程分解为一个个子问题，用递归来解决
namespace s108o1
{   // 将数组从中间一分为二，左边和右边的子数组也转化成平衡二叉搜索树，变成子问题，用递归来解决
    // 递归边界：如果数组长度等于 0，返回空节点。
    // 答案可能不是唯一的。如果 n 是偶数，我们可以取数组正中间左边那个数作为根节点的值，
    // 也可以取数组正中间右边那个数作为根节点的值。下面代码取的是正中间右边那个数，即下标为 n / 2（当 n 是偶数时）

    // 为什么生成的二叉树一定是平衡的
    // 因为int m = left + (right - left) / 2 这个计算确保了每次都选取当前子数组的中间元素，
    // 因此左右子树的节点数量最多相差1，也即左右子树的高度差最大为1，生成的树一定是平衡的
    class Solution {
    private:
        TreeNode* dfs(vector<int>& nums, int left, int right) {
            // [left,right) 的定义是左闭右开，所以 left = right 表示空节点
            if (left == right) {
                return nullptr;
            }
            int m = left + (right - left) / 2;
            return new TreeNode(nums[m], dfs(nums, left, m), dfs(nums, m + 1, right));
        }
    public:
        TreeNode* sortedArrayToBST(vector<int>& nums) {
            return dfs(nums, 0, nums.size());
        }
    };
}

// 基本和s108一样，同样和左闭右开区间即可，递归边界同样是left == right
namespace s654m1
{
    class Solution {
    private:
        TreeNode* dfs(vector<int>& nums, int left, int right) {
            if (left == right) return nullptr;
            int index = max_element(nums.begin() + left, nums.begin() + right) - nums.begin();
            return new TreeNode(nums[index], dfs(nums, left, index), dfs(nums, index + 1, right));
        }
    public:
        TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
            return dfs(nums, 0, nums.size());
        }
    };
}

// 模板题11.5：热门题的漏网之鱼，初见自己最多能做出O(n * n)做法
namespace s105o1
{   // 稍微容易点的暴力时间复杂度O(n * n)做法：
    // 从前序遍历中找到根节点，然后在中序遍历中查找根节点元素所在下标
    // 也就知道了根节点左右子树的节点数量。比如n = 6, 中序遍历中root_index = 3
    // 那么左子树是0，1，2，共3个节点。右子树是4，5，共2个节点
    // 前序遍历的下标0是根节点，向右的3个节点都是左子树节点，剩下的2个是右子树节点
    // 此时分别知道根节点的左子树和右子树的前序/中序遍历数组，问题规模变小，然后继续递归下去

    class Solution {
    public:
        TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
            if (preorder.empty()) {
                return nullptr;
            }
            // O(n)的find，注意题干指出数组中无重复元素，可以直接简单调用find
            // 最坏情况是一条链，需要递归调用n次find，所以时间复杂度是O(n * n)
            auto root_iter = find(inorder.begin(), inorder.end(), preorder[0]);
            int left_size = root_iter - inorder.begin();
            // 每次递归都需要拷贝复制一次数组，空间复杂度是O(n * n)
            vector<int> pre_l(preorder.begin() + 1, preorder.begin() + left_size + 1);
            vector<int> pre_r(preorder.begin() + left_size + 1, preorder.end());
            vector<int> in_l(inorder.begin(), root_iter);
            vector<int> in_r(root_iter + 1, inorder.end());

            TreeNode* left = buildTree(pre_l, in_l);
            TreeNode* right = buildTree(pre_r, in_r);
            return new TreeNode(preorder[0], left, right);
        }
    };
}
namespace s105o2
{   // 用哈希表优化每次的find查询根节点
    // 把递归参数改成数组下标，避免复制数组
    class Solution {
    public:
        TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
            int n = preorder.size();
            unordered_map<int, int> mp;
            for (int i = 0; i < n; ++i) {
                mp[inorder[i]] = i;
            }
            // 这里的preL, preR等变量的含义和o1完全不同
            // 表示当前完整的前序中序遍历的左右端点下标，左闭右开
            auto dfs = [&](auto&& self, int preL, int preR, int inL)->TreeNode* {
                if (preL == preR) {
                    return nullptr;
                }
                int index = mp[preorder[preL]];
                int leftSize = index - inL;
                // 递归中真正用到的参数只有preL和preR用来判断是否退出递归
                // 以及inL来计算leftSize
                // 注意，这里加上inL并计算rightSize提高代码可读性也是没问题的
                TreeNode* left = self(self, preL + 1, preL + 1 + leftSize, inL);
                TreeNode* right = self(self, preL + 1 + leftSize, preR, index + 1);
                return new TreeNode(preorder[preL], left, right);
                };

            return dfs(dfs, 0, n, 0); // 左闭右开区间
        }
    };
}

// s105变化一点点，会做s105就会s106
namespace s106m1
{
    class Solution {
    public:
        TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
            int n = inorder.size();
            unordered_map<int, int> mp;
            for (int i = 0; i < n; ++i) {
                mp[inorder[i]] = i;
            }

            auto dfs = [&](auto&& dfs, int posL, int posR, int inL)->TreeNode* {
                if (posL == posR) {
                    return nullptr;
                }

                int index = mp[postorder[posR - 1]];
                int leftSize = index - inL;
                TreeNode* left = dfs(dfs, posL, posL + leftSize, inL);
                TreeNode* right = dfs(dfs, posL + leftSize, posR - 1, index + 1);
                return new TreeNode(postorder[posR - 1], left, right);
                };
            return dfs(dfs, 0, n, 0);
        }
    };
}
// ---------------------
// 【2.11】插入/删除节点 (2)
// 和2.10创建二叉树一样，初见做不出来，直接看题解，考验分类讨论，基本是二叉树里最难的一类题目
// 这类问题递归法都比较容易写和理解，迭代法则更多的直接考验对树结构本身的理解，用不到stack，而是需要记录父节点
/*
*/
// ---------------------
// 模板题12：加深BST插入的理解
namespace s701m1
{   // 二叉搜索树的插入，总是存在一个不需调整树结构，就能插入新节点的方法
    // 递归法：
    class Solution {
    public:
        TreeNode* insertIntoBST(TreeNode* root, int val) {
            if (!root) {
                root = new TreeNode(val);
            }
            else if (root->val > val) {
                root->left = insertIntoBST(root->left, val);
            }
            else {
                root->right = insertIntoBST(root->right, val);
            }
            return root;
        }
    };
}
namespace s701o1
{   // 迭代法
    class Solution {
    public:
        TreeNode* insertIntoBST(TreeNode* root, int val) {
            TreeNode* newNode = new TreeNode(val);
            // 节点数可能为0
            if (!root) return newNode;

            TreeNode* cur = root;
            TreeNode* parent = nullptr;

            // 寻找合适的插入位置
            while (cur) {
                parent = cur; // 保存父节点
                if (cur->val > val) {
                    cur = cur->left;
                }
                else {
                    cur = cur->right;
                }
            }

            // 根据值与父节点比较，决定插入左还是右
            if (val < parent->val) {
                parent->left = newNode;
            }
            else {
                parent->right = newNode;
            }

            return root;
        }
    };
}

// 模板题13：锻炼分类讨论能力，题目比较难，虽然有取巧的方法，但是会让树退化成链表，增加树的高度
namespace s450o1
{   // 递归法（本题不需要自己去delete被删除的节点，可能leetcode有外部的函数进行了内存释放操作
    // 自行添加delete会报错，可能导致了二次释放
    /*
    1.root 为空，代表未搜索到值为 key 的节点，返回空。
    2.root.val>key，表示值为 key 的节点可能存在于 root 的左子树中，需要递归地在 root.left 调用 deleteNode，并返回 root。
    3.root.val<key，表示值为 key 的节点可能存在于 root 的右子树中，需要递归地在 root.right 调用 deleteNode，并返回 root。
    4.root.val=key，root 即为要删除的节点。此时要做的是删除 root，并将它的子树合并成一棵子树，保持有序性，并返回根节点。
    根据 root 的子树情况分成以下情况讨论：
        4.1root 为叶子节点，没有子树。此时可以直接将它删除，即返回空。
        4.1root 只有左子树，没有右子树。此时可以将它的左子树作为新的子树，返回它的左子节点。
        4.1root 只有右子树，没有左子树。此时可以将它的右子树作为新的子树，返回它的右子节点。
        4.1root 有左右子树，这时可以将 root 的后继节点（比 root 大的最小节点，即它的右子树中的最小节点，记为 successor）
        作为新的根节点替代 root，并将 successor 从 root 的右子树中删除，使得在保持有序性的情况下合并左右子树。
        简单证明，successor 位于 root 的右子树中，因此大于 root 的所有左子节点；successor 是 root 的右子树中的最小节点，
        因此小于 root 的右子树中的其他节点。以上两点保持了新子树的有序性。
        在代码实现上，我们可以先寻找 successor，再删除它。successor 是 root 的右子树中的最小节点，
        可以先找到 root 的右子节点，再不停地往左子节点寻找，直到找到一个不存在左子节点的节点，
        这个节点即为 successor。然后递归地在 root.right 调用 deleteNode 来删除 successor。
        因为 successor 没有左子节点，因此这一步递归调用不会再次步入这一种情况。然后将 successor 更新为新的 root 并返回。
    */
    class Solution {
    public:
        TreeNode* deleteNode(TreeNode* root, int key) {
            if (root == nullptr) return nullptr;
            if (root->val > key) {
                root->left = deleteNode(root->left, key);
                return root;
            }
            if (root->val < key) {
                root->right = deleteNode(root->right, key);
                return root;
            }
            if (root->val == key) {
                if (!root->left && !root->right) {
                    return nullptr;
                }
                if (!root->right) {
                    return root->left;
                }
                if (!root->left) {
                    return root->right;
                }
                TreeNode* successor = root->right;
                while (successor->left) {
                    successor = successor->left;
                }
                root->right = deleteNode(root->right, successor->val);
                successor->right = root->right;
                successor->left = root->left;
                return successor;
            }
            return root;// 这一行永远不会运行，随便返回什么都可以，只是因为如果前面用if-else代码会非常不美观
        }
    };
}
namespace s450o2
{   // 迭代法，比o1递归法空间复杂度更低,O(1)
    // 但因为不能像递归那样保存父节点的消息，需要对cur和successor节点的父节点进行保存与额外处理
    class Solution {
    public:
        TreeNode* deleteNode(TreeNode* root, int key) {
            TreeNode* cur = root;
            TreeNode* curParent = nullptr;
            // 先尝试搜索node->val == key的节点，并记录其父节点
            while (cur && cur->val != key) {
                curParent = cur;
                if (cur->val > key) {
                    cur = cur->left;
                }
                else {
                    cur = cur->right;
                }
            }
            // node->val == key的节点不存在，直接返回原树
            if (!cur) {
                return root;
            }
            // 删除节点
            if (!cur->left && !cur->right) {
                cur = nullptr;
            }
            else if (!cur->right) {
                cur = cur->left;
            }
            else if (!cur->left) {
                cur = cur->right;
            }
            else {
                TreeNode* successor = cur->right;
                TreeNode* successorParent = cur;
                while (successor->left) {
                    successorParent = successor;
                    successor = successor->left;
                }
                // 重构successor的父节点的指针
                if (successorParent->val == cur->val) {
                    // 情况1: cur->right就是successor, cur的右子树没有左孩子
                    successorParent->right = successor->right;
                    // cur->right = cur->right->right; 和这行代码等效
                }
                else {
                    // 情况2: successor是cur右子树的某个左子树的最底左节点
                    successorParent->left = successor->right;
                }
                // 用successor替代cur
                successor->left = cur->left;
                successor->right = cur->right;
                cur = successor;
            }

            // 重构cur的父节点的指针
            if (!curParent) {
                // cur在根节点的位置，但被替换成了succesor，不能return root
                return cur;
            }
            else {
                if (curParent->left && curParent->left->val == key) {
                    curParent->left = cur;
                }
                else {
                    curParent->right = cur;
                }
                return root;
            }
        }
    };

}

// 模板题14：继续强化分类讨论能力，比s450难度稍低一些
namespace s669o1
{   // 递归法比较简单，但是有递归的栈空间开销
    class Solution {
    public:
        TreeNode* trimBST(TreeNode* root, int low, int high) {
            if (!root) {
                return nullptr;
            }
            if (root->val < low) {
                // 说明root以及其左子树都不在[low, high]内，需要全部删掉
                return trimBST(root->right, low, high);
            }
            else if (root->val > high) {
                // 说明root以及其右子树都不在[low, high]内，需要全部删掉
                return trimBST(root->left, low, high);
            }
            else {
                // root符合要求，更新其左右子树
                root->left = trimBST(root->left, low, high);
                root->right = trimBST(root->right, low, high);
                return root;
            }
        }
    };
}
namespace s669o2
{   // 迭代法，空间复杂度O(1)
    /*
    我们先讨论左子树的修剪：
    1.node 的左结点为空结点：不需要修剪
    2.node 的左结点非空：
        2.1如果它的左结点 left 的值小于 low，那么 left 以及 left 的左子树都不符合要求，
        我们将 node 的左结点设为 left 的右结点，然后再重新对 node 的左子树进行修剪。
        2.2如果它的左结点 left 的值大于等于 low，又因为 node 的值已经符合要求，
        所以 left 的右子树一定符合要求。基于此，我们只需要对 left 的左子树进行修剪。
        我们令 node 等于 left ，然后再重新对 node 的左子树进行修剪。 
    以上过程可以迭代处理。对于右子树的修剪同理。
    */
    class Solution {
    public:
        TreeNode* trimBST(TreeNode* root, int low, int high) {
            // 从根节点开始大块的预处理修剪删除掉一些节点得到新root
            while (root && (root->val < low || root->val > high)) {
                if (root->val < low) {
                    root = root->right;
                }
                else {// root->val > high
                    root = root->left;
                }
            }
            // 树内的元素全部删完了都没找到一个在[low, high]内的
            if (root == nullptr) {
                return nullptr;
            }
            // 对符合条件的新root的左右子树进行判断
            TreeNode* node = root;
            // 新root的左子树处理
            while (node->left) {
                if (node->left->val < low) {
                    // node->left以及其左子树可以全部删除
                    node->left = node->left->right;
                }
                else {
                    // 删除的范围还需要继续细化，继续向左，更深搜索
                    node = node->left;
                }
            }
            // 新root的右子树处理
            node = root;
            while (node->right) {
                if (node->right->val > high) {
                    node->right = node->right->left;
                }
                else {
                    node = node->right;
                }
            }
            // 返回更新后的root
            return root;
        }
    };
}
// ---------------------
// 【2.12】树形DP ()
/*

*/
// ---------------------
// 暂时略过
// ---------------------
// 【2.13】二叉树BFS ()
// 104, 111, 112, 129, 199, 100
/*

*/
// ---------------------
// 模板题15：o1为双数组法，o2为队列迭代法（标准模板），o3为递归法
namespace s102o1
{
    class Solution {
    public:
        vector<vector<int>> levelOrder(TreeNode* root) {
            if (!root) return {};

            vector<vector<int>> ans;
            vector<TreeNode*> cur = { root };
            while (!cur.empty()) {
                vector<TreeNode*> nxt;
                vector<int> vals;
                for (auto node : cur) {
                    vals.push_back(node->val);
                    if (node->left) {
                        nxt.push_back(node->left);
                    }
                    if (node->right) {
                        nxt.push_back(node->right);
                    }
                }
                cur = nxt;
                ans.push_back(vals);
            }
            return ans;
        }
    };
}
namespace s102o2
{   // 迭代法，需注意：迭代法不一定需要在while内部进行for循环，见6.3 s112o2解法，加深BFS迭代法的理解
    class Solution {
    public:
        vector<vector<int>> levelOrder(TreeNode* root) {
            queue<TreeNode*> que;
            vector<vector<int>> ans;
            
            if (root) que.push(root);
            while (!que.empty()) {
                int n = que.size();// n为该层的结点数量
                vector<int> vec(n);
                // for循环的判断条件不能写i < que.size()，因为que.size()大小会在循环中改变
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();
                    vec[i] = node->val;
                    if (node->left) que.push(node->left); 
                    if (node->right) que.push(node->right);
                }
                ans.push_back(move(vec)); // 避免复制
            }
            return ans;
        }
    };
}
namespace s102o3
{   // 递归法（注：这种递归方式只对简单打印有效，不是模板，复杂操作不能硬套，一般需要全局的vector变量辅助）
    // 而且实际遍历数据的过程是前序遍历，可以对于s144o1递归写法，只是答案正确，最好还是用o2写法
    class Solution {
    private:
        void order(TreeNode* node, vector<vector<int>>& vec, int depth) {
            if (node == nullptr) return;
            if (vec.size() == depth) {
                vec.push_back({});
            }
            vec[depth].push_back(node->val);
            order(node->left, vec, depth + 1);
            order(node->right, vec, depth + 1);
        }
    public:
        vector<vector<int>> levelOrder(TreeNode* root) {
            vector<vector<int>> ans;
            order(root, ans, 0);
            return ans;
        }
    };
}

namespace s107m1
{
    class Solution {
    public:
        vector<vector<int>> levelOrderBottom(TreeNode* root) {
            if (!root) return {};
            queue<TreeNode*> que;
            vector<vector<int>> ans;
            que.push(root);
            while (!que.empty()) {
                int n = que.size(); 
                vector<int> vec;

                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();
                    vec.push_back(node->val);
                    if (node->left) que.push(node->left);
                    if (node->right) que.push(node->right);
                }
                ans.push_back(vec);
            }
            // 最后reverse ans即可
            reverse(ans.begin(), ans.end());
            return ans;
        }
    };
}

// 锯齿状层次遍历，加个奇偶判断即可，没必要用双端队列deque
namespace s103o1
{
    class Solution {
    public:
        vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
            if (!root) return {};

            vector<vector<int>> ans;
            queue<TreeNode*> que;
            que.push(root);

            while (!que.empty()) {
                vector<int> vals;
                int n = que.size();
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();
                    vals.push_back(node->val);
                    if (node->left) que.push(node->left);
                    if (node->right) que.push(node->right);
                }
                if (ans.size() % 2) {// 可以直接用ans.size()，而不需要额外的int cnt
                    reverse(vals.begin(), vals.end());
                }
                ans.push_back(vals);
            }
            return ans;
        }
    };
}

// 求最大深度，一般用DFS，但BFS也能做
namespace s104o2
{   // 套模板，空间复杂度O(n)
    class Solution {
    public:
        int maxDepth(TreeNode* root) {
            int depth = 0;
            queue<TreeNode*> que;
            if (root) que.push(root);
            while (!que.empty()) {
                int n = que.size();
                ++depth;
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();
                    if (node->left) que.push(node->left);
                    if (node->right) que.push(node->right);
                }
            }
            return depth;
        }
    };
}
// 求最小深度的BFS解
namespace s111o3
{   // 模板做法，只有当前结点的左右孩子都为空时，才判断为叶子节点
    class Solution {
    public:
        int minDepth(TreeNode* root) {
            int depth = 0;
            queue<TreeNode*> que;
            if (root) que.push(root);
            while (!que.empty()) {
                int n = que.size();
                ++depth;
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();
                    if (node->left) que.push(node->left);
                    if (node->right) que.push(node->right);
                    if (!node->left && !node->right) return depth;
                }
            }
            return depth;// root = nullptr 的情况也包含了
        }
    };
}

// 模板题16：双队列求解，路径总和的BFS解，一般不这么做，DFS更适合这道题，本题用于加深BFS迭代的理解
namespace s112o2
{   // BFS迭代写法，维护两个队列，本题用于加深BFS迭代的理解
    // 一个队列维护节点，一个队列维护对应节点路径下值的总和
    // 由于node-val与node都是同时入队出队，所以各自的总和值也是能对应上的
    // BFS不一定需要记录int n = que.size()和for循环，只要队列去记录就能保持层序遍历的顺序
    class Solution {
    public:
        bool hasPathSum(TreeNode* root, int targetSum) {
            if (root == nullptr) return false;

            queue<TreeNode*> queNode;
            queue<int> queVal;
            queNode.push(root);
            queVal.push(root->val);

            while (!queNode.empty()) {
                TreeNode* node = queNode.front();
                int temp = queVal.front();
                queNode.pop(); queVal.pop();

                if (node->left == nullptr && node->right == nullptr) {
                    if (targetSum == temp) {
                        return true;
                        // 不能像递归一样写成return target == temp
                        // 因为可能在其他分支出现答案，这里没找到接着找
                    }
                }

                if (node->left) {
                    queNode.push(node->left);
                    queVal.push(node->left->val + temp);
                }
                if (node->right) {
                    queNode.push(node->right);
                    queVal.push(node->right->val + temp);
                }
            }
            return false;
        }
    };
}

// 与s112一致，双队列求解，一个队列储存节点，一个队列储存路径的值
namespace s129o1
{   // 解法思路和s112o2一致，不解释了
    class Solution {
    public:
        int sumNumbers(TreeNode* root) {
            long long result = 0;
            if (root == nullptr) {
                return result;
            }

            queue<TreeNode*> queNode;
            queue<int> queVal;
            queNode.push(root);
            queVal.push(root->val);

            while (!queNode.empty()) {
                TreeNode* node = queNode.front();
                int temp = queVal.front();
                queNode.pop(); queVal.pop();

                if (node->left == nullptr && node->right == nullptr) {
                    result += temp;
                }

                if (node->left) {
                    queNode.push(node->left);
                    queVal.push(node->left->val + 10 * temp);
                }
                if (node->right) {
                    queNode.push(node->right);
                    queVal.push(node->right->val + 10 * temp);
                }
            }
            return result;
        }
    };
}

// 二叉树右视图，BFS更适合
namespace s199o2
{   // 迭代法：在每层循环中，只输出最后一个结点即为右视图，如果只输出第一个元素则为左视图
    class Solution {
    public:
        vector<int> rightSideView(TreeNode* root) {
            vector<int> ans;
            queue<TreeNode*> que;
            if (root) que.push(root);
            while (!que.empty()) {
                int n = que.size();
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();
                    if (i == n - 1) ans.push_back(node->val);// 只输出每层的最后一个即可
                    // 或者改成if (i == 0)，然后下面先push node->right再push node->left
                    if (node->left) que.push(node->left);
                    if (node->right) que.push(node->right);
                }
            }
            return ans;
        }
    };
}

// 套模板题16
namespace s1448o2
{   // BFS迭代法，两个队列，关键在于queVal.push(max(temp, node->val))，一直维护该条路径下的节点最大值
    class Solution {
    public:
        int goodNodes(TreeNode* root) {
            int ans = 0;
            queue<TreeNode*> queNode;
            queue<int> queVal;
            queNode.push(root);
            queVal.push(root->val);

            while (!queNode.empty()) {
                TreeNode* node = queNode.front();
                int temp = queVal.front();
                queNode.pop(); queVal.pop();

                if (node->val >= temp) {
                    ++ans;
                }
                if (node->left) {
                    queNode.push(node->left);
                    queVal.push(max(temp, node->val));
                }
                if (node->right) {
                    queNode.push(node->right);
                    queVal.push(max(temp, node->val));
                }
            }
            return ans;
        }
    };
}

// DFS简单的多得多，这道题用BFS同样是为了加深理解，这两个解法仅作了解
namespace s100o2
{   // BFS迭代法，定义两个队列，最直接的做法，但是稍显啰嗦，优先用o3
    class Solution {
    public:
        bool isSameTree(TreeNode* p, TreeNode* q) {
            if (p == nullptr || q == nullptr) {
                return p == nullptr && q == nullptr;
            }
            queue<TreeNode*> que1, que2;
            que1.push(p);
            que2.push(q);

            while (!que1.empty() && !que2.empty()) {
                TreeNode* pnode = que1.front();
                TreeNode* qnode = que2.front();
                que1.pop(); que2.pop();

                if (pnode->val != qnode->val) {
                    return false;
                }

                if ((pnode->left == nullptr) ^ (qnode->left == nullptr)) {// 按位异或，左边和右边只要不同，异或结果就为1，通过条件判断return false
                    return false;
                }
                if ((pnode->right == nullptr) ^ (qnode->right == nullptr)) {
                    return false;
                }

                if (pnode->left) que1.push(pnode->left);
                if (pnode->right) que1.push(pnode->right);
                if (qnode->left) que2.push(qnode->left);
                if (qnode->right) que2.push(qnode->right);
            }
            return que1.empty() && que2.empty();
        }
    };
}
namespace s100o3
{   // s100o2解法的改良版，BFS迭代法不一定只能把非空节点放进队列，nullptr指针放进去也可以！
    // 此解法只用到一个队列
    class Solution {
    public:
        bool isSameTree(TreeNode* p, TreeNode* q) {
            queue<TreeNode*> que;
            que.push(p);
            que.push(q);
            while (!que.empty()) {
                TreeNode* pnode = que.front();
                que.pop();
                TreeNode* qnode = que.front();
                que.pop();
                if (pnode == nullptr && qnode == nullptr) {
                    // continue之后，下面的pnode->left, qnode->left等代码就不会运行，不会产生错误越界访问
                    continue;
                }
                // 此处为短路求值，pnode和qnode值的比较要放在最后一个判断条件处，保障两者都是非空节点
                if (pnode == nullptr || qnode == nullptr || pnode->val != qnode->val) {
                    return false;
                }
                que.push(pnode->left);// 这里没有判断是否为空指针
                que.push(qnode->left);// 顺序必须要是两颗树各入队一个，且要对应
                que.push(pnode->right);
                que.push(qnode->right);
            }
            return true;
        }
    };
}

// BFS解法，与s100几乎相同
namespace s101o1
{   // 代码和s100几乎一样
    class Solution {
    public:
        bool isSymmetric(TreeNode* root) {
            queue<TreeNode*> que;
            // 先分别push root->left 和 root->right，就相当于s100中的p和q
            que.push(root->left);
            que.push(root->right);
            while (!que.empty()) {
                TreeNode* node1 = que.front(); que.pop();
                TreeNode* node2 = que.front(); que.pop();
                if (!node1 && !node2) {
                    continue;
                }
                if (!node1 || !node2 || node1->val != node2->val) {
                    return false;
                }
                // 入队顺序改变，相对于s100有变化，node1的左和node2的右对应，才能判断是否为对称
                que.push(node1->left);
                que.push(node2->right);
                que.push(node1->right);
                que.push(node2->left);
            }
            return true;
        }
    };
}

// 跟DFS迭代法写法几乎一模一样
namespace s226o2
{   
    class Solution {
    public:
        TreeNode* invertTree(TreeNode* root) {
            queue<TreeNode*> que;
            if (root) que.push(root);
            while (!que.empty()) {
                // 这里也不需要用到int n = que.size() + for loop
                TreeNode* node = que.front();
                que.pop();
                swap(node->left, node->right);
                if (node->left) que.push(node->left);
                if (node->right) que.push(node->right);
            }
            return root;
        }
    };
}

namespace s513m1
{   // 最容易想到的做法：层序遍历模板 + 最后一层的第一个元素就是答案
    class Solution {
    public:
        int findBottomLeftValue(TreeNode* root) {
            queue<TreeNode*> que;
            int ans = root->val;
            que.push(root);
            while (!que.empty()) {
                int n = que.size();
                for (int i = 0; i < n; ++i) {// 甚至这个for循环都没必要...
                    TreeNode* node = que.front(); que.pop();
                    if (i == 0) ans = node->val;
                    if (node->left) que.push(node->left);
                    if (node->right) que.push(node->right);
                }
            }
            return ans;
        }
    };
}
namespace s513o1
{   
    class Solution {
    public:
        int findBottomLeftValue(TreeNode* root) {
            queue<TreeNode*> que;
            que.push(root);
            // node变量定义在while循环之外，node的最后指向的就是最下层最左边的节点
            TreeNode* node = nullptr;
            while (!que.empty()) {
                node = que.front(); que.pop();
                // 改变入队出队的顺序，此时每层从右到左遍历
                if (node->right) que.push(node->right);
                if (node->left) que.push(node->left);
            }
            return node->val;
        }
    };
}

namespace s515m1
{   // 依旧模板题，每层记录下最大值即可
    class Solution {
    public:
        vector<int> largestValues(TreeNode* root) {
            if (!root) return {};

            vector<int> ans;
            queue<TreeNode*> que;
            que.push(root);
            while (!que.empty()) {
                int n = que.size();
                int maxVal = INT_MIN;
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();
                    maxVal = max(maxVal, node->val);
                    if (node->left) que.push(node->left);
                    if (node->right) que.push(node->right);
                }
                ans.push_back(maxVal);
            }
            return ans;
        }
    };
}

namespace s637m1
{
    class Solution {
    public:
        vector<double> averageOfLevels(TreeNode* root) {
            vector<double> ans;
            queue<TreeNode*> que;
            que.push(root);
            while (!que.empty()) {
                int n = que.size();
                double sum = 0;
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();
                    sum += node->val;
                    if (node->left) que.push(node->left);
                    if (node->right) que.push(node->right);
                }
                ans.push_back(sum / n);
            }
            return ans;
        }
    };
}

// 这道题更适合用DFS做，不知道为什么灵神把这题放BFS里
namespace s993m1
{   // 虽然我的代码是正确的，但感觉是屎山
    // 确定x, y是否在同一层容易，但难点在于x, y需要拥有不同的父节点
    // 采用unordered_set来记录x, y的父节点，如果set大小为2，说明两者父节点不同，为堂兄弟
    class Solution {
    public:
        bool isCousins(TreeNode* root, int x, int y) {
            queue<TreeNode*> que;
            que.push(root);
            bool haveX = false;
            bool haveY = false;
            unordered_set<TreeNode*> parents;

            while (!que.empty()) {
                int n = que.size();
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();

                    if (node->left) {
                        que.push(node->left);
                        if (node->left->val == x) {
                            haveX = true;
                            parents.insert(node);
                        }
                        if (node->left->val == y) {
                            haveY = true;
                            parents.insert(node);
                        }
                    }
                    if (node->right) {
                        que.push(node->right);
                        if (node->right->val == x) {
                            haveX = true;
                            parents.insert(node);
                        }
                        if (node->right->val == y) {
                            haveY = true;
                            parents.insert(node);
                        }
                    }
                }
                if (haveX || haveY) {
                    return haveX && haveY && parents.size() == 2;
                }
            }
            return false;
        }
    };
}
namespace s993m2
{   // DFS做法（可以做一些优化让递归提前结束，当前做法会遍历完所有节点）
    class Solution {
    public:
        bool isCousins(TreeNode* root, int x, int y) {
            vector<pair<TreeNode*, int>> pairs;

            auto dfs = [&](auto&& self, TreeNode* node, TreeNode* parent, int depth) -> void {
                if (!node) return;
                if (node->val == x || node->val == y) {
                    pairs.emplace_back(parent, depth);
                }
                self(self, node->left, node, depth + 1);
                self(self, node->right, node, depth + 1);
                };

            dfs(dfs, root, nullptr, 0);
            if (pairs.size() == 2) {
                auto& p1 = pairs[0];
                auto& p2 = pairs[1];
                if (p1.first != p2.first && p1.second == p2.second) {
                    return true;
                }
            }
            return false;
        }
    };
}

// m1为BFS,o1为DFS
namespace s623m1
{
    class Solution {
    public:
        TreeNode* addOneRow(TreeNode* root, int val, int depth) {
            // depth == 1的情况单独处理
            if (depth == 1) {
                return new TreeNode(val, root, nullptr);
            }

            // depth >= 2
            queue<TreeNode*> que;
            que.push(root);
            int d = 1;
            while (!que.empty()) {

                int n = que.size();
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = que.front();
                    que.pop();

                    // 遍历到了新行的上一行
                    if (d == depth - 1) {
                        TreeNode* newNodeL = new TreeNode(val, node->left, nullptr);
                        TreeNode* newNodeR = new TreeNode(val, nullptr, node->right);
                        node->left = newNodeL;
                        node->right = newNodeR;
                        /* 上面四行可以简化为下面两行
                        node->left = new TreeNode(val, node->left, nullptr);
                        node->right = new TreeNode(val, nullptr, node->right);
                        */
                    }
                    else {
                        if (node->left) que.push(node->left);
                        if (node->right) que.push(node->right);
                    }
                }
                if (d == depth - 1) return root;// 新行已经添加完毕，直接返回
                ++d;
            }
            return root;// 这里返回什么都可以
        }
    };
}
namespace s623o1
{   // DFS的代码还是简洁优雅些
    class Solution {
    public:
        TreeNode* addOneRow(TreeNode* root, int val, int depth) {
            if (!root) {
                return nullptr;
            }
            if (depth == 1) {
                return new TreeNode(val, root, nullptr);
            }
            if (depth == 2) {
                root->left = new TreeNode(val, root->left, nullptr);
                root->right = new TreeNode(val, nullptr, root->right);
            }
            else {
                root->left = addOneRow(root->left, val, depth - 1);
                root->right = addOneRow(root->right, val, depth - 1);
            }
            return root;
        }
    };
}

// 模板题17：套模板题16 + 二叉树性质，根节点编号为index，则左子节点编号为2 * index, 右子节点为2 * index + 1
namespace s662m1
{   // 本题用到了双队列的技巧，一个队列存节点，一个队列存编号
    // 最大的坑是节点数目最大为3000，当二叉树退化为链时，节点编号将是个巨大的数字，甚至连long long都存不下，要用ULL
    class Solution {
    public:
        int widthOfBinaryTree(TreeNode* root) {
            queue<unsigned long long> indexs;
            queue<TreeNode*> nodes;
            indexs.push(1ULL);
            nodes.push(root);
            unsigned long long ans = 0;
            while (!nodes.empty()) {
                int n = nodes.size();
                unsigned long long mx = 0;
                unsigned long long mn = ULLONG_MAX;
                for (int i = 0; i < n; ++i) {
                    TreeNode* node = nodes.front();
                    unsigned long long idx = indexs.front();
                    nodes.pop();
                    indexs.pop();
                    // 只在i = 0时记录mn + 在i = n - 1时更新ans也可以
                    mx = max(mx, idx);
                    mn = min(mn, idx);
                    if (node->left) {
                        nodes.push(node->left);
                        indexs.push(2 * idx);
                    }
                    if (node->right) {
                        nodes.push(node->right);
                        indexs.push(2 * idx + 1);
                    }
                }
                ans = max(ans, mx - mn + 1);
            }
            return ans;
        }
    };
}

namespace s863m1
{   // 难点在于路径走到target后，还可能有新答案，需要继续深入

}

// ---------------------
// 【2.14】链表 + 二叉树 (3)
/*

*/
// ---------------------
// 模板题18：二叉树转成链表，o2o3为后序遍历的逆序，反向修改二叉树
// 最简单的是m2做法，但是需要用到额外O(n)空间，最好的做法是o1，空间复杂度O(1)
namespace s114o1
{   // 整体拼接法，非常巧妙，完美利用了树的性质，看windliang的题解
    // 思路跟morris遍历有点类似
    class Solution {
    public:
        void flatten(TreeNode* root) {
            while (root) {
                // 左子树为空，那么直接考虑下一个节点
                if (!root->left) {
                    root = root->right;
                    continue;
                }
                // 找左子树的最右边的节点
                TreeNode* pre = root->left;
                while (pre->right) {
                    pre = pre->right;
                }
                // 注意下面四行，经典的链表链式修改，非常优雅
                // 将原来的右子树接到左子树的最右边节点
                pre->right = root->right;
                // 将左子树插入到右子树的位置
                root->right = root->left;
                root->left = nullptr;
                // 考虑下一个节点
                root = root->right;
            }
        }
    };
}
namespace s114o2
{   // 先序遍历的逆序，因为如果直接按照先序遍历顺序逐步修改指针指向，会丢失之前父节点的右孩子，所以逆向思维
    // 按照遍历顺序： 右子树->左子树->根节点 进行遍历，逐步修改指针指向
    class Solution {
    public:
        void flatten(TreeNode* root) {
            TreeNode* prev = nullptr;

            auto dfs = [&](auto&& self, TreeNode* node) -> void {
                if (!node) return;
                self(self, node->right);
                self(self, node->left);
                node->right = prev;
                node->left = nullptr;
                prev = node;
                };

            dfs(dfs, root);
        }
    };
}
namespace s114o3
{   // o2写法的迭代版本
    class Solution {
    public:
        void flatten(TreeNode* root) {
            TreeNode* node = root;
            TreeNode* prev = nullptr;
            stack<TreeNode*> stk;
            while (!stk.empty() || node) {
                while (node) {
                    stk.push(node);
                    node = node->right;
                }
                node = stk.top();
                stk.pop();
                if (node->left && node->left != prev) {
                    stk.push(node);
                    node = node->left;
                }
                else {
                    node->right = prev;
                    node->left = nullptr;
                    prev = node;
                    node = nullptr;
                }
            }
        }
    };
}
namespace s114m1
{   // 时隔数月之后自己写出来的，而且接近秒杀
    // 直接按照前序遍历一遍二叉树即可，只要把当前节点的左右孩子都入栈了，就可以修改当前节点
    class Solution {
    public:
        void flatten(TreeNode* root) {
            if (!root) return;

            TreeNode dummy;
            TreeNode* cur = &dummy;
            stack<TreeNode*> stk;
            stk.push(root);

            while (!stk.empty()) {
                TreeNode* node = stk.top();
                stk.pop();
                if (node->right) stk.push(node->right);
                if (node->left) stk.push(node->left);
                cur->right = node;
                node->left = nullptr;
                cur = node;
            }
        }
    };
}
namespace s114m2
{   // 去掉dummy节点的版本
    class Solution {
    public:
        void flatten(TreeNode* root) {
            stack<TreeNode*> stk;
            TreeNode* prev = nullptr;
            if (root) stk.push(root);

            while (!stk.empty()) {
                TreeNode* node = stk.top(); stk.pop();
                if (node->right) stk.push(node->right);
                if (node->left) stk.push(node->left);
                if (prev) {
                    prev->right = node;
                    // node->left = nullptr; 
                    // 这行代码放进prev里就错了，因为头节点那里的左子树也必须要断开，即使prev = nullptr
                }
                node->left = nullptr;
                prev = node;
            }
        }
    };
}

// 模板题19：链表转成平衡BST，s108是转换有序数组，这题是链表，难度更高, o1做法要求掌握
namespace s109m1
{   // 比较暴力的做法是遍历一次链表将值存到数组里，然后用108代码解题
    // 空间复杂度O(n)，但这样空间复杂度为O(n)，可以优化为O(logn)
    class Solution {
    private:
        vector<int> nums;

        TreeNode* dfs(vector<int>& nums, int left, int right) {
            if (left == right) {
                return nullptr;
            }
            int m = left + (right - left) / 2;
            return new TreeNode(nums[m], dfs(nums, left, m), dfs(nums, m + 1, right));
        }
    public:
        TreeNode* sortedListToBST(ListNode* head) {
            while (head) {
                nums.push_back(head->val);
                head = head->next;
            }

            return dfs(nums, 0, nums.size());
        }
    };
}
namespace s109o1
{   // 空间复杂度仅由递归深度决定，为 O(log n)（栈空间），比起m1做法节省了O(n)的数组空间
    // 时间复杂度也为O(n)
    // 按照 中序遍历的顺序（左 → 根 → 右）来递归构建节点，同时让链表头指针 head 随着构建过程依次后移
    // 使用 ListNode*& head 引用传递，确保递归调用中链表指针的移动能正确影响后续调用

    class Solution {
    private:
        TreeNode* buildBST(int start, int end, ListNode*& head) {
            if (start > end) return nullptr;

            int mid = (start + end) / 2;   // 取中间索引（向下取整）
            // 先构建左子树（会消耗链表前面的节点）
            TreeNode* left = buildBST(start, mid - 1, head);
            // 当前根节点使用链表当前节点的值
            TreeNode* root = new TreeNode(head->val);
            head = head->next;            // 链表指针步进
            root->left = left;
            // 构建右子树
            root->right = buildBST(mid + 1, end, head);
            return root;
        }

    public:
        TreeNode* sortedListToBST(ListNode* head) {
            // 计算链表长度
            int len = 0;
            ListNode* cur = head;
            while (cur) {
                ++len;
                cur = cur->next;
            }
            // 递归构建，head 传引用使其在递归中能够步进
            return buildBST(0, len - 1, head);
        }
    };
}

// 模板题20：BFS解法每层从右到左更新，链表特化解法o2空间复杂度O(1)只能在这道题用（s117和本题解法相同，故不列出）
namespace s116m1
{   // 套模板，层序遍历的变体，right和left的顺序调换， 从右至左更新每个节点的next指针
    class Solution {
    public:
        Node* connect(Node* root) {
            if (!root) return nullptr;
            queue<Node*> que;
            que.push(root);

            while (!que.empty()) {
                int n = que.size();
                Node* prev = nullptr;

                for (int i = 0; i < n; ++i) {
                    Node* node = que.front();
                    que.pop();
                    node->next = prev;
                    prev = node;
                    if (node->right) que.push(node->right);
                    if (node->left) que.push(node->left);
                }
            }
            return root;
        }
    };
}
namespace s116o1
{   // DFS递归法
    class Solution {
    private:
        vector<Node*> vec;

        void dfs(Node* node, int depth) {
            if (!node) return;
            if (vec.size() == depth) {
                vec.push_back(node);
            }
            else {
                vec[depth]->next = node;
                vec[depth] = node;
            }
            dfs(node->left, depth + 1);
            dfs(node->right, depth + 1);
        }

    public:
        Node* connect(Node* root) {
            dfs(root, 0);
            return root;
        }
    };
}
namespace s116o2
{   // BFS + 链表：不是常规BFS遍历方式，只是因为这道题的特殊性才导致可行，空间复杂度O(1)
    // 每层进行cur = cur->next连接，同时当前层的dummy位置的下一个位置，就是下一层的开始
    class Solution {
    public:
        Node* connect(Node* root) {
            Node* cur = root;
            while (cur) {
                // 为每一层（从第二层开始）创建一个哨兵节点（dummy），避免未初始化问题
                Node dummy;  // 栈上分配，自动释放内存
                // 也可以把dummy定义在while循环外，这里改成dummy.next = nullptr;
                Node* nxt = &dummy;  // nxt 指向 dummy 的地址
                // 遍历当前层，连接下一层
                while (cur) {
                    if (cur->left) {
                        nxt->next = cur->left;
                        nxt = nxt->next;
                    }
                    if (cur->right) {
                        nxt->next = cur->right;
                        nxt = nxt->next;
                    }
                    cur = cur->next; // 移动到当前层的下一个节点
                }
                // 移动到下一层的第一个节点（dummy.next 是下一层的头节点）
                cur = dummy.next;
            }
            return root;
        }
    };
}
// ---------------------
// 【2.15】N叉树 (4)
// 换汤不换药，只要熟练掌握二叉树的遍历方式，N叉树的遍历很容易写出来（注意，N叉树没有中序遍历）
/*

*/
// ---------------------
namespace s589m1
{
    class Solution {
    private:
        vector<int> ans;
        void dfs(Node* node) {
            if (!node) return;
            ans.push_back(node->val);
            for (auto child : node->children) {
                dfs(child);
            }
        }
    public:
        vector<int> preorder(Node* root) {
            dfs(root);
            return ans;
        }
    };

    class Node {
    public:
        int val;
        vector<Node*> children;

        Node() {}

        Node(int _val) {
            val = _val;
        }

        Node(int _val, vector<Node*> _children) {
            val = _val;
            children = _children;
        }
    };
}
namespace s589m2
{
    class Solution {
    public:
        vector<int> preorder(Node* root) {
            if (!root) return {};

            vector<int> ans;
            stack<Node*> stk;
            stk.push(root);
            while (!stk.empty()) {
                Node* node = stk.top();
                stk.pop();
                ans.push_back(node->val);
                for (auto it = node->children.rbegin(); it != node->children.rend(); ++it) {
                    if (*it) {
                        stk.push(*it);
                    }
                }
            }
            return ans;
        }
    };

    class Node {
    public:
        int val;
        vector<Node*> children;

        Node() {}

        Node(int _val) {
            val = _val;
        }

        Node(int _val, vector<Node*> _children) {
            val = _val;
            children = _children;
        }
    };
}

namespace s590m1
{
    class Solution {
    private:
        vector<int> ans;
        void dfs(Node* node) {
            if (!node) return;
            ans.push_back(node->val);
            vector<Node*>& vec = node->children;
            for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
                dfs(*it);
            }
        }

    public:
        vector<int> postorder(Node* root) {
            dfs(root);
            reverse(ans.begin(), ans.end());
            return ans;
        }
    };

    class Node {
    public:
        int val;
        vector<Node*> children;

        Node() {}

        Node(int _val) {
            val = _val;
        }

        Node(int _val, vector<Node*> _children) {
            val = _val;
            children = _children;
        }
    };

}

namespace s559m1
{
    class Solution {
    private:
        int dfs(Node* node, int depth) {
            if (!node) return depth;
            ++depth;
            int mx = depth;
            for (auto child : node->children) {
                mx = max(mx, dfs(child, depth));
            }
            return mx;
        }
    public:
        int maxDepth(Node* root) {
            return dfs(root, 0);
        }
    };

    class Node {
    public:
        int val;
        vector<Node*> children;

        Node() {}

        Node(int _val) {
            val = _val;
        }

        Node(int _val, vector<Node*> _children) {
            val = _val;
            children = _children;
        }
    };
}
namespace s559m2
{
    class Solution {
    public:
        int maxDepth(Node* root) {
            if (!root) return 0;
            int mx = 0;
            for (auto child : root->children) {
                mx = max(mx, maxDepth(child));
            }
            return mx + 1;
        }
    };

    class Node {
    public:
        int val;
        vector<Node*> children;

        Node() {}

        Node(int _val) {
            val = _val;
        }

        Node(int _val, vector<Node*> _children) {
            val = _val;
            children = _children;
        }
    };
}

namespace s429m1
{   
    class Solution {
    public:
        vector<vector<int>> levelOrder(Node* root) {
            if (!root) return {};

            queue<Node*> que;
            vector<vector<int>> result;
            que.push(root);

            while (!que.empty()) {
                int n = que.size();
                vector<int> vec;

                for (int i = 0; i < n; ++i) {
                    Node* node = que.front();
                    que.pop();
                    vec.push_back(node->val);
                    for (Node* child : node->children) {
                        if (child) {
                            que.push(child);
                        }
                    }
                }
                result.push_back(vec);
            }
            return result;
        }
    };
    class Node {
    public:
        int val;
        vector<Node*> children;

        Node() {}

        Node(int _val) {
            val = _val;
        }

        Node(int _val, vector<Node*> _children) {
            val = _val;
            children = _children;
        }
    };
}
// ---------------------
// 【2.16】其他 (4)
// 整体难度都比较高，没有确切的规律可言，需要对二叉树有很深的理解
/*

*/
// ---------------------

namespace s222m1
{   // 不利用完全二叉树的性质直接使用通用的节点个数计算递归方法
    class Solution {
    public:
        int countNodes(TreeNode* root) {
            if (!root) return 0;
            return countNodes(root->left) + countNodes(root->right) + 1;
        }
    };
}
namespace s222o1
{	// 利用完全二叉树的性质，完全二叉树一定可以拆成一颗满二叉树加一颗完全二叉树，借此完成递归
    // 总时间复杂度为O(logn * logn)，空间复杂度为O(logn)（logn层栈空间）
    class Solution {
    public:
        int countNodes(TreeNode* root) {
            if (root == nullptr) {
                return 0;
            }
            int leftDep = 0, rightDep = 0;
            TreeNode* leftChild = root->left;
            TreeNode* rightChild = root->right;
            while (leftChild) {
                leftChild = leftChild->left;
                ++leftDep;
            }
            while (rightChild) {
                rightChild = rightChild->right;
                ++rightDep;// 求左右子树深度的时间复杂度为O(logn)
            }
            if (leftDep == rightDep) {
                // 左右孩子都能走到底，说明是一颗满二叉树
                // leftDep 和 rightDep都是子树的高度，完整计算过程是 (1 << leftDep - 1) * 2 + 1，也相当于深度+1，初始为2
                return (2 << leftDep) - 1;// 用位移操作代替2的指数函数
            }
            // 这里的时间复杂度为O(logn)，因为一定有左右子树中的一个会立即停止递归（为满二叉树）
            return countNodes(root->left) + countNodes(root->right) + 1;
        }
    };
}
namespace s222o2
{   // 同样是运用完全二叉树性质加速
    class Solution {
    private:
        int getDepth(TreeNode* node) {
            int depth = 0;
            while (node) {
                node = node->left;
                ++depth;
            }
            return depth;
        }
    public:
        int countNodes(TreeNode* root) {
            if (!root) return 0;
            // 统计左右子树的深度（都以左孩子为例，跟o1的深度计算方式不同）
            int leftDepth = getDepth(root->left);
            int rightDepth = getDepth(root->right);
            if (leftDepth == rightDepth) {
                // 左子树深度等于右子树深度, 则左子树是满二叉树
                return countNodes(root->right) + (1 << leftDepth);
                // 位运算加速x2操作，但是记得加括号，因为位运算优先级很低
            }
            else {
                // 左子树深度大于右子树深度, 则右子树是满二叉树
                return countNodes(root->left) + (1 << rightDepth);
            }
        }
    };
}

// 下面三个都暂时不做，时间紧
namespace s297o1
{

}
namespace s449m1
{

}
namespace s652o1
{
    // 暂时没做
}