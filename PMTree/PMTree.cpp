#include "PMTree.h"
#include <QGLWidget>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

#define M_PI	3.141592653589

PMTree::PMTree() {
	shape = 7;
	baseSize = 0.4;
	scale = 13.0;
	scaleV = 3.0;
	zScale = 1.0;
	zScaleV = 0.0;
	levels = 3;
	ratio = 0.015;
	ratioPower = 1.2;
	lobes = 5.0;
	lobeDepth = 0.07;
	flare = 0.6;

	nScale.resize(levels);
	nScaleV.resize(levels);
	nLength.resize(levels);
	nLengthV.resize(levels);
	nTaper.resize(levels);
	nBaseSplits.resize(levels);
	nSegSplits.resize(levels);
	nSplitAngle.resize(levels);
	nSplitAngleV.resize(levels);
	nCurveRes.resize(levels);
	nCurve.resize(levels);
	nCurveBack.resize(levels);
	nCurveV.resize(levels);
	nDownAngle.resize(levels);
	nDownAngleV.resize(levels);
	nRotate.resize(levels);
	nRotateV.resize(levels);
	nBranches.resize(levels);

	nBranches[0] = 10.0;	// <- 論文ではないけど。。。
	nScale[0] = 1.0;
	nScaleV[0] = 0.0;
	nLength[0] = 1.0;
	nLengthV[0] = 0.0;
	nTaper[0] = 1.0;
	nBaseSplits[0] = 0;
	nSegSplits[0] = 0;
	nSplitAngle[0] = 0.0;
	nSplitAngleV[0] = 0.0;
	nCurveRes[0] = 3;
	nCurve[0] = 0;
	nCurveBack[0] = 0;
	nCurveV[0] = 20;

	nDownAngle[1] = 60.0;
	nDownAngleV[1] = -5.0;//-50.0;
	nRotate[1] = 140.0;
	nRotateV[1] = 0.0;
	nBranches[1] = 50.0;
	nLength[1] = 0.3;
	nLengthV[1] = 0.0;
	nTaper[1] = 1.0;
	nSegSplits[1] = 0;
	nSplitAngle[1] = 0.0;
	nSplitAngleV[1] = 0.0;
	nCurveRes[1] = 5;
	nCurve[1] = -4.0;//-40;
	nCurveBack[1] = 0;
	nCurveV[1] = 5.0;//50;

	nDownAngle[2] = 45.0;
	nDownAngleV[2] = 10.0;
	nRotate[2] = 140.0;
	nRotateV[2] = 0.0;
	nBranches[2] = 30.0;
	nLength[2] = 0.6;
	nLengthV[2] = 0.0;
	nTaper[2] = 1.0;
	nSegSplits[2] = 0;
	nSplitAngle[2] = 0.0;
	nSplitAngleV[2] = 0.0;
	nCurveRes[2] = 3;
	nCurve[2] = -40;
	nCurveBack[2] = 0;
	nCurveV[2] = 75;

	nDownAngle[2] = 45.0;
	nDownAngleV[2] = 10.0;
	nRotate[2] = 77.0;
	nRotateV[2] = 0.0;
	nBranches[2] = 10.0;
	nLength[2] = 0.0;
	nLengthV[2] = 0.0;
	nTaper[2] = 1.0;
	nSegSplits[2] = 0;
	nSplitAngle[2] = 0.0;
	nSplitAngleV[2] = 0.0;
	nCurveRes[2] = 1;
	nCurve[2] = 0;
	nCurveBack[2] = 0;
	nCurveV[2] = 0;

	leaves = 25;
	leafShapes = 0;
	leafScale = 0.17;
	leafScaleX = 1;
	attractionUp = 0.5;
	pruneRatio = 0;
	pruneWidth = 0.5;
	pruneWidthPeak = 0.5;
	prunePowerLow = 0.5;
	prunePowerHigh = 0.5;
}

void PMTree::generate(VBORenderManager* rendManager) {
	this->rendManager = rendManager;

	float length0 = genRand(scale, scaleV) * genRand(nLength[0], nLengthV[0]);
	float radius0 = length0 * ratio * genRand(nScale[0], nScaleV[0]);

	float unit_taper;
	if (nTaper[0] < 1) {
		unit_taper = nTaper[0];
	} else if (nTaper[0] < 2) {
		unit_taper = 2.0 - nTaper[0];
	} else {
		unit_taper = 0.0;
	}

	mat4 modelMat;

	glPushMatrix();
	for (int i = 0; i < nCurveRes[0]; ++i) {
		float length = length0 / nCurveRes[0];

		float taper_z1 = radius0 * (1.0 - unit_taper * (float)i / nCurveRes[0]);
		float taper_z2 = radius0 * (1.0 - unit_taper * (float)(i + 1) / nCurveRes[0]);

		float r1 = taper_z1;
		float r2 = taper_z2;

		generateSegment(0, i, modelMat, r1, r2, length0, length);

		modelMat = translate(modelMat, vec3(0, 0, length));
		modelMat = rotate(modelMat, deg2rad(genRand(nCurve[0], nCurveV[0])), vec3(1, 0, 0));
	}
	glPopMatrix();
}

/**
 * Stemを生成する。
 *
 * @param level		レベル
 * @param modelMat	モデル行列
 * @param radius	Stemの一番下の部分の半径
 * @param length	Stemの長さ
 */
void PMTree::generateStem(int level, mat4 modelMat, float radius, float length) {
	float unit_taper;
	if (nTaper[level] < 1) {
		unit_taper = nTaper[level];
	} else if (nTaper[level] < 2) {
		unit_taper = 2.0 - nTaper[level];
	} else {
		unit_taper = 0.0;
	}

	glPushMatrix();
	for (int i = 0; i < nCurveRes[level]; ++i) {
		float segment_length = length / nCurveRes[level];

		float taper_z1 = radius * (1.0 - unit_taper * (float)i / nCurveRes[level]);
		float taper_z2 = radius * (1.0 - unit_taper * (float)(i + 1) / nCurveRes[level]);

		float r1 = taper_z1;
		float r2 = taper_z2;

		//generateSegment(level, i, modelMat, r1, r2, length, segment_length);
		rendManager->addCylinder("tree", modelMat, QVector3D(0, 0, 0), r1, r2, segment_length, QColor(i * 100, 0, 255));

		modelMat = translate(modelMat, vec3(0, 0, segment_length));
		modelMat = rotate(modelMat, deg2rad(genRand(nCurve[level], nCurveV[level])), vec3(1, 0, 0));
	}
	glPopMatrix();
}

void PMTree::generateSegment(int level, int index, mat4 modelMat, float radius1, float radius2, float stem_length, float segment_length) {
	rendManager->addCylinder("tree", modelMat, QVector3D(0, 0, 0), radius1, radius2, segment_length, QColor(index * 100, 0, 255));

	float length_base = baseSize * genRand(scale, scaleV);

	float start_substem = 0.0f;	// substemを開始するoffset位置

	if (segment_length * (index + 1) < length_base) {
		// segmentが全てbaseの一部なので、substemは生成しない
		return;
	} else if (segment_length * index >= length_base) {
		// segmentが、完全にbaseの外なので、一番下からsubstemを生成出来る
	} else {
		start_substem = length_base - segment_length * (int)(length_base / segment_length);
	}

	float subst_per_segm = nBranches[level] / nCurveRes[level];
	int substems_eff = subst_per_segm / segment_length * (segment_length - start_substem) + 0.5f; // substemの数
	float dist = (segment_length - start_substem) / (substems_eff + 1);	// substem間の距離

	// z軸まわりに移動
	start_substem += dist;
	modelMat = translate(modelMat, vec3(0, 0, start_substem));

	for (int s = 0; s < substems_eff; ++s) {
		float offset_child = segment_length * index + start_substem;

		// x軸まわりに回転
		mat4 modelMat2 = rotate(modelMat, deg2rad(genRand(nDownAngle[level + 1], nDownAngleV[level + 1])), vec3(1, 0, 0));

		float length_max = genRand(nLength[level + 1], nLengthV[level + 1]);
		float length_child = stem_length * length_max * shapeRatio(shape, (stem_length - offset_child) / (stem_length - length_base));
		float radius_child = radius1 * (length_child / stem_length);

		//rendManager->addCylinder("tree", modelMat2, QVector3D(0, 0, 0), 0.1, 0.1, 2, QColor(index * 100, 0, 255));
		generateStem(level + 1, modelMat2, radius_child, length_child);

		// z軸まわりに回転
		modelMat = rotate(modelMat, deg2rad(genRand(nRotate[level + 1], nRotate[level + 1])), vec3(0, 0, 1));

		// z軸まわりに移動
		modelMat = translate(modelMat, vec3(0, 0, dist));
		start_substem += dist;
	}
}

float PMTree::shapeRatio(int shape, float position) {
	if (shape == 0) {
		return 0.2f + 0.8f * position;
	} else if (shape == 1) {
		return 0.2f + 0.8f * sinf(M_PI * position);
	} else if (shape == 2) {
		return 0.2f + 0.8f * sinf(0.5f * M_PI * position);
	} else if (shape == 3) {
		return 1.0f;
	} else if (shape == 4) {
		return 0.5f + 0.5f * position;
	} else if (shape == 5) {
		if (position <= 0.7) return position / 0.7f;
		else return (1.0f - position) / 0.3f;
	} else if (shape == 6) {
		return 1.0f - 0.8f * position;
	} else if (shape == 7) {
		if (position <= 0.7) return 0.5f + 0.5f * position / 0.7f;
		else return 0.5f + 0.5f * (1.0f - position) / 0.3f;
	} else if (shape == 8) {
		return 0.0f;		// not implemented yet!!
	} else {
		return 0.0f;
	}
}


/**
 * Uniform乱数[0, 1)を生成する
 */
float PMTree::genRand() {
	return rand() / (float(RAND_MAX) + 1);
}

/**
 * meanを中心とし、varianceの幅でuniformに乱数を生成する。
 */
float PMTree::genRand(float mean, float variance) {
	return genRand() * variance * 2.0 + mean - variance;
	/*
	float m = mean;
	float s = sqrt(variance);

	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if (use_last) {
		y1 = y2;
		use_last = 0;
	} else {
		do {
			x1 = 2.0 * genRand(0.0f, 1.0f) - 1.0;
			x2 = 2.0 * genRand(0.0f, 1.0f) - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return m + y1 * s;
	*/
}

float PMTree::deg2rad(float deg) {
	return deg * M_PI / 180.0;
}
