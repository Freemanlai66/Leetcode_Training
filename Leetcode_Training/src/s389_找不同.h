#pragma once
#include <iostream>
#include <string>
using namespace std;

/*
本题与 136：只出现一次的数字本质相同

给定两个字符串 s 和 t ，它们只包含小写字母。
字符串 t 由字符串 s 随机重排，然后在【随机】位置添加一个字母。
请找出在 t 中被添加的字母。
*/


namespace s389o1 {
	  //位运算：
	  //1.把两个字符串当做是一个整体
	  //2.一个数与另一个数进行两次异或操作得到的是自身
	  //3.只有多出来的数单次出现
	  //4.0与任何数异或的到就是那个数
class Solution {
public:
	char findTheDifference(string s, string t)
	{
        char ret = 0;
        for( char ch : s)
        {
            ret ^= ch;
        }
        for (char ch : t)
        {
            ret ^= ch;
        }
        return ret;
	}
};

}
namespace s389o2 {
	//计数法：
	//	1.小写字母实际上是数
	//	2.把两个字符串每个字符代表的数求和
	//	3.两者相减就是多出来的那个数（字符）

class Solution {
public:
    char findTheDifference(string s, string t) {
        int as = 0, at = 0;
        for (char ch: s) {
            as += ch;
        }
        for (char ch: t) {
            at += ch;
        }
        return at - as;//最后是int到char的隐式转换
    }
};


}