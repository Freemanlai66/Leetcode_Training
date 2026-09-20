#pragma once
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
// 二分算法：二分查找 + 二分答案 + 其他
// 三、其他(7)

/*

69.x 的平方根：给你一个非负整数 x ，计算并返回 x 的 算术平方根 。
	由于返回类型是整数，结果只保留 整数部分 ，小数部分将被 舍去 。
	注意：不允许使用任何内置指数函数和算符，例如 pow(x, 0.5) 或者 x ** 0.5 。
	注：与367相同，不在此重复列出

74.搜索二维矩阵：给你一个满足下述两条属性的 m x n 整数矩阵：
每行中的整数从左到右按非严格递增顺序排列。
每行的第一个整数大于前一行的最后一个整数。
给你一个整数 target ，如果 target 在矩阵中，返回 true ；否则，返回 false 。

278.第一个错误的版本：你是产品经理，目前正在带领一个团队开发新的产品。
不幸的是，你的产品的最新版本没有通过质量检测。由于每个版本都是基于之前的版本开发的，
所以错误的版本之后的所有版本都是错的。
假设你有 n 个版本 [1, 2, ..., n]，你想找出导致之后所有版本出错的第一个错误的版本。
你可以通过调用 bool isBadVersion(version) 接口来判断版本号 version 是否在单元测试中出错。
实现一个函数来查找第一个错误的版本。你应该尽量减少对调用 API 的次数。

374.猜数字大小：我们正在玩猜数字游戏。猜数字游戏的规则如下：
我会从 1 到 n 随机选择一个数字。 请你猜选出的是哪个数字。
如果你猜错了，我会告诉你，我选出的数字比你猜测的数字大了还是小了。
你可以通过调用一个预先定义好的接口 int guess(int num) 来获取猜测结果，返回值一共有三种可能的情况：
-1：你猜的数字比我选出的数字大 （即 num > pick）。
1：你猜的数字比我选出的数字小 （即 num < pick）。
0：你猜的数字与我选出的数字相等。（即 num == pick）。返回我选出的数字。

162.寻找峰值：峰值元素是指其值严格大于左右相邻值的元素。
给你一个整数数组 nums，找到峰值元素并返回其索引。数组可能包含多个峰值，
在这种情况下，返回 任何一个峰值 所在位置即可。
你可以假设 nums[-1] = nums[n] = -∞ 。
你必须实现时间复杂度为 O(log n) 的算法来解决此问题。

1901.寻找峰值 II：一个 2D 网格中的 峰值 是指那些 严格大于 其相邻格子(上、下、左、右)的元素。
给你一个 从 0 开始编号 的 m x n 矩阵 mat ，其中任意两个相邻格子的值都 不相同 。
找出 任意一个 峰值 mat[i][j] 并 返回其位置 [i,j] 。
你可以假设整个矩阵周边环绕着一圈值为 -1 的格子。
要求必须写出时间复杂度为 O(m log(n)) 或 O(n log(m)) 的算法

852.山脉数组的峰顶索引：给定一个长度为 n 的整数 山脉 数组 arr ，其中的值递增到一个 峰值元素 然后递减。
返回峰值元素的下标。你必须设计并实现时间复杂度为 O(log(n)) 的解决方案。
*/
// ---------------------
// 二分答案
namespace s69m1
{	// 二分答案 + 闭区间，将区间设为[0,x]，在其间不断二分进行判断求解
	// 时间复杂度O(logN)，闭区间写法，开区间写法中右端点需要写成x + 1，会越界
	class Solution {
	public:
		int mySqrt(int x) {
			int left = 0;
			int right = x;
			while (left <= right)
			{
				long long mid = left + (right - left) / 2;
				if (mid * mid <= x) {
					left = mid + 1;
				}
				else {
					right = mid - 1;
				}
			}
			return right;
		}
	};
}
namespace s69o1
{	// 官方二分解法，闭区间二分写法可以借鉴这种方式，避免想不起来是return left还是right
	class Solution {
	public:
		int mySqrt(int x) {
			int l = 0, r = x, ans = -1;
			while (l <= r) {
				int mid = l + (r - l) / 2;
				if ((long long)mid * mid <= x) {
					ans = mid;//在查找过程中逐渐获得ans
					l = mid + 1;
				}
				else {
					r = mid - 1;
				}
			}
			return ans;
		}
	};
}
namespace s69o2
{	// 使用除法，直接不需要使用long long
	class Solution {
	public:
		int mySqrt(int x) {
			int left = 1, right = x;

			while (left <= right) {
				int mid = left + (right - left) / 2;
				if (mid <= x / mid) {
					// 乘法转成除法，两者是等价的
					left = mid + 1;
				}
				else {
					right = mid - 1;
				}
			}

			return right;
		}
	};
}

// 模板题1：矩阵转换为数组，或是用搜索二维矩阵的排除法进行结合
namespace s74m1
{	// 先找到行再在行内进行二分，共要进行两次二分，代码比较繁琐
	class Solution {
	public:
		bool searchMatrix(vector<vector<int>>& matrix, int target) {
			int m = matrix.size(), n = matrix[0].size();
			if (target < matrix[0][0] || target > matrix[m - 1][n - 1]) {
				return false;
			}

			// 先找到正确的行
			int left = -1, right = m;
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				if (matrix[mid][n - 1] == target) {
					return true;
				}
				(matrix[mid][n - 1] > target ? right : left) = mid;
			}

			// 在行内搜索
			vector<int>& vec = matrix[right];
			auto it = lower_bound(vec.begin(), vec.end(), target);
			if (it != vec.end()) {
				return *it == target;
			}
			return false;
		}
	};
}
namespace s74o1
{	// 其实可以将矩阵每行首位相连拼起来，当成一个大数组，取元素则用 [mid / n][mid % n]来实现
	// 时间复杂度O(logmn)
	class Solution {
	public:
		bool searchMatrix(vector<vector<int>>& matrix, int target) {
			int m = matrix.size(), n = matrix[0].size();
			int left = -1, right = m * n;
			// 注意，这里right并不能设为(m - 1)*(n - 1)，总元素个数为m * n个，所以right就是m * n
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				int x = matrix[mid / n][mid % n];
				if (x == target) {
					return true;
				}
				(x > target ? right : left) = mid;
			}
			return false;// 如果有等于target的元素，必定会被找到
		}
	};
}
namespace s74o2
{	// 如果不在每次二分时进行x与target的等于判断，拆分到最后进行
	// 那么需要把right的范围从m * n改成m * n - 1，因为可能退出后right = m * n，此时就越界了
	// matrix[right / n][right % n]求的值就不是预想的

	// 总的来说还是不推荐o2写法，o1更好
	class Solution {
	public:
		bool searchMatrix(vector<vector<int>>& matrix, int target) {
			int m = matrix.size(), n = matrix[0].size();
			int left = -1, right = m * n - 1;// 但需要注意这里要写成m * n - 1，如果还写m * n，最后right可能越界
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				int x = matrix[mid / n][mid % n];
				(x >= target ? right : left) = mid;
			}
			return matrix[right / n][right % n] == target;
		}
	};
}
namespace s74o3
{	// 排除法，跟s240类型，时间复杂度为O(m + n)，没有二分做法快
	class Solution {
	public:
		bool searchMatrix(vector<vector<int>>& matrix, int target) {
			int m = matrix.size(), n = matrix[0].size();
			int i = 0, j = n - 1;
			while (i < m && j >= 0) {
				if (matrix[i][j] == target) {
					return true;
				}
				if (matrix[i][j] < target) {
					++i;
				}
				else {
					--j;
				}
			}
			return false;
		}
	};

}
namespace s74m2
{	// 排除法 + 二分结合，在右上角开始排除，找到对应行后在该行进行二分，时间复杂度O(m + logn)

	class Solution {
	public:
		bool searchMatrix(vector<vector<int>>& matrix, int target) {
			int m = matrix.size(), n = matrix[0].size();
			int i = 0, j = n - 1;
			while (i < m) {
				int num = matrix[i][j];
				if (target > num) {
					++i;
				}
				else {
					auto& row = matrix[i];
					auto it = lower_bound(row.begin(), row.end(), target);
					if (it != row.end()) {
						return *it == target;
					}
					return false;
				}
			}
			return false;
		}
	};
}

// 标准二分答案
namespace s278m1
{
	bool isBadVersion(int version) {};

	class Solution {
	public:
		int firstBadVersion(int n) {
			int left = 0, right = n;
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				(isBadVersion(mid) ? right : left) = mid;
			}
			return right;
		}
	};
}

// 标准二分答案
namespace s374m1
{
	int guess(int num) {};
	class Solution {
	public:
		int guessNumber(int n) {
			int left = 0, right = n;
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				if (guess(mid) == 0) return mid;
				if (guess(mid) == -1) {
					right = mid;
				}
				else {
					left = mid;
				}
			}
			return right;
		}
	};
}

// 模板题2：二分的本质不是单调性而是二段性，check是true还是false(0/1)，或是一定可能/不一定满足(1?)也可以二分
namespace s162o1
{	// 这道题灵神的视频讲解不够清楚，去看宫水三叶的题解，评论区还有大神进一步解释

	// 二分可以将mid与端点比较，可以与target比较，可以与mid左右两侧比较，很灵活
	
	// 题干中的条件：对于所有有效的 i 都有 nums[i] != nums[i + 1]，对这种解法也是必要的
	// 否则可以有类似vector<int> vec = {3, 3, 3, 2, 4}这样的输入，不会得到正确答案

	// nums[mid] > nums[mid + 1]代表[0, mid]中一定有峰值
	// nums[mid] < nums[mid + 1]代表[mid + 1, n -1]中一定有峰值
	// 可以想象爬山，如果在上坡段，接着走下去，一定能发现峰值，而下坡则不一定
	// 这题就是一定存在峰值 / 可能存在峰值的二段性，题目只要求找到一个峰值，所以二分是可行的
	// 二分范围为(-1, n - 1)，也即[0, n - 2]
	// 一是因为循环中用了mid + 1，右端点取n - 1会出现越界
	// 二是因为如果最后答案是n - 1是峰值，那么意味着右半区是单调增数组，每次循环中left都等于mid，最后right就是n - 1
	// 有点像二分答案，但这是因为答案一定存在，一定存在峰值，最后一个能选的就是n - 1的话，n - 1一定是峰值
	// 二分范围(left, right)，区间代表所有可能的取值的范围
	// 二分答案(left, right]，区间代表答案的范围，左边是一定不满足条件的答案，右边是一定满足条件的答案
	class Solution {
	public:
		int findPeakElement(vector<int>& nums) {
			// (-1, n - 1) --> [0, n - 2]
			int left = -1, right = nums.size() - 1;
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] > nums[mid + 1]) {
					right = mid;
				}
				else {
					left = mid; // 这里left == n - 2时就会结束循环，不会越界
				}
			}
			return right;
		}
	};
}

// s162的进阶版本
namespace s1901o1
{	// 人往高处走，从矩阵任意元素开始，只要一直往比当前元素大的格子移动，一定能到峰值处
	/*
	left 初始为 -1，表示“已知不可能的行”的下界（开区间）。
	right 初始为 m-1，表示“可能包含峰值的行”的上界（闭区间）。
	循环保持的不变量：峰值一定在区间 (left, right] 内。
	
	如果 mat[i][j] > mat[i+1][j]：
	当前行的最大值已经大于下一行的同列值，说明峰值可能出现在第 i 行或更上面的行（因为往下只会更小）。
	所以我们将搜索范围缩小到 (left, i]，即把 right 更新为 i（仍保持右闭）。

	如果 mat[i][j] <= mat[i+1][j]：
	下一行的同列值更大，说明峰值可能出现在第 i+1 行或更下面的行。
	所以我们将搜索范围扩大到 (i, right]，即把 left 更新为 i（左开，排除第 i 行）。
	
	循环结束时left+1 == right，区间 (left, right] 内只有一个整数 right，所以返回right
	*/
	class Solution {
	private:
		int indexOfMax(vector<int>& vec) {
			return max_element(vec.begin(), vec.end()) - vec.begin();
		}

	public:
		vector<int> findPeakGrid(vector<vector<int>>& mat) {
			int m = mat.size();
			// [0, m - 2]
			int left = -1, right = m - 1;

			while (left + 1 < right) {
				int i = left + (right - left) / 2;
				int j = indexOfMax(mat[i]);
				(mat[i][j] > mat[i + 1][j] ? right : left) = i;
			}

			return{ right, indexOfMax(mat[right])};
		}
	};
}

// 二分
namespace s852m1
{
	class Solution {
	public:
		int peakIndexInMountainArray(vector<int>& arr) {
			int left = 0, right = arr.size() - 2;
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				(arr[mid] > arr[mid + 1] ? right : left) = mid;
			}
			return right;
		}
	};
}

// 类似模板2的s162，二分的本质是二段性，将数组分成两段，mid跟首元素/尾元素比较，选o2做法，跟尾元素比较
namespace s153o1
{	// 跟nums[0]比较，代码比跟nums[n - 1]相比，稍微更复杂一丢丢，也更容易错
	class Solution {
	public:
		int findMin(vector<int>& nums) {
			int target = nums[0];
			int left = 0, right = nums.size();// 不含0，开区间为(0, n)
			// 假设数组可以分成左右两段，左边一段所有元素大于最小值，右边所有元素>= 最小值
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] > target) {// 此处改成>= 也可以，因为每个元素都各不相同，不可能与target相等，但没必要这样写
					// 说明num在第一段，在最小值左边
					// 红色：一定在最小值左边
					left = mid;
				}
				else {
					// 说明num在第二段，是最小值或者最小值右边
					// 蓝色：一定在最小值右边或本身是最小值
					right = mid;
				}
			}
			// 如果返回值是n，说明(0, n)之间的所有元素都在最小值左边，而这是不可能的，所以假设不成立
			// 也即数组只有一段，单调增，最小值为nums[0]
			// 如果返回值不是n，那right的左边元素在最小值左边，自身又在最小值右边或本身是最小值
			// 也即nums[right]处就是最小值
			return right == nums.size() ? nums[0] : nums[right];
			// return nums[right % n];

		}
	};
}
namespace s153o2
{	// 灵神的写法，跟nums[n - 1]比较，代码更简单，因为跟尾端元素比较不需要特殊处理数组只有一段的情况
	// 红：mid在最小值左侧
	// 蓝：mid是最小值，或最小值右侧

	// 对于nums[0]，可能是最小值，也可能是最小值左侧，红蓝都有可能，所以需要额外判定
	// 对于nums[n - 1]，要么是最小值，要么是最小值右侧，一定是蓝色，所以不需要额外判定
	class Solution {
	public:
		int findMin(vector<int>& nums) {
			int target = nums.back();
			int n = nums.size();
			int left = -1, right = n - 1; // [0, n - 2]
			// 这里如果right还是写成n，虽然不影响答案，但是代码逻辑是不正确的
			// 二分查找区间内应该放不确定是红还是蓝的元素，对于该题的红蓝定义，n - 1位置一定是蓝色的
			// 所以区间中不应该存在n - 1

			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] > target) {
					// mid比尾元素大-> mid在左段，最小值一定mid右边
					left = mid;
				}
				else {
					// mid比尾元素小-> mid在右段，最小值是mid或者mid左边
					right = mid;
				}
			}
			return nums[right];
		}
	};
}
namespace s153o3
{	// 如果题目是求最大值，则跟首个元素比较比较合适
	// 也可以复用求最小值的代码。设最小值下标为i，那么最大值下标为(i + n - 1) % n。
	class Solution {
	public:
		int findMin(vector<int>& nums) {
			int n = nums.size();
			int target = nums[0];
			// [1, n - 1]
			int left = 0, right = n;
			// 看答案靠近哪个边界
			// 就返回哪一边，求最小值时<= 更新right，靠近right，返回right。
			// 求最小值时>=更新left, 靠近left，返回left
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] >= target) {
					left = mid;
				}
				else {
					right = mid;
				}
			}

			return nums[left];
		}
	};
}

// m1：结合s153，先找出最小值的下标，然后在两段中的其中一段再次二分，总共需要二分两次
// o1：只需要二分一次，对target和mid是否处于同一递增段进行分类讨论
namespace s33m1
{
	class Solution {
	private:
		// s153o2
		int findMin(vector<int>& nums) {
			int left = -1, right = nums.size() - 1;
			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				if (nums[mid] < nums.back()) {
					right = mid;
				}
				else {
					left = mid;
				}
			}
			return right;
		}
	public:
		int search(vector<int>& nums, int target) {
			if (nums.back() == target) {
				return nums.size() - 1;
			}
			int minIndex = findMin(nums);
			if (nums.back() > target) {
				// 在第二段找
				auto it = lower_bound(nums.begin() + minIndex, nums.end(), target);
				if (it == nums.end()) return -1;
				return *it == target ? (it - nums.begin()) : -1;
			}
			else {
				// 在第一段找
				auto it = lower_bound(nums.begin(), nums.begin() + minIndex, target);
				if (it == nums.end()) return -1;
				return *it == target ? (it - nums.begin()) : -1;
			}
		}
	};
}
namespace s33o1
{	// 对target和mid是否处于同一递增段进行分类讨论
	class Solution {
	public:
		int search(vector<int>& nums, int target) {
			int n = nums.size(), end = nums.back();
			int left = -1, right = n;
			// 这里right写成n没关系，因为只有一种情况可能会产生nums[n]的越界，也即target是比最大值要大的
			// 问题转化为思考数组最大值可能在哪：
			// 如果有两个递增段，最大值点在数组中间，x > target以及x <= end && target > end至少会触发一次
			//					也即，至少会触发一次right = mid，那么就不会越界了
			// 如果只有一个递增段，也即原数组升序排列，会因x <= end && target > end此判断条件使right = 0退出

			// 而且关键是这道题返回值是-1，如果有答案，查找中途早就返回mid了，nums[mid]中的mid也取不到n，所以100%安全

			while (left + 1 < right) {
				int mid = left + (right - left) / 2;
				int x = nums[mid];
				// 题干中注明了升序排列没有重复元素，所以可以提前返回
				if (x == target) return mid;

				if (x <= end && target > end) {
					// mid在右边，target在左边
					right = mid;
				}
				else if (x > end && target <= end) {
					// mid在左边，target在右边
					left = mid;
				}
				else {
					// mid和target在同一段，就和普通的二分查找一样处理
					if (x > target) {
						right = mid;
					}
					else {
						left = mid;
					}
				}
			}
			return  -1;
		}
	};
}

// TOP100里最难的题之一，理解不能，背下来吧，面试考这道题基本就是不想要你了，要背题目直接看o5，理解则是按顺序看过去
namespace s4o1
{	// 枚举做法：时间复杂度O(m + n), 空间复杂度(m + n)，o1最简单，为后面写起来更复杂的o2,o3,o4铺垫
	class Solution {
	public:
		double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
			/*
			nums1长度为m, nums2长度为n
			题目本质为求合并数组的中位数
			也即求合并数组，共m + n个元素中第k小的数
			k = (m + n) / 2 （上取整，如5 / 2 = 3）
			如果m + n为偶数，则找第k小和第k + 1小的数
			如果m + n为奇数，则找第k小的数

			枚举做法细节（时间复杂度O(m + n), 空间复杂度(m + n)）:
			1.保证m <= n，如果不满足，则交换a,b两个数组
			2.在a,b最左边都插入-∞哨兵，最右边都插入∞哨兵
			3.i和j的定义：a有i个数在第一组，b有j个数在第一组
			  i初始化为0，j = (m + n + 1) / 2 - i(初始化时不-i  )
			4.目标：a[i] <= b[j + 1] && a[i + 1] > b[j]
			5.如果m + n为偶数，中位数为max(ai, bj)和min(ai+1, bj+1)的平均值
			  如果m + n为奇数，规定第一组比第二组多一个数，中位数为max(ai,bj)
			*/
			// 细节1
			if (nums1.size() > nums2.size()) {
				swap(nums1, nums2);
			}
			// 细节2
			int m = nums1.size(), n = nums2.size();
			nums1.insert(nums1.begin(), INT_MIN);// O(m)
			nums2.insert(nums2.begin(), INT_MIN);// O(n)
			nums1.push_back(INT_MAX);
			nums2.push_back(INT_MAX);
			// 细节3
			int i = 0, j = (m + n + 1) / 2;
			while (true) {
				// 细节4 目标：max(ai, bj) <= min(ai+1, bj+1)，等价于下式
				// 因为存在性证明，后一个判断条件需要更强的>而不是>=
				if (nums1[i] <= nums2[j + 1] && nums1[i + 1] > nums2[j]) {
					// 细节5
					int max1 = max(nums1[i], nums2[j]);
					int min2 = min(nums1[i + 1], nums2[j + 1]);
					// int 转成 double的小技巧
					return (m + n) % 2 ? max1 : (max1 + min2) / 2.0;
				}
				++i;
				--j;
			}
		}
	};
}
namespace s4o2
{	// o1的小优化，将nums1[i] <= nums2[j + 1] && nums1[i + 1] > nums2[j]的条件进行简化
	class Solution {
	public:
		double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
			/*
			nums1长度为m, nums2长度为n
			题目本质为求合并数组的中位数
			也即求合并数组，共m + n个元素中第k小的数
			k = (m + n) / 2 （上取整，如5 / 2 = 3）
			如果m + n为偶数，则找第k小和第k + 1小的数
			如果m + n为奇数，则找第k小的数

			枚举做法细节（时间复杂度O(m + n), 空间复杂度(m + n)）:
			1.保证m <= n，如果不满足，则交换a,b两个数组
			2.在a,b最左边都插入-∞哨兵，最右边都插入∞哨兵
			3.i和j的定义：a有i个数在第一组，b有j个数在第一组
			  i初始化为0，j = (m + n + 1) / 2 - i(初始化时不-i  )
			4.如果m + n为偶数，中位数为max(ai, bj)和min(ai+1, bj+1)的平均值
			  如果m + n为奇数，规定第一组比第二组多一个数，中位数为max(ai,bj)
			*/
			// 细节1
			if (nums1.size() > nums2.size()) {
				swap(nums1, nums2);
			}
			// 细节2
			int m = nums1.size(), n = nums2.size();
			nums1.insert(nums1.begin(), INT_MIN);// O(m)
			nums2.insert(nums2.begin(), INT_MIN);// O(n)
			nums1.push_back(INT_MAX);
			nums2.push_back(INT_MAX);
			// 细节3
			int i = 0, j = (m + n + 1) / 2;
			// 细节4优化 
			// nums1[i] <= nums2[j + 1] && nums1[i + 1] > nums2[j]
			// 可以简化成下式，因为循环中i是增加的，j是减少的
			// 退出while前的最后一次循环恰恰满足nums1[i] <= nums2[j + 1]
			while (nums1[i + 1] <= nums2[j]) {
				++i;
				--j;
			}
			// 细节5
			int max1 = max(nums1[i], nums2[j]);
			int min2 = min(nums1[i + 1], nums2[j + 1]);
			// int 转成 double的小技巧
			return (m + n) % 2 ? max1 : (max1 + min2) / 2.0;
		}
	};

}
namespace s4o3
{	// 二分做法：时间复杂度O(m + n)，空间复杂度O(1)
	// 枚举做法还能勉强看懂一点，二分直接放弃治疗了，先背吧

	// 注意：下面是二分写法一，因为还是用到了哨兵元素，在nums1, nums2前插入了INT_MIN，时间复杂度还是O(m + n)
	// 核心：ai <= bj+1，i越小越能成立，i越大越不能成立，对“最大的”满足ai <= bj+1的i进行二分

	class Solution {
	public:
		double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
			// 细节1
			if (nums1.size() > nums2.size()) {
				swap(nums1, nums2);
			}
			// 细节2
			int m = nums1.size(), n = nums2.size();
			nums1.insert(nums1.begin(), INT_MIN);// O(m)
			nums2.insert(nums2.begin(), INT_MIN);// O(n)
			nums1.push_back(INT_MAX);
			nums2.push_back(INT_MAX);

			// 细节3
			// 循环不变量：nums1[left] <= nums2[j + 1]
			// 循环不变量：nums1[right] > nums2[j + 1]
			// 在i = 0时（注意插入了元素），check一定成立
			// 在i = m + 1时，check一定不成立
			int left = 0, right = m + 1;// [1, m]
			while (left + 1 < right) {
				int i = left + (right - left) / 2;
				int j = (m + n + 1) / 2 - i;
				if (nums1[i] <= nums2[j + 1]) {
					left = i;// 缩小二分区间为 (i, right)
				}
				else {
					right = i;// 缩小二分区间为 (left, i)
				}
			}

			// 细节4
			// 此时left + 1 = right
			// nums1[left] <= nums2[j+1] 且 nums1[right] > nums2[j'+1] = nums2[j]
			// 所以答案是 i = left
			int i = left;
			int j = (m + n + 1) / 2 - i;
			int max1 = max(nums1[i], nums2[j]);
			int min2 = min(nums1[i + 1], nums2[j + 1]);
			return (m + n) % 2 ? max1 : (max1 + min2) / 2.0;
		}
	};

}
namespace s4o4
{	//	二分做法最终版本：时间复杂度O(log min(m,n))，空间复杂度O(1)
	/*	去掉前后插入的INT_MIN和INT_MAX，i和j都减一
		1.定义发生变化： 
			i ： a 有 i+1 个数在第一组
			j ： b 有 j+1 个数在第一组
		2.i和j的关系式变化：
			j = (m + n + 1) / 2 - i				---->
			j + 1 = (m + n + 1) / 2 - (i + 1)	---->
			j = (m + n + 1) - i - 2 = (m + n + 3) / 2 - i (用前面的更好记忆)
		3.二分左右边界变化，边界改成-1和m，也即[0, m - 1]
	*/

	class Solution {
	public:
		double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
			// 细节1（nums1的元素数量不能比nums2多）
			if (nums1.size() > nums2.size()) {
				swap(nums1, nums2);
			}
			// 细节2，此时不插入元素了
			int m = nums1.size(), n = nums2.size();

			// 细节3，边界改变，i和j定义改变，关系式改变
			// 循环不变量：nums1[left] <= nums2[j + 1]
			// 循环不变量：nums1[right] > nums2[j + 1]
			// 在i = -1时，check一定成立（假装有元素）
			// 在i = m时，check一定不成立（假装有元素）
			int left = -1, right = m;// [0, m - 1]
			while (left + 1 < right) {
				int i = left + (right - left) / 2;
				int j = (m + n + 1) / 2 - i - 2;
				if (nums1[i] <= nums2[j + 1]) {// nums1对应i，nums2对应j + 1，<=
					left = i;// 缩小二分区间为 (i, right)
				}	// 直接记忆left -> right，都比较好记忆
				else {
					right = i;// 缩小二分区间为 (left, i)
				}
			}

			// 细节4，与细节3相同，计算中位数时i和j也要调整，注意不能越界
			// 此时left + 1 = right
			// nums1[left] <= nums2[j+1] 且 nums1[right] > nums2[j'+1] = nums2[j]
			// 所以答案是 i = left
			int i = left;// 记住i是left
			int j = (m + n + 1) / 2 - i - 2;
			int ai = i >= 0 ? nums1[i] : INT_MIN;
			int bj = j >= 0 ? nums2[j] : INT_MIN;
			int ai1 = i + 1 < m ? nums1[i + 1] : INT_MAX;// 记住这里是i + 1 < m，而不是i < m，下同
			int bj1 = j + 1 < n ? nums2[j + 1] : INT_MAX;
			int max1 = max(ai, bj);//max1和min2的变量名也记住，很方便，说明是第一组的max和第二组的min
			int min2 = min(ai1, bj1);
			return (m + n) % 2 ? max1 : (max1 + min2) / 2.0;// 记住默认第一组多一个元素，所以m + n为奇数时，直接返回max1
		}
	};
}
namespace s4o5
{	// o5开始是全新的思路推导路径，来自B站波波微课
	// 双指针计数法，可以省掉普通合并排序法的O(m + n)空间，空间复杂度为O(1)
	// 时间复杂度为O(log(m + n))
	class Solution {
	public:
		double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
			int m = nums1.size(), n = nums2.size();
			int total = m + n;
			int i = 0, j = 0;
			int cur = 0, prev = 0;

			for (int k = 0; k < total / 2 + 1; ++k) {
				prev = cur;
				// 这行判断条件非常精准、简洁，可以多消化下
				if (i < m && (j >= n || nums1[i] <= nums2[j])) {
					cur = nums1[i];
					++i;
				}
				else {
					cur = nums2[j];
					++j;
				}
			}

			return (total % 2) ? cur : ((prev + cur) / 2.0);
		}
	};
}
namespace s5o6
{
	// 中位数本质：把总序列切成左右两半，左半元素数量等于右半（或多1）
	// 不用真的合并排序，分别在两个有序数组上切一刀，组成左右两个部分
	// 实现数量对半分，且左边的数都不超过右边的数，那么就找到了中位数
	// 每次尝试切割后要跨数组检查，只需交叉进行，因为同组间本来就是有序的不用对比
	// 数组a切割完后的左半部分最大值与数组b切割完后右半部分最小值对比，另一组同理

	// 然后对较短的数组进行二分，二分的对象是短数组中要放多少个到左边
	// 假设短数组元素个数为m，那么二分范围就是[0, m]，最少可以放0个到左边，最多则是m个
	// 每次二分都可以排除掉一半的可能性，将时间复杂度从每个切割位置逐个分析缩小到O(log(m + n))

	/*	例子，m = 4，左边要放5个，右边要放4个
		1, 3, 5, 14
		2, 4, 8, 9, 11
		第一刀分割：
		1，3		5， 14
		2，4，8		9，11
		3 < 9符合，但8 > 5不符合，想要往满足条件的方向靠拢，只能将分割线右倾，从a中选更多数放入左边
		从a中选更多的数，右边露出的就不会是5，而是14这个更大的数，更容易满足交叉对比条件
		相对的，b中就会选更少的数放入左边，左边露出的就不会是8，而是4这个更小的数，更容易满足交叉对比条件
		第二刀分割：
		1，3，5		14
		2，4		8，9，11
		5 < 8符合，4 < 14符合，找到了中位数分组方案，那么答案就是两个左边右端点中最大的数
		也即5和4中的最大值：5
	*/
	class Solution {
	public:
		double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
			int m = nums1.size(), n = nums2.size();
			// 确保m是更短的数组
			if (m > n) {
				return findMedianSortedArrays(nums2, nums1);
			}

			// 闭区间二分
			int low = 0, high = m;
			while (low <= high) {
				// 二分查找的是分割点，左边为分割点-1，右边包含分割点
				// 范围是[0, m], 而不是[0, m - 1]
				int i = (low + high) / 2;       // 对 nums1 的分割点
				int j = (m + n + 1) / 2 - i;    // 对 nums2 的对应分割点

				// 处理边界，使用 ±∞ 保持比较正确
				int left1 = (i > 0) ? nums1[i - 1] : -1e9;
				int right1 = (i < m) ? nums1[i] : 1e9;
				int left2 = (j > 0) ? nums2[j - 1] : -1e9;
				int right2 = (j < n) ? nums2[j] : 1e9;

				// 判断是否找到正确的分割
				if (left1 <= right2 && left2 <= right1) {
					// 找到了
					if ((m + n) % 2 == 1) {
						// 总长度奇数，中位数是左半部分的最大值
						return max(left1, left2);
					}
					else {
						// 总长度偶数，中位数是左半最大和右半最小的平均值
						return (max(left1, left2) + min(right1, right2)) / 2.0;
					}
				}
				else {
					// 没找到
					if (left1 > right2) {
						// i 太大，需要减小 i（向左移动）
						high = i - 1;
					}
					else {
						// i 太小，需要增大 i（向右移动）
						low = i + 1;
					}
				}
			}
			// 正常情况下不会走到这里
			return 0.0;
		}
	};
}