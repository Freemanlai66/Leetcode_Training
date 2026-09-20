#pragma once
#include <vector>
#include <array>
#include <queue>
#include <unordered_map>

using namespace std;

// 问题待定：
/*
1.
*/

/*
模板题：
模板1.计算每个连通块的大小：7_1_1
1.岛屿数量，逐步遍历所有网格格子，一旦发现岛屿，就通过递归散播病毒，修改地形:200
2.脑筋急转弯，其实根本用不到DFS，普通遍历一遍并逐个检查当前网格周围是不是水就行了：463
模板2.计算单源最短距离（最短路）：7_2_1
3.典型BFS路径最短问题，类比岛屿问题中的病毒传播，每轮只能传播固定距离，单个病毒源：1091
4.多源BFS，"多个起点，多个终点"的最短路径问题，不是直接遍历grid，而是遍历病毒源：994 + 542
5.

*/

// 网格图：DFS + BFS + 0-1BFS + Dijkstra + 综合

// 一、网格图DFS (3)

// 【1.1】网格图DFS (3)，部分题目也可以用BFS和并查集实现
// 网格是一种简化的图结构，适用于需要计算连通块个数、大小的题目，代表是岛屿类问题，1代表陆地，0代表海洋
// 类比二叉树DFS，这里的节点就是格子，递归方向是四周的相邻格子，递归边界是出界、障碍与已经访问
/*
200.岛屿数量：给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量。
岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。
此外，你可以假设该网格的四条边均被水包围。

695.岛屿的最大面积：给你一个大小为 m x n 的二进制矩阵 grid 。
岛屿 是由一些相邻的 1 (代表土地) 构成的组合，这里的「相邻」要求两个 1 必须在 水平或者竖直的四个方向上 相邻。
你可以假设 grid 的四个边缘都被 0（代表水）包围着。
岛屿的面积是岛上值为 1 的单元格的数目。
计算并返回 grid 中最大的岛屿面积。如果没有岛屿，则返回面积为 0 。

463.岛屿的周长：给定一个 row x col 的二维网格地图 grid ，其中：grid[i][j] = 1 表示陆地， grid[i][j] = 0 表示水域。
网格中的格子 水平和垂直 方向相连（对角线方向不相连）。整个网格被水完全包围，
但其中恰好有一个岛屿（或者说，一个或多个表示陆地的格子相连组成的岛屿）。
岛屿中没有“湖”（“湖” 指水域在岛屿内部且不和岛屿周围的水相连）。
格子是边长为 1 的正方形。网格为长方形，且宽度和高度均不超过 100 。计算这个岛屿的周长。

130.被围绕的区域：给你一个 m x n 的矩阵 board ，由若干字符 'X' 和 'O' 组成，捕获 所有 被围绕的区域：
连接：一个单元格与水平或垂直方向上相邻的单元格连接。
区域：连接所有 'O' 的单元格来形成一个区域。
围绕：如果一个区域中的所有 'O' 单元格都不在棋盘的边缘，则该区域被包围。这样的区域 完全 被 'X' 单元格包围。
通过 原地 将输入矩阵中的所有 'O' 替换为 'X' 来 捕获被围绕的区域。你不需要返回任何值。
*/
// ---------------------
// 模板1：计算每个连通块的大小，刷下面三道题后再看这个模板
namespace s7_1_1 {
    // 模板来源于灵神题单页面
    constexpr int DIRS[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; // 左右上下

    // 返回网格图 grid 每个连通块的大小
    // 时间复杂度 O(mn)
    vector<int> dfsGrid(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        vector vis(m, vector<int8_t>(n));

        // 返回当前连通块的大小
        auto dfs = [&](auto&& dfs, int i, int j) -> int {
            vis[i][j] = true;
            int size = 1;
            for (auto [dx, dy] : DIRS) {
                int x = i + dx, y = j + dy;
                // 这里 grid[x][y] == '.' 根据题意修改
                if (0 <= x && x < m && 0 <= y && y < n && grid[x][y] == '.' && !vis[x][y]) {
                    size += dfs(dfs, x, y);
                }
            }
            return size;
            };

        vector<int> comp_size;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != '.' || vis[i][j]) { 
                    // grid[i][j] != '.' 根据题意修改
                    // 注：灵神题单中直接对输入数组grid进行了修改，但更规范的做法是定义visited数组
                    // 如：vector<vector<bool>> visited(m, vector<bool>(n, false));
                    // 相应模板中的部分细节也需要进行相应的修改，面试时要询问面试官是否允许通过修改输入来简化代码
                    continue;
                }
                int size = dfs(dfs, i, j);
                comp_size.push_back(size);
            }
        }
        return comp_size;
    }
}

// 模板题1：二重循环线性遍历矩阵，途中用递归病毒感染发现的岛屿，修改地形
namespace s200o1
{	/*
    搬运liweiwei和liuyubobo大佬的一段话：棋盘上的搜索问题，不能，也没有必要在输入的 board 上修改，
    必须使用标准的做法：建立 visited 布尔数组辅助完成题目给出的任务。
    实在是觉得没有必要用 visited 布尔数组的话，写代码之前问一句面试官都是加分的。
    因为，这样一个简单的问题，面试官很可能期待的就是对这些实现细节的讨论，而非仅仅是给出一个结果。
    */
    // o1为灵神原始做法，o2为更标准的规范做法
    class Solution {
    public:
        int numIslands(vector<vector<char>>& grid) {
            int m = grid.size();
            int n = grid[0].size();

            auto dfs = [&](auto&& self, int r, int c) -> void {
                // 递归终止条件：不越界 + 没有访问过 + 当前网格不是陆地（不是陆地这个条件通过递归外的两重for循环进行筛选）
                if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != '1') {
                    return;
                }
                // 将grid[r][c]改为'2'代表已经访问过该岛屿
                grid[r][c] = '2';
                // 当前探索顺序是左右上下，不同递归顺序会导致探索路径形状发生改变，虽然不改变结果，但会影响递归深度
                self(self, r, c - 1);
                self(self, r, c + 1);
                self(self, r - 1, c);
                self(self, r + 1, c);
                };

            int ans = 0;
            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c) {
                    // 相当于传染病毒，调用dfs递归后，当前发现的岛屿地块就会全部变成'2'
                    if (grid[r][c] == '1') {
                        dfs(dfs, r, c);
                        ++ans;
                    }
                }
            }
            return ans;
        }
    };
}
namespace s200o2
{   // 更标准的用visited数组的做法
    class Solution {
    public:
        int numIslands(vector<vector<char>>& grid) {
            int m = grid.size(), n = grid[0].size();
            vector<vector<bool>> visited(m, vector<bool>(n, false));

            auto dfs = [&](auto&& dfs, int i, int j)->void {
                // 尤其要注意grid是字符矩阵，不是二进制矩阵
                if (i < 0 || i >= m || j < 0 || j >= n
                    || visited[i][j] || grid[i][j] == '0') {
                    return;
                }

                visited[i][j] = true;
                dfs(dfs, i, j - 1);
                dfs(dfs, i, j + 1);
                dfs(dfs, i - 1, j);
                dfs(dfs, i + 1, j);
                };

            int ans = 0;
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (!visited[i][j] && grid[i][j] == '1') {
                        dfs(dfs, i, j);
                        ++ans;
                    }
                }
            }
            return ans;
        }
    };
}

// 基于s200改写，注意这里地图变成int矩阵了，而不是字符矩阵
namespace s695m1
{
    class Solution {
    public:
        int maxAreaOfIsland(vector<vector<int>>& grid) {
            int ans = 0;
            int m = grid.size();
            int n = grid[0].size();

            auto dfs = [&](auto&& self, int r, int c) -> int {
                if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != 1) {
                    return 0;
                }
                grid[r][c] = 2;
                // 自底向上递归返回面积值
                return 1 + self(self, r, c - 1) + self(self, r, c + 1) +
                    self(self, r - 1, c) + self(self, r + 1, c);
                };

            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c) {
                    if (grid[r][c] == 1) {
                        ans = max(ans, dfs(dfs, r, c));
                    }
                }
            }
            return ans;
        }
    };
}
namespace s695m2
{   // 用visited数组的更标准做法
    class Solution {
    public:
        int maxAreaOfIsland(vector<vector<int>>& grid) {
            int m = grid.size(), n = grid[0].size();
            vector<vector<bool>> visited(m, vector<bool>(n, false));
            int cnt = 0, ans = 0;

            auto dfs = [&](auto&& dfs, int i, int j)->void {
                if (i < 0 || i >= m || j < 0 || j >= n
                    || visited[i][j] || grid[i][j] == 0) {
                    return;
                }

                visited[i][j] = true;
                ++cnt;

                dfs(dfs, i, j + 1);
                dfs(dfs, i, j - 1);
                dfs(dfs, i - 1, j);
                dfs(dfs, i + 1, j);
                // 也可以return四个dfs的值 + 1，上面的边界条件就返回0，直接用dfs递归求面积值
                };

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (!visited[i][j] && grid[i][j]) {
                        dfs(dfs, i, j);
                        ans = max(ans, cnt);
                        cnt = 0;
                    }
                }
            }
            return ans;
        }
    };
}

// 模板题2：脑筋急转弯，其实根本用不到DFS，普通遍历一遍并逐个检查当前网格周围是不是水就行了
namespace s463m1
{
    class Solution {
    public:
        int islandPerimeter(vector<vector<int>>& grid) {
            int m = grid.size();
            int n = grid[0].size();
            int ans = 0;

            for (int r = 0; r < m; ++r) {
                for (int c = 0; c < n; ++c) {
                    if (grid[r][c] == 0) {
                        continue;
                    }
                    if (r == 0 || grid[r - 1][c] == 0) {
                        ++ans;
                    }
                    if (r == m - 1 || grid[r + 1][c] == 0) {
                        ++ans;
                    }
                    if (c == 0 || grid[r][c - 1] == 0) {
                        ++ans;
                    }
                    if (c == n - 1 || grid[r][c + 1] == 0) {
                        ++ans;
                    }
                }
            }
            return ans;
        }
    };
}

namespace s130o1
{   // 逆向思维，从只要把边界上的 O 特殊处理了，那么剩下的 O 替换成 X 就可以了
    // 问题转化为，如何 寻找和边界联通的 O，DFS遍历找到了就改成占位符（不是O或X就行）
    // 最后遍历board一遍，是占位符的就改回来，O改为X
    class Solution {
    public:
        void solve(vector<vector<char>>& board) {
            int m = board.size(), n = board[0].size();

            // DFS函数：标记与边界相连的'O'
            auto dfs = [&](auto&& dfs, int i, int j)->void {
                if (i < 0 || i >= m || j < 0 || j >= n || board[i][j] != 'O') {
                    return;
                }

                board[i][j] = '#';
                dfs(dfs, i + 1, j);
                dfs(dfs, i - 1, j);
                dfs(dfs, i, j - 1);
                dfs(dfs, i, j + 1);
                };

            // 从边界上的'O'开始DFS
            for (int i = 0; i < m; ++i) {
                if (board[i][0] == 'O') dfs(dfs, i, 0);
                if (board[i][n - 1] == 'O') dfs(dfs, i, n - 1);
            }
            for (int j = 0; j < n; ++j) {
                if (board[0][j] == 'O') dfs(dfs, 0, j);
                if (board[m - 1][j] == 'O') dfs(dfs, m - 1, j);
            }

            // 遍历整个board，修改状态
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (board[i][j] == '#') {
                        board[i][j] = 'O';  
                    }
                    else if (board[i][j] == 'O') {
                        board[i][j] = 'X';
                    }
                }
            }
        }
    };
}
// ---------------------
// 二、网格图BFS(3)

// 【2.1】网格图BFS (3)
// 适用于需要计算最短距离（最短路）的题目
/*
542.01矩阵：给定一个由 0 和 1 组成的矩阵 mat ，请输出一个大小相同的矩阵，
其中每一个格子是 mat 中对应位置元素到最近的 0 的距离。两个相邻元素间的距离为 1 。

994.腐烂的橘子：在给定的 m x n 网格 grid 中，每个单元格可以有以下三个值之一：
值 0 代表空单元格；值 1 代表新鲜橘子；值 2 代表腐烂的橘子。
每分钟，腐烂的橘子 周围 4 个方向上相邻 的新鲜橘子都会腐烂。
返回 直到单元格中没有新鲜橘子为止所必须经过的最小分钟数。如果不可能，返回 -1 。
*/
// ---------------------
// 模板2：计算单源最短距离（最短路）
namespace s7_2_1
{
    constexpr int DIRS[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; // 左右上下

    // 返回从 (start_x, start_y) 出发，到其余格子的最短距离
    // 时间复杂度 O(mn)
    vector<vector<int>> bfsGrid(vector<vector<char>>& grid, int start_x, int start_y) {
        int m = grid.size(), n = grid[0].size();
        vector dis(m, vector<int>(n, -1));
        queue<pair<int, int>> q;

        dis[start_x][start_y] = 0;
        q.emplace(start_x, start_y);

        while (!q.empty()) {
            auto [i, j] = q.front();
            q.pop();
            for (auto [dx, dy] : DIRS) {
                int x = i + dx, y = j + dy;
                // 这里 grid[x][y] == '.' 根据题意修改
                if (0 <= x && x < m && 0 <= y && y < n && grid[x][y] == '.' && dis[x][y] < 0) {
                    dis[x][y] = dis[i][j] + 1;
                    q.emplace(x, y);
                }
            }
        }

        return dis;
    }
}

// 模板题3：典型BFS路径最短问题，类比岛屿问题中的病毒传播，每轮只能传播固定距离，单个病毒源
namespace s1091o1
{
    /*
    Q1.在二维矩阵中搜索，什么时候用BFS，什么时候用DFS？
    A1.
    1.如果只是要找到某一个结果是否存在，那么DFS会更高效。
    因为DFS会首先把一种可能的情况尝试到底，才会回溯去尝试下一种情况，只要找到一种情况，就可以返回了。
    但是BFS必须所有可能的情况同时尝试，在找到一种满足条件的结果的同时，也尝试了很多不必要的路径；

    2.如果是要找所有可能结果中最短的，那么BFS会更高效。
    因为DFS是一种一种的尝试，在把所有可能情况尝试完之前，无法确定哪个是最短，
    所以DFS必须把所有情况都找一遍，才能确定最终答案（DFS的优化就是剪枝，不剪枝很容易超时）。
    而BFS从一开始就是尝试所有情况，所以只要找到第一个达到的那个点，那就是最短的路径，可以直接返回了，
    其他情况都可以省略了，所以这种情况下，BFS更高效。

    Q2.BFS解法中的visited为什么可以全局使用？
    A2.
    BFS是在尝试所有的可能路径，哪个路径先到达终点，哪个就是最短路径。所以每一条路径走过的路不同，
    visited（也就是这条路径上走过的点）也应该不同，那么为什么visited可以全局使用呢？
    因为我们要找的是最短路径，那么如果在此之前某个点已经在visited中，也就是说有其他路径在小于或等于当前步数的情况下，
    到达过这个点，证明到达这个点的最短路径已经被找到。那么显然这个点没必要再尝试了，
    因为即便去尝试了，最终的结果也不会是最短路径了，所以直接放弃这个点即可。
    每一步都会更新visited数组，如果之前就让某个元素对应的visited数组为true，说明已找到对应的最短路径，后续无需再判断
    */
    class Solution {
    private:
    private:
        // 定义8个移动方向：上、下、左、右、左上、右上、左下、右下
        // 注，这里不能用vector，因为constexpr必须是编译时就能确定的变量，vector的动态内存特性无法与constexpr共存
        static inline constexpr int directions[8][2] = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1},
            {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
        };
        // 更规范、现代的写法如下，注意有双重大括号，array的初始化上下文中，隐式转换规则比较严格
        // 编译器可能会在尝试将{-1, 0}转换为pair<int, int>时失败，可以用双重大括号解决
        // 外层括号用于初始化array这个聚合体，内层括号用于初始化数组的每个pair元素
        static inline constexpr array<pair<int, int>, 8> directions = { {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
        } };

    public:
        int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
            int n = grid.size();

            // 确保起点终点都是通畅的
            if (grid[0][0] == 1 || grid[n - 1][n - 1] == 1) {
                return -1;
            }

            // 如果网格只有1x1，且起点为0，直接返回1
            // 这个特例是必须单独写出来的，因为下面的流程默认起点和终点不在一起，如果剩了这一步
            // 对于grid = [[0]]，会返回-1
            if (n == 1) {
                return 1;
            }

            // 初始化距离矩阵，-1表示未访问，既记录答案，也充当visited矩阵的角色，-1代表未访问过
            vector<vector<int>> dist(n, vector<int>(n, -1));
            dist[0][0] = 1; // 起点距离为1（包括起点本身）

            // 使用队列进行BFS，存储坐标 (x, y)
            queue<pair<int, int>> q;
            q.emplace(0, 0);

            while (!q.empty()) {
                auto [x, y] = q.front(); // 取出队首元素
                q.pop();

                // 一次同时遍历所有8个方向，尝试构造8叉树，但每次只前进一层
                for (const auto& [dx, dy] : directions) {
                    int nx = x + dx; // 新坐标x
                    int ny = y + dy; // 新坐标y

                    // 检查新坐标是否在网格范围内、是否为0（可通行）、是否未访问
                    if (nx >= 0 && nx < n && ny >= 0 && ny < n && grid[nx][ny] == 0 && dist[nx][ny] == -1) {
                        dist[nx][ny] = dist[x][y] + 1; // 更新距离
                        // 如果到达终点，直接返回当前距离
                        if (nx == n - 1 && ny == n - 1) {
                            return dist[nx][ny];
                        }

                        q.emplace(nx, ny); // 新坐标入队
                    }
                }
            }

            // 如果BFS结束仍未发现到达终点的路径，则返回-1
            return -1;
        }
    };
}
namespace s1091o2
{   // o2解法没o1解法好，用到了哈希表充当visited，没数组效率高，同时路径长度还需要额外用step维护
    // o1则将存储答案 + visited两个作用合二为一了
    // o2优点在于“BFS”的特征更明显，在入门阶段看这份代码更容易理解
    // o1是单个节点遍历，o2是一层一层遍历，但实际上节点（格子）的遍历顺序是相同的，不一定都需要每次取q.size()才能BFS
    class Solution {
    public:
        int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
            int n = grid.size();

            // 确保起点终点都是通畅的
            if (grid[0][0] == 1 || grid[n - 1][n - 1] == 1) {
                return -1;
            }

            // 如果网格只有1x1，且起点为0，直接返回1
            if (n == 1) {
                return 1;
            }

            queue<pair<int, int>> q;
            unordered_map<int, bool> visited;

            q.push({ 0, 0 });
            visited[0 * n + 0] = true; // 使用哈希映射存储访问状态

            int steps = 1;

            // 8个移动方向
            vector<pair<int, int>> directions = {
                {-1, -1}, {-1, 0}, {-1, 1},
                {0, 1}, {1, 1}, {1, 0},
                {1, -1}, {0, -1}
            };

            while (!q.empty()) {
                int levelSize = q.size();

                // 处理当前层的所有节点
                for (int i = 0; i < levelSize; i++) {
                    auto [x, y] = q.front();
                    q.pop();

                    // 遍历8个方向
                    for (auto [dx, dy] : directions) {
                        int nx = x + dx;
                        int ny = y + dy;

                        // 检查边界和可通行性
                        if (nx >= 0 && nx < n && ny >= 0 && ny < n &&
                            grid[nx][ny] == 0 && !visited[nx * n + ny]) {

                            // 如果到达终点
                            if (nx == n - 1 && ny == n - 1) {
                                return steps + 1;
                            }

                            q.push({ nx, ny });
                            visited[nx * n + ny] = true;
                        }
                    }
                }
                steps++; // 完成一层，步数增加
            }

            return -1;
        }
    };
}

// 多源BFS，与s994类似，但这里有个思维变换，不是从1开始找0，而是从0开始找1，将0变成当成病毒，由全是1的岛屿外到内感染所有1
namespace s542o1
{   // 从所有0同时开始BFS，当多个0同时"竞争"到达同一个1时，最先到达的路径一定是最短路径
    // 如果是从1开始找0，那么就得用对每个1都要用一个新的visited数组，答案还要另外存在一个新矩阵里
    // 不仅如此，BFS的思路也用不起来，正难则反，这道题最优思路就是从0开始找1

    class Solution {
    private:
        static inline constexpr int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    public:
        vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
            int m = mat.size(), n = mat[0].size();

            // 初始化结果矩阵
            vector<vector<int>> dist(m, vector<int>(n, -1));
            queue<pair<int, int>> q;

            // 将所有0的位置加入队列，并设置距离为0
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (mat[i][j] == 0) {
                        dist[i][j] = 0;
                        q.emplace (i, j);
                    }
                }
            }

            // 多源BFS：对队列中每一个元素，每轮只能上下左右四个方向各走一格
            while (!q.empty()) {
                auto [x, y] = q.front();
                q.pop();

                for (const auto [dx, dy] : dirs) {
                    int nx = x + dx;
                    int ny = y + dy;

                    // 检查新坐标是否在网格范围内且未被访问过
                    if (nx >= 0 && nx < m && ny >= 0 && ny < n && dist[nx][ny] == -1) {
                        dist[nx][ny] = dist[x][y] + 1; // 更新距离
                        q.emplace( nx, ny); // 新位置入队
                    }
                }
            }

            return dist;
        }
    };
}

// 模板题4：多源BFS，"多个起点，多个终点"的最短路径问题，不是直接遍历grid，而是遍历病毒源，与s542一起对比着理解
namespace s994o1
{   // o1为灵神题解复制过来，但修改了输入数组grid，更规范的做法见m1
    class Solution {
        int DIRECTIONS[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    public:
        int orangesRotting(vector<vector<int>>& grid) {
            int m = grid.size(), n = grid[0].size();

            int fresh = 0;// 新鲜橘子的个数
            vector<pair<int, int>> q;// 统计一开始就腐烂的橘子的位置
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (grid[i][j] == 1) {
                        fresh++; // 统计新鲜橘子个数
                    }
                    else if (grid[i][j] == 2) {
                        q.emplace_back(i, j); // 一开始就腐烂的橘子
                    }
                }
            }

            int ans = 0;
            // 可能存在永远不会腐烂的橘子，fresh变量也应为循环条件
            while (fresh && !q.empty()) {
                ans++; // 经过一分钟
                vector<pair<int, int>> nxt;
                for (auto& [x, y] : q) { // 已经腐烂的橘子
                    for (auto d : DIRECTIONS) { // 四方向
                        int i = x + d[0], j = y + d[1];
                        if (0 <= i && i < m && 0 <= j && j < n && grid[i][j] == 1) { // 新鲜橘子
                            fresh--;
                            grid[i][j] = 2; // 变成腐烂橘子
                            nxt.emplace_back(i, j);
                        }
                    }
                }
                q = move(nxt);
            }

            return fresh ? -1 : ans;
        }
    };
}
namespace s994m1
{   // m1为不修改输入grid的规范形式，且将数组改成BFS更常见的队列，不需要每轮结束后再move了
    // 多源BFS需要将所有起点加入队列，特别适合"多个起点，多个终点"的最短路径问题
    class Solution {
        static inline constexpr int dirs[4][2] = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
        };

    public:
        int orangesRotting(vector<vector<int>>& grid) {
            int m = grid.size(), n = grid[0].size();

            // 统计需要腐烂的橘子数量，已经初始腐烂的橘子（病毒源）的位置
            int fresh = 0;
            queue<pair<int, int>> rots;
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (grid[i][j] == 1) {
                        ++fresh;
                    }
                    else if (grid[i][j] == 2) {
                        rots.emplace(i, j);
                    }
                }
            }

            // 如果没有新鲜橘子，直接返回0
            if (fresh == 0) return 0;

            vector<vector<bool>> visited(m, vector<bool>(n, false));
            int ans = 0;
            while (fresh && !rots.empty()) {
                ++ans;// 经过一分钟
                int levelSize = rots.size();
                for (int i = 0; i < levelSize; ++i) {
                    auto [x, y] = rots.front();
                    rots.pop();
                    // 对ranged for loop，一维数组是可以自动生效的，标准库为其准备了重载版本
                    for (auto& [dx, dy] : dirs) {
                        int row = x + dx;
                        int col = y + dy;
                        if (row >= 0 && row < m && col >= 0 && col < n
                            && grid[row][col] == 1 && !visited[row][col]) {
                            --fresh;
                            rots.emplace(row, col);
                            visited[row][col] = true;
                        }
                    }
                }                
            }
            return fresh ? -1 : ans;// 可能存在永远无法腐烂的橘子
        }
    };
}

namespace s909o1
{   // 难点在于通过序号给出行号列号
    // 主要思想不难理解，就是六叉树下的BFS
    class Solution {
    public:
        int snakesAndLadders(vector<vector<int>>& board) {
            int n = board.size();
            vector<char> visited(n * n + 1, false);
            queue<int> q;
            visited[1] = true;
            q.push(1);

            for (int step = 0; !q.empty(); ++step) {
                int levelSize = q.size();
                for (int i = 0; i < levelSize; ++i) {
                    int x = q.front(); q.pop();

                    if (x == n * n) {
                        return step;
                    }
                    // 行号用y - 1来计算更直观
                    for (int y = x + 1; y <= min(x + 6, n * n); ++y) {
                        int r = (y - 1) / n; // 这里的r不代表最终行号，初始化为过渡值
                        int c = (y - 1) % n; // 如果目标与起点行号之差为偶数，则c与起点行同向
                        if (r % 2) {
                            c = n - 1 - c;   // 与起点行反向，从右往左移动
                        }
                        r = n - 1 - r;       // 行号

                        int nxt = board[r][c];
                        if (nxt < 0) {
                            nxt = y;         // 普通格子，不是蛇也不是梯子
                        }
                        if (!visited[nxt]) {
                            visited[nxt] = true; // 避免BFS路径上出现环，死循环
                            q.push(nxt);
                        }
                    }
                }
            }

            return -1;
        }
    };

}
// ---------------------
// 三、网格图 0-1 BFS()
/*

*/
// ---------------------
// 暂时跳过
// ---------------------
// 四、网格图 Dijkstra()
/*

*/
// ---------------------
// 详见图论题单
// ---------------------
// 五、综合(1)
/*
329

*/
// ---------------------
