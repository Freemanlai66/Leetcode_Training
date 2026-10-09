#pragma once
#include <vector>
#include <iostream>
#include <algorithm> // 包含 min max
#include <climits> // 提供INT_MAX
#include <cmath> // 提供abs（包含整数、浮点数的重载，c++11以上的版本，只需要cmath）

using namespace std;

// 求解数组区间和的常用技巧：前缀和 prefix sum

/*

k58：给定一个整数数组 Array，请计算该数组在每个指定区间内元素的总和。

k44：在一个城市区域内，被划分成了n * m个连续的区块，每个区块都拥有不同的权值，代表着其土地价值。
    目前，有两家开发公司，A 公司和 B 公司，希望购买这个城市区域的土地。 
    现在，需要将这个城市区域的所有区块分配给 A 公司和 B 公司。
    然而，由于城市规划的限制，只允许将区域按横向或纵向划分成两个子区域，而且每个子区域都必须包含一个或多个区块。 
    为了确保公平竞争，你需要找到一种分配方式，使得 A 公司和 B 公司各自的子区域内的土地总价值之差最小。 

*/

namespace k58o1
{	// 数组技巧：前缀和，只要涉及到数组区间的计算，都可以考虑使用！
    // 如果直接暴力解法，每次输入新的边界（长度为n）后，加入for循环挨个读取，查询m次时的时间复杂度为O(m*n),非常慢
    // 而如果将sumArray[0] = array[0], sumArray[1] =  array[0] +  array[1], ...
    // 在输入查询的边界后，直接使用sumArray元素的差值即可快速的得到区间和
    int main()
    {
        int lb, rb, arrayLen;
        int sum = 0, index = 0;
        int ans;
        // initializing array
        scanf("%d", &arrayLen);
        vector<int> sumArray(arrayLen);

        while (arrayLen--) {
            int temp;
            scanf("%d", &temp);
            sum += temp;
            sumArray[index++] = sum;
        }
        // input bounrady
        // 重要！！！： scanf的返回值为成功读取的变量数目，如果未读取成功则返回EOF = -1，但是-1并不能退出循环，所以会出现死循环
        // 可以将循环条件改成scanf("%d", &a) != EOF， 或是在最前面加个按位取反运算符~，将-1转化成0
        // 整数-1的二进制表示为1111111111111111，经过~后，会变成0000000000000000
        // 可以使用#include<bitset>; cout << (bitset<16>) value << endl; 查看变量的二进制表示
        while (~scanf("%d%d", &lb, &rb)) {
            if (lb == 0) {
                ans = sumArray[rb];
            }
            else {
                ans = sumArray[rb] - sumArray[lb - 1];
            }
            printf("%d\n", ans);
        }
    }
}

namespace k44m
{   // 二维前缀和，先将每行和每列当成一个整体，分别得到行、列的前缀和
    // 然后通过移动cut切割边界来得到最小差值
    int main() {
        // input n, m
        int n, m;
        scanf("%d%d", &n, &m);
        vector<int> sumRow(n), sumCol(m);
        // initialzing matrix sum
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                int temp;
                scanf("%d", &temp);
                // calculating sumRow, sumCol
                sumRow[i] += temp;
                sumCol[j] += temp;
            }
        }
        int dif = INT_MAX;
        vector<int> comRow(n, 0), comCol(m, 0);
        // comparing row
        if (n > 1) {    // 如果 n = 1，说明只有一行，那么在行上分割是没有意义的
            comRow[0] = sumRow[0];
            for (int i = 1; i < n; ++i) {
                comRow[i] = sumRow[i] + comRow[i - 1]; // 这里可能忘记加上comRow[i - 1]
            }
            for (int cut = 0; cut < n - 1; ++cut) {
                // A公司价值（左边/上边）：comRow[cut]
                // B公司价值（右边/下边）：comRow[n - 1] - comRow[cut]
                // 两者差值（A - B）：2 * comRow[cut] - comRow[n - 1]
                dif = min(dif, abs(2 * comRow[cut] - comRow[n - 1]));
            }

        }
        // comparing col  
        if (m > 1) {
            comCol[0] = sumCol[0];
            for (int j = 1; j < m; ++j) {
                comCol[j] = sumCol[j] + comCol[j - 1];
            }
            for (int cut = 0; cut < m - 1; ++cut) {
                dif = min(dif, abs(2 * comCol[cut] - comCol[m - 1]));
            }
        }
        // ouput dif
        printf("%d", dif);
    }

}