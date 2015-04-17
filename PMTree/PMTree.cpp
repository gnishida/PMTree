#include "PMTree.h"
#include <QGLWidget>

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
	nDownAngleV[1] = -50.0;
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
	nCurve[1] = -40;
	nCurveBack[1] = 0;
	nCurveV[1] = 50;

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

void PMTree::draw() {
	float length0 = genRand(scale, scaleV) * genRand(nLength[0], nLengthV[0]);
	float radius0 = length0 * ratio * genRand(nScale[0], nScaleV[0]);

	glPushMatrix();
	for (int i = 0; i < nCurveRes[0]; ++i) {
		float unit_taper;
		if (nTaper[0] < 1) {
			unit_taper = nTaper[0];
		} else if (nTaper[0] < 2) {
			unit_taper = 2.0 - nTaper[0];
		} else {
			unit_taper = 0.0;
		}

		float taper_z1 = radius0 * (1.0 - unit_taper * (float)i / nCurveRes[0]);
		float taper_z2 = radius0 * (1.0 - unit_taper * (float)(i + 1) / nCurveRes[0]);

		float r1 = taper_z1;
		float r2 = taper_z2;

		drawCone(r1, r2, length0 / nCurveRes[0], QColor(i * 100, 0, 255));

		for (int split = 0; split < nBaseSplits[0]; ++split) {
		}


		glTranslatef(0, 0, length0 / nCurveRes[0]);
		glRotatef(genRand(nCurve[0], nCurveV[0]), 1, 0, 0);
	}
	glPopMatrix();
}

void PMTree::drawCone(float baseRadius, float topRadius, float height, const QColor& color) {
	int slices = 10;
	int stacks = 10;

	glColor3f(color.redF(), color.greenF(), color.blueF());
	glBegin(GL_TRIANGLES);
	for (int st = 0; st < stacks; ++st) {
		float z1 = height / stacks * st;
		float z2 = height / stacks * (st + 1);
		float r1 = (topRadius - baseRadius) / stacks * st + baseRadius;
		float r2 = (topRadius - baseRadius) / stacks * (st + 1) + baseRadius;

		for (int sl = 0; sl < slices; ++sl) {
			float theta1 = 2 * M_PI * (float)sl / slices;
			float theta2 = 2 * M_PI * (float)(sl + 1) / slices;

			glNormal3f(cosf(theta1), sinf(theta1), 0);
			glVertex3f(cosf(theta1) * r1, sinf(theta1) * r1, z1);
			glNormal3f(cosf(theta2), sinf(theta2), 0);
			glVertex3f(cosf(theta2) * r1, sinf(theta2) * r1, z1);
			glNormal3f(cosf(theta2), sinf(theta2), 0);
			glVertex3f(cosf(theta2) * r2, sinf(theta2) * r2, z2);

			glNormal3f(cosf(theta1), sinf(theta1), 0);
			glVertex3f(cosf(theta1) * r1, sinf(theta1) * r1, z1);
			glNormal3f(cosf(theta2), sinf(theta2), 0);
			glVertex3f(cosf(theta2) * r2, sinf(theta2) * r2, z2);
			glNormal3f(cosf(theta1), sinf(theta1), 0);
			glVertex3f(cosf(theta1) * r2, sinf(theta1) * r2, z2);
		}
	}
	glEnd();
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
	return mean + variance * 0.3;
	//return genRand() * variance * 2.0 + mean - variance;
}
