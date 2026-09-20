#pragma once
using namespace std;
#include <vector>
#include <algorithm>// max(), sort(), lower_bound(), upper_bound()
#include <numeric>// partial_sum()
#include <unordered_map>
#include <map>
#include <string>

// 二分算法：二分查找 + 二分答案
// 一、二分查找：最基础的搜索算法(16)

// 【1.1】基础（此阶段，二分查找手动实现，熟悉流程）(5)
/*

前面几题对于代码随想录的做法不用细究，在34o4、744、2529开始正式规范化套模板
需要做到lower_bound函数能闭着眼睛写出来，掌握开区间，闭区间，左闭右开区间三种做法，并掌握>,>=, <, <=如何只用lower_bound实现

704.二分查找：给定一个n个元素有序的（升序）整型数组nums和一个目标值target，写一个函数搜索nums中的target，如果目标值存在返回下标，否则返回-1。

35.搜索插入位置：给定一个排序数组和一个目标值，在数组中找到目标值，并返回其索引。如果目标值不存在于数组中，返回它将会被按顺序插入的位置。
	注意：无重复元素！

34.在排序数组中查找元素的第一个和最后一个位置：给你一个按照非递减顺序排列的整数数组nums，和一个目标值target。
   请你找出给定目标值在数组中的开始位置和结束位置。如果数组中不存在目标值target，返回[-1, -1]。

744.寻找比目标字母大的最小字母：给你一个字符数组 letters，该数组按非递减顺序排序，以及一个字符 target。letters 里至少有两个不同的字符。
	返回 letters 中大于 target 的最小的字符。如果不存在这样的字符，则返回 letters 的第一个字符

2529.正整数和负整数的最大计数：给你一个按 非递减顺序 排列的数组 nums ，返回正整数数目和负整数数目中的最大值。
	换句话讲，如果 nums 中正整数的数目是 pos ，而负整数的数目是 neg ，返回 pos 和 neg二者中的最大值。
	注意：0 既不是正整数也不是负整数
*/
// ---------------------
// 模板题1，红蓝染色法写lowerBound
namespace s34o1
{
	class Solution {
	private:
		// lowerBound用于获取>=处的下标
		// 如果想要获取>处下标，那么等同于>= (x+1)
		// 如果想要获取<处下标，那么等同于获取>=处下标的左边一个位置，即最后的返回值-1
		// 如果想要获取<=处下标，那么等同于获取>处下标的左边一个位置
		int lowerBound(vector<int>& nums, int target) {
			int left = 0, right = nums.size() - 1;
			while (left <= right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] >= target) {
					right = mid - 1;
				}
				else {
					left = mid + 1;
				}
			}
			return left;
		}
	public:
		vector<int> searchRange(vector<int>& nums, int target) {
			int start = lowerBound(nums, target);
			if (start == nums.size() || nums[start] != target) {// 短路求值，先写是否等于nums.size(),否则nums[start]的判断可能越界
				return {-1, -1};// 如果数组所有元素均小于target，或是没有等于target的元素
			}
			int end = lowerBound(nums, target + 1) - 1;// end为<=的下标
			return {start, end};
		}
	};

}
namespace s34m
{	//简单套壳二分查找，对于单调非减的数组，返回的序号可能是满足目标值的元素的下标中间值，于是在找到位置后向两边依次判断
	//能做而且很好理解，但是不够稳定，若是碰上{ 1, 1, 1, 1, 1 }，target = 1类似的情况，得遍历完整个数组，效率低
	class Solution {
	public:
		vector<int> searchRange(vector<int>& nums, int target) {
			int left = 0;
			int right = nums.size() - 1;
			int lb, ub;// lower_bound, upper_bound
			while (left <= right)
			{
				int m = left + (right - left) / 2;
				if (nums[m] < target)
					left = m + 1;
				else if (nums[m] > target)
					right = m - 1;
				else
				{
					lb = m, ub = m;
					while ((lb - 1) >= 0 && (nums[lb - 1] == target))//需要注意数组下标不能越界
						lb--;
					while ((ub + 1) <= (nums.size() - 1) && (nums[ub + 1] == target))
						ub++;
					return { lb, ub };
				}
			}
			return{ -1, -1 };
		}
	};

}
namespace s34o2
{	// 比较巧妙的方法，直接找等于target的地方,然后向左右二分扩展，简单易懂
	class Solution {
	public:
		vector<int> searchRange(vector<int>& nums, int target) {
			int left = 0, right = nums.size() - 1;
			int lb = -1, rb = -1; // target不在数组中的情况
			while (left <= right) {
				int middle = left + (right - left) / 2;
				if (nums[middle] == target) {
					lb = middle;
					right = middle - 1;// 重点，将查找区域向左区间转移，寻找lower bound
				}
				else if (nums[middle] < target) {
					left = middle + 1;
				}
				else {
					right = middle - 1;
				}
			}
			left = 0, right = nums.size() - 1;
			while (left <= right) {
				int middle = left + (right - left) / 2;
				if (nums[middle] == target) {
					rb = middle;
					left = middle + 1;
				}
				else if (nums[middle] < target) {
					left = middle + 1;
				}
				else {
					right = middle - 1;
				}
			}
			return { lb, rb };
		}
	};
}
namespace s34o3
{	// 比较讨巧的做法，把区间长度弄成正负0.5，直接用二分法可以得到区间
	// 尽量不要用这个方法，虽然是我自己写的代码，但是我也讲不明白...
	class Solution {
	public:
		vector<int> searchRange(vector<int>& nums, int target)
		{
			int l = binarySearch(nums, (double)target - 0.5);
			int r = binarySearch(nums, (double)target + 0.5) + 1;//这里为什么要+1,我自己也不知道，大概是这种二分求的都是小数的左边界吧,所以用小数求右边界时要+1...
			if (r - l >= 2)
				return { l + 1, r - 1 };
			else
				return { -1, -1 };
		}
		int binarySearch(vector<int>& nums, double half)
		{
			int l = 0, r = nums.size() - 1;
			while (l <= r)
			{
				int m = l + (r - l) / 2;
				if (nums[m] > half)
					r = m - 1;
				else
					l = m + 1;
			}
			return r;
		}
	};
}

namespace s35o1
{	// 灵神染色做法，缺点在于每次退出循环时都会遍历区间直至只剩一个元素
	// 题目等于在问数组中第一个>= target的元素的下标
	class Solution
	{
	public:
		int searchInsert(vector<int>& nums, int target)
		{
			int n = nums.size();
			int left = 0, right = n - 1;
			while (left <= right)
			{
				int mid = left + (right - left) / 2;
				if (nums[mid] < target)//这里必须是<，把剩下的>和=归到一起
					left = mid + 1;// 循环不变量，left - 1处一定是小于target的，红色
				else right = mid - 1;// 循环不变量，right + 1处一定是大于等于target的，蓝色
			}// 推出循环时，right + 1 = left
			return left;// 也可以写return right + 1
		}
	};
}
namespace s35m
{	// 我的解法很好理解，与其他两者解法相比效率也不会低
	class Solution {
	public:
		int searchInsert(vector<int>& nums, int target) {
			int left = 0;
			int right = nums.size() - 1;
			while (left < right)//定义的区间其实是左闭右闭区间，但是将其改为 <，则循环退出时left == right，为后续判断铺垫
			{
				int i = left + (right - left) / 2;
				if (nums[i] == target)
					return i;
				else if (nums[i] > target)
					right = i - 1;
				else
					left = i + 1;
			}
			// 循环中尝试确定target是否在nums中，最后区间会缩小成left = right，此时不满足循环条件退出
			// 在此基础上的三种可能中，>、= 可以合并，如此考虑也不需要额外考虑在目标值在数组所有元素之前或之后的情况，一样处理
			if (nums[left] >= target)
				return left;
			else return left + 1;
		}
	};
}
namespace s35o2
{	// 随想录做法（注意，该题目能用二分法的前提是数组中无重复元素，因为如果有重复元素，二分法得到的下标可能不唯一）
	class Solution {
	public:
		int searchInsert(vector<int>& nums, int target) {
			int n = nums.size();
			int left = 0;
			int right = n - 1; // 定义target在左闭右闭的区间里，[left, right]
			while (left <= right) { // 当left==right，区间[left, right]依然有效
				int middle = left + ((right - left) / 2);
				if (nums[middle] > target) {
					right = middle - 1;
				}
				else if (nums[middle] < target) {
					left = middle + 1;
				}
				else { // nums[middle] == target
					return middle;
				}
			}
			//分别处理如下四种情况
			//目标值在数组所有元素之前，此时最后区间为[0, -1]；
			//目标值等于数组中某一个元素  return middle，循环中已经完成处理;
			//目标值插入数组中的位置 [left, right]，两种if条件结果都是return  right + 1
			//目标值在数组所有元素之后的情况 [right+1, right]， 因为是右闭区间，所以 return right + 1
			//其实本质上数组所有元素之前和之后的两种情况可以归到插入数组中位置哪一类中，总共就两种情况
			//综上，所有情况下都是返回right + 1
			return right + 1;
		}
	};
}

namespace s704o1
{
	class Solution
	{// 保持区间开闭性不变，闭区间 或者 左闭右开区间 或开区间
	public:
		int search(vector<int>& nums, int target)
		{
			int left = 0;
			int right = nums.size() - 1;//左闭右闭
			while (left <= right)//当left > right时，区间内不存在任何整数元素
			{
				int middle = left + (right - left) / 2;
				if (nums[middle] > target)
					right = middle - 1;
				else if (nums[middle] < target)
					left = middle + 1;
				else return middle;
			}
			return -1;
		}
	};
}
namespace s704o2
{
	class Solution
	{
	public:
		int search(vector<int>& nums, int target)
		{	// 一直保证检测的区间都是左闭右开的，区间开闭性不变
			int left = 0;
			int right = nums.size();// 采用左闭右开区间时，需要将nums最右边的数据也包含进来，所以不能-1
			while (left < right) // left == right时，左闭右开的区间内就不存在元素了
			{
				int middle = left + (right - left) / 2;
				if (nums[middle] > target)
					right = middle;// 开区间侧不用+1-1
				else if (nums[middle] < target)
					left = middle + 1;
				else return middle;
			}
			return -1;
		}
	};
}
namespace s704o3
{
	class Solution
	{
	public:
		int search(vector<int>& nums, int target)
		{	// 开区间做法
			int left = -1;
			int right = nums.size();// 采用左闭右开区间时，需要将nums最右边的数据也包含进来，所以不能-1
			while (left + 1 < right)// left + 1 = right时，区间内就没有任何整数了
			{
				int middle = left + (right - left) / 2;
				if (nums[middle] > target)
					right = middle;// 开区间侧不用+1-1
				else if (nums[middle] < target)
					left = middle;
				else return middle;
			}
			return -1;
		}
	};

}

namespace s744m
{	// 灵神模板的简易运用，等效于找大于的第一个元素
	class Solution {
	public:
		char nextGreatestLetter(vector<char>& letters, char target) {
			int left = 0, right = letters.size() - 1;
			while (left <= right) {
				int mid = left + (right - left) / 2;
				if (letters[mid] < target + 1) {
					left = mid + 1;
				}
				else {
					right = mid - 1;
				}
			}
			return left == letters.size() ? letters[0] : letters[left];
		}
	};
}

namespace s2529m
{	// 简单套用灵神模板,找<0和>0的下标
	class Solution {
	private:
		int lowerBound(vector<int>& nums, int target) {
			int left = 0, right = nums.size() - 1;
			while (left <= right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] < target) {
					left = mid + 1;
				}
				else {
					right = mid - 1;
				}
			}
			return left;
		}
	public:
		int maximumCount(vector<int>& nums) {
			int pos = nums.size() - lowerBound(nums, 1);
			int neg = lowerBound(nums, 0);
			return max(pos, neg);
		}
	};
}
// ---------------------

// 【1.2】进阶（此阶段开始直接调用库函数进行二分查找）(11)
/*

难点在于问题的抽象化，如何判断出题目可以使用二分（一般加个排序即可），带query字符类型的问题可以往二分考虑
一般是排序，哈希表和二分查找的组合应用，加上一些数学性质的转换
需要熟练掌握库函数二分查找，包括随机访问和有序关联容器两种

2300.咒语和药水的成功次数：给你两个正整数数组 spells 和 potions ，长度分别为 n 和 m 
	其中 spells[i] 表示第 i 个咒语的能量强度，potions[j] 表示第 j 瓶药水的能量强度。
	同时给你一个整数 success 。一个咒语和药水的能量强度 相乘 如果 大于等于 success ，那么它们视为一对 成功 的组合。
	请你返回一个长度为 n 的整数数组 pairs，其中 pairs[i] 是能跟第 i 个咒语成功组合的 药水 数目。

1385.两个数组间的距离值：给你两个整数数组 arr1 ， arr2 和一个整数 d ，请你返回两个数组之间的 距离值 。
	「距离值」 定义为符合此距离要求的元素数目：对于元素 arr1[i] ，不存在任何元素 arr2[j] 满足 |arr1[i]-arr2[j]| <= d 。

2389.和有限的最长子序列：给你一个长度为 n 的整数数组 nums ，和一个长度为 m 的整数数组 queries 。
	返回一个长度为 m 的数组 answer ，其中 answer[i] 是 nums 中 元素之和小于等于 queries[i] 的 子序列 的 最大 长度  。
	子序列 是由一个数组删除某些元素（也可以不删除）但不改变剩余元素顺序得到的一个数组。

1170.比较字符串最小字母出现频次：定义一个函数 f(s)，统计 s  中（按字典序比较）最小字母的出现频次 ，其中 s 是一个非空字符串。
	例如，若 s = "dcce"，那么 f(s) = 2，因为字典序最小字母是 "c"，它出现了 2 次。
	现在，给你两个字符串数组待查表 queries 和词汇表 words 。对于每次查询 queries[i] ，需统计 words 中满足 f(queries[i]) < f(W) 的 词的数目 ，W 表示词汇表 words 中的每个词。
	请你返回一个整数数组 answer 作为答案，其中每个 answer[i] 是第 i 次查询的结果。
	注意：1 <= queries[i].length, words[i].length <= 10
		queries[i][j]、words[i][j] 都由小写英文字母组成

2080.区间内查询数字的频率：请你设计一个数据结构，它能求出给定子数组内一个给定值的 频率 。
	子数组中一个值的 频率 指的是这个子数组中这个值的出现次数。
	请你实现 RangeFreqQuery 类：
		RangeFreqQuery(int[] arr) 用下标从 0 开始的整数数组 arr 构造一个类的实例。
		int query(int left, int right, int value) 返回子数组 arr[left...right] 中 value 的 频率 。
	一个 子数组 指的是数组中一段连续的元素。arr[left...right] 指的是 nums 中包含下标 left 和 right 在内 的中间一段连续元素。

3488.距离最小相等元素查询：给你一个 循环 数组 nums 和一个数组 queries 。
	对于每个查询 i ，你需要找到以下内容：
	数组 nums 中下标 queries[i] 处的元素与 任意 其他下标 j（满足 nums[j] == nums[queries[i]]）之间的 最小 距离。如果不存在这样的下标 j，则该查询的结果为 -1 。
	返回一个数组 answer，其大小与 queries 相同，其中 answer[i] 表示查询i的结果。

2563.统计公平数对的数目：给你一个下标从 0 开始、长度为 n 的整数数组 nums ，和两个整数 lower 和 upper ，返回 公平数对的数目 。
	如果 (i, j) 数对满足以下情况，则认为它是一个 公平数对 ：
	0 <= i < j < n，且lower <= nums[i] + nums[j] <= upper

2070.每一个查询的最大美丽值：给你一个二维整数数组 items ，其中 items[i] = [pricei, beautyi] 分别表示每一个物品的 价格 和 美丽值 。
	同时给你一个下标从 0 开始的整数数组 queries 。对于每个查询 queries[j]，
	你想求出价格小于等于 queries[j] 的物品中，最大的美丽值 是多少。如果不存在符合条件的物品，那么查询的结果为 0 。
	请你返回一个长度与 queries 相同的数组 answer，其中 answer[j]是第 j 个查询的答案。

1146.快照数组：实现支持下列接口的「快照数组」- SnapshotArray：
	SnapshotArray(int length) - 初始化一个与指定长度相等的 类数组 的数据结构。初始时，每个元素都等于 0。
	void set(index, val) - 会将指定索引 index 处的元素设置为 val。
	int snap() - 获取该数组的快照，并返回快照的编号 snap_id（快照号是调用 snap() 的总次数减去 1）。
	int get(index, snap_id) - 根据指定的 snap_id 选择快照，并返回该快照指定索引 index 的值。
	
981.基于时间的键值存储：设计一个基于时间的键值数据结构，该结构可以在不同时间戳存储对应同一个键的多个值，并针对特定时间戳检索键对应的值。
	实现 TimeMap 类：TimeMap() 初始化数据结构对象
	void set(String key, String value, int timestamp) 存储给定时间戳 timestamp 时的键 key 和值 value。
	String get(String key, int timestamp) 返回一个值，该值在之前调用了 set，其中 timestamp_prev <= timestamp 。
	如果有多个这样的值，它将返回与最大  timestamp_prev 关联的值。如果没有值，则返回空字符串（""）。

思维拓展：这里不需要熟练掌握，开拓下思维即可

1287.有序数组中出现次数超过25%的元素：给你一个非递减的 有序 整数数组，已知这个数组中恰好有一个整数，
	它的出现次数超过数组元素总数的 25%。请你找到并返回这个整数，要求做到O(logn)

*/
// ---------------------
// 乘法与除法的转换：x*y >= z 等价于 y >= (z - 1)/x + 1 （但前提是z，x > 0)
// a/b（向上去整） 等价于 (a-1)/b + 1（向下去整）
namespace s2300o1
{	// 这题的关键是如何将乘法转化为除法，避免在二分搜索时进行相乘
	// c++的除法默认是向下去整，x*y >= z 等价于 y >= (z - 1)/x + 1，记住就好
	// 其他没有特别需要注意的地方，还是套>=的模板
	class Solution {
	private:
		int lowerBound(vector<int>& nums, int target) {
			int left = 0, right = nums.size() - 1;
			while (left <= right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] < target) {
					left = mid + 1;
				} else {
					right = mid - 1;
				}
			}
			return left;
		}
	public:
		vector<int> successfulPairs(vector<int>& spells, vector<int>& potions,
			long long success) {
			sort(potions.begin(), potions.end());
			vector<int> res(spells.size());
			for (int i = 0; i < spells.size(); ++i) {
				long long target = (success - 1) / spells[i] + 1;
				// 这里如果不把乘法转成除法，在二分搜索内部进行相乘判断乘积和success的大小，速度会变慢很多
				if (target <= potions.back()) {// 通过这个判断条件，二分搜索时就不用每次都把potions元素转换成long long了
					res[i] = potions.size() - lowerBound(potions, (int)target);// 确保这个分支下target转换成int没有损失数据
				} else {
					res[i] = 0;
				}
			}
			return res;
		}
	};
}

// 绝对值的理解
namespace s1385m
{	// 相当于在arr2中找和arr1[i]最相近的两个数，这两个数可以代表整个arr2（前提是arr2经过排序，可以使用二分查找）
	class Solution {
	private:
		int lowerBound(vector<int>& nums, int target) {
			int left = 0, right = nums.size() - 1;
			while (left <= right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] < target) {
					left = mid + 1;
				}
				else {
					right = mid - 1;
				}
			}
			return left;
		}
	public:
		int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
			sort(arr2.begin(), arr2.end());
			int cnt = 0;
			for (int x : arr1) {
				int index = lowerBound(arr2, x);
				bool valid = true;
				if (index < arr2.size() && arr2[index] - x <= d) {
					valid = false;
				}
				if (index > 0 && x - arr2[index - 1] <= d) {
					valid = false;
				}
				if (valid) ++cnt;
			}
			return cnt;
		}
	};
}
namespace s1385o1
{	// 灵神做法，判断条件上比我的更加简洁
	// 转换成寻找[x - d, x + d]区间内的arr2元素
	class Solution {
	private:
		int lowerBound(vector<int>& nums, int target) {
			int left = 0, right = nums.size() - 1;
			while (left <= right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] < target) {
					left = mid + 1;
				}
				else {
					right = mid - 1;
				}
			}
			return left;
		}
	public:
		int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
			int count = 0;
			sort(arr2.begin(), arr2.end());
			for (int x : arr1) {
				int index = lowerBound(arr2, x - d);
				if (index == arr2.size() || arr2[index] > x + d) {
					// 如果arr2中所有元素都小于x-d 或 arr2中第一个大于x-d的数就大于x+d， 那么符合条件
					++count;
				}
			}
			return count;
		}
	};
}

// 前缀和 + 二分查找，库函数upper_bound和lower_bound的应用，还有partial_sum求前缀和
namespace s2389m
{	// 第一次用上lower_bound/upper_bound函数，C++标准库自带
	class Solution {
	public:
		vector<int> answerQueries(vector<int>& nums, vector<int>& queries) {
			sort(nums.begin(), nums.end());
			int n = nums.size(), sum = 0;
			vector<int> presum(n, 0);
			for (int i = 0; i < n; ++i) {
				sum += nums[i];
				presum[i] = sum;
			}
			for (int& q : queries) {
				q = lower_bound(presum.begin(), presum.end(), q + 1) - presum.begin();
				// q = upper_bound(presum.begin(), presum.end(), q) - presum.begin();// 两个答案一样
			}
			return queries;
		}
	};
} 
namespace s2389o1
{	// 用partial_sum函数来计算前缀和，同时原地存储前缀和
	class Solution {
	public:
		vector<int> answerQueries(vector<int>& nums, vector<int>& queries) {
			sort(nums.begin(), nums.end());
			partial_sum(nums.begin(), nums.end(), nums.begin()); // 原地求前缀和，partial_sum的默认用法
			for (int& q : queries) { // 用 queries 保存答案
				q = upper_bound(nums.begin(), nums.end(), q) - nums.begin();
			}
			return queries;
		}
	};
}

// m1：f(s)写慢了，导致整体速度上不去，f需要被频繁调用
// m2：寻找字符串中最小字母的频次的o(n)做法，不用排序，加快f(s)速度
// o1：后缀和代替lower_bound提速，线性遍历一次完成所有元素的lower_bound
namespace s1170m1
{	// 思路是把排序放在f函数之外
	// 然后计算f(W)，并将其排序，之后就是套模板
	// 但是这个预计算f(W)的时间复杂度是m*nlogn，太复杂
	class Solution {
	private:
		int f(string& s) {
			return upper_bound(s.begin(), s.end(), s[0]) - s.begin();
		}
		// f(x)的时间复杂度设计成了nlogn + logn = nlogn，不如直接n复杂度遍历一遍
	public:
		vector<int> numSmallerByFrequency(vector<string>& queries, vector<string>& words) {
			vector<int> wordsIndex(words.size());
			vector<int> ans(queries.size());
			for (int i = 0; i < words.size(); ++i) {
				sort(words[i].begin(), words[i].end());
				wordsIndex[i] = f(words[i]);
			}
			sort(wordsIndex.begin(), wordsIndex.end());
			for (int i = 0; i < queries.size(); ++i) {
				sort(queries[i].begin(), queries[i].end());
				ans[i] = wordsIndex.end() - upper_bound(wordsIndex.begin(), wordsIndex.end(), f(queries[i]));
			}
			return ans;
		}
	};
}
namespace s1170m2
{	// 还是沿用m1的思路，但是f(s)优化成了o(m*n)，快了不少
	// 但是二分搜索的logn的时间其实也是可以优化掉的，见o1写法，思路是后缀和
	class Solution {
	private:
		int f(string& s) {// 利用题干信息，全部为小写字母
			char minChar = 'z'; // 最小字母初始化为 'z'
			int cnt = 0;        // 最小字母的频次
			for (char c : s) {
				if (c < minChar) {
					minChar = c;
					cnt = 1;    // 更新最小字母，重置频次
				}
				else if (c == minChar) {
					cnt++;      // 最小字母出现次数增加
				}
			}
			return cnt;
		}
	public:
		vector<int> numSmallerByFrequency(vector<string>& queries, vector<string>& words) {
			vector<int> wordsIndex(words.size());
			vector<int> ans(queries.size());
			for (int i = 0; i < words.size(); ++i) {
				wordsIndex[i] = f(words[i]);
			}
			sort(wordsIndex.begin(), wordsIndex.end());
			for (int i = 0; i < queries.size(); ++i) {
				ans[i] = wordsIndex.end() - upper_bound(wordsIndex.begin(), wordsIndex.end(), f(queries[i]));
			}
			return ans;
		}
	};
}
namespace s1170o1
{	// 后缀和思路
	// 利用题干信息，words和queries中单个单词长度最大为10
	class Solution {
	public:
		int f(string& s) {
			int cnt = 0;
			char ch = 'z';
			for (auto& c : s) {
				if (c < ch) {
					ch = c;
					cnt = 1;
				}
				else if (c == ch) {
					cnt++;
				}
			}
			return cnt;
		}
		vector<int> numSmallerByFrequency(vector<string>& queries, vector<string>& words) {
			vector<int> count(12);// count[0]空闲，count[11]用于避免越界，实际只用10个元素
			for (string& s : words) {
				count[f(s)]++;// count[i] 表示 words 中 f(s) == i 的单词数量。
			}
			for (int i = 9; i >= 1; --i) {
				count[i] += count[i + 1];// count[i] 表示 words 中 f(s) >= i 的单词数量。
				// 后缀和相当于在计算的时候就完成了所有元素的二分查找lower_bound
			}
			vector<int> res(queries.size());
			for (int i = 0; i < queries.size(); ++i) {
				res[i] = count[f(queries[i]) + 1];// 计算 f(s)，找到 words 中 f(s) > f(queries[i]) 的单词数量，即 count[f(s) + 1]。
				// 如果count只给11个位子，这里可能会越界，因为下标是i + 1
				// count用下标访问元素相当于lower_bound查询，查的是>=，题目要求的是>，那么把target+1即可
			}
			return res;
		}
	};
}

// 在使用unordered_map<int, vector<int>>这类的哈希表时，使用value时注意用引用，避免拷贝
// 如果想要通过迭代器获得子数组，那么用vector<int>的构造函数 vector<int> subVec(start, end + 1)
// 几乎所有接受迭代器范围的函数或构造函数都遵循左闭右开的约定，即 [start, end),所以需要写end + 1
// 但是需要注意，end可能等于vec.end(),构造子数组前需要加一个end != vec.end()的判断
namespace s2080o1
{
	class RangeFreqQuery {
	private:
		unordered_map<int, vector<int>> frequencyMap;// 不能放在构造函数内部，否则是局部变量，query函数用不了
	public:
		RangeFreqQuery(vector<int>& arr) {
			for (int i = 0; i < arr.size(); ++i) {
				frequencyMap[arr[i]].push_back(i);
				// 如果每次查询时都构建一次哈希表必定超时，所以在初始化时就对整体构建一次
				// 之后对value使用两次二分查找获取频率
			}
		}
		int query(int left, int right, int value) {
			vector<int>& indexVec = frequencyMap[value];// 这里必须用引用，要不然超时
			if (frequencyMap.size() == 0) return 0;// 这个判断加不加都是对的，但是加了稍微快一丢丢
			auto start = lower_bound(indexVec.begin(), indexVec.end(), left);
			auto end = upper_bound(indexVec.begin(), indexVec.end(), right) - 1;
			return end - start + 1;
		}
	};
}

// m1：哈希表 + 二分查找 + 分类讨论
// o1：分组生成下标列表 + 循环数组下标的处理哨兵），哈希表 + 二分查找复习，类似于s2080，能做易理解，但比较慢
// o2：预处理左右最近相同元素的下标，对每个元素记录其左右元素的下标，需要遍历nums四次
// o3：o2做法的优化，left和right放在同一次遍历下，省下一次遍历nums + nums的时间
// o4：换成哈希表记录首次和最后出现位置，只需遍历一次nums，但经测试没有o3快
namespace s3488m
{	// 哈希表 + 二分查找，尽量不用这种方法，虽然是自己写的，但太容易错了
	class Solution {
	private:
		unordered_map<int, vector<int>> indexMap;
	public:
		vector<int> solveQueries(vector<int>& nums, vector<int>& queries) {
			int n = nums.size();
			for (int i = 0; i < n; ++i) {
				indexMap[nums[i]].push_back(i);
			}
			for (int& i : queries) {
				vector<int>& indexVec = indexMap[nums[i]];// 这里用引用
				if (indexVec.size() == 1) {
					i = -1;// 原地修改
					continue;
				}
				// 至少有两个相等元素
				auto start = lower_bound(indexVec.begin(), indexVec.end(), i);
				if (start == indexVec.begin()) {
					i = min(*(start + 1) - *start, *start + n - indexVec.back());
					continue;
				}
				if (start == indexVec.end() - 1) {
					i = min(*start - *(start - 1), indexVec.front() + n - *start);
					continue;
				}
				// 至少有三个相等元素
				i = min(*(start + 1) - *start, *start - *(start - 1));
			}
			return queries;
		}
	};
}
namespace s3488o1
{	// 对于循环数组，加哨兵的做法值得学习
	// 但是对每个indexVec都进行两遍插入的操作会比较慢
	// 时间复杂度O(n + qlogn)
	/*
	看示例 1，其中所有 1 的下标列表是 p=[0,2,4]。由于 nums 是循环数组：
	在下标列表前面添加 4-n=-3，相当于认为在 -3 下标处也有一个 1。
	在下标列表末尾添加 0+n=7，相当于认为在 7 下标处也有一个 1。
	修改后的下标列表为 p=[-3,0,2,4,7]。
	*/
	class Solution {
	private:
		unordered_map<int, vector<int>> indexMap;
	public:
		vector<int> solveQueries(vector<int>& nums, vector<int>& queries) {
			for (int i = 0; i < nums.size(); ++i) {
				indexMap[nums[i]].push_back(i);
			}
			int n = nums.size();
			for (auto& pair : indexMap) {
				vector<int>& p = pair.second;// 必须为引用，要不然就换成结构化绑定
				int i0 = p[0];// 注意，这里要提前保存一下原本的begin()位置元素，否则第一次insert后就变了
				p.insert(p.begin(), p.back() - n);
				p.push_back(i0 + n);// 也可以用p.emplace_back(p[1] + n)，这样就不用提前保存
			}
			for (int& i : queries) {// i用引用，可以原地修改queries
				vector<int>& indexVec = indexMap[nums[i]];// 必须为引用
				if (indexVec.size() == 3) {// nums[i]对应的元素必有，头尾的哨兵也必有，共3个，若size为3，说明无其他相同元素
					i = -1;
					continue;
				}
				/*
				在这里加哨兵的话过不去的，因为每个相等元素都会在首尾插一遍哨兵，会污染后面的indexVec
				如果这里的indexVec不用引入，确实可以做到不污染，但是碰到超大型的indexVec就会超时，所以无解
				想在二分法里哨兵，就只能提前对每个indexVec都加一遍，但是那样频繁insert又太慢了...
				int i0 = indexVec[0];
				indexVec.insert(indexVec.begin(), indexVec.back() - n);
				indexVec.emplace_back(i0 + n);
				*/
				int idx = lower_bound(indexVec.begin(), indexVec.end(), i) - indexVec.begin();
				i = min(i - indexVec[idx - 1], indexVec[idx + 1] - i);
			}
			return queries;
		}
	};
}
namespace s3488o2
{	
	class Solution {
	public:
		vector<int> solveQueries(vector<int>& nums, vector<int>& queries) {
			int n = nums.size();
			vector<int> left(n), right(n);// left记录每个元素的最近左边相同元素的下标
			unordered_map<int, int> pos;
			for (int i = -n; i < n; i++) {// 相当于遍历nums + nums
				if (i >= 0) {
					left[i] = pos[nums[i]];// 先查询，后修改，每次left的值变动都滞后于pos的更新
				}
				pos[nums[(i + n) % n]] = i;// 记录最新出现的nums[i]，i + n是必要的，因为负数取模还是负数
			}
			pos.clear();
			for (int i = n * 2 - 1; i >= 0; i--) {
				if (i < n) {
					right[i] = pos[nums[i]];
				}
				pos[nums[i % n]] = i;
			}

			for (int& i : queries) {
				int l = left[i];
				i = i - l == n ? -1 : min(i - l, right[i] - i);
			}   // 如果频次为1，那么left和right与i之间的距离都是n，因为是循环数组，记录的其实是自己的下标
			return queries;
		}
	};
}
namespace s3488o3
{	// 这种单次遍历left和right的技巧值得学习
	class Solution {
	public:
		vector<int> solveQueries(vector<int>& nums, vector<int>& queries) {
			int n = nums.size();
			vector<int> left(n), right(n);
			unordered_map<int, int> pos;
			for (int i = -n; i < n; i++) {
				if (i >= 0) {
					int j = pos[nums[i]];
					left[i] = j;
					// 对于左边的 j 来说，它的 right 就是 i
					if (j >= 0) {
						right[j] = i;
					}
					else {
						right[j + n] = i + n;
					}
				}
				pos[nums[(i + n) % n]] = i;
			}

			for (int& i : queries) {
				int l = left[i];
				i = i - l == n ? -1 : min(i - l, right[i] - i);
			}
			return queries;
		}
	};
}
namespace s3488o4
{
	class Solution {
	public:
		vector<int> solveQueries(vector<int>& nums, vector<int>& queries) {
			int n = nums.size();
			vector<int> left(n), right(n);
			unordered_map<int, int> first, last; // 记录首次出现和最后一次出现的位置
			for (int i = 0; i < n; i++) {
				int x = nums[i];
				left[i] = last.find(x) != last.end() ? last[x] : -1;
				if (left[i] >= 0) {
					right[left[i]] = i;
				}
				if (first.find(x) == last.end()) {
					first[x] = i;
				}
				last[x] = i;
			}// 这个遍历会漏掉right的最后一个元素，因为更新最后一个left时，将其作为倒数第二个right的值
			// 最后一个right的值放在queries里面更新

			for (int& i : queries) {
				int l = left[i] >= 0 ? left[i] : last[nums[i]] - n;
				if (i - l == n) {
					i = -1;
				}
				else {// 如果i的右边没有元素的话，对应的right[i]是等于0的（即最开始的默认初始化状态）
					int r = right[i] != 0 ? right[i] : first[nums[i]] + n;
					i = min(i - l, r - i);
				}
			}
			return queries;
		}
	};
}

// 使用long long的经验，二分查找问题的识别练习(判断题目是二分查找类型）
// 用类似前缀和的思想计算 x <= i <= y的元素个数：
// i <= y的元素个数 减去 i < x的元素个数， 即可得到x <= i <= y的元素个数
namespace s2563m1
{	// nlogn时间复杂度，每次查询范围缩小，来进一步优化
	class Solution {
	public:
		long long countFairPairs(vector<int>& nums, int lower, int upper) {
			sort(nums.begin(), nums.end());
			long long count = 0;// 不用long long会溢出，题目中nums长度最大为10^5，数对数量则最多是5e9
			// 其实看题干中的返回值要求也知道count类型是long long
			// int的最大范围是2^31 - 1，也即2e9的范围
			for (int i = 0; i < nums.size(); ++i) {
				int t1 = lower - nums[i];
				int t2 = upper - nums[i];// 每次查询范围缩小，来进一步优化
				auto index1 = lower_bound(nums.begin() + i + 1, nums.end(), t1);// 每次只查询当前元素右边的值
				auto index2 = upper_bound(nums.begin() + i + 1, nums.end(), t2);// 左边不要忘了+1
				count += index2 - index1;// 灵神题解是固定右端点，我的答案是固定左端点，没区别
			}
			return count;
		}
	};
}

// map等有序关联容器的lower_bound函数调用，以及sort、lower_bound、upper_bound的自定义比较规则lambda写法
namespace s2070m1
{	// 哈希表 + 二分查找，比较笨的方法
	class Solution {
	public:
		vector<int> maximumBeauty(vector<vector<int>>& items, vector<int>& queries) {
			// 创建一个哈希map，储存同价格下，美丽数最大的item，构造时就完成排序
			map<int, int> itemMap;
			for (auto& p : items) {
				itemMap[p[0]] = max(itemMap[p[0]], p[1]);
			}
			// 预处理哈希表，整理每个价格与其对应的最大美丽数
			// （因为可能有一个价格更小但美丽值更大的item，题目只要求美丽数最大）
			int prevmax = 0;
			for (auto& p : itemMap) {
				if (prevmax > p.second) {
					p.second = prevmax;
				} else {
					prevmax = p.second;
				}
			}
			// 二分查找
			for (auto& i : queries) {
				auto it = itemMap.upper_bound(i);
				if (it == itemMap.begin()) {
					i = 0;
				} else {
					--it;
					i = it->second;
				}
			}
			return queries;
		}
	};
}
namespace s2070m2
{	// 两个改进： (灵神的写法用了C++20的ranges和投影，整不明白，我的是C++11的形式，性能没差)
	// 1.排序不用map，而是sort的自定义规则，对于连续内存操作，命中率更高，快了不少，也不需要额外的空间
	//	 map需要维护红黑树的平衡，插入和删除操作会引入额外开销。总空间复杂度为O(1)
	// 2.相对的二分也改用了自定义lambda规则，这部分性能基本没差别
	class Solution {
	public:
		vector<int> maximumBeauty(vector<vector<int>>& items, vector<int>& queries) {
			sort(items.begin(), items.end(), 
				[](const vector<int>& a, const vector<int>& b) {return a[0] < b[0]; });
			// 也可以直接用sort不用lambda，但会浪费时间对同价格下的beauty值进行排序，这是不需要的
			for (int i = 1; i < items.size(); ++i) {// i从1开始
				items[i][1] = max(items[i][1], items[i - 1][1]);
			}
			// 二分查找（lower_bound和upper_bound的lambda写法逻辑是反的，而且不能随意改动，库函数内部定死了
			for (auto& i : queries) {
				auto it = lower_bound(items.begin(), items.end(), i + 1, 
					[](const vector<int>& v, int value) {return v[0] < value; });
				//auto it = upper_bound(items.begin(), items.end(), i, 
				// [] (int value, const vector<int>& v) {return value < v[0];});
				if (it == items.begin()) {
					i = 0;
				} else {
					--it;
					i = (*it)[1];//这里不能写*it[1]，必须带括号
				}
			}
			return queries;
		}
	};
}

// 快照系统，通过存储操作信息减少内存占用，pair的应用（可以通过emplace_back提升性能）
namespace s1146m1
{	// 如果按照最笨的方式，每次快照都新创建一个数组，那么内存绝对超了
	// 转而保存每次操作的信息{snapID, value}
	// 这个写法还是比较笨，选择了三维数组，没有想到用pair，如果用了pair就可以利用emplace_back原地进行构造提升性能
	class SnapshotArray {
	private:
		int snapID = 0;
		vector<vector<vector<int>>> snapShots;
	public:
		SnapshotArray(int length) {
			snapShots.resize(length);
		}

		void set(int index, int val) {
			snapShots[index].push_back({ snapID, val });
			// 接收参数的是一维数组，想要构造一个{ snapID, val }，用emplace_back也会产生拷贝构造
			// snapShots[index].emplace_back(vector<int>({snapID, val}));
			/* 如果一次snap下，会重复set了很多次，那么可以考虑改成下面的版本
			if (snapShots[index].size() == 0) {
				snapShots[index].push_back({ snapID, val });// 当前下标的元素第一次set
			} else {
				int lastID = snapShots[index].back()[0];// 判断当前snapID下的val是否是最新的
				if (lastID == snapID) {
					snapShots[index].back()[1] = val;// 当前snapID下重复set了，所以替换原有的val
				} else {
					snapShots[index].push_back({ snapID, val });// 当前snapID下第一次set
				}
			}	

			*/
		}

		int snap() {
			++snapID;// id = 0
			return snapID - 1;
		}

		int get(int index, int snap_id) {
			vector<vector<int>>& indexShots = snapShots[index];
			auto it = upper_bound(indexShots.begin(), indexShots.end(), snap_id,
				[](int value, const vector<int>& v) {return value < v[0]; });
			// return it == indexShots.begin() ? 0 : (*--it)[1]; 等于这一行
			if (it == indexShots.begin()) {
				return 0;
			} else {
				--it;
				return (*it)[1];
			}
		}
	};
}
namespace s1146m2
{
	class SnapshotArray {
	private:
		int snapID = 0;
		vector<vector<pair<int, int>>> snapShots;
		// 也可以换成unordered_map<int, vector<pair<int, int>>>，对于少量查询占用内存更少，但查询量大之后用vector更快
	public:
		SnapshotArray(int length) {
			snapShots.resize(length);
		}

		void set(int index, int val) {
			snapShots[index].emplace_back(snapID, val);// 这里就可以用emplace_back来提升性能了
		}// pair可以接收(int, int)的参数

		int snap() {
			return snapID++;
		}

		int get(int index, int snap_id) {
			vector<pair<int, int>>& indexShots = snapShots[index];
			auto it = upper_bound(indexShots.begin(), indexShots.end(), snap_id,
				[](int value, const pair<int, int>& p) {return value < p.first; });
			// auto it = upper_bound(indexShot.begin(), indexShot.end(), pair<int, int>(snap_id, INT_MAX));
			// 不用lambda的做法，效率比较低
			return it == indexShots.begin() ? 0 : (*--it).second;
		}
	};
}

// 同s1146，更简单
namespace s981m
{
	class TimeMap {
	private:
		unordered_map<string, vector<pair<int, string>>> timeMap;
	public:
		TimeMap() {
		}

		void set(string key, string value, int timestamp) {
			timeMap[key].emplace_back(timestamp, value);
		}

		string get(string key, int timestamp) {
			vector<pair<int, string>>& vecs = timeMap[key];
			auto it = upper_bound(vecs.begin(), vecs.end(), timestamp,
				[](int value, const pair<int, string>& p) {return value < p.first; });
			return it == vecs.begin() ? "" : (--it)->second;
		}
	};
}

// 思维拓展：要求做到O(logn)，通过反证法，找出间隔点，对其在整个数组上二分查找获取频次
namespace s1287o1
{	// 遍历做法，时间复杂度O(n)
	class Solution {
	public:
		int findSpecialInteger(vector<int>& arr) {
			int cnt = 0;
			int n = arr.size();
			int cur = arr[0];
			for (int i = 0; i < n; ++i) {
				if (cur == arr[i]) {
					++cnt;
					if (cnt * 4 > n) {
						return arr[i];
					}
				} else {
					cur = arr[i];
					cnt = 1;
				}
			}
			return -1;
		}
	};
}
namespace s1287o2
{	// 其中一个间隔点必定是答案，否则通过反证法可知本题没有答案，但题干明确指出有且仅有一个答案，所以可以优化
	class Solution {
	public:
		int findSpecialInteger(vector<int>& arr) {
			int n = arr.size();
			int span = n / 4;
			for (int i = span; i < n; i += span) {
				auto start = lower_bound(arr.begin(), arr.end(), arr[i]);
				if (*start == *(start + span)) {
					return *start;
				}
			}
			return -1;
		}
	};
}
// ---------------------