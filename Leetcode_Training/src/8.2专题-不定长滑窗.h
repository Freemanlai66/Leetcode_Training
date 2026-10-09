#pragma once
#include<unordered_map>
#include<algorithm>
#include<string>
#include<numeric> // accumulate
#include<climits> // INT_MAX
#include<queue>
#include<unordered_map>
#include<unordered_set>
#include<map>
using namespace std;

// s632 堆

/*
模板题：
1.求最长/最大子数组：3
2.求最短/最小子数组：209
3.越长越合法：1358
4.越短越合法：713
5.恰好型滑窗：930
6.修改/替换k次获得最长的相同子序列：424
*/

// 不定长滑动窗口(40)
// 包含三类：求最长子数组，求最短子数组，以及求子数组个数。

// 【2.1】求最长/最大子数组
// 【2.1.1】基础(10)
/*
3.无重复字符的最长子串：给定一个字符串 s ，请你找出其中不含有重复字符的 最长 子串 的长度。

3090.每个字符最多出现两次的最长子字符串：给你一个字符串 s ，
请找出满足每个字符最多出现两次的最长子字符串，并返回该子字符串的 最大 长度。

1493.删掉一个元素以后全为 1 的最长子数组：给你一个二进制数组 nums ，你需要从中删掉一个元素。
请你在删掉元素的结果数组中，返回最长的且只包含 1 的非空子数组的长度。
如果不存在这样的子数组，请返回 0 。

1208.尽可能使字符串相等：给你两个长度相同的字符串，
s 和 t。将 s 中的第 i 个字符变到 t 中的第 i 个字符需要 |s[i] - t[i]| 的开销（开销可能为 0），
也就是两个字符的 ASCII 码值的差的绝对值。
用于变更字符串的最大预算是 maxCost。在转化字符串时，总开销应当小于等于该预算，这也意味着字符串的转化可能是不完全的。
如果你可以将 s 的子字符串转化为它在 t 中对应的子字符串，则返回可以转化的最大长度。
如果 s 中没有子字符串可以转化成 t 中对应的子字符串，则返回 0。

904.水果成篮：你正在探访一家农场，农场从左到右种植了一排果树。
这些树用一个整数数组 fruits 表示，其中 fruits[i] 是第 i 棵树上的水果 种类 。
你想要尽可能多地收集水果。然而，农场的主人设定了一些严格的规矩，你必须按照要求采摘水果：
你只有 两个 篮子，并且每个篮子只能装 单一类型 的水果。每个篮子能够装的水果总量没有限制。
你可以选择任意一棵树开始采摘，你必须从 每棵 树（包括开始采摘的树）上 恰好摘一个水果 。
采摘的水果应当符合篮子中的水果类型。每采摘一次，你将会向右移动到下一棵树，并继续采摘。
一旦你走到某棵树前，但水果不符合篮子的水果类型，那么就必须停止采摘。
给你一个整数数组 fruits ，返回你可以收集的水果的 最大 数目。

1695.删除子数组的最大得分：给你一个正整数数组 nums ，请你从中删除一个含有 若干不同元素 的子数组。
删除子数组的 得分 就是子数组各元素之 和 。返回 只删除一个 子数组可获得的 最大得分 。

2958.最多 K 个重复元素的最长子数组：给你一个整数数组 nums 和一个整数 k 。
一个元素 x 在数组中的 频率 指的是它在数组中的出现次数。
如果一个数组中所有元素的频率都 小于等于 k ，那么我们称这个数组是 好 数组。
请你返回 nums 中 最长好 子数组的长度。

2024.考试的最大困扰度：一位老师正在出一场由 n 道判断题构成的考试，
每道题的答案为 true （用 'T' 表示）或者 false （用 'F' 表示）。
老师想增加学生对自己做出答案的不确定性，方法是 最大化 有 连续相同 结果的题数。
（也就是连续出现 true 或者连续出现 false）。
给你一个字符串 answerKey ，其中 answerKey[i] 是第 i 个问题的正确结果。
除此以外，还给你一个整数 k ，表示你能进行以下操作的最多次数：
每次操作中，将问题的正确答案改为 'T' 或者 'F' （也就是将 answerKey[i] 改为 'T' 或者 'F' ）。
请你返回在不超过 k 次操作的情况下，最大 连续 'T' 或者 'F' 的数目。

1004.最大连续1的个数 III：给定一个二进制数组 nums 和一个整数 k，
假设最多可以翻转 k 个 0 ，则返回执行操作后 数组中连续 1 的最大个数 。

1658.将 x 减到 0 的最小操作数：给你一个整数数组 nums 和一个整数 x 。
每一次操作时，你应当移除数组 nums 最左边或最右边的元素，然后从 x 中减去该元素的值。
请注意，需要 修改 数组以供接下来的操作使用。
如果可以将 x 恰好 减到 0 ，返回 最小操作数 ；否则，返回 -1 。
*/
// ---------------------
// 模板题1
namespace s3
{
	class Solution {
	public:
		int lengthOfLongestSubstring(string s) {
			unordered_map<char, int> map;
			int n = s.size();
			int left = 0;
			int ans = 0;
			for (int right = 0; right < n; ++right) {
				++map[s[right]];// 整体布局和定长滑窗的模板3比较像，for循环内先满足窗口长度，再判断，再修正窗口
				while (map[s[right]] > 1) {// 与定长滑窗的区别在于，left的修改在条件判断中，且不是if，而是while
					--map[s[left]];
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

namespace s3090
{
	class Solution {
	public:
		int maximumLengthSubstring(string s) {
			int n = s.size();
			unordered_map<char, int> map;
			int left = 0, ans = 0;
			for (int right = 0; right < n; ++right) {
				++map[s[right]];
				while (map[s[right]] > 2) {
					--map[s[left]];
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

namespace s1493
{
	class Solution {
	public:
		int longestSubarray(vector<int>& nums) {
			int n = nums.size();
			int left = 0, ans = 0, zeroCount = 0;
			for (int right = 0; right < n; ++right) {
				if (nums[right] == 0) {
					++zeroCount;
				}
				while (zeroCount > 1) {
					if (nums[left] == 0) {
						--zeroCount;
					}
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans - 1;// 陷阱在于返回值需要-1，答案的子数组长度是已经删除掉一个元素之后的
		}
	};
}

namespace s1208
{
	class Solution {
	public:
		int equalSubstring(string s, string t, int maxCost) {
			int n = s.size();
			int sum = 0, left = 0, ans = 0;
			for (int right = 0; right < n; ++right) {
				sum += abs(t[right] - s[right]);
				while (sum > maxCost) {
					sum -= abs(t[left] - s[left]);
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

namespace s904
{
	class Solution {
	public:
		int totalFruit(vector<int>& fruits) {
			int n = fruits.size();
			int left = 0, ans = 0;
			unordered_map<int, int> map;
			for (int right = 0; right < n; ++right) {
				++map[fruits[right]];
				while (map.size() > 2) {
					if (--map[fruits[left]] == 0) {
						map.erase(fruits[left]);
					}
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

namespace s1695
{
	class Solution {
	public:
		int maximumUniqueSubarray(vector<int>& nums) {
			int sum = 0, left = 0, ans = 0;
			int n = nums.size();
			unordered_map<int, int> map;
			for (int right = 0; right < n; ++right) {
				sum += nums[right];
				++map[nums[right]];
				while (map[nums[right]] > 1) {
					--map[nums[left]];
					sum -= nums[left];
					++left;
				}
				ans = max(ans, sum);
			}
			return ans;
		}
	};
}

namespace s2958
{
	class Solution {
	public:
		int maxSubarrayLength(vector<int>& nums, int k) {
			int left = 0, ans = 0;
			int n = nums.size();
			unordered_map<int, int> map;
			for (int right = 0; right < n; ++right) {
				++map[nums[right]];
				while (map[nums[right]] > k) {
					--map[nums[left]];
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

// 窗口变动条件稍微复杂一些些
namespace s2024m1
{	// 最容易想到的方法，T和F各遍历一遍，但是两者其实可以合并进行提速
	class Solution {
	public:
		int maxConsecutiveAnswers(string answerKey, int k) {
			int ansTrue = 0, ansFalse = 0, left = 0;
			int falseCount = 0, trueCount = 0;
			int n = answerKey.size();
			for (int right = 0; right < n; ++right) {
				if (answerKey[right] == 'F') {
					++falseCount;
				}
				while (falseCount > k) {
					if (answerKey[left] == 'F') {
						--falseCount;
					}
					++left;
				}
				ansTrue = max(ansTrue, right - left + 1);
			}
			left = 0;
			for (int right = 0; right < n; ++right) {
				if (answerKey[right] == 'T') {
					++trueCount;
				}
				while (trueCount > k) {
					if (answerKey[left] == 'T') {
						--trueCount;
					}
					++left;
				}
				ansFalse = max(ansFalse, right - left + 1);
			}
			return ansFalse > ansTrue ? ansFalse : ansTrue;
		}
	};
}
namespace s2024o1
{
	class Solution {
	public:
		int maxConsecutiveAnswers(string answerKey, int k) {
			int ans = 0, left = 0, trueCount = 0;
			int n = answerKey.size();
			for (int right = 0; right < n; ++right) {
				if (answerKey[right] == 'T') {
					++trueCount;
				}// T和F的个数“同时”不满足要求时才改动窗口左端点
				while (trueCount > k && (right - left + 1 - trueCount) > k) {
					if (answerKey[left] == 'T') {
						--trueCount;
					}
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}
namespace s2024o2
{	// 用到了s424的思路，更快
	class Solution {
	public:
		int maxConsecutiveAnswers(string answerKey, int k) {
			int maxn = 0, n = answerKey.size();
			int left = 0, right = 0;
			vector<int> cnt(2, 0);
			while (right < n) {
				int i = answerKey[right] == 'T';
				++cnt[i];
				maxn = max(maxn, cnt[i]);
				if (right - left + 1 > maxn + k) {
					--cnt[answerKey[left] == 'T'];
					++left;
				}
				++right;
			}
			return right - left;
		}
	};
}

namespace s1004
{
	class Solution {
	public:
		int longestOnes(vector<int>& nums, int k) {
			int left = 0, zeroCount = 0, ans = 0;
			int n = nums.size();
			for (int right = 0; right < n; ++right) {
				if (nums[right] == 0) {
					++zeroCount;
				}
				while (zeroCount > k) {
					if (nums[left] == 0) {
						--zeroCount;
					}
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

// 正难则反，顺应题目意思直接求解困难，那么可以尝试进行题目的逆向思维与转化
namespace s1658o1
{
	class Solution {
	public:
		int minOperations(vector<int>& nums, int x) {
			int target = accumulate(nums.begin(), nums.end(), 0) - x;
			int n = nums.size();// 将问题转化为中间那段子数组之和满足sum - x的情况下，求最长数组
			// 此时问题变成了最常见、最简单的模板题，最后返回时换为n - ans即可
			if (target < 0) return -1;// 所有元素都不够减
			if (target == 0) return n;// 正好够减
			int left = 0, sum = 0, ans = 0;
			for (int right = 0; right < n; ++right) {
				sum += nums[right];
				while (sum > target) {
					sum -= nums[left];
					++left;
				}
				if (sum == target) {
					ans = max(ans, right - left + 1);
				}
			}
			return ans == 0 ? -1 : n - ans;
		}
	};
}
// ---------------------

// 【2.1.2】进阶，结合排序、分组、逆向思维来提高难度，选做(5)
/*
2730.找到最长的半重复子字符串：给你一个下标从 0 开始的字符串 s ，这个字符串只包含 0 到 9 的数字字符。
如果一个字符串 t 中至多有一对相邻字符是相等的，那么称这个字符串 t 是 半重复的 。
例如，"0010" 、"002020" 、"0123" 、"2002" 和 "54944" 是半重复字符串，而 "00101022" 
（相邻的相同数字对是 00 和 22）和 "1101234883" （相邻的相同数字对是 11 和 88）不是半重复字符串。
请你返回 s 中最长 半重复 子字符串 的长度。

2779.数组的最大美丽值：给你一个下标从 0 开始的整数数组 nums 和一个 非负 整数 k 。
在一步操作中，你可以执行下述指令：
在范围 [0, nums.length - 1] 中选择一个 此前没有选过 的下标 i 。
将 nums[i] 替换为范围 [nums[i] - k, nums[i] + k] 内的任一整数。
数组的 美丽值 定义为数组中由相等元素组成的最长子序列的长度。
对数组 nums 执行上述操作任意次后，返回数组可能取得的 最大 美丽值。
注意：你 只 能对每个下标执行 一次 此操作。
数组的 子序列 定义是：经由原数组删除一些元素（也可能不删除）得到的一个新数组，且在此过程中剩余元素的顺序不发生改变。

1838.最高频元素的频数：元素的 频数 是该元素在一个数组中出现的次数。
给你一个整数数组 nums 和一个整数 k 。在一步操作中，你可以选择 nums 的一个下标，并将该下标对应元素的值增加 1 。
执行最多 k 次操作后，返回数组中最高频元素的 最大可能频数 。

2516.每种字符至少取 K 个：给你一个由字符 'a'、'b'、'c' 组成的字符串 s 和一个非负整数 k 。
每分钟，你可以选择取走 s 最左侧 还是 最右侧 的那个字符。
你必须取走每种字符 至少 k 个，返回需要的 最少 分钟数；如果无法取到，则返回 -1 。

2831.找出最长等值子数组：给你一个下标从 0 开始的整数数组 nums 和一个整数 k 。
如果子数组中所有元素都相等，则认为子数组是一个 等值子数组 。注意，空数组是 等值子数组 。
从 nums 中删除最多 k 个元素后，返回可能的最长等值子数组的长度。
子数组 是数组中一个连续且可能为空的元素序列。
*/
// ---------------------

namespace s2730m1
{	// 
	class Solution {
	public:
		int longestSemiRepetitiveSubstring(string s) {
			int n = s.size();
			if (n == 1) return 1;
			int dupeCount = 0, left = 0, ans = 0;
			for (int right = 1; right < n; ++right) {
				if (s[right] == s[right - 1]) {
					++dupeCount;
				}
				while (dupeCount > 1) {
					if (s[left] == s[left + 1]) {
						--dupeCount;
					}
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

// 排序 + 问题转化
namespace s2779m1
{
	class Solution {
	public:
		int maximumBeauty(vector<int>& nums, int k) {
			int n = nums.size();
			sort(nums.begin(), nums.end());// 排序后就可以用滑动窗口进行求解
			int ans = 0, left = 0;
			for (int right = 0; right < n; ++right) {
				while (nums[right] - nums[left] > 2 * k) {
					// 窗口左端点的最大数值范围要大于窗口右端点的最小数值范围
					// nums[left] + k >= nums[right] - k
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

// 排序
namespace s1838m1
{
	class Solution {
	public:
		int maxFrequency(vector<int>& nums, int k) {
			sort(nums.begin(), nums.end());
			int n = nums.size();
			int left = 0, ans = 0;
			long long sum = 0;
			for (int right = 0; right < n; ++right) {
				sum += nums[right];
				while ((long long)nums[right] * (right - left + 1) > sum + k) {
					sum -= nums[left];
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

// s2024 + s1658的结合，窗口判断条件复杂化 + 正难则反问题转化
// 进一步总结：从数组/字符串左右两头取元素的，都可以考虑逆向思维
namespace s2516m1
{
	class Solution {
	public:
		int takeCharacters(string s, int k) {
			vector<int> record(3, 0);
			for (char c : s) {
				++record[c - 'a'];
			}
			if (record[0] < k || record[1] < k || record[2] < k) {
				return -1;
			}
			vector<int> count(3, 0);
			int midStrMaxLen = 0, left = 0;
			int n = s.size();
			for (int right = 0; right < n; ++right) {
				++count[s[right] - 'a'];
				while (count[0] > record[0] - k ||
					count[1] > record[1] - k || count[2] > record[2] - k) {
				// while (count[s[right] - 'a'] > record[s[right] - 'a'] - k) 这样写更好
				// 也可以将count和record两个数组用一个数组表示，for循环内对record进行--，进一步简化代码
					--count[s[left] - 'a'];
					++left;
				}
				midStrMaxLen = max(midStrMaxLen, right - left + 1);
			}
			return n - midStrMaxLen;
		}
	};
}
namespace s2516o1
{
	class Solution {
	public:
		int takeCharacters(string s, int k) {
			vector<int> cnt(3, 0);
			for (char c : s) {
				++cnt[c - 'a'];
			}
			if (cnt[0] < k || cnt[1] < k || cnt[2] < k) {
				return -1;
			}
			int mx = 0, left = 0;
			int n = s.size();
			for (int right = 0; right < n; ++right) {
				--cnt[s[right] - 'a'];
				while (cnt[s[right] - 'a'] < k) {
					++cnt[s[left] - 'a'];
					++left;
				}
				mx = max(mx, right - left + 1);
			}
			return n - mx;
		}
	};
}

// o1为分组 + 滑动窗口，比较难的组合，多熟悉吧...
// o2为s424的派生题，更简单，直接套模板
namespace s2831o1
{
	class Solution {
	public:
		int longestEqualSubarray(vector<int>& nums, int k) {
			int n = nums.size();
			vector<vector<int>> posList(n + 1);// 因为vector下标从0开始，取n不能覆盖所有元素值，所以要n + 1
			int ans = 0;
			for (int i = 0; i < n; ++i) {
				posList[nums[i]].push_back(i);
			}
			// 把所有相同的元素进行分组，在各自的下标上进行滑窗
			for (auto& pos : posList) {
				int left = 0;
				int m = pos.size();
				for (int right = 0; right < m; ++right) {
					while (pos[right] - pos[left] - (right - left) > k) {
						++left;
					}
					ans = max(ans, right - left + 1);
					// 注意，题干要求的删除之后的最长子数组长度，所以不是用pos[right] - pos[left] + 1
				}
			}
			return ans;
		}
	};
}
namespace s2831o2
{
	class Solution {
	public:
		int longestEqualSubarray(vector<int>& nums, int k) {
			int left = 0, right = 0;
			int maxn = 0, n = nums.size();
			unordered_map<int, int> cnt;
			while (right < n) {
				++cnt[nums[right]];
				maxn = max(maxn, cnt[nums[right]]);
				if (right - left + 1 > maxn + k) {
					--cnt[nums[left]];
					++left;
				}
				++right;
			}
			return maxn;
		}
	};
}
// ---------------------

// 【2.2】求最短/最小(6)
/*
209.长度最小的子数组：给定一个含有 n 个正整数的数组和一个正整数 target 。
找出该数组中满足其总和大于等于 target 的长度最小的 子数组 [numsl, numsl+1, ..., numsr-1, numsr] ，并返回其长度。
如果不存在符合条件的子数组，返回 0 。

2904.最短且字典序最小的美丽子字符串：给你一个二进制字符串 s 和一个正整数 k 。
如果 s 的某个子字符串中 1 的个数恰好等于 k ，则称这个子字符串是一个 美丽子字符串 。
令 len 等于 最短 美丽子字符串的长度。
返回长度等于 len 且字典序 最小 的美丽子字符串。如果 s 中不含美丽子字符串，则返回一个 空 字符串。
对于相同长度的两个字符串 a 和 b ，如果在 a 和 b 出现不同的第一个位置上，
a 中该位置上的字符严格大于 b 中的对应字符，则认为字符串 a 字典序 大于 字符串 b 。
例如，"abcd" 的字典序大于 "abcc" ，因为两个字符串出现不同的第一个位置对应第四个字符，而 d 大于 c 。

1234.替换子串得到平衡字符串：有一个只含有 'Q', 'W', 'E', 'R' 四种字符，且长度为 n 的字符串。
假如在该字符串中，这四个字符都恰好出现 n/4 次，那么它就是一个「平衡字符串」。
给你一个这样的字符串 s，请通过「替换一个子串」的方式，使原字符串 s 变成一个「平衡字符串」。
你可以用和「待替换子串」长度相同的 任何 其他字符串来完成替换。
请返回待替换子串的最小可能长度。
如果原字符串自身就是一个平衡字符串，则返回 0。

2875.无限数组的最短子数组：给你一个下标从 0 开始的数组 nums 和一个整数 target 。
下标从 0 开始的数组 infinite_nums 是通过无限地将 nums 的元素追加到自己之后生成的。
请你从 infinite_nums 中找出满足 元素和 等于 target 的 最短 子数组，并返回该子数组的长度。
如果不存在满足条件的子数组，返回 -1 。

76.最小覆盖子串：给你一个字符串 s 、一个字符串 t 。返回 s 中涵盖 t 所有字符的最小子串。
如果 s 中不存在涵盖 t 所有字符的子串，则返回空字符串 "" 。
注意：对于 t 中重复字符，我们寻找的子字符串中该字符数量必须不少于 t 中该字符数量。
如果 s 中存在这样的子串，我们保证它是唯一的答案。s和t只包含大小写英文字母。

632.最小区间：你有 k 个 非递减排列 的整数列表。找到一个 最小 区间，
使得 k 个列表中的每个列表至少有一个数包含在其中。
我们定义如果 b-a < d-c 或者在 b-a == d-c 时 a < c，则区间 [a,b] 比 [c,d] 小。
*/
// ---------------------
// 模板题2，和求最大/最长比较类似，个人比较喜欢写法o2
namespace s209o1
{// 写法o1：窗口变化的判断条件更进一步，更新答案时也多了个条件判断
	class Solution {
	public:
		int minSubArrayLen(int target, vector<int>& nums) {
			int n = nums.size();
			int left = 0, sum = 0, ans = n + 1;

			for (int right = 0; right < n; ++right) {
				sum += nums[right];
				while (sum - nums[left] >= target) {// 不像求最大/最长中的sum >= target，这里要进一步判断减之后的
					sum -= nums[left];
					++left;
				}
				if (sum >= target) {// 因为是要求最短，更新答案时要注意下判断条件
					ans = min(ans, right - left + 1);
				}
			}
			return ans == n + 1 ? 0 : ans;
		}
	};
}
namespace s209o2
{// 也可以将答案更新放到窗口变化的while循环内，与最长/最大类型的模式进一步差异化，代码更简洁
	class Solution {
	public:
		int minSubArrayLen(int target, vector<int>& nums) {
			int n = nums.size();
			int left = 0, sum = 0, ans = n + 1;
			for (int right = 0; right < n; ++right) {
				sum += nums[right];
				while (sum >= target) {
					ans = min(ans, right - left + 1);
					sum -= nums[left];
					++left;
				}
			}
			return ans == n + 1 ? 0 : ans;
		}
	};
}

// 实时更新返回字符串
namespace s2904m1
{	// 用strList存储字符串，最后一起比较，但这样容易出错
	class Solution {
	public:
		string shortestBeautifulSubstring(string s, int k) {
			int n = s.size();
			int cnt = 0, left = 0, len = n + 1;
			vector<string> strList;
			for (int right = 0; right < n; ++right) {
				if (s[right] == '1') {
					++cnt;
				}
				while (cnt == k) {
					int curLen = right - left + 1;
					if (curLen < len) {
						len = curLen;
						strList.clear();// 必须加上这一行，因为字典序比较有陷阱，0101比101小，所以最后答案会是0101，而不是101
					}
					if (curLen == len) {
						string sub = s.substr(left, len);
						strList.push_back(sub);
					}
					if (s[left] == '1') {
						--cnt;
					}
					++left;
				}
			}
			if (strList.size() == 0) {
				return "";
			}
			string minStr = strList[0];
			for (string& s : strList) {
				minStr = min(minStr, s);
			}
			return minStr;
		}
	};
}
namespace s2904o1
{	// 实时更新最小字符串
	class Solution {
	public:
		string shortestBeautifulSubstring(string s, int k) {
			int n = s.size();
			int cnt = 0, left = 0, len = n + 1;
			string ans = "";
			for (int right = 0; right < n; ++right) {
				if (s[right] == '1') {
					++cnt;
				}
				while (cnt == k) {
					int curLen = right - left + 1;
					if (curLen < len) {
						len = curLen;
						ans = s.substr(left, len);
					}
					if (curLen == len) {
						string sub = s.substr(left, len);
						ans = min(ans, sub);
					}
					if (s[left] == '1') {
						--cnt;
					}
					++left;
				}
			}
			return ans;
		}
	};
}

// 问题转化 + 窗口判断条件复杂化
namespace s1234m1
{	// 可以用数组替代哈希表来提速（空间换时间），另外我的写法比较笨，直接按照题意写
	// 具体看o1解法，其实不需要p.second -= target这种步骤，将窗口判断条件进行简化
	class Solution {
	public:
		int balancedString(string s) {
			unordered_map<char, int> map;
			for (char c : s) {
				++map[c];
			}
			int n = s.size();
			int target = n / 4;
			if (map['Q'] == target && map['E'] == target &&
				map['W'] == target && map['R'] == target) {
				return 0;
			}
			for (auto& p : map) {
				p.second -= target;
				if (p.second < 0) {
					p.second = 0;
				}// 此时map内存放的是多余字母的多余个数
				// 不多余字母就默认设置其为0
			}
			int left = 0, ans = n + 1;
			for (int right = 0; right < n; ++right) {
				--map[s[right]];
				while (map['Q'] <= 0 && map['E'] <= 0 &&
					map['W'] <= 0 && map['R'] <= 0) {
					ans = min(ans, right - left + 1);
					++map[s[left]];
					++left;
				}
			}
			return ans;
		}
	};
}
namespace s1234o1
{	
	/*
	根据题意，如果在待替换子串之外的任意字符的出现次数超过 m = n / 4,
	那么无论怎么替换，都无法使这个字符在整个字符串中的出现次数为 m。
	反过来说，如果在待替换子串之外的任意字符的出现次数都不超过 m，
	那么可以通过替换，使 s 为平衡字符串，即每个字符的出现次数均为 m。
	*/
	class Solution {
	public:
		int balancedString(string s) {
			int map['X']{};// 需要比'W'大一位
			for (char c : s) {
				++map[c];
			}
			int n = s.size(), m = n / 4;
			if (map['Q'] == m && map['E'] == m &&
				map['W'] == m && map['R'] == m) {
				return 0;
			}
			int left = 0, ans = n + 1;
			for (int right = 0; right < n; ++right) {
				--map[s[right]];// 所有在窗口内的字符都通过--反应到了下面的窗口判断条件中
				while (map['Q'] <= m && map['E'] <= m &&
					map['W'] <= m && map['R'] <= m) {// 这四个map判断的是窗口外的字符
					ans = min(ans, right - left + 1);
					++map[s[left]];
					++left;
				}
			}
			return ans;
		}
	};
}

// 无限数组 + 问题转化，不关注无限的部分，而是关注剩下的部分，这道题可以多做做，以后有类似的都可以照着写
namespace s2875
{
	class Solution {
	public:
		int minSizeSubarray(vector<int>& nums, int target) {
			long long total = accumulate(nums.begin(), nums.end(), 0LL);
			int n = nums.size();
			int mults = target / total;
			int mods = target % total;
			int N = 2 * n;
			int ans = N + 1, left = 0;
			long long sum = 0;
			for (int right = 0; right < N; ++right) {
				sum += nums[right % n];
				while (sum >= mods) {
					if (sum == mods) {
						ans = min(ans, right - left + 1);
					}
					sum -= nums[left % n];
					++left;
				}
			}
			return ans == N + 1 ? -1 : ans + mults * n;
		}
	};
}

// ！！！不要在循环内部反复创建新的子串（会增加大量内存消耗），可以记录子串的起始位置下标及长度
namespace s76m1
{	// 我的方法用的是哈希表，内存占用和速度都慢很多
	class Solution {
	public:
		string minWindow(string s, string t) {
			int n = s.size(), m = t.size();
			string ans = "";
			if (m > n) return ans;
			unordered_map<char, int> map;
			// t遍历一次生成频次哈希表
			for (char c : t) {
				++map[c];
			}
			// 滑动窗口
			int left = 0, cnt = 0, len = n + 1, minStart = 0;
			unordered_map<char, int> ref;
			int letterCnt = map.size();
			// 每次移动右端点，对ref[s[right]]进行递增，若和map中对应元素频次一致则增加cnt，
			// 当cnt == map.size()时视为找到了一个符合要求的字符串，并更新答案与滑动窗口
			for (int right = 0; right < n; ++right) {
				char rc = s[right];
				++ref[rc];
				if (map.count(rc) && map[rc] == ref[rc]) {
					++cnt;
				}
				while (cnt == letterCnt) {
					if (right - left + 1 < len) {
						len = right - left + 1;
						minStart = left;
					}
					char lc = s[left];
					--ref[lc];
					if (map.count(lc) && map[lc] > ref[lc]) {
						--cnt;
					}
					++left;
				}
			}
			return len == n + 1 ? ans : s.substr(minStart, len);
		}
	};
}
namespace s76m2
{	// 用数组提速，但是判定的matches过程很繁琐，每次都要遍历128次，其实在移动右端点时就可以顺便进行判断（见O1做法）
	class Solution {
	public:
		string minWindow(string s, string t) {
			int n = s.size(), m = t.size();
			if (n < m) return "";
			int cnt[128]{};
			for (char c : t) {
				++cnt[c];
			}
			int left = 0, len = INT_MAX, start = 0;
			for (int right = 0; right < n; ++right) {
				--cnt[s[right]];
				int matches = 0;
				for (int i = 0; i < 128; ++i) {
					if (cnt[i] <= 0) {
						++matches;
					}
				}
				while (matches == 128) {
					if (right - left + 1 < len) {
						start = left;
						len = right - left + 1;
					}
					++cnt[s[left]];
					if (cnt[s[left]] > 0) {
						--matches;
					}
					++left;
				}
			}
			return len == INT_MAX ? "" : s.substr(start, len);
		}
	};
}
namespace s76o1
{
	class Solution {
	public:
		string minWindow(string s, string t) {
			int n = s.size(), m = t.size();
			if (m > n) return "";

			int cnt[128]{};// ascii表大小为128
			for (char c : t) {
				++cnt[c];
			}

			int left = 0, matches = m;
			int len = INT_MAX, minStart = 0;
			for (int right = 0; right < n; ++right) {
				if (--cnt[s[right]] >= 0) {// --map后如果>=0，说明窗口内部增加了一个“有效字符”
					--matches;// count代表t中还未被窗口包括的字符数量
				}
				while (matches == 0) {
					if (right - left + 1 < len) {
						len = right - left + 1;
						minStart = left;// 只记录子串的起点位置
					}
					if (++cnt[s[left++]] > 0) {
						++matches;// left的改变与right相对应
					}
				}
			}
			return len == INT_MAX ? "" : s.substr(minStart, len);
		}
	};
}
namespace s76o2
{	// 很早以前写的旧方法，看看得了，没o1好，怀旧怀旧
	class Solution {
	public:
		string minWindow(string s, string t) {
			int n = s.size(), m = t.size(), minStart = 0;
			if (m > n) return "";
			int left = 0, valid = 0, len = INT_MAX;
			vector<int> sChar(128);
			vector<int> tChar(128);
			for (char c : t) {
				++tChar[c];
			}
			for (int right = 0; right < n; ++right) {
				++sChar[s[right]];
				if (sChar[s[right]] <= tChar[s[right]]) {
					++valid;
				}
				while (left < right && sChar[s[left]] > tChar[s[left]]) {
					--sChar[s[left]];
					++left;
				}
				if (valid == m) {
					if (right - left + 1 < len) {
						len = right - left + 1;
						minStart = left;
					}
				}
			}
			return len == INT_MAX ? "" : s.substr(minStart, len);
		}
	};
}

// 合并后排序 + 类似s76的思路
namespace s632o1
{
	class Solution {
	public:
		vector<int> smallestRange(vector<vector<int>>& nums) {
			vector<pair<int, int>> pairs;
			int k = nums.size();
			for (int i = 0; i < k; ++i) {
				for (int x : nums[i]) {
					pairs.emplace_back(x, i);
				}
			}
			sort(pairs.begin(), pairs.end());

			int start = pairs[0].first;
			int end = pairs.back().first;
			int minRange = end - start;
			int& counter = k;
			int n = pairs.size();
			vector<int> map(k, 1);// 每个列表至少要出现一次
			int left = 0;
			for (int right = 0; right < n; ++right) {
				if (--map[pairs[right].second] == 0) {
					--counter;
				}
				while (counter == 0) {
					int curRange = pairs[right].first - pairs[left].first;
					if (curRange < minRange) {
						minRange = curRange;
						start = pairs[left].first;
						end = pairs[right].first;
					}
					if (++map[pairs[left].second] > 0) {
						++counter;
					}
					++left;
				}
			}
			return { start, end };
		}
	};
}
namespace s632o2
{	// 灵神的滑动窗口写法，和m1写法差不多，可以对照参考
	class Solution {
	public:
		vector<int> smallestRange(vector<vector<int>>& nums) {
			vector<pair<int, int>> pairs;
			for (int i = 0; i < nums.size(); i++) {
				for (int x : nums[i]) {
					pairs.emplace_back(x, i);
				}
			}
			// 看上去 std::sort 比 ranges::sort 更快
			sort(pairs.begin(), pairs.end());

			int ans_l = pairs[0].first;
			int ans_r = pairs.back().first;
			int empty = nums.size();
			vector<int> cnt(empty);
			int left = 0;
			for (auto [r, i] : pairs) {
				if (cnt[i] == 0) { // 包含 nums[i] 的数字
					empty--;
				}
				cnt[i]++;
				while (empty == 0) { // 每个列表都至少包含一个数
					auto [l, i] = pairs[left];
					if (r - l < ans_r - ans_l) {
						ans_l = l;
						ans_r = r;
					}
					cnt[i]--;
					if (cnt[i] == 0) { // 不包含 nums[i] 的数字
						empty++;
					}
					left++;
				}
			}
			return { ans_l, ans_r };
		}
	};
}
namespace s632o3
{	// 灵神的堆做法，现在暂时还看不懂，比滑动窗口做法要快一些
	/*
	class Solution {
	public:
		vector<int> smallestRange(vector<vector<int>>& nums) {
			priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
			int r = INT_MIN;
			for (int i = 0; i < nums.size(); i++) {
				pq.emplace(nums[i][0], i, 0); // 把每个列表的第一个元素入堆
				r = max(r, nums[i][0]);
			}

			int ans_l = get<0>(pq.top()); // 第一个合法区间的左端点
			int ans_r = r; // 第一个合法区间的右端点
			while (true) {
				auto [_, i, j] = pq.top();// 结构化绑定？
				if (j + 1 == nums[i].size()) { // 堆顶列表没有下一个元素
					break;
				}
				pq.pop();
				int x = nums[i][j + 1]; // 堆顶列表的下一个元素
				pq.emplace(x, i, j + 1); // 入堆
				r = max(r, x); // 更新合法区间的右端点
				int l = get<0>(pq.top()); // 当前合法区间的左端点
				if (r - l < ans_r - ans_l) {
					ans_l = l;
					ans_r = r;
				}
			}
			return { ans_l, ans_r };
		}
	};
	*/
}
// ---------------------

// 【2.3】求子数组个数
// 【2.3.1】越长越合法(6)，一般写ans += left
/*
1358.包含所有三种字符的子字符串数目：给你一个字符串 s ，它只包含三种字符 a, b 和 c 。
请你返回 a，b 和 c 都 至少 出现过一次的子字符串数目。

2962.统计最大元素出现至少 K 次的子数组：给你一个整数数组 nums 和一个 正整数 k 。
请你统计有多少满足 「 nums 中的 最大 元素」至少出现 k 次的子数组，并返回满足这一条件的子数组的数目。
子数组是数组中的一个连续元素序列。

3325.字符至少出现 K 次的子字符串 I：给你一个字符串 s 和一个整数 k，在 s 的所有子字符串中，
请你统计并返回 至少有一个 字符 至少出现 k 次的子字符串总数。

2799.统计完全子数组的数目：给你一个由 正 整数组成的数组 nums 。
如果数组中的某个子数组满足下述条件，则称之为 完全子数组 ：
子数组中 不同 元素的数目等于整个数组不同元素的数目。返回数组中 完全子数组 的数目。

2537.统计好子数组的数目：给你一个整数数组 nums 和一个整数 k ，请你返回 nums 中 好 子数组的数目。
一个子数组 arr 如果有 至少 k 对下标 (i, j) 满足 i < j 且 arr[i] == arr[j] ，那么称它是一个 好 子数组。

3298.统计重新排列后包含另一个字符串的子字符串数目 II：给你两个字符串 word1 和 word2 。
如果一个字符串 x 重新排列后，word2 是重排字符串的 前缀 ，那么我们称字符串 x 是 合法的 。
请你返回 word1 中 合法 子字符串 的数目。注意 ，
这个问题中的内存限制比其他题目要 小 ，所以你 必须 实现一个线性复杂度的解法。
*/
// ---------------------
// 模板题3（重要），越长越合法类型题和之前的不一样，模板好好消化
namespace s1358o1
{	// 计算左边的答案，比计算右边难懂一点，但是这题可以省下一个变量right
	class Solution {
	public:
		int numberOfSubstrings(string s) {
			int ans = 0, left = 0;
			int cnt[3]{};
			for (char c : s) {
				cnt[c - 'a']++;// 先固定右端点，每次都是找第一个"恰好"不满足要求的[left, right]区间
				// 随着右端点的右移，最后一个不满足要求的左端点也必定是右移的（right右移，窗口内元素更多，更容易满足要求），
				// 所以left单调递增，只用遍历一次
				while (cnt[0] > 0 && cnt[1] > 0 && cnt[2] > 0) {// 循环内部移动左端点
					// ans += s.size() - right;//在while循环中加右边的也可以当作模板
					--cnt[s[left] - 'a'];
					left++;
				}
				ans += left;// [left - 1, right]是恰好满足要求的区间，同时left - 1的更左边所有点也都满足要求
				// 那么对此时的right，有left - 1 - 0 + 1 = left个子数组满足要求
				// 最前面几次迭代都是 + 0
			}
			return ans;
		}
	};
}
namespace s1358o2
{	// 更新答案时计算右边的，满足条件后，右端点在[right, n - 1]范围内的数组都是可以计入答案的，也即n - 1 - right + 1
	class Solution {
	public:
		int numberOfSubstrings(string s) {
			int cnt[3]{};
			int n = s.size();
			int left = 0, ans = 0;
			for (int right = 0; right < n; ++right) {
				++cnt[s[right] - 'a'];
				while (cnt[0] > 0 && cnt[1] > 0 && cnt[2] > 0) {
					ans += n - right;
					--cnt[s[left++] - 'a'];
				}
			}
			return ans;
		}
	};
}
namespace s1358o3
{	// AI的答案，同样是固定右端点，更简洁，但是貌似不能作为模板来使用
	int numberOfSubstrings(string s) {
		int n = s.size();
		int res = 0;
		// 记录每个字符最后出现的位置
		vector<int> last_pos(3, -1);

		for (int i = 0; i < n; ++i) {
			// 更新当前字符的位置
			last_pos[s[i] - 'a'] = i;

			// 计算最小的位置作为窗口左边界
			int min_pos = min({ last_pos[0], last_pos[1], last_pos[2] });
			// 每次以满足条件的位置作为右端点更新答案
			// 只有当三个字符都至少出现一次时才计算
			if (min_pos != -1) {
				res += min_pos + 1;
			}
		}
		return res;
	}
}

// 套模板
namespace s3298m1 {
	class Solution {
	public:
		long long countSubarrays(vector<int>& nums, int k) {
			int maxNum = *max_element(nums.begin(), nums.end());
			int n = nums.size();
			int left = 0, count = 0;
			long long ans = 0;
			for (int i : nums) {
				if (i == maxNum) {
					++count;
				}
				while (count == k) {
					if (nums[left] == maxNum) {
						--count;
					}
					++left;
				}
				ans += left;
			}
			return ans;
		}
	};
}

// 套模板，但是我写的解法比较冗余
namespace s3325m1
{	// 其实加flag的做法是没必要的，具体看o1
	class Solution {
	public:
		int numberOfSubstrings(string s, int k) {
			vector<int> map(26, 0);
			int left = 0, ans = 0;
			bool flag = false;
			for (char c : s) {
				++map[c - 'a'];
				while (map[c - 'a'] >= k || flag) {// 外层循环首次时map[c - 'a'] >= k足矣，
					// 因为前一次收缩必定已经把所有可能满足条件的情况排除了
					flag = false;
					--map[s[left] - 'a'];
					for (int i : map) {// 收缩窗口时要收到窗口不满足条件为止，即没有一个超过k次的子串
						if (i >= k) {
							flag = true;
						}
					}
					++left;
				}
				ans += left;
			}
			return ans;
		}
	};
}
namespace s3325o1
{
	class Solution {
	public:
		int numberOfSubstrings(string s, int k) {
			vector<int> map(26, 0);
			int left = 0, ans = 0;
			for (char c : s) {
				++map[c - 'a'];
				while (map[c - 'a'] >= k) {
					// 因为只有 ++cnt[c - 'a'] 导致了 cnt[c - 'a'] 达到 k，
					// 所以其余字母的出现次数必然小于 k，无需判断
					--map[s[left] - 'a'];
					++left;
				}
				ans += left;
			}
			return ans;
		}
	};
}

namespace s2799m1
{
	class Solution {
	public:
		int countCompleteSubarrays(vector<int>& nums) {
			int n = nums.size();
			unordered_set<int> set;
			for (int i : nums) {
				set.insert(i);
			}
			// 初始化set的过程可以简写为：unordered_set<int> set(nums.begin(), nums.end());
			int totalNum = set.size();
			int left = 0, ans = 0;
			unordered_map<int, int> map;
			for (int i : nums) {
				++map[i];
				while (map.size() == totalNum) {
					if (--map[nums[left]] == 0) {
						map.erase(nums[left]);
					}
					++left;
				}
				ans += left;
			}
			return ans;
		}
	};
}

// 稍微复杂一点，搞清楚窗口进出对判定条件的影响，能简化解题过程
namespace s2537m1
{	// 不断加减组合数的做法，比较笨，时间耗时长
	class Solution {
	public:
		long long countGood(vector<int>& nums, int k) {
			unordered_map<int, int> map;
			int left = 0, counter = 0;
			long long ans = 0;
			for (int i : nums) {
				counter -= map[i] * (map[i] - 1) / 2;
				++map[i];
				counter += map[i] * (map[i] - 1) / 2;
				while (counter >= k) {
					int l = map[nums[left]];
					counter -= l * (l - 1) / 2;
					--map[nums[left]];
					counter += (l - 1) * (l - 2) / 2;
					++left;
				}
				ans += left;
			}
			return ans;
		}
	};
}
namespace s2537o1
{
	/*
	如果窗口中有 c 个元素 x，再进来一个 x，会新增 c 个相等数对。
	如果窗口中有 c 个元素 x，再去掉一个 x，会减少 c - 1 个相等数对。
	*/
	class Solution {
	public:
		long long countGood(vector<int>& nums, int k) {
			long long ans = 0;
			int left = 0, pairs = 0;// pairs的上限为2 x 10^9，用int足够
			unordered_map<int, int> cnt;
			for (int i : nums) {
				pairs += cnt[i];
				++cnt[i];
				while (pairs >= k) {
					pairs -= cnt[nums[left]] - 1;
					--cnt[nums[left]];
					++left;
				}
				ans += left;
			}
			return ans;
		}
	};
}

// 和s76最小覆盖子串非常相似
namespace s3298m1
{
	class Solution {
	public:
		long long validSubstringCount(string word1, string word2) {
			int n = word1.size(), m = word2.size();
			if (n < m) return 0LL;
			vector<int> cnt(26, 0);
			for (char c : word2) {
				++cnt[c - 'a'];
			}
			int left = 0, counter = m;
			long long ans = 0;
			for (char c : word1) {
				if (--cnt[c - 'a'] >= 0) {
					--counter;
				}
				while (counter == 0) {
					if (++cnt[word1[left] - 'a'] > 0) {
						++counter;
					}
					++left;
				}
				ans += left;
			}
			return ans;
		}
	};
}
// ---------------------
// 【2.3.2】越短越合法(5)，一般写ans = right - left + 1
/*
713.乘积小于 K 的子数组：给你一个整数数组 nums 和一个整数 k ，
请你返回子数组内所有元素的乘积严格小于 k 的连续子数组的数目。

3258.统计满足 K 约束的子字符串数量 I：给你一个 二进制 字符串 s 和一个整数 k。
如果一个 二进制字符串 满足以下任一条件，则认为该字符串满足 k 约束：
字符串中 0 的数量最多为 k。
字符串中 1 的数量最多为 k。
返回一个整数，表示 s 的所有满足 k 约束 的子字符串的数量。

2302.统计得分小于 K 的子数组数目：一个数组的 分数 定义为数组之和 乘以 数组的长度。
比方说，[1, 2, 3, 4, 5] 的分数为 (1 + 2 + 3 + 4 + 5) * 5 = 75 。
给你一个正整数数组 nums 和一个整数 k ，请你返回 nums 中分数 严格小于 k 的 非空整数子数组数目。

2762.不间断子数组：给你一个下标从 0 开始的整数数组 nums 。nums 的一个子数组如果满足以下条件，那么它是 不间断 的：
i，i + 1 ，...，j  表示子数组中的下标。对于所有满足 i <= i1, i2 <= j 的下标对，都有 0 <= |nums[i1] - nums[i2]| <= 2 。
请你返回 不间断 子数组的总数目。

LCP 68.美观的花束：力扣嘉年华的花店中从左至右摆放了一排鲜花，
记录于整型一维矩阵 flowers 中每个数字表示该位置所种鲜花的品种编号。
你可以选择一段区间的鲜花做成插花，且不能丢弃。 
在你选择的插花中，如果每一品种的鲜花数量都不超过 cnt 朵，那么我们认为这束插花是 「美观的」。
例如：[5,5,5,6,6] 中品种为 5 的花有 3 朵， 品种为 6 的花有 2 朵，每一品种 的数量均不超过 3
请返回在这一排鲜花中，共有多少种可选择的区间，使得插花是「美观的」。
注意：答案需要以 1e9 + 7 (1000000007) 为底取模，如：计算初始结果为：1000000008，请返回 1
*/
// ---------------------
// 模板题4（重要）
namespace s713o1 {
	// [left,right] 这个子数组是满足题目要求的。由于子数组越短，越能满足题目要求，
	// 所以除了 [left,right]，还有 [left+1,right],[left+2,right],...,[right,right] 都是满足要求的。
	// 也就是说，当右端点固定在 right 时，左端点在 left,left+1,left+2,...,right 的所有子数组都是满足要求的，
	// 这一共有 right-left+1 个，加到答案中。
	// right增大的过程就是固定右端点逐个枚举的过程，不会漏掉答案
	class Solution {
	public:
		int numSubarrayProductLessThanK(vector<int>& nums, int k) {
			// 也可以把这个删掉，在while里面加left <= right的判断，但没有预先判断k <= 1快
			// 根据题干nums[i]最小值为1，所以累计乘积一定>= 1，如果不提前排除特例且不额外增加代码，下面缩小窗口的过程会使下标越界
			if (k <= 1) {
				return 0;
			}

			int n = nums.size();
			int product = 1, left = 0, ans = 0;
			for (int right = 0; right < n; ++right) {
				product *= nums[right];
				while (product >= k) {
					product /= nums[left];
					++left;
				}
				ans += right - left + 1;
			}
			return ans;
		}
	};
}

// 套模板 位运算 c mod 2 等价于 c & 1
namespace s3258m1
{
	class Solution {
	public:
		int countKConstraintSubstrings(string s, int k) {
			int ans = 0, left = 0, n = s.size();
			vector<int> cnt(2, 0);
			for (int right = 0; right < n; ++right) {
				++cnt[s[right] - '0'];
				// 这里可以改成++cnt[s[right] & 1]，因为'0'（对应48）和'1'（对应49）是连在一起的，
				// 一定是一个奇一个偶，与...001进行与运算后，只保留最后一位，c & 1等价于c mod 2
				while (cnt[0] > k && cnt[1] > k) {
					--cnt[s[left] - '0'];
					++left;
				}
				ans += right - left + 1;
			}
			return ans;
		}
	};
}

namespace s2302m1
{
	class Solution {
	public:
		long long countSubarrays(vector<int>& nums, long long k) {
			long long sum = 0, pnt = 0, ans = 0;
			int left = 0, n = nums.size();
			for (int right = 0; right < n; ++right) {
				sum += nums[right];
				pnt = sum * (right - left + 1);
				while (pnt >= k) {
					sum -= nums[left];
					++left;
					pnt = sum * (right - left + 1);
				}
				ans += right - left + 1;
			}
			return ans;
		}
	};
}
namespace s2302o1
{	// 代码更简洁一些的版本
	class Solution {
	public:
		long long countSubarrays(vector<int>& nums, long long k) {
			long long sum = 0, ans = 0;
			int left = 0, n = nums.size();
			for (int right = 0; right < n; ++right) {
				sum += nums[right];
				while (sum * (right - left + 1) >= k) {
					sum -= nums[left];
					++left;
				}
				ans += right - left + 1;
			}
			return ans;
		}
	};
}

// 哈希表中，unordered_map和map的区别（map.rbegin()代表最后一个元素）
namespace s2762m1
{	// 用unordered_map需要额外的check函数进行排序检查，可以像o1一样用set，代码更简洁
	// unordered_map的哈希操作虽然理论上是O(1)，但因为需要处理冲突、计算哈希值等，常数因子可能较大
	// map的红黑树实现虽然理论上是O(log k)，但对于k≤3的情况，实际上几乎可以认为是常数时间
	// check函数虽然短小，但在频繁调用时会产生一定开销
	class Solution {
	private:// check函数检查当前窗口的最大最小值的差值
		bool check(unordered_map<int, int>& cnt) {
			int mx = 0;
			int mn = INT_MAX;
			for (auto& p : cnt) {
				mx = max(mx, p.first);
				mn = min(mn, p.first);
			}
			return mx - mn > 2;
		}
	public:
		long long continuousSubarrays(vector<int>& nums) {
			unordered_map<int, int> cnt;
			long long ans = 0;
			int n = nums.size(), left = 0;
			for (int right = 0; right < n; ++right) {
				++cnt[nums[right]];
				while (cnt.size() > 3 || check(cnt)) {// 短路判断，当cnt.size() > 3时，必然不满足窗口条件
					if (--cnt[nums[left]] == 0) {
						cnt.erase(nums[left]);
					}
					++left;
				}
				ans += right - left + 1;
			}
			return ans;
		}
	};
}
namespace s2762o1
{	// 换成了set，思路一致
	class Solution {
	public:
		long long continuousSubarrays(vector<int>& nums) {
			long long ans = 0;
			map<int, int> cnt;
			int left = 0, n = nums.size();
			for (int right = 0; right < n; right++) {
				cnt[nums[right]]++;
				while (cnt.rbegin()->first - cnt.begin()->first > 2) {
					if (--cnt[nums[left]] == 0) {
						cnt.erase(nums[left]);
					}
					left++;
				}
				ans += right - left + 1;
			}
			return ans;
		}
	};
}

// 1e9默认为double类型
namespace ls68
{
	class Solution {
	public:
		int beautifulBouquet(vector<int>& flowers, int cnt) {
			unordered_map<int, int> map;
			int n = flowers.size(), left = 0;
			long long ans = 0;
			for (int right = 0; right < n; ++right) {
				++map[flowers[right]];
				while (map[flowers[right]] > cnt) {
					--map[flowers[left]];
					++left;
				}
				ans += right - left + 1;
			}
			return ans % ((int)1e9 + 7);
		}
	};
}
// ---------------------

// 【2.3.3】恰好型滑动窗口(2)
/*
930.和相同的二元子数组：给你一个二元数组 nums ，和一个整数 goal ，请你统计并返回有多少个和为 goal 的 非空 子数组。

1248.统计「优美子数组」：给你一个整数数组 nums 和一个整数 k。
如果某个连续子数组中恰好有 k 个奇数数字，我们就认为这个子数组是「优美子数组」。
请返回这个数组中 「优美子数组」 的数目。
*/
// ---------------------
// 模板题5，将问题转化为越长越合法类型
namespace s930o1
{	// 计算有多少个元素和 ≥k 的子数组。(也即越长越合法问题)
	// 计算有多少个元素和 > k，也就是 ≥k + 1 的子数组。
	// 答案就是元素和 ≥k 的子数组个数，减去元素和 ≥k+1 的子数组个数
	// slide的写法参见模板3，越长越合法，s1358o1
	class Solution {
	public:
		int numSubarraysWithSum(vector<int>& nums, int goal) {
			auto slide = [&](int target)->int {
				int left = 0, ans = 0, sum = 0, n = nums.size();
				for (int right = 0; right < n; ++right) {
					sum += nums[right];
					// 这里最大的陷阱是容易漏掉left <= right这个额外判断条件
					// 因为goal（target）有可能是0，那么可以将[left, right]区间内的元素一直清空都满足 >= goal
					// 在[0, left - 1]上的元素每一个都能和窗口组成一个满足条件的子数组，个数总共有left - 1 - 0 + 1 = left个
					// 特例：如果goal == 0，while循环结束后left最大为right + 1，此时在[0, right]区间上的每个元素都能和这个空区间组合成一个满足条件的子数组
					// 此时子数组的个数为right - 0 + 1 = left个
					while (sum >= target && left <= right) {
						sum -= nums[left++];
						// 或ans += n - right;
					}
					ans += left;
				}
				return ans;
				};

			return slide(goal) - slide(goal + 1);
		}
	};
}

// 套模板
namespace s1248m1
{
	class Solution {
	private:
		long long slide(vector<int>& nums, int k) {
			int n = nums.size(), left = 0, cnt = 0;
			long long ans = 0;
			for (int right = 0; right < n; ++right) {
				if (nums[right] & 1) {
					++cnt;
				}
				// cnt += (nums[right] & 1) 也可以这样写，更简洁
				while (cnt >= k) {
					if (nums[left] & 1) {
						--cnt;
					}
					++left;
				}
				ans += left;
			}
			return ans;
		}
	public:
		int numberOfSubarrays(vector<int>& nums, int k) {
			return slide(nums, k) - slide(nums, k + 1);
		}
	};
}
// ---------------------

// 【2.4】其他（选做）(6)
/*
1438.绝对差不超过限制的最长连续子数组：给你一个整数数组 nums ，和一个表示限制的整数 limit，
请你返回最长连续子数组的长度，该子数组中的任意两个元素之间的绝对差必须小于或者等于 limit 。
如果不存在满足条件的子数组，则返回 0 。

825.适龄的朋友：在社交媒体网站上有 n 个用户。给你一个整数数组 ages ，其中 ages[i] 是第 i 个用户的年龄。
如果下述任意一个条件为真，那么用户 x 将不会向用户 y（x != y）发送好友请求：
ages[y] <= 0.5 * ages[x] + 7
ages[y] > ages[x]
ages[y] > 100 && ages[x] < 100
否则，x 将会向 y 发送一条好友请求。
注意，如果 x 向 y 发送一条好友请求，y 不必也向 x 发送一条好友请求。另外，用户不会向自己发送好友请求。
返回在该社交媒体网站上产生的好友请求总数。

2401.最长优雅子数组：给你一个由 正 整数组成的数组 nums 。
如果 nums 的子数组中位于 不同 位置的每对元素按位 与（AND）运算的结果等于 0 ，则称该子数组为 优雅 子数组。
返回 最长 的优雅子数组的长度。
子数组 是数组中的一个 连续 部分。注意：长度为 1 的子数组始终视作优雅子数组。

1156.单字符重复子串的最大长度：如果字符串中的所有字符都相同，那么这个字符串是单字符重复的字符串。
给你一个字符串 text，你只能交换其中两个字符一次或者什么都不做，然后得到一些单字符重复的子串。返回其中最长的子串的长度。
*/
// ---------------------

// 有序哈希表 + 不定长滑动窗口，真正O(n)的做法要用到单调队列，详见11.4专题队列4.4单调队列小节
namespace s1438m1
{	// 用multiset效果是一样的，时间复杂度O(nlogn)，插入红黑树的调整需要O(logn)
	class Solution {
	public:
		int longestSubarray(vector<int>& nums, int limit) {
			map<int, int> cnt;
			int ans = 0, left = 0, n = nums.size();
			for (int right = 0; right < n; ++right) {
				++cnt[nums[right]];
				while (cnt.rbegin()->first - cnt.begin()->first > limit) {
					if (--cnt[nums[left]] == 0) {
						cnt.erase(nums[left]);
					}
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

// 窗口端点为数值，而非下标
namespace s825m1
{	// 瓶颈在排序上，性能比灵神的解法差
	// 排序后，先从左到右滑窗，记录右端点给左边人发送的请求数
	// 然后再遍历一遍，记录相同年龄的人中左边给右边发送请求的数量，最后汇总
	class Solution {
	public:
		int numFriendRequests(vector<int>& ages) {
			int n = ages.size(), ans = 0, cnt = 0;
			sort(ages.begin(), ages.end());// 瓶颈在sort上，O(nlogn)
			for (int left = 0, right = 1; right < n; ++right) {
				while (left < right && ages[right] >= (ages[left] - 7) * 2)// 式子变个形就用不到除法和double了
					++left;// 其实只有年龄大于等于15的用户才能接收到好友请求和发送好友请求，所以需要加left < right
				ans += (right - left);
				if (ages[right] >= 15 && ages[right] == ages[right - 1]) {
					cnt++;
					ans += cnt;
				} else {
					cnt = 0;
				}
			}
			return ans;
		}
	};
}
namespace s825o1
{	// 计数 + 滑动窗口，O(n)做法：
	// 1.window的端点不一定需要是数组的端点，也可以是题干中的某个值，比如这题中年龄
	// 这样每次移动窗口都是针对某个年龄中的所有人
	// 2.计数后其实就相当于做了排序sort，甚至还额外记录了每个年龄的人数
	// 因为年龄只有120个选择，范围很小，可以这样做
	// 3.窗口内维护年龄在区间 [ageY,ageX] 中的人数 cntWindow
	// ageY是对ageX来说符合发送好友要求的最小年龄
	class Solution {
	public:
		int numFriendRequests(vector<int>& ages) {
			vector<int> cnt(121, 0);
			for (int i : ages) {
				++cnt[i];
			}
			int n = ages.size(), left = 0, winCnt = 0, ans = 0;
			for (int right = 0; right < 121; ++right) {
				winCnt += cnt[right];
				while (right + 14 >= 2 * left) {// left <= right可以不加，即使最开始left会超过right，但winCnt一定是负数
					// 对结果没有影响，后面winCnt += cnt[right]会慢慢加成正的
					winCnt -= cnt[left];
					++left;
				}
				// while循环也可以改成：for (right + 14 >= 2 * left) ，因为right增大1，满足的区间左端点最多也只增大1
				// 也即，right的斜率只有1/2，后续while循环最多只会运行一次
				if (winCnt > 0) {
					ans += winCnt * cnt[right] - cnt[right];
				}
			}
			return ans;
		}
	};
}

// 位运算 + 滑动窗口
namespace s2401m1
{	// 暴力做法，时间复杂度奔着O(n^3)去了，不过由于check函数中right和left的长度最多不超过31，相当于n * 961，勉强能AC
	class Solution {
	private:
		bool check(vector<int>& nums, int left, int right) {
			if (left == right) return true;
			for (int i = left; i <= right; ++i) {
				for (int j = i + 1; j <= right; ++j) {
					if ((nums[i] & nums[j]) != 0) {
						return false;
					}
				}
			}
			return true;
		}
	public:
		int longestNiceSubarray(vector<int>& nums) {
			// 正整数
			// 长度为 1 的子数组始终视作优雅子数组
			int left = 0, ans = 1, n = nums.size();
			for (int right = 0; right < n; ++right) {
				if (!check(nums, left, right)) {
					++left;
				}
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}
namespace s2401o1
{	// 位运算技巧 + 暴力枚举
	// 依照题意，需要所有元素两两之间的与运算为0，且所有元素都是正整数
	// int类型，共32个坑位（比特位），数组元素因为是正整数，所以必定至少要占一个坑位（比特位为1）
	// 又因为nums[i] <= 1e9，那么意味着最多有30个坑位被占据（2^29 约等于 0.5e9）
	// 所以窗口内至多30个元素，可以采用暴力枚举的做法

	// 记住！带位运算的地方，做好补上括号，因为优先级不好确定
	// 暴力做法没有考虑如何将左端点移出区间，而是直接对每个右端点暴力枚举左边所有元素
	// 但由于窗口元素有限，时间复杂度是O(n * 30)
	class Solution {
	public:
		int longestNiceSubarray(vector<int>& nums) {
			int ans = 0;
			for (int i = 0; i < nums.size(); i++) { // 枚举子数组右端点 i
				int or_ = 0, j = i;
				while (j >= 0 && (or_ & nums[j]) == 0) { // nums[j] 与子数组中的任意元素 AND 均为 0
					or_ |= nums[j--]; // 加到子数组中
				}
				ans = max(ans, i - j);
			}
			return ans;
		}
	};
}
namespace s2401o2
{	// 位运算技巧 + 滑动窗口
	class Solution {
	public:
		int longestNiceSubarray(vector<int>& nums) {
			int or_ = 0, left = 0, ans = 0, n = nums.size();
			for (int right = 0; right < n; ++right) {
				while ((or_ & nums[right]) != 0) {// 有交集，也可以直接写while (or_ & nums[right])
					or_ ^= nums[left];// 异或操作相当于把左端点弹出，从 or 中去掉集合 nums[left]
					++left;
				}
				or_ |= nums[right];// 或操作相当于统计每个坑位的占用（窗口内元素的并集）
				// 把集合 nums[right] 并入 or 中
				ans = max(ans, right - left + 1);
			}
			return ans;
		}
	};
}

// o1是双指针 + 滑动窗口？感觉更像是双指针，分段解题思路可以学习，但真要做还是套o2模板
// o2是滑动窗口，时间复杂度是O(n)，几乎没有多余操作，比o1快，与s424类似
namespace s1156o1
{	// 因为j和k选取时会重复扫描一些元素，所以比o2的做法要慢一点
	class Solution {
	public:
		int maxRepOpt1(string text) {
			vector<int> cnt(26, 0);
			for (char c : text) {
				++cnt[c - 'a'];
			}
			int n = text.size(), ans = 0, i = 0;
			while (i < n) {
				int j = i;
				while (j < n && text[j] == text[i]) {
					++j;
				}
				int left = j - i;// 左边的字符段
				int k = j + 1;
				while (k < n && text[k] == text[i]) {
					++k;
				}
				int right = k - j - 1;// 右边的字符段
				ans = max(ans, min(left + right + 1, cnt[text[i] - 'a']));
				// left + right + 1是指两端字符间的那个多的字符可以被换掉
				// cnt[text[i] - 'a']是指没有多的字符用来替换中间字符
				i = j;// 移动指针
			}
			return ans;
		}
	};
}
namespace s1156o2
{	// 和s424类似，比424更难
	class Solution {
	public:
		int maxRepOpt1(string text) {
			vector<int> cntTotal(26, 0);
			for (char c : text) {
				++cntTotal[c - 'a'];
			}
			vector<int> cnt(26, 0);
			int n = text.size(), maxCnt = 0;
			int left = 0, right = 0;
			for (; right < n; ++right) {
				int index = text[right] - 'a';
				++cnt[index];
				if (cntTotal[index] - 1 > maxCnt) {// 最关键的就是这句话
					maxCnt = max(maxCnt, cnt[index]);
				} // 需要有得换，如果全部加进来算maxCnt就没字符用了
				  // 比如aaabaaa
				if (right - left > maxCnt) {
					--cnt[text[left] - 'a'];
					++left;
				}
			}
			return right - left;
		}
	};
}

// 模板题6（修改/替换k次获得最长的相同子序列），同类题s2024, s1156，s2831
namespace s424o1
{
	class Solution {
	public:
		int characterReplacement(string s, int k) {
			vector<int> num(26);
			int n = s.length();
			int maxn = 0; // maxn代表窗口内最高出现频次最高的字符的个数
			int left = 0, right = 0;
			while (right < n) {
				num[s[right] - 'A']++;
				maxn = max(maxn, num[s[right] - 'A']);
				if (right - left + 1 - maxn > k) {// 关键点在于if，当次循环内如果maxn没有更新，则窗口整体向右平移一个元素
					num[s[left] - 'A']--;// 窗口长度只会增大，不会减小，会保持答案要求的最大的长度直到右端点遍历完成
					left++;
				}
				right++;
				// ans = max(ans, right - left + 1），也可以这样写，但是没有直接最后返回right - left快
			}
			return right - left;// 最后返回时由于right = n，比left多加了一次，所以不是right - left + 1
		}
	};
}

// 本题有定长/不定长两种解题思路，定长的可以参考8.1专题1.2进阶
namespace s438o1
{
	class Solution {
	public:
		vector<int> findAnagrams(string s, string p) {
			int n = s.size(), k = p.size();
			if (n < k) return {};

			int cnt[26]{}; // 统计 p 的每种字母的出现次数
			for (char c : p) {
				cnt[c - 'a']++;
			}

			vector<int> ans;
			int left = 0;
			for (int right = 0; right < n; right++) {
				int c = s[right] - 'a';
				--cnt[c]; // 右端点字母进入窗口
				while (cnt[c] < 0) { // 字母 c 太多了
					++cnt[s[left++] - 'a']; // 左端点字母离开窗口
				}
				if (right - left + 1 == k) { // s' 和 p 的每种字母的出现次数都相同
					ans.push_back(left); // s' 左端点下标加入答案
				}
			}
			return ans;
		}
	};
}