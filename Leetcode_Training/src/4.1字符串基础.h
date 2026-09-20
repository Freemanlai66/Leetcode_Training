#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

/*

344.反转字符串：编写一个函数，其作用是将输入的字符串反转过来。输入字符串以字符数组 s 的形式给出。
				不要给另外的数组分配额外的空间，你必须原地修改输入数组、使用 O(1) 的额外空间解决这一问题。

541.反转字符串II：给定一个字符串 s 和一个整数 k，从字符串开头算起，每计数至 2k 个字符，就反转这 2k 字符中的前 k 个字符。
                如果剩余字符少于 k 个，则将剩余字符全部反转。
                如果剩余字符小于 2k 但大于或等于 k 个，则反转前 k 个字符，其余字符保持原样。

k44.替换数字：给定一个字符串 s，它包含小写字母和数字字符，请编写一个函数。
            将字符串中的字母字符保持不变，而将每个数字字符替换为number。
            例如，对于输入字符串 "a1b2c3"，函数应该将其转换为 "anumberbnumbercnumber"。
            对于输入字符串 "a5b"，函数应该将其转换为 "anumberb"
            输入：一个字符串 s,s 仅包含小写字母和数字字符。
            输出：打印一个新的字符串，其中每个数字字符都被替换为了number
            样例输入：a1b2c3
            样例输出：anumberbnumbercnumber

58.最后一个单词的长度：给你一个字符串 s，由若干单词组成，单词前后用一些空格字符隔开。返回字符串中最后一个单词的长度。

151.反转字符串中的单词：给你一个字符串 s ，请你反转字符串中单词的顺序。
                单词是由非空格字符组成的字符串。s 中使用至少一个空格将字符串中的单词分隔开。
                返回单词顺序颠倒且单词之间用单个空格连接的结果字符串。
                注意：输入字符串 s中可能会存在前导空格、尾随空格或者单词间的多个空格。
                返回的结果字符串中，单词间应当仅用单个空格分隔，且不包含任何额外的空格。

k55.右旋字符串：字符串的右旋转操作是把字符串尾部的若干个字符转移到字符串的前面。
                给定一个字符串 s 和一个正整数 k，请编写一个函数，将字符串中的后面 k 个字符移到字符串的前面，实现字符串的右旋转操作。 
                例如，对于输入字符串 "abcdefg" 和整数 2，函数应该将其转换为 "fgabcde"。
*/

namespace s344
{   // 双指针，没什么太多解释的，主要是记得用swap
    class Solution {
    public:
        void reverseString(vector<char>& s) {
            for (int i = 0, j = s.size() - 1; i < j; i++, j--) {
                swap(s[i], s[j]);
            }
        }
    };
}

namespace s541o1
{   // 既然题目没有要求自己实现reverse，那么就直接调用库函数
    // reverse的区间为[0,k), [2k,3k), [4k,5k), ...
    class Solution {
    public:
        string reverseStr(string s, int k) {
            int n = s.size();
            for (int i = 0; i < n; i += 2 * k) {
                reverse(s.begin() + i, s.begin() + min(n, k + i));
            }
            return s;
        }
    };
}
namespace s541o2
{   // 自己编写reverse的版本
    class Solution {
    private:
        void reverse(string& s, int start, int end) {
            for (int i = start, j = end; i < j; i++, j--) {
                swap(s[i], s[j]);
            }
        }
    public:
        string reverseStr(string s, int k) {
            for (int i = 0; i < s.size(); i += (2 * k)) {
                // 1. 每隔 2k 个字符的前 k 个字符进行反转
                // 2. 剩余字符小于 2k 但大于或等于 k 个，则反转前 k 个字符
                if (i + k <= s.size()) {
                    reverse(s, i, i + k - 1);
                    continue;
                }
                // 3. 剩余字符少于 k 个，则将剩余字符全部反转。
                reverse(s, i, s.size() - 1);
            }
            return s;
        }
    };
}
namespace s541m
{   // 自己写的方法，先计算到底有几个需要反转的区间，然后根据最后一个区间的长度确定下标，仅做纪念，算是垃圾代码
    class Solution {

    public:
        string reverseStr(string s, int k) {
            int lens = s.size();
            int loop = lens / (2 * k);
            string ans;
            for (int i = 0; i < loop; ++i) {
                for (int m = i * 2 * k, n = i * 2 * k + k - 1; m < n; ++m, --n) {
                    swap(s[m], s[n]);
                }
            }
            int rest = lens % (2 * k);
            if (rest > k) {
                for (int i = loop * 2 * k, j = loop * 2 * k + k - 1; i < j; ++i, --j) {
                    swap(s[i], s[j]);
                }
            }
            else {
                for (int i = loop * 2 * k, j = s.size() - 1; i < j; ++i, --j) {
                    swap(s[i], s[j]);
                }
            }
            return s;
        }
    };
}

namespace k44
{   // 双指针做法，从后往前填充
    // 
    int main() {
        string s;
        while (cin >> s) {
            int count = 0;
            int oldIndex = s.size() - 1;
            for (const char& c : s) {
                if (c >= '0' && c <= '9') {
                    ++count;// 统计数字的个数
                }
            }
            // 扩充字符串s的大小，也就是将每个数字替换成"number"之后的大小
            s.resize(s.size() + count * 5);
            int newIndex = s.size() - 1;
            while (oldIndex >= 0) {
                if (s[oldIndex] >= '0' && s[oldIndex] <= '9') {
                    // 从后往前将数字替换为"number"
                    s[newIndex--] = 'r';
                    s[newIndex--] = 'e';
                    s[newIndex--] = 'b';
                    s[newIndex--] = 'm';
                    s[newIndex--] = 'u';
                    s[newIndex--] = 'n';
                }
                else {
                    s[newIndex--] = s[oldIndex];
                }
                --oldIndex;
            }
            cout << s << endl;
        }
        return 0;
    }
}

namespace s58
{   // 双指针截取单词
    class Solution {
    public:
        int lengthOfLastWord(string s) {
            int right = s.size() - 1;
            while (right >= 0 && s[right] == ' ') {
                --right;
            }
            int left = right;
            while (left >= 0 && s[left] != ' ') {
                --left;
            }
            return right - left;
        }
    };
}

namespace s151o1
{   // 双指针法，使用额外O(n)空间构造wordsVector，逆序遍历获取单词
    class Solution {
    public:
        string reverseWords(string s) {
            vector<string> wordsVector;
            int i = s.size() - 1;
            int j = i;
            while (i >= 0) {
                while (i >= 0 && s[i] == ' ') {
                    --i;
                    j = i;
                }
                while (i >= 0 && s[i] != ' ') {
                    --i;
                }
                if (i != j) {
                    wordsVector.push_back(s.substr(i + 1, (j - i)));
                }                 
            }
            string result;
            for (int n = 0; n < wordsVector.size(); ++n) {
                result += wordsVector[n];
                if (n != wordsVector.size() - 1) {
                    result += ' ';
                }                 
            }
            return result;
        }
    };
}
namespace s151o2
{   // 多次分步双指针，原地修改，空间复杂度为O(1)
    // 1.去除所有额外空格; 2.整体reverse; 3.将所有单词reverse
    class Solution {
    public:
        string reverseWords(string s) {
            // remove extra spaces
            int slow = 0, n = s.size();
            for (int fast = 0; fast < n; ++fast) {
                if (s[fast] != ' ') {
                    // 将插入中间空格的过程与扫描新单词的行为绑定，这样就不会出错
                    if (slow != 0) {
                        // 不是第一个单词，就要加上空格
                        s[slow++] = ' ';
                    }
                    while (fast < n && s[fast] != ' ') {
                        s[slow++] = s[fast++];
                    }
                }
            }
            n = slow;
            s.resize(n);
            // reverse the entire string
            reverse(s.begin(), s.end());
            // reverse separate words
            int i = 0, j = 0;
            while (j < n) {
                while (j < n && s[j] != ' ') {
                    ++j;
                }
                reverse(s.begin() + i, s.begin() + j);
                ++j;
                i = j;
            }
            return s;
        }
    };
}
namespace s151m1
{   // 如果要求自己写reverse函数的写法
    // 时隔一年半重新按照自己的编程习惯写一次
    class Solution {
    private:
        void reverseStr(string& s, int idx1, int idx2) {
            while (idx1 < idx2) {
                swap(s[idx1++], s[idx2--]);
            }
        }

    public:
        string reverseWords(string s) {
            // 反转后的字符串前后不能有多余的空格
            int low = 0, n = s.size();
            for (int high = 0; high < n; ++high) {
                if (s[high] != ' ') {
                    if (low != 0) {
                        s[low++] = ' ';
                    }
                    while (high < n && s[high] != ' ') {
                        s[low++] = s[high++];
                    }
                }
            }

            // [0, low - 1]之间是去除多余空格的单词，进行反转
            int high = low - 1;
            reverseStr(s, 0, high);

            // 逐个反转每个单词
            int start = 0;
            while (start <= high) {
                int end = start;
                while (end <= high && s[end] != ' ') ++end;
                reverseStr(s, start, end - 1);

                start = ++end;
            }

            return s.substr(0, high + 1);
        }
    };
}
namespace s151m2
{   // 又隔了一天写出来的
    class Solution {
    public:
        string reverseWords(string s) {
            // 去除多余空格
            int slow = 0, n = s.size();
            for (int fast = 0; fast < n; ++fast) {
                if (s[fast] == ' ') continue;

                if (slow != 0) {
                    s[slow++] = ' ';
                }

                while (fast < n && s[fast] != ' ') {
                    s[slow++] = s[fast++];
                }
            }

            // 整个反转
            s.resize(slow);
            n = s.size();
            reverse(s.begin(), s.end());

            // 逐个反转单词
            for (int end = 0; end < n; ++end) {
                int start = end;
                while (end < n && s[end] != ' ') {
                    ++end;
                }
                reverse(s.begin() + start, s.begin() + end);
            }
            return s;
        }
    };
}

namespace k55
{   // 考验技巧，先整体反转一次，然后再局部反转一次，就能得到答案
    int main()
    {
        int n;
        string s;
        cin >> n;
        cin >> s;
        reverse(s.begin(), s.end());
        reverse(s.begin(), s.begin() + n);
        reverse(s.begin() + n, s.end());
        cout << s << endl;
        return 0;
    }
}