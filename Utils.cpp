#include "Utils.h"
using namespace std;
vector<int> Utils::marioStates = { STATE_MARIO_WALK, STATE_MARIO_RUN, STATE_MARIO_JUMP };
vector<int> Utils::extractIntegers(string line) {
	vector<int> out;
	stringstream stream;
	stream << line;
	
	string tmp;
	int found;
	while (!stream.eof()) {
		stream >> tmp;
		if (stringstream(tmp) >> found)
			out.push_back(found);
		tmp = "";
	}
	return out;
}
//void Utils::analyseXAxis(string line, vector<int> *left, vector<int> *right) {
//	vector<int> l, r;
//	vector<int> integers = extractIntegers(line);
//	for (int i = 0;i > 0 ;i--) {
//		l.push_back(getLeft(integers[i] + integers[0], i));
//		r.push_back(l.back() + integers[i] + integers[0] );
//	}
//	// left.reserve(l.size() * 2); right.reserve(r.size() * 2);
//	left->clear(); right->clear();
//	left->insert(left->begin(), l.begin(), l.end());
//	right->insert(right->begin(), r.begin(), r.end());
//
//	reflexXAxis(&l); reflexXAxis(&r);
//	left->insert(left->end(), l.begin(), l.end());
//	right->insert(right->end(), r.begin(), r.end());
//
//	return;
//}
//void Utils::analyseYAxis(string line, vector<int> *top, vector<int> *bottom, int atLine) {
//	vector<int> t, b;
//	vector<int> integers = extractIntegers(line);
//	for (int i = 1;i < integers.size();i++) {
//		t.push_back(getTop(integers[i] + integers[0], atLine));
//		b.push_back(t.back() + integers[i] + integers[0] );
//	}
//	// left.reserve(l.size() * 2); right.reserve(r.size() * 2);
//	top->clear(); bottom->clear();
//	reflexYAxis(&t, &b);
//	top->insert(top->begin(), t.begin(), t.end());
//	bottom->insert(bottom->begin(), b.begin(), b.end());
//	reflexYAxis(&t, &b);
//	top->insert(top->end(), t.begin(), t.end());
//	bottom->insert(bottom->end(), b.begin(), b.end());
//	return;
//}
void Utils::analyseAxesForMario(string line,int *atLine, int* top, int* bottom, vector<int>* left, vector<int>* right) {
	vector<int> l, r;
	vector<int> integers = extractIntegers(line);
	*atLine = integers[0];
	*top = integers[1];
	*bottom = integers[2] +1;
	int ls = integers[3];
	for (int i = 4;i < integers.size() ;i++) {
		l.push_back(ls + TEX_RIGHT_CENTER_MARIO);
		r.push_back(ls + integers[i] + TEX_RIGHT_CENTER_MARIO);
		ls = ls + integers[i] + 2;
	}
	// left.reserve(l.size() * 2); right.reserve(r.size() * 2);
	left->clear(); right->clear();
	left->insert(left->begin(), l.begin(), l.end());
	right->insert(right->begin(), r.begin(), r.end());

	reflexXAxis(&l, &r, TEX_RIGHT_CENTER_MARIO, TEX_LEFT_CENTER_MARIO);
	l.insert(l.end(), left->begin(), left->end());
	r.insert(r.end(), right->begin(), right->end());
	left->clear(); right->clear();
	left->insert(left->begin(), l.begin(), l.end());
	right->insert(right->begin(), r.begin(), r.end());

	return;
}
void Utils::analyseAxesForEnemies(string line, int* objId, int* top, int* bottom, vector<int>* left, vector<int>* right) {
	vector<int> l, r;
	vector<int> integers = extractIntegers(line);
	*objId = integers[0] +1;
	*top = integers[1];
	*bottom = integers[2] + 1;
	int ls = integers[3];
	for (int i = 4;i < integers.size();i++) {
		l.push_back(ls + TEX_RIGHT_CENTER_ENEMIES);
		r.push_back(ls + integers[i] + TEX_RIGHT_CENTER_ENEMIES);
		ls = ls + integers[i] + 2;
	}
	left->clear(); right->clear();
	left->insert(left->begin(), l.begin(), l.end());
	right->insert(right->begin(), r.begin(), r.end());

	reflexXAxis(&l, &r, TEX_RIGHT_CENTER_ENEMIES, TEX_LEFT_CENTER_ENEMIES);
	l.insert(l.end(), left->begin(), left->end());
	r.insert(r.end(), right->begin(), right->end());
	left->clear(); right->clear();
	left->insert(left->begin(), l.begin(), l.end());
	right->insert(right->begin(), r.begin(), r.end());
}
//int Utils::getLeft(int width, int colInd) {
//	if (width % 2 == 0) 
//		return LEFT_CENTER - X_GAP * colInd - (width / 2 - 1);
//	else
//		return LEFT_CENTER - X_GAP * colInd - ((width+1) / 2 - 1);
//}
//int Utils::getTop(int height, int rowInd) {
//	if (height % 2 == 0)
//		return UPPER_CENTER + Y_GAP * rowInd - (height / 2 - 1);
//	else
//		return UPPER_CENTER + Y_GAP * rowInd - ((height + 1) / 2 - 1);
//}
void Utils::reflexXAxis(vector<int> *left, vector<int>* right, int right_center, int left_center) {

	for (int i = 0;i < left->size();i++) {
		(*left)[i] = right_center + (left_center -(*left)[i]) +1;
	}
	for (int i = 0;i < right->size();i++) {
		(*right)[i] = right_center + (left_center - (*right)[i]) +1;
	}
	std::reverse(left->begin(), left->end());
	std::reverse(right->begin(), right->end());
	left->swap(*right);
}
//void Utils::reflexYAxis(vector<int>* vec1, vector<int>* vec2) {
//	
//	std::reverse(vec1->begin(), vec1->end());
//	std::reverse(vec2->begin(), vec2->end());
//}
vector<int> Utils::getMarioStates() {
	return marioStates;
}
int Utils::getSpriteId(bool toRight, int objID, int id, int atLine) {
	return toRight * 100000 + objID * 1000 + atLine * 100 + id;
}
int Utils::getAnimationId(bool toRight, int objID, int state, int form) {
	return toRight * 10000 + objID * 1000 + form * 100 + state;
}