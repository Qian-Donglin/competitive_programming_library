vector<int> SlideMinimum(vector<int>& data, int range) {
	vector<int> ret;
	deque<int> dq;
	for (int i = 0; i < data.size(); i++) {
		while (dq.size() && data[dq.back()] > data[i])dq.pop_back();
		dq.push_back(i);
		if (range <= i) {
			if (i - range == dq.front())
				dq.pop_front();
		}
		if (range - 1 <= i)
			ret.push_back(data[dq.front()]);
	}
	return ret;
}