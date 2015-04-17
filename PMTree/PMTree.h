#pragma once

#include "VBORenderManager.h"
#include <vector>
#include <QColor>

using namespace std;

class PMTree {
private:
	VBORenderManager* rendManager;

	int shape;
	float baseSize;
	float scale;
	float scaleV;
	float zScale;
	float zScaleV;
	int levels;
	float ratio;
	float ratioPower;
	float lobes;
	float lobeDepth;
	float flare;
	vector<float> nScale;
	vector<float> nScaleV;
	vector<float> nLength;
	vector<float> nLengthV;
	vector<float> nTaper;
	vector<float> nBaseSplits;
	vector<float> nSegSplits;
	vector<float> nSplitAngle;
	vector<float> nSplitAngleV;
	vector<float> nCurveRes;
	vector<float> nCurve;
	vector<float> nCurveBack;
	vector<float> nCurveV;
	vector<float> nDownAngle;
	vector<float> nDownAngleV;
	vector<float> nRotate;
	vector<float> nRotateV;
	vector<float> nBranches;
	int leaves;
	float leafShapes;
	float leafScale;
	float leafScaleX;
	float attractionUp;
	float pruneRatio;
	float pruneWidth;
	float pruneWidthPeak;
	float prunePowerLow;
	float prunePowerHigh;

public:
	PMTree();

	void generate(VBORenderManager* rendManager);

private:
	void generateStem(int level, glm::mat4 modelMat, float radius, float length);
	void generateSegment(int level, int index, glm::mat4 modelMat, float radius1, float radius2, float stem_length, float length);
	float shapeRatio(int shape, float position);
	float genRand();
	float genRand(float a, float b);
	float deg2rad(float deg);
};

