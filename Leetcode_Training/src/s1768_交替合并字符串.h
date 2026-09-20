#pragma once
#include <string>
#include <iostream>
using std::string;

/*
给你两个字符串 word1 和 word2 。请你从 word1 开始，通过交替添加字母来合并字符串。如果一个字符串比另一个字符串长，就将多出来的字母追加到合并后字符串的末尾。
*/

namespace s1768m
{
class Solution {
public:
	string mergeAlternately(string word1, string word2) const;
};
string Solution::mergeAlternately(string word1, string word2) const
{
	unsigned int len1 = word1.length();
	unsigned int len2 = word2.length();
	string mergeword = "";
	int flag_word1_is_longer = (len1 >= len2) ? 1 : 0;
	if (flag_word1_is_longer)
	{
		for (unsigned int i = 0; i < len2; i++)
		{
			mergeword += word1[i];
			mergeword += word2[i];
		}
		for (unsigned int j = len2; j < len1; j++)
			mergeword += word1[j];
	}
	else
	{
		for (unsigned int i = 0; i < len1; i++)
		{
			mergeword += word1[i];
			mergeword += word2[i];
		}
		for (unsigned int j = len1; j < len2; j++)
			mergeword += word2[j];
	}
	return mergeword;
}

}
namespace s1768o
{
class Solution {
public:
    string mergeAlternately(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        int i = 0, j = 0;
        
        string ans;
        ans.reserve(m + n);
        while (i < m || j < n) {
            if (i < m) {
                ans.push_back(word1[i]);
                ++i;
            }
            if (j < n) {
                ans.push_back(word2[j]);
                ++j;
            }
        }
        return ans;
    }
};

}




