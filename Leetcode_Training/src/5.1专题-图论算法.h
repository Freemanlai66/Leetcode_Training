#pragma once
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// 问题待定：
/*
1.
*/

/*
模板题：
1.static constexpr静态变量的定义 + 回溯入门级题目，理解回溯思想:17

*/

// 图论算法：图的遍历（跳跃游戏） + 拓扑排序 + 最短路（Dijkstra + Floyd） + 最小生成树
// + 欧拉路径/欧拉回路 + 强连通分量/双连通分量 + 二分图染色 + 网络流 + 其他 + 树上算法

// 一、图的遍历()

// 【1.1】深度优先搜索（DFS）(1)
// 找连通块、判断是否有环（如207题拓扑排序）等。部分题目做法不止一种。
/*
207.课程表：你这个学期必须选修 numCourses 门课程，记为 0 到 numCourses - 1 。
在选修某些课程之前需要一些先修课程。 先修课程按数组 prerequisites 给出，
其中 prerequisites[i] = [ai, bi] ，表示如果要学习课程 ai 则 必须 先学习课程  bi 。
例如，先修课程对 [0, 1] 表示：想要学习课程 0 ，你需要先完成课程 1 。
请你判断是否可能完成所有课程的学习？如果可以，返回 true ；否则，返回 false 。
*/
// ---------------------
namespace s547o1
{


}

// 模板题1：简化拓扑排序，判断有向图中是否有环，有BFS（Kahn算法）和DFS两种解法，都需要掌握
// 两种方法的时间复杂度都是O(V + E)（V是节点数，E是边数），空间复杂度O(V + E)
namespace s207o1
{   /*
    BFS方法基于入度（indegree）：每个节点的入度是指向它的边的数量。
    我们从入度为0的节点（没有先修课的课程）开始，逐步“完成”课程，并减少依赖它的课程的入度。
    如果所有节点都能被处理，则无环；否则有环。

    步骤：
    构建邻接表，并计算每个节点的入度。
    初始化队列，将所有入度为0的节点入队。
    BFS循环：
    出队一个节点，计数加一（表示完成一门课）。
    遍历该节点的所有邻居，减少邻居的入度。如果邻居入度变为0，入队。
    如果计数等于总课程数，返回true；否则，返回false（有环，因为剩余节点入度不为0）。
    */
    // BFS
    class Solution {
    public:
        bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
            // 步骤1: 构建邻接表和入度数组
            vector<vector<int>> graph(numCourses);
            vector<int> indegree(numCourses, 0); // 存储每个节点的入度
            for (auto& pre : prerequisites) {
                int ai = pre[0], bi = pre[1];
                graph[bi].push_back(ai); // 边: bi -> ai
                indegree[ai]++; // ai的入度加1（因为边指向ai，所以ai的入度增加）
            }

            // 步骤2: 初始化队列，将所有入度为0的节点入队
            // （BFS从入度为0的节点开始，这些节点没有先修课，可以直接学习）
            queue<int> q;
            for (int i = 0; i < numCourses; i++) {
                if (indegree[i] == 0) {
                    q.push(i);
                }
            }

            // 步骤3: BFS遍历
            int count = 0; // 记录已处理的节点数
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                count++; // 完成一门课

                for (int neighbor : graph[node]) {
                    // 每处理一个节点，就减少其邻居的入度，模拟“完成先修课”的效果
                    --indegree[neighbor]; // 邻居的入度减1
                    if (indegree[neighbor] == 0) {
                        q.push(neighbor); // 如果入度变为0，入队
                    }
                }
            }

            // 步骤4: 检查是否所有节点都被处理
            // 如果最终计数不等于总课程数，说明有节点无法被处理（存在环）
            return count == numCourses;
        }
    };
}
namespace s207o2
{
    /*
    DFS方法的思路是：遍历图时，如果发现一条边指向一个“正在访问中”的节点（即Back Edge），说明有环。
    重点是访问路径上是否有环，而不是每条边的前后关系/访问路径是如何排布，入度出度，与o1相比，关注的重点并不同
    我们用一个状态数组来跟踪每个节点的状态：
    visited[i] = 0：节点i还未被访问。
    visited[i] = 1：节点i正在访问中（当前DFS路径上）。
    visited[i] = 2：节点i已访问完成（安全，无环）。
    步骤：
    构建图的邻接表（adjacency list）。
    初始化状态数组，全部设为0（未访问）。
    对每个节点进行DFS遍历：
    如果节点状态为1，发现环，返回false。
    如果状态为2，跳过。
    否则，标记为1，递归访问所有邻居。递归中如果发现环，返回false。完成后标记为2。
    如果所有节点都成功完成DFS，返回true。
    */
    class Solution {
    public:
        bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
            // 步骤1: 构建邻接表表示的图
            vector<vector<int>> graph(numCourses);
            for (auto& pre : prerequisites) {
                int ai = pre[0], bi = pre[1]; // 学习ai前需要先学bi
                graph[bi].push_back(ai); // 添加边: bi -> ai
            }

            // 步骤2: 初始化状态数组，0=未访问，1=访问中，2=已访问
            vector<int> visited(numCourses, 0);

            // 步骤3: 对每个未访问的节点进行DFS
            for (int i = 0; i < numCourses; i++) {
                if (visited[i] == 0) { // 只处理未访问的节点
                    if (!dfs(i, graph, visited)) {
                        return false; // 发现环
                    }
                }
            }
            return true; // 所有节点无环
        }

    private:
        // DFS递归中，先检查状态：如果遇到状态1，说明有环；状态2直接跳过
        bool dfs(int node, vector<vector<int>>& graph, vector<int>& visited) {
            if (visited[node] == 1) return false; // 发现环：节点正在当前路径上
            if (visited[node] == 2) return true;  // 已访问过，安全跳过

            visited[node] = 1; // 标记为“访问中”
            for (int neighbor : graph[node]) {
                if (!dfs(neighbor, graph, visited)) {
                    return false; // 递归检查邻居，发现环则返回false
                }
            }
            visited[node] = 2; // 标记为“已访问完成”（递归后标记为2，避免重复计算）
            // 能运行到visited[node] = 2这一步的，要么是访问路径的最尾端，要么是从尾端一层层返回上去的中间节点
            // 所以只要状态是2，当前节点以及之后的节点都不需要去重复检查了，全部都是安全无环的
            // 访问路径上的节点状态是 1 -> 1 -> 1 -> 1 -> 尾端
            // 当运行到尾端时，尾端状态为2，然后一层层返回状态逐步从后向前全部改为2
            return true;       // 走到当前链条的尾端
        }
    };
}
namespace s207o3
{   // 将0,1,2整数表示状态改为强枚举类型，更规范
    class Solution {
    private:
        // 定义三种访问状态，使用枚举类（强类型）
        // 强类型枚举 enum class，since C++11，必须使用域限定符State::访问，不会暴露到外层空间
        // 而且不能隐式转换成整数，保证了类型安全
        // 传统的弱类型枚举enum则没有这些优先，不推荐使用，能使用enum class就用enum class
        enum class State {
            UNVISITED,  // 未访问
            VISITING,   // 正在访问（当前DFS路径上）
            VISITED     // 已访问完成
        };

        // DFS 递归函数，使用枚举类型
        bool dfs(int node, const vector<vector<int>>& graph, vector<State>& visited) {
            if (visited[node] == State::VISITING) return false; // 发现环
            if (visited[node] == State::VISITED)  return true;  // 已处理过，安全跳过

            visited[node] = State::VISITING; // 标记为“访问中”
            for (int neighbor : graph[node]) {
                if (!dfs(neighbor, graph, visited)) {
                    return false; // 邻接节点发现环
                }
            }
            visited[node] = State::VISITED; // 标记为“已访问完成”
            return true;
        }

    public:
        bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
            // 1. 构建邻接表（图）
            vector<vector<int>> graph(numCourses);
            for (auto& pre : prerequisites) {
                int ai = pre[0], bi = pre[1]; // 学习 ai 前需学 bi
                graph[bi].push_back(ai);      // 有向边 bi -> ai
            }

            // 2. 初始化状态数组，全部设为 UNVISITED
            vector<State> visited(numCourses, State::UNVISITED);

            // 3. 对每个未访问节点进行 DFS
            for (int i = 0; i < numCourses; ++i) {
                if (visited[i] == State::UNVISITED) {
                    if (!dfs(i, graph, visited)) {
                        return false; // 发现环
                    }
                }
            }
            return true; // 所有课程可以完成
        }
    };

}
namespace s207o4
{   // 将DFS做法改为迭代栈做法，仅作了解
    // 跟二叉树的栈写法不一样，取出栈顶元素时用引用auto& [u, idx] = stk.top();
    // 遍历时可能修改栈顶元素，同一路径内idx逐渐增加，统一处理完整条链路后才pop掉
    class Solution {
    public:
        bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
            vector<vector<int>> graphs(numCourses);
            for (auto& p : prerequisites) {
                int ai = p[0], bi = p[1];
                graphs[bi].push_back(ai);
            }

            // 0: UNVISITED, 1: VISITING, 2: VISITED
            vector<int> visit(numCourses, 0);

            for (int start = 0; start < numCourses; ++start) {
                if (visit[start] != 0) continue;

                // 用栈模拟递归：stack< (节点, 下一个要处理的邻居索引) >
                stack<pair<int, int>> stk;
                stk.emplace(start, 0);
                visit[start] = 1; // 标记 VISITING

                while (!stk.empty()) {
                    // 这里必须用引用，要不然idx的修改不会生效，否则就只会遍历第一个邻居（idx = 0)
                    auto& [u, idx] = stk.top();

                    // 还有未处理的邻居
                    if (idx < (int)graphs[u].size()) {
                        int v = graphs[u][idx];
                        ++idx; // 移动索引

                        if (visit[v] == 1) { // 发现环
                            return false;
                        }
                        if (visit[v] == 0) {
                            visit[v] = 1; // 立即标记 VISITING
                            stk.emplace(v, 0);
                        }
                        // 如果 v 是 VISITED，跳过
                    }
                    else {
                        // 所有邻居处理完成
                        visit[u] = 2; // 标记 VISITED
                        stk.pop();
                    }
                }
            }

            return true;
        }
    };
}
// ---------------------
// 【1.2】广度优先搜索（BFS）()
/*

*/
// ---------------------

// ---------------------
// 【1.3】图论建模 + BFS 最短路 ()
// 把状态抽象成图上的点，用 BFS 遍历这张图，计算从初始状态到目标状态的最短路长度
/*
127.单词接龙：字典 wordList 中从单词 beginWord 到 endWord 的 转换序列 是一个按下述规格形成的序列
beginWord -> s1 -> s2 -> ... -> sk：
每一对相邻的单词只差一个字母。对于 1 <= i <= k 时，每个 si 都在 wordList 中。
注意， beginWord 不需要在 wordList 中。sk == endWord
给你两个单词 beginWord 和 endWord 和一个字典 wordList ，返回 从 beginWord 到 endWord 的 最短转换序列 中的 单词数目 。
如果不存在这样的转换序列，返回 0 。
*/
// ---------------------
// 模板题2：双向广搜，将单词视为点，转换关系视为边，将字符关系转化为无向无权图，基础思路有点像s1091，但有很多优化空间
namespace s127o1 {
    // 问题转化之后套用最简单的单源最短BFS方法求解，没有进行优化
    // 单词长度为m，字典个数为n
    // 每次查阅需要遍历n个字符，每次需要对比m个字母，最坏查阅过程需要重复n次
    // 时间复杂度为O(n * n * m)，空间复杂度为O(n)

    // 完整推导见：
    // https://leetcode.cn/problems/word-ladder/solutions/2817913/chao-xiang-xi-de-ceng-ceng-di-jin-san-ch-5kmy/?envType=problem-list-v2&envId=grjj1UoG
    class Solution {
    public:
        int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
            queue<string> q;
            q.push(beginWord);
            unordered_map<string, int> dists; // 存储各单词被初次转换时的距离（序列中的单词数量）
            dists[beginWord] = 1;

            while (!q.empty()) {
                string word = q.front();
                q.pop();
                // 遍历单词数组，找到能转换的所有单词
                for (const string& nextWord : wordList) {
                    // 使用过则跳到下一个
                    if (dists.count(nextWord)) continue;

                    // 比较单词，确认不同的字符数量
                    int distinctCnt = 0;
                    for (int i = 0; i < word.size(); ++i) {
                        if (word[i] != nextWord[i])
                            distinctCnt++;
                    }
                    // 不同的字符有且只有一个时才能转换
                    if (distinctCnt == 1) {
                        dists[nextWord] = dists[word] + 1;
                        q.push(nextWord);
                        // 遇到终点即返回距离
                        if (nextWord == endWord) return dists[nextWord];
                    }
                }
            }
            return 0;
        }
    };
}
namespace s127o2
{   // 因为题干信息指出，wordList最多有5000个字符，而每次转变1个字母最多有26种情况
    // 那么可以使用hash set使得o1中查询distinctCnt的过程优化为O(1)
    // 时间复杂度优化为O(26mn)，空间复杂度为为O(mn)
    class Solution {
    public:
        int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
            queue<string> q;
            q.push(beginWord);

            unordered_map<string, int> dists; // 存储各单词被初次转换时的距离（序列中的单词数量）
            dists[beginWord] = 1;
            unordered_set<string> wordSet(wordList.begin(), wordList.end());

            while (!q.empty()) {
                string origin = q.front(); // 原件
                q.pop();
                string copy = origin;

                // 逐一替换单词中的字符
                for (int i = 0; i < copy.size(); i++) {
                    char oldChar = copy[i]; // 供复原
                    for (char c = 'a'; c <= 'z'; c++) {
                        copy[i] = c;
                        if (wordSet.count(copy) && !dists.count(copy)) {
                            // 此处执行 O(V) 次，每次的时间空间复杂度都是 Θ(m)
                            dists[copy] = dists[origin] + 1;
                            q.push(copy);
                            if (copy == endWord)
                                return dists[copy];
                        }
                    }
                    copy[i] = oldChar; // 复原
                }
            }

            return 0;
        }
    };
}
namespace s127o3
{   // 借助通配符构建邻接表
    // 举例：先只关注 hit 和 hot 这两个单词之间的转换，如下所示，可以借助 h*t 来作媒介。
    // 所以我们可以借助通配符为每个单词构建含 * 的邻接点和边。
    // 用 BFS 求解，记起点和终点在图中的距离为 d，则最终的序列点数为 (d / 2) + 1

    // 图中顶点数变为n * (m + 1)，出队列的元素个数也就随即发生了变化，每次出队列的判断最多需要O(m)
    // 综合时间复杂度为：建图 O(n * m * m) + BFS O(n * m * m) = O(m * m * n)
    // 每个顶点的大小是O(m)，所以顶点存储空间复杂度为O(m * m * n)，边的存储也类似，总共O(n * m)条边，每条边为长m的字符
    // 综合空间复杂度为：顶点 O(n * m * m) + 边O(n * m * m) = O(m * m * n)
    class Solution {
    private:
        // 此函数的时间复杂度为O(m * m)
        void addWordToAdj(const string& word, unordered_map<string, list<string>>& adj) {
            string copy = word;             // O(m)
            for (int i = 0; i < copy.size(); i++) {// 处理m个位置
                char oldChar = copy[i];  // 留作复原
                copy[i] = '*';
                adj[word].push_back(copy);// O(m)
                adj[copy].push_back(word);// O(m)
                copy[i] = oldChar;  // 复原
            }
        }

    public:
        int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
            queue<string> q;
            q.push(beginWord);

            // 存储各单词被初次转换时的距离，但源点处为 0，在最后调整
            unordered_map<string, int> dists;
            dists[beginWord] = 0;
            // 邻接表
            unordered_map<string, list<string>> adj;

            // 填充无向图的邻接表
            for (const string& word : wordList) {
                addWordToAdj(word, adj);
            }
            addWordToAdj(beginWord, adj);

            // BFS
            while (!q.empty()) {
                string word = q.front();
                q.pop();
                for (const string& nextWord : adj[word]) {
                    if (dists.find(nextWord) == dists.end()) {
                        dists[nextWord] = dists[word] + 1;
                        q.push(nextWord);
                        if (nextWord == endWord) {
                            return dists[nextWord] / 2 + 1;  // 调整为序列中的单词数
                        }
                    }
                }
            }

            return 0;
        }
    };
}
namespace s127o4
{   // 基于o3通配符解法进行优化，改为双向BFS
    // 将beginWord和endWord同时作为起点，进行BFS
    // 但要注意endWord可能不在wordList中，所以要进行特判
    // 时空复杂度和o3相同
    class Solution {
    private:
        void addWordToAdj(const string& word, unordered_map<string, vector<string>>& adj) {
            string copy = word;
            adj[word].reserve(word.size());
            for (int i = 0; i < copy.size(); i++) {
                char oldChar = copy[i];
                copy[i] = '*';
                adj[word].push_back(copy);
                adj[copy].push_back(word);
                copy[i] = oldChar;
            }
        }

    public:
        int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
            unordered_map<string, vector<string>> adj;

            // 填充无向图的邻接表
            for (const string& word : wordList) {
                addWordToAdj(word, adj);
            }
            addWordToAdj(beginWord, adj);
            // 特判：endWord可能不在wordList中
            if (adj.find(endWord) == adj.end()) {
                return 0;
            }

            // 存储一对各单词被初次转换时的距离，但源点终点处为 0，在最后调整
            unordered_map<string, int> dists1, dists2;
            dists1[beginWord] = 0;
            dists2[endWord] = 0;

            // 两个队列
            queue<string> q1, q2;
            q1.push(beginWord);
            q2.push(endWord);

            while (!q1.empty() && !q2.empty()) {
                // 找到周长小的一方
                // 用引用来实现二选一的效果
                queue<string>& q = q1.size() < q2.size() ? q1 : q2;
                unordered_map<string, int>& dists = q1.size() < q2.size() ? dists1 : dists2;
                unordered_map<string, int>& otherDists = q1.size() < q2.size() ? dists2 : dists1;

                // 确保遍历完一层
                for (int i = 0, size = q.size(); i < size; i++) {
                    string word = q.front();
                    q.pop();
                    for (const string& nextWord : adj[word]) {
                        if (dists.find(nextWord) == dists.end()) {
                            dists[nextWord] = dists[word] + 1;
                            // 相交即刻返回
                            if (otherDists.find(nextWord) != otherDists.end()) {
                                return (dists1[nextWord] + dists2[nextWord]) / 2 + 1;
                            }
                            q.push(nextWord);
                        }
                    }
                }
            }

            return 0;
        }
    };
}

// ---------------------
// 【1.4】跳跃游戏 ()
/*

*/
// ---------------------



// ------------------------------------------------------------------------------------
// 二、拓扑排序()

// 【2.1】 ()
/*

*/
// ---------------------




// ---------------------

// ------------------------------------------------------------------------------------
// 三、最短路()

// 【3.1】单源最短路：Dijkstra算法()
/*


*/
// 模板
namespace s5_1_3_1
{
// 返回从起点 start 到每个点的最短路长度 dis，如果节点 x 不可达，则 dis[x] = LLONG_MAX
// 要求：没有负数边权
// 时间复杂度 O(n + mlogm)，注意堆中有 O(m) 个元素
    vector<long long> shortestPathDijkstra(int n, vector<vector<int>>& edges, int start) {
        // 注：如果节点编号从 1 开始（而不是从 0 开始），可以把 n 加一
        vector<vector<pair<int, int>>> g(n); // 邻接表
        for (auto& e : edges) {
            int x = e[0], y = e[1], wt = e[2];
            g[x].emplace_back(y, wt);
            // g[y].emplace_back(x, wt); // 无向图加上这行
        }

        vector<long long> dis(n, LLONG_MAX);
        // 堆中保存 (起点到节点 x 的最短路长度，节点 x)
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        dis[start] = 0; // 起点到自己的距离是 0
        pq.emplace(0, start);

        while (!pq.empty()) {
            auto [dis_x, x] = pq.top();
            pq.pop();
            if (dis_x > dis[x]) { // x 之前出堆过
                continue;
            }
            for (auto& [y, wt] : g[x]) {
                auto new_dis_y = dis_x + wt;
                if (new_dis_y < dis[y]) {
                    dis[y] = new_dis_y; // 更新 x 的邻居的最短路
                    // 懒更新堆：只插入数据，不更新堆中数据
                    // 相同节点可能有多个不同的 new_dis_y，除了最小的 new_dis_y，其余值都会触发上面的 continue
                    pq.emplace(new_dis_y, y);
                }
            }
        }

        return dis;
    }
}
// ---------------------
// 模板题3：标准Dijkstra类型题，但是因为图的存储格式，题解并不是标准Dijkstra的解法，标准的应该用邻接表
namespace s743m1
{   // 看了波波微课后根据理解写出来的，使用优先队列
    // 再次测试后发现优先队列的写法不需要visit数组
    // 因为第一次从队列中弹出某个节点时，它一定拥有当前最短距离
    // 后续再弹出该节点时，curDist > dist[curVex] 的条件会直接跳过它
    class Solution {
    public:
        int networkDelayTime(vector<vector<int>>& times, int n, int k) {
            auto cmp = [](const auto& a, const auto& b) {
                return a.first > b.first;
                };
            // (dist[vex], vex)
            priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pq(cmp);

            int numEdge = times.size();
            // INT_MAX代表最短距离没更新过
            vector<int> dist(n, INT_MAX);
            // visit数组标记顶点是否被处理过（这个解法下不需要，可删去）
            vector<char> visit(n, false);

            int start = k - 1; // 转换为下标从0开始
            pq.emplace(0, k - 1);
            dist[start] = 0;

            while (!pq.empty()) {
                auto [curDist, curVex] = pq.top();
                pq.pop();

                // 注：这个解法下不需要visit，可删去
                if (visit[curVex] || curDist > dist[curVex]) continue;
                // 标记当前顶点为已访问注：这个解法下不需要visit，可删去
                visit[curVex] = true;

                // 遍历所有从curVex出发的边
                // 每次遍历所有边（for (int i = 0; i < numEdge; ++i)），这是O(E)的操作，重复了V次：
                // 总复杂度变成了 O(V × E)，不是Dijkstra的标准写法
                for (int i = 0; i < numEdge; ++i) {
                    int from = times[i][0] - 1; // times中顶点编号从1开始
                    int to = times[i][1] - 1;
                    int w = times[i][2];
                    // 跳过非邻接顶点和已访问的顶点
                    // 注：这个解法下不需要visit，可删去
                    if (from != curVex || visit[to]) continue;

                    if (dist[curVex] + w < dist[to]) {
                        dist[to] = dist[curVex] + w;
                        pq.emplace(dist[to], to);
                    }
                }
            }

            int mx = *max_element(dist.begin(), dist.end());
            return mx == INT_MAX ? -1 : mx;
        }
    };
}
namespace s743m1extention
{   // 新增了求最短路径的函数，以{start, v, v, v, end}的vector<int>形式返回
    class Solution {
    private:
        // 最短路径函数
        vector<int> shortestPath(vector<int>& prevNode, int start, int end) {
            vector<int> path;
            // 从终点向前回溯
            for (int cur = end; cur != start; cur = prevNode[cur]) {
                path.push_back(cur);
                // 检查是否存在路径（防止无限循环）
                if (cur == -1) {  // prevNode初始化为-1表示无前驱
                    return {};    // 无法到达终点
                }
            }
            path.push_back(start);  // 添加起点
            reverse(path.begin(), path.end());  // 反转得到正向路径
            return path;
        }

    public:
        int networkDelayTime(vector<vector<int>>& times, int n, int k) {
            auto cmp = [](const auto& a, const auto& b) {
                return a.first > b.first;
                };
            priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pq(cmp);
            vector<int> dist(n, INT_MAX);
            vector<int> prevNode(n, -1);// -1表示无前驱

            int start = k - 1; // 转换为下标从0开始
            int numEdge = times.size();
            pq.emplace(0, k - 1);
            dist[start] = 0;

            while (!pq.empty()) {
                auto [curDist, curVex] = pq.top();
                pq.pop();

                // 注：这个解法下不需要visit，可删去
                if (curDist > dist[curVex]) continue;

                // 遍历所有从curVex出发的边
                for (int i = 0; i < numEdge; ++i) {
                    int from = times[i][0] - 1; // times中顶点编号从1开始
                    int to = times[i][1] - 1;
                    int w = times[i][2];
                    if (from != curVex) continue;

                    if (dist[curVex] + w < dist[to]) {
                        dist[to] = dist[curVex] + w;
                        pq.emplace(dist[to], to);
                        prevNode[to] = curVex; // 记录前驱顶点
                    }
                }
            }

            int mx = *max_element(dist.begin(), dist.end());
            return mx == INT_MAX ? -1 : mx;
        }
    };
}
namespace s743o1
{   // 优化点1：对于本题，由于要求是能覆盖到所有顶点的信号，所以计算最短路时，
    // 如果发现当前找到的最小最短路等于INT_MAX，就说明有顶点无法到达，可以提前返回-1
    // 优化点2：将times数组转化先转化为邻接矩阵，避免每次找邻接点时要遍历整个边集，
    // 这对于稠密图非常致命(边数E远大于顶点数n)，现在每次只需遍历整个点集

    // 朴素Dijkstra 适用于稠密图，时空复杂度都是O(n^2)
    class Solution {
    public:
        int networkDelayTime(vector<vector<int>>& times, int n, int k) {
            // 将times转换为邻接矩阵
            // 注这里邻接矩阵和下面的dist不用INT_MAX的原因在于:
            // 更新x的邻居时由于dist[x] + g[x][y],用INT_MAX可能溢出
            vector<vector<int>> g(n, vector<int>(n, INT_MAX / 2));
            for (auto& t : times) {
                int from = t[0] - 1; // 顶点编号从0开始
                int to = t[1] - 1;
                int w = t[2];
                g[from][to] = w;
            }

            vector<int> dist(n, INT_MAX / 2);
            vector<char> visit(n);

            int start = k - 1;
            dist[start] = 0;

            while (true) {
                int x = -1; // 每次用来找当前最小路径dist值的判断基准
                for (int i = 0; i < n; i++) {
                    // 未结束访问 && (这轮中找到首个未结束访问的顶点 || 路径值比当前基准值x小)
                    if (!visit[i] && (x < 0 || dist[i] < dist[x])) {
                        x = i;
                    }
                }

                // x仍然保持为-1没有变动,说明找不到未结束访问的顶点，所有节点都已结束访问(visit[i] = true)
                if (x < 0) {
                    return *max_element(dist.begin(), dist.end());
                }

                // 有节点无法到达
                if (dist[x] == INT_MAX / 2) {
                    return -1;
                }

                // 当前最短路长度已确定（无法变得更小）
                visit[x] = true;

                // 更新 x 的邻居的最短路
                for (int y = 0; y < n; y++) {
                    dist[y] = min(dist[y], dist[x] + g[x][y]);
                }
            }
        }
    };
}
namespace s743o1extention
{   // 更规范的朴素Dijkstra模板写法，不用while true这种循环

    // 朴素 Dijkstra：求从起点 s 到所有其他节点的最短路径
    // n: 节点数 (节点编号 0..n-1)
    // g: 邻接矩阵，g[u][v] 表示边 u->v 的权值，INF 表示无边
    // s: 起点编号
    // 返回值: dist 数组，其中 dist[i] 为 s 到 i 的最短距离，若不可达则仍为 INF
    vector<int> dijkstra(int n, const vector<vector<int>>& g, int s) {
        const int INF = INT_MAX / 2;     // 注意 /2 防止加法溢出
        vector<int> dist(n, INF);        // 距离数组
        vector<bool> visited(n, false); // 标记是否已确定最短路

        dist[s] = 0;

        for (int i = 0; i < n; ++i) {
            // 1. 在未访问的节点中，选出距离最小的节点 u
            int u = -1;
            int minDist = INF;
            for (int j = 0; j < n; ++j) {
                if (!visited[j] && dist[j] < minDist) {
                    minDist = dist[j];
                    u = j;
                }
            }

            // 如果所有剩余节点都不可达，提前结束（可选）
            if (u == -1) break;

            // 2. 标记 u 为已访问
            visited[u] = true;

            // 3. 松弛操作：更新 u 的所有邻居
            for (int v = 0; v < n; ++v) {
                if (!visited[v] && g[u][v] != INF) {
                    if (dist[u] + g[u][v] < dist[v]) {
                        dist[v] = dist[u] + g[u][v];
                    }
                }
            }
        }

        return dist;
    }
}
namespace s743o2
{   // 堆优化版，相较于m1，多出了构建邻接表的过程，更规范
    class Solution {
    public:
        int networkDelayTime(vector<vector<int>>& times, int n, int k) {
            // ---------- 1. 构建邻接表 ----------
            // g[u] 存储所有从 u 出发的边 (v, weight)
            vector<vector<pair<int, int>>> g(n);
            for (const auto& t : times) {
                int from = t[0] - 1;   // 题目从1开始，转为0基
                int to = t[1] - 1;
                int w = t[2];
                g[from].emplace_back(to, w);
            }

            // ---------- 2. 初始化 ----------
            const int INF = INT_MAX / 2;   // 防止后续加法溢出
            vector<int> dist(n, INF);      // 起点到每个节点的最短距离
            int src = k - 1;               // 起点（0基）
            dist[src] = 0;

            // ---------- 3. 堆优化 Dijkstra ----------
            // 小根堆: pair<距离, 节点>
            using P = pair<int, int>;
            priority_queue<P, vector<P>, greater<P>> pq;
            pq.emplace(0, src);

            while (!pq.empty()) {
                auto [currentDist, u] = pq.top();
                pq.pop();

                // 如果当前取出的距离大于已知最短距离，说明该节点之前已经用更短的距离更新过，跳过
                if (currentDist > dist[u]) continue;

                // 松弛操作：遍历 u 的所有邻居
                for (const auto& [v, w] : g[u]) {
                    int newDist = currentDist + w;
                    if (newDist < dist[v]) {          // 找到更短的路径
                        dist[v] = newDist;
                        pq.emplace(newDist, v);       // 将新距离入堆
                    }
                }
            }

            // ---------- 4. 找出最大传播时间 ----------
            int maxDist = 0;
            for (int d : dist) {
                if (d > maxDist) maxDist = d;
            }
            // 如果仍有节点不可达 (距离保持 INF)，返回 -1
            return maxDist < INF ? maxDist : -1;
        }
    };
}
// ---------------------

