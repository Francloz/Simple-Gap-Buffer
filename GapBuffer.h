#pragma once
#include <string>
#include <algorithm>
class GapBuffer {
	std::string front, back;
	size_t undeleted_left, undeleted_right;
	int position;
public:
	GapBuffer() : undeleted_left(0), undeleted_right(0), position(0) {}
	inline size_t size() { 
		return front.size() + back.size();
	}
	inline void append(const std::string& str) {
		back.append(str);
	}
	inline void insert(const std::string& str) {
		if (!undeleted_left) 
			front.append(str);
		else{
			const auto overlap = std::min(str.size(), undeleted_left);
			std::move(str.begin(), str.begin() + overlap, front.end() - undeleted_left);
			undeleted_left -= overlap;
			front.append(str.begin() + overlap, str.end());
		}
		
	}
	inline void moveCursor(const size_t& pos) {
		const auto cursor = front.size() - undeleted_left;
		if (pos < cursor) {
			const auto toDelete = cursor - pos;
			undeleted_left += toDelete;
			auto overlap = std::min(undeleted_right, toDelete);
			std::move(front.begin() + pos, front.begin() + pos + overlap, back.begin() + undeleted_right - overlap);
			undeleted_right -= overlap;
			back.append(front.begin() + pos + overlap, front.begin() + cursor);
		}
		else if (pos > cursor) {
			const auto toDelete = pos - cursor;
			auto overlap = std::min(undeleted_left, toDelete);
			std::move(back.begin() + undeleted_right, back.begin() + undeleted_right + overlap, front.begin() + cursor);
			front.append(back.begin() + undeleted_right + overlap, back.begin() + undeleted_right + toDelete);
			undeleted_left -= overlap;
			undeleted_right += toDelete;
		}
	}
	inline void insertAt(const size_t& pos, const std::string& str) {
		moveCursor(pos);
		insert(str);
	}
	inline std::string getStr() {
		std::string str(front.begin(),front.end()-undeleted_left);
		str.append(back.begin() + undeleted_right, back.end());
		return str;
	}
};