// reference
// https://ei1333.github.io/luzhiled/snippets/string/suffix-array.html

#pragma once

#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

/*******************************************************
// argument: string
// construction: O(NlogN)
// substring search: O(MlogN) (M := length of substring to find)

SuffixArray(const std::string&)
: construct suffix array

std::pair<int,int> lower_upper_bound(const std::string&)
: return the lower and upper bounds (half-open interval) of the indices of suffixes that contain the given string t.

*******************************************************/

class SuffixArray {
	private:
		std::vector<int> SA_;
		const std::string s_;

		bool lt_substr_(const std::string&t, int si = 0, int ti = 0) {
			int sn = s_.size(), tn = t.size();
			while (si < sn & ti < tn) {
				if (s_[si] < t[ti]) return true;
				if (s_[si] > t[ti]) return false;
				++si; ++ti;
			}
			return si >= sn && ti < tn;
		}

		int lower_bound_(const std::string&t) {
			int low = -1, high = static_cast<int>(SA_.size());
			while (high - low > 1) {
				int mid = low + (high - low) / 2;
				if (lt_substr_(t, SA_[mid])) low = mid;
				else high = mid;
			}
			return high;
		}

	public:
		// constructor
		SuffixArray(const std::string& s) : s_(s) {
			SA_.resize(s.size());
			std::iota(SA_.begin(), SA_.end(), 0);
			std::sort(SA_.begin(), SA_.end(), [&](int a, int b) {
				return s_[a] == s[b] ? a > b : s[a] < s[b];
			});
			std::vector<int> classes(s.size()), c(s.begin(), s.end());
			// doubling
			for (size_t len=1; len<s.size(); len<<= 1) {
				for (size_t i=0; i<s.size(); i++) {
					if (i > 0 && c[SA_[i-1]] == c[SA_[i]] && SA_[i-1] + len < s.size() && c[SA_[i-1] + len / 2] == c[SA_[i] + len / 2]) {
						classes[SA_[i]] = classes[SA_[i-1]];
					} else {
						classes[SA_[i]] = static_cast<int>(i);
					}
				}
				std::vector<int> cnt(s.size());
				std::iota(cnt.begin(), cnt.end(), 0);
				std::copy(SA_.begin(), SA_.end(), c.begin());
				for (size_t i=0; i<s.size(); i++) {
					int s1 = c[i] - len;
					if (s1 >= 0) SA_[cnt[classes[s1]]++] = s1;
				}
				classes.swap(c);
			}
		}

		std::pair<int,int> lower_upper_bound(std::string& t) {
			int idx = lower_bound_(t);
			int low = idx-1, high = static_cast<int>(SA_.size());
			t.back()++;
			while (high - low > 1) {
				int mid = (low + high) / 2;
				if (lt_substr_(t, SA_[mid])) low = mid;
				else high = mid;
			}
			t.back()--;
			return std::make_pair(idx, high);
		}

		int operator[](int index) const {
			return SA_[index];
		}

		size_t size() const {
			return s_.size();
		}
};
