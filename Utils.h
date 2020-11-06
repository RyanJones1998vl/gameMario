#pragma once
#include <fstream>
#include <sstream> 
#include <vector>
#include "Constants.h"
#include <math.h>
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
	static int getSpriteId(int toRight, int objID, int id, int atLine = 0);
	static int getAnimationId(int toRight, int objID, int state, int form=0);
	static void alignAnimationPosition(int* x, int* y);
	static float getAcceleratedSpeed(float currentSpeed, float acclerator, float target);
	static bool equalTo(float num1, float num2);
	static int getTileId(int row, int col, int* left, int* top, int* right, int* bottom, int scale=1);
	static int getTileId(int row, int col);
	static int getItemId(int objId, int index);
};

