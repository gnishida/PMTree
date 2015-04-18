#pragma once

#include "VBORenderManager.h"
#include <vector>
#include <QColor>

using namespace std;

class PMTree {
private:
	VBORenderManager* rendManager;

	// PMパラメータ
	int shape;
	float baseSize;
	float scale;
	float scaleV;
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
	float leafShape;
	float leafScale;
	float leafScaleX;
	float leafQuality;		// 論文にはないけど、qualityという記述はあるので、追加
	float attractionUp;
	float pruneRatio;
	float pruneWidth;
	float pruneWidthPeak;
	float prunePowerLow;
	float prunePowerHigh;

	QColor colorStem;
	QColor colorLeave;

	// 初期化時に、乱数によって決定される値
	float scale_tree;
	float length_base;

public:
	PMTree(VBORenderManager* rendManager);

	void load(const QString& filename);

private:
	void generate();
	void generateStem(int level, glm::mat4 modelMat, float radius, float length);
	float generateSegment(int level, int index, glm::mat4 modelMat, float radius1, float radius2, float stem_length, float length);
	float generateLeaves(int level, glm::mat4 modelMat, int leaves_per_branch, float interval);
	float shapeRatio(int shape, float position);
	float computeFlare(float Z);
	float computeRadius(float nTaper, float radius, float length, float Z);
	float genRand();
	float genRand(float a, float b);
	float deg2rad(float deg);
};

