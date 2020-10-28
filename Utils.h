#pragma once
#include <fstream>
#include <sstream> 
#include <vector>
#include "Constants.h"
using namespace std;

class Utils {
private:
	static vector<int> marioStates;
public:
	/*static void analyseXAxis(string line, vector<int> *left, vector<int> *right);
	static void analyseYAxis(string line, vector<int> *top, vector<int> *bottom, int atLine);
	static int getLeft(int width, int colInd);
	static int getTop(int height, int rowInd);*/
	static void reflexXAxis(vector<int>* left, vector<int>* right, int right_center, int left_center);
	//static void reflexYAxis(vector<int>* vec1, vector<int>* vec2);
	static vector<int> extractIntegers(string);
	static vector<int> getMarioStates();
	static void analyseAxesForMario(string line, int* atLine, int* top, int* bottom, vector<int>* left, vector<int>* right);
	static void analyseAxesForEnemies(string line,int* objId, int* top, int* bottom, vector<int>* left, vector<int>* right);
	static int getSpriteId(bool toRight, int objID, int id, int atLine = 0);
	static int getAnimationId(bool toRight, int objID, int state, int form=0);
};

