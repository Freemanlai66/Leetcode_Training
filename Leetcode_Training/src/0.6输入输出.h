#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>  // setprecison(), fixed, setw(), setfill()
#include <cmath>    //  M_PI（VS没有，但牛客网有）

using namespace std;

// 接收带空格的字符串：cin.get()跳过换行 + getline读取包含空格的整行
namespace sPIO13 
{
    int main() {
        int t;
        cin >> t;

        while (t--) {
            int n;
            cin >> n;
            cin.get(); // '\n'
            string s;
            getline(cin, s);

            for (int i = n - 1; i >= 0; --i) {
                if (s[i] != ' ') {
                    cout << s[i];
                }
            }
            cout << endl;
        }
    }
}

// 输出固定位数的定点小数，位数不符时要补0或四舍五入：setprecision() + fixed
namespace sPIO14 
{
    // #include <iomanip>: setprecision()
    // setprecision(n) 单独使用时，表示输出总共 n 位有效数字（即整数部分和小数部分加起来一共 n 位，不包括小数点）
    // 当数值较大或较小时，C++ 还会自动使用科学记数法（e 格式）来保证有效数字位数。
    // fixed 强制输出定点小数格式（不使用科学记数法）
    int main() {
        float x;
        cin >> x;
        cout << fixed << setprecision(3) << x << endl;
    }
}

// 输出补充前导0的数：setfill() + setw()
namespace sPIO15
{   /*  必须包含头文件<iomanip>
    setw(9)：设置输出宽度为 9 个字符。如果实际数字的位数不足 9，就在左边用填充字符补满到 9 位。
    setfill('0')：设置填充字符为 '0'（默认是空格）。
    持久性：setfill 一旦设置，会一直有效，直到下一次修改。setw 只对紧随其后的一次输出有效，
        所以每次输出前都需要重新设置宽度。
    超出宽度的行为：如果数字位数超过 9，则不受 setw 限制，会按实际宽度全部输出（不会截断）。这正是大多数题目的期望。
    */
    int main() {
        long long n;     // 用 long long 防溢出
        cin >> n;
        cout << setw(9) << setfill('0') << n << endl;
        return 0;
    }
}

// 输出double，要求满足精度要求：setprecision + fixed
namespace sPIO17
{   // C++ 中 double 的数据精度 和 默认输出格式是不一样的
    // double 运算结果的存储精度约为 15～16 位十进制有效数字，但默认用 cout 输出时只会显示 6 位有效数字
    const double M_PI = 3.14159265358979323846;
    int main() {
        double r;
        cin >> r;
        double area = M_PI * r * r;
        // 不设置定点固定小数位，则只会显示6位有效数字
        cout << fixed << setprecision(6) << area << endl;
    }
}