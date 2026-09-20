#pragma once
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

// 问题待定：
/*
1.
*/

/*
模板题：
1.树直径DP的最标准模板，递归的返回值不是答案，而是更新答案所需要的值：543

*/



// 十二、树形DP (3)
// 树形DP：树的直径 + 树上最大独立集 + 树上最小支配集 + 换根 DP（二次扫描法） + 其他树形DP

// 【12.1】树的直径
/*
543.二叉树的直径：给你一棵二叉树的根节点，返回该树的 直径 。
二叉树的 直径 是指树中任意两个节点之间最长路径的 长度 。这条路径可能经过也可能不经过根节点 root 。
两节点之间路径的 长度 由它们之间边数表示。

124.二叉树中的最大路径和：二叉树中的 路径 被定义为一条节点序列，序列中每对相邻节点之间都存在一条边。
同一个节点在一条路径序列中 至多出现一次 。该路径 至少包含一个 节点，且不一定经过根节点。
路径和 是路径中各节点值的总和。给你一个二叉树的根节点 root ，返回其 最大路径和 。
*/
// ---------------------
// 模板题1：树直径DP的最标准模板，递归的返回值不是答案，而是更新答案所需要的值
namespace s543o1
{	// 推荐直接看m1，更好懂
    
    /*	推论1：最长路径的起点和端点一定在叶子上（如果不是在叶子上，那就可以继续延伸）
		推论2：直径等价于由两条（或者一条）链拼成的路径。我们枚举每个 node，假设直径在这里「拐弯」，
			    也就是计算由左右两条从下面的叶子节点到 node 的链的节点值之和，去更新答案的最大值
                （直径一定是某个节点的左右高度之和，因为最长路径必然经过某个根节点）
		注1：dfs返回的是链的长度，不是直径的长度。如果返回直径，那么上面与其他的链继续拼接，得到的就不是直径了
		注2：dfs返回的是以当前node为根节点的子树的最大链长（也可以理解为子树的最大高度），不包含当前节点node和相连的边

		边界条件：空节点的链长是-1，叶子节点的链长是0

        如果对边界条件以及dfs的返回值含义有疑问，可以看m1，个人感觉更好懂
	*/
    class Solution {
    public:
        int diameterOfBinaryTree(TreeNode* root) {
            int ans = 0; // 初始化直径结果为0
            auto dfs = [&](auto&& dfs, TreeNode* node) -> int {
                if (node == nullptr) {
                    return -1; // 空节点的高度为-1（基准值）
                }

                // 在当前节点node拐弯的直径长度 = 左子树的最大链长 + 右子树的最大链长 + 2
                int l_len = dfs(dfs, node->left) + 1;  // 计算左子树高度加1：表示从当前节点到左子树最远叶子的边数
                int r_len = dfs(dfs, node->right) + 1; // 计算右子树高度加1：表示从当前节点到右子树最远叶子的边数
                ans = max(ans, l_len + r_len);    // 更新直径：通过当前节点的路径边数 = l_len + r_len
                return max(l_len, r_len);         // 返回当前节点的高度（最大边数）
                };

            dfs(dfs, root); // 从根节点开始DFS遍历
            return ans; // 返回最终直径
        }
    };

}
namespace s543m1
{
    // 没那么多的分析，只是在求最大深度的时候顺便更新答案
    class Solution {
    public:
        int diameterOfBinaryTree(TreeNode* root) {
            int ans = 0;
            auto dfs = [&](auto&& dfs, TreeNode* node)->int {
                // 空节点的深度就是0，不像o1一样边界条件需要凑
                if (!node) return 0;

                int left = dfs(dfs, node->left);// 左子树的最大深度
                int right = dfs(dfs, node->right);// 右子树的最大深度
                ans = max(ans, left + right);
                // 最大直径 = 左子树的最大深度 + 右子树的最大深度
                // 把节点当成一个一个“蝌蚪”，两边跟当前节点连在一起

                return max(left, right) + 1;
                };
            dfs(dfs, root);
            return ans;
        }
    };
}

// 跟s543类似，枚举当前节点，dfs(node)返回的是当前节点的左右子树的最大链和
namespace s124o1
{   // o1是沿用s543o1思路，可以看m1，更简单
    class Solution {
    public:
        int maxPathSum(TreeNode* root) {
            int ans = -1000;// 题干：node->val 在[-1000, 1000]范围上

            auto dfs = [&](auto&& dfs, TreeNode* node)->int {
                if (!node) return 0;// 这个边界条件是猜的，草稿纸上模拟了下还真是对的

                int l = dfs(dfs, node->left);
                int r = dfs(dfs, node->right);
                ans = max(ans, l + r + node->val);
                return max({ 0, l + node->val, r + node->val });// 如果是负值就不选，返回0
                };
            dfs(dfs, root);
            return ans;
        }
    };
}
namespace s124m1
{
    class Solution {
    public:
        int maxPathSum(TreeNode* root) {
            int ans = INT_MIN;
            auto dfs = [&](auto&& dfs, TreeNode* node)->int {
                if (!node) return 0;
                // dfs()的定义为，包含当前节点的路径的最大值
                int left = max(0, dfs(dfs, node->left));// 左子树的路径最大值如果为负，就不选
                int right = max(0, dfs(dfs, node->right));// 右子树同理
                ans = max(ans, node->val + left + right);
                return node->val + max(left, right);
                };
            dfs(dfs, root);
            return ans;
        }
    };
}

// （暂时跳过）模板题2：从二叉树到一般树的直径DP推广，引入邻居概念
namespace s2246o1
{
}
// ---------------------

/*

*/
// ---------------------




// ---------------------

