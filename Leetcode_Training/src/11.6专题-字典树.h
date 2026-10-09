#pragma once
#include <vector>
#include <string>
#include <memory>
#include <array>

using namespace std;

// 问题待定：
/*
sxxx：xxx
*/

/*
模板题：
1.用两个队列实现栈，或用一个队列实现栈，环形结构，纯考察数据结构理解，无实际应用价值，每次入队都是O(n)：225

*/

// 字典树（Trie）：基础 + 进阶 + 字典树优化DP + 0-1字典树（异或字典树）

// 【6.1】基础 (1)
// 26叉树
/*
208.实现 Trie (前缀树)：Trie（发音类似 "try"）或者说 前缀树 是一种树形数据结构，
用于高效地存储和检索字符串数据集中的键。这一数据结构有相当多的应用情景，例如自动补全和拼写检查。
请你实现 Trie 类：Trie() 初始化前缀树对象。
void insert(String word) 向前缀树中插入字符串 word 。
boolean search(String word) 如果字符串 word 在前缀树中，返回 true（即，在检索之前已经插入）；否则，返回 false 。
boolean startsWith(String prefix) 如果之前已经插入的字符串 word 的前缀之一为 prefix ，返回 true ；否则，返回 false 。
*/
// ---------------------
// 模板题1：字典树基础，看完灵神思路后没看题解自己写了一版m1出来，字典树简单实现出来不难
namespace s208m1
{   // 我的版本没有设计析构函数，且search, startWith两个成员函数共通的代码没有进一步抽象成工具private函数
    struct myTree {
        bool isEnd = false;
        vector<myTree*> sons;

        myTree() : sons(26, nullptr) {}
    };

    class Trie {
    private:
        myTree tree;
        myTree* root = &tree;
    public:
        Trie() {

        }

        void insert(string word) {
            myTree* cur = root;
            int i = 0, len = word.size();
            while (i < len) {
                int index = word[i] - 'a';
                if (!(cur->sons[index])) {
                    cur->sons[index] = new myTree();
                }
                cur = cur->sons[index];
                ++i;
            }
            cur->isEnd = true;
        }

        bool search(string word) {
            myTree* cur = root;
            int len = word.size(), i = 0;
            while (cur && i < len) {
                int index = word[i] - 'a';
                if (cur->sons[index]) {
                    cur = cur->sons[index];
                    ++i;
                }
                else {
                    break;
                }
            }
            return cur->isEnd;
        }

        bool startsWith(string prefix) {
            myTree* cur = root;
            int len = prefix.size(), i = 0;
            while (cur && i < len) {
                int index = prefix[i] - 'a';
                if (cur->sons[index]) {
                    cur = cur->sons[index];
                    ++i;
                }
                else {
                    break;
                }
            }
            return i == len;
        }
    };

    /**
     * Your Trie object will be instantiated and called as such:
     * Trie* obj = new Trie();
     * obj->insert(word);
     * bool param_2 = obj->search(word);
     * bool param_3 = obj->startsWith(prefix);
     */
}
namespace s208o1
{   // 学到了二叉树怎么析构（递归运行成员函数destroy()），以及使用状态int精简代码
    struct Node {
        bool isEnd = false;
        vector<Node*> sons;

        Node() : sons(26, nullptr) {}
    };

    class Trie {
    private:
        Node* root;

        // 将析构功能独立成函数的原因，N叉树的析构可以用递归
        void destroy(Node* node) {
            if (!node) return;
            for (Node* son : node->sons) {
                destroy(son);
            }
            delete node;
        }

        int find(string word) {
            Node* cur = root;
            for (char c : word) {
                int index = c - 'a';
                if (cur->sons[index] == nullptr) {
                    return 0; // 0 = 没匹配上
                }
                cur = cur->sons[index];
            }
            // 2 = 完全匹配，1 = 前缀匹配
            return cur->isEnd ? 2 : 1;
        }

    public:
        Trie() {
            // 这一行最容易漏，如果不对root初始化分配内存，那么root就是一个空指针
            root = new Node();
        }
        // 析构和禁止拷贝构造，要么不写，要么写全，分析详见s211o1
        ~Trie() {
            destroy(root);
        }
        Trie(const Trie&) = delete;
        Trie& operator= (const Trie&) = delete;

        void insert(string word) {
            Node* cur = root;
            for (char c : word) {
                int index = c - 'a';
                if (cur->sons[index] == nullptr) {
                    cur->sons[index] = new Node();
                }
                cur = cur->sons[index];
            }
            cur->isEnd = true;
        }

        bool search(string word) {
            return find(word) == 2;
        }

        bool startsWith(string prefix) {
            return find(prefix) != 0;
        }
    };
}
namespace s208m2
{   // 在m1基础上精简了代码（但没加上析构）
    struct myTree {
        bool isEnd = false;
        vector<myTree*> sons;

        myTree() : sons(26, nullptr) {}
    };

    class Trie {
    private:
        myTree tree;
        myTree* root = &tree;

        int find(string word) {
            myTree* cur = root;
            int len = word.size(), i = 0;
            while (cur && i < len) {
                int index = word[i] - 'a';
                if (cur->sons[index]) {
                    cur = cur->sons[index];
                    ++i;
                }
                else {
                    return 0;
                }
            }
            return cur->isEnd ? 2 : 1;
        }
    public:
        Trie() {

        }

        void insert(string word) {
            myTree* cur = root;
            int i = 0, len = word.size();
            while (i < len) {
                int index = word[i] - 'a';
                if (!(cur->sons[index])) {
                    cur->sons[index] = new myTree();
                }
                cur = cur->sons[index];
                ++i;
            }
            cur->isEnd = true;
        }

        bool search(string word) {
            return find(word) == 2;
        }

        bool startsWith(string prefix) {
            return find(prefix) != 0;
        }
    };
    /**
     * Your Trie object will be instantiated and called as such:
     * Trie* obj = new Trie();
     * obj->insert(word);
     * bool param_2 = obj->search(word);
     * bool param_3 = obj->startsWith(prefix);
     */
}
namespace s208o2
{   // 在o1基础上，改用强枚举类型表示find的查询结果
    struct Node {
        bool isEnd;
        vector<Node*> sons;
        Node() : isEnd(false), sons(26, nullptr) {}
    };

    class Trie {
    private:
        Node* dummy;

        enum class State {
            PERFECT_MATCH, PREFIX_MATCH, NO_MATCH
        };

        void destroy(Node* node) {
            if (!node) return;
            for (Node* son : node->sons) {
                destroy(son);
            }
            delete node;
        }

        State find(string word) {
            Node* cur = dummy;
            for (char c : word) {
                int index = c - 'a';
                if (cur->sons[index] == nullptr) {
                    return State::NO_MATCH;
                }
                cur = cur->sons[index];
            }
            return cur->isEnd ? State::PERFECT_MATCH : State::PREFIX_MATCH;
        }

    public:
        Trie() : dummy(new Node()) {

        }
        ~Trie() {
            destroy(dummy);
        }

        void insert(string word) {
            Node* cur = dummy;
            for (char c : word) {
                int index = c - 'a';
                if (cur->sons[index] == nullptr) {
                    cur->sons[index] = new Node();
                }
                cur = cur->sons[index];
            }
            cur->isEnd = true;
        }

        bool search(string word) {
            return find(word) == State::PERFECT_MATCH;
        }

        bool startsWith(string prefix) {
            return find(prefix) != State::NO_MATCH;
        }
    };

    /**
     * Your Trie object will be instantiated and called as such:
     * Trie* obj = new Trie();
     * obj->insert(word);
     * bool param_2 = obj->search(word);
     * bool param_3 = obj->startsWith(prefix);
     */
}
namespace s208o3
{   // 改用array，用RAII的root，并且将sons析构直接放在Node里
    class Trie {
    private:
        enum class State {
            PERFECT_MATCH, PREFIX_MATCH, NO_MATCH
        };
        struct Node {
            bool isEnd = false;
            array<Node*, 26> sons{};
            ~Node() {
                // 递归析构，整棵树一起释放                      
                for (Node* p : sons) delete p;
            }
        };

        // RAII，自动禁拷贝，同时dummy改成root，命名更规范
        // 如果不用智能指针，那么根据rule of three：
        // (一旦你写了析构函数，通常也必须写（或显式删除）拷贝构造和拷贝赋值)
        // 就需要额外的代码禁掉拷贝
        // Trie(const Trie&) = delete;
        // Trie& operator=(const Trie&) = delete;  
        // 最好的方式还是直接改成智能指针
        std::unique_ptr<Node> root = std::make_unique<Node>();

        // 传引用，避免拷贝，const引用 + const方法
        State find(const string& word) const {
            const Node* cur = root.get();
            for (char c : word) {
                int index = c - 'a';
                cur = cur->sons[index];      // 先走
                if (!cur) return State::NO_MATCH;   // 走到空就算失败
                /* 与下面写法等价，取出->再验证，比之前那样重复写cur->sons[index]好
                const Node* nxt = cur->sons[index];   // 取槽位
                if (!nxt) return State::NO_MATCH;       // 缺失 → 判定失败
                cur = nxt;
                */
            }
            return cur->isEnd ? State::PERFECT_MATCH : State::PREFIX_MATCH;
        }

    public:
        Trie() = default;

        // leetcode题干给的签名就不用改了，不用改成传引用
        void insert(string word) {
            Node* cur = root.get();
            for (char c : word) {
                int index = c - 'a';
                Node*& nxt = cur->sons[index];
                if (!nxt) {
                    nxt = new Node();
                }
                cur = nxt;
            }
            cur->isEnd = true;
        }

        bool search(string word) {
            return find(word) == State::PERFECT_MATCH;
        }

        bool startsWith(string prefix) {
            return find(prefix) != State::NO_MATCH;
        }
    };
}
// ---------------------
// 【6.2】进阶 (1)
/*
211.请你设计一个数据结构，支持 添加新单词 和 查找字符串是否与任何先前添加的字符串匹配 。
实现词典类 WordDictionary ：
WordDictionary() 初始化词典对象
void addWord(word) 将 word 添加到数据结构中，之后可以对它进行匹配
bool search(word) 如果数据结构中存在字符串与 word 匹配，则返回 true ；
否则，返回  false 。word 中可能包含一些 '.' ，每个 . 都可以表示任何一个字母。
*/
// ---------------------
// 模板题2：字典树的递归搜索，与s208不同，查询路径不再是固定的
namespace s211o1
{   // 因为'.'可以代表任何字符，所以不能再用迭代了，而是需要使用DFS递归（也可以用迭代写，但不够直观）
    struct Node {
        bool isEnd;
        vector<Node*> sons;

        Node() : isEnd(false), sons(26, nullptr) {}
    };

    class WordDictionary {
    private:
        Node* root;

        void destroy(Node* node) {
            if (!node) return;
            for (Node* son : node->sons) {
                destroy(son);
            }
            delete node;
        }

        bool find(const string& word, Node* cur, int idx) {
            if (!cur) return false;
            if (idx == word.size()) return cur->isEnd;

            char c = word[idx];
            if (c != '.') {
                // 将判断是否字母匹配丢给下层递归去做，这样idx == word.size()的判断也刚好
                return find(word, cur->sons[c - 'a'], idx + 1);
            }

            for (Node* son : cur->sons) {
                if (find(word, son, idx + 1)) {
                    return true;
                }
            }
            return false;
        }

    public:
        WordDictionary() : root(new Node()) {}

        // 下面的析构函数和禁止拷贝构造的操作，在面试时可以不写，因为算法题默认可以省略一些技术细节
        // 如果你写出来了析构，那么就得做到尽善尽美，比如必须禁止拷贝构造
        // 而且最佳实践其实是不用裸指针，而是智能指针，详见o2
        ~WordDictionary() {
            destroy(root);
        }
        WordDictionary(const WordDictionary&) = delete;             // 禁止默认的浅拷贝构造
        WordDictionary& operator=(const WordDictionary&) = delete;  // 防止double free

        void addWord(string word) {
            Node* cur = root;
            for (char c : word) {
                int i = c - 'a';
                if (cur->sons[i] == nullptr) {
                    cur->sons[i] = new Node();
                }
                cur = cur->sons[i];
            }
            cur->isEnd = true;
        }

        bool search(string word) {
            return find(word, root, 0);
        }
    };
}
namespace s211o2
{   // 智能指针写法，无需析构和禁止默认拷贝构造，且有非常多坑点和细节
    class WordDictionary {
    private:
        struct Node {
            std::array<std::unique_ptr<Node>, 26> sons;  // 值初始化 → 26 个 nullptr
            bool isEnd = false;                          // NSDMI，不用写构造函数
            // Non-Static Data Member Initializer, 效果等同于在构造函数初始化列表里写 isEnd(false)
        };
        std::unique_ptr<Node> root = std::make_unique<Node>();   // NSDMI，也不用写构造函数

        // 只读操作，参数用 const Node*，天然 const 正确。搜索本来就只读，用 const 表达出来，还能防止手滑改坏树
        // static：这个函数不碰 this，声明成静态成员函数，语义更准确，也省掉一个隐式参数
        static bool dfs(const Node* node, const std::string& word, int idx) {
            if (!node) return false;                             // 空分支剪掉
            if (idx == static_cast<int>(word.size()))
                return node->isEnd;                              // 走完了 → 结算
            char c = word[idx];
            if (c != '.')
                // 取裸指针一律用.get()，unique_ptr::get() 返回的是 Node*（非 const）
                return dfs(node->sons[c - 'a'].get(), word, idx + 1);
            for (const auto& son : node->sons)   // 通配符：逐个试
                // son 是 const unique_ptr<Node>&，只能读、只能 .get()，不能 Node* p = son();
                // 下面的if判断语句只能写if (son && dfs(son.get(), ...))
                if (son && dfs(son.get(), word, idx + 1)) return true;
            return false;
        }
    public:
        // 注意：构造函数、析构函数、destroy() 全都不需要写 —— 这就是 Rule of Zero
        void addWord(const std::string& word) {
            Node* cur = root.get();
            for (char c : word) {
                int i = c - 'a';
                if (!cur->sons[i]) cur->sons[i] = std::make_unique<Node>();
                cur = cur->sons[i].get();
                // 千万别写成 cur = cur->sons[i];（类型是 unique_ptr<Node>，赋不给 Node*
            }
            cur->isEnd = true;
        }
        bool search(const std::string& word) const {
            return dfs(root.get(), word, 0);
        }
    };
}

// ---------------------
// 【6.3】字典树优化DP ()
// 
/*

*/
// ---------------------


// ---------------------
// 【6.4】0-1字典树 / 异或字典树 ()
// 
/*

*/
// ---------------------


// ---------------------