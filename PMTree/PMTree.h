#pragma once

#include <vector>
#include <QColor>

using namespace std;

class PMTree {
private:
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

	void draw();
	void drawCone(float baseRadius, float topRadius, float height, const QColor& color);

private:
	float genRand();
	float genRand(float a, float b);
};

