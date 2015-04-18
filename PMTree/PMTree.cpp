#include "PMTree.h"
#include <QGLWidget>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <QDomDocument>

using namespace std;
using namespace glm;

#define M_PI	3.141592653589

PMTree::PMTree(VBORenderManager* rendManager) : rendManager(rendManager) {
	shape = 7;
	baseSize = 0.4;
	scale = 13.0;
	scaleV = 3.0;
	levels = 3;
	ratio = 0.015;
	ratioPower = 1.2;
	lobes = 5.0;
	lobeDepth = 0.07;
	flare = 0.6;

	nScale.resize(levels + 1);
	nScaleV.resize(levels + 1);
	nLength.resize(levels + 1);
	nLengthV.resize(levels + 1);
	nTaper.resize(levels + 1);
	nBaseSplits.resize(levels + 1);
	nSegSplits.resize(levels + 1);
	nSplitAngle.resize(levels + 1);
	nSplitAngleV.resize(levels + 1);
	nCurveRes.resize(levels + 1);
	nCurve.resize(levels + 1);
	nCurveBack.resize(levels + 1);
	nCurveV.resize(levels + 1);
	nDownAngle.resize(levels + 1);
	nDownAngleV.resize(levels + 1);
	nRotate.resize(levels + 1);
	nRotateV.resize(levels + 1);
	nBranches.resize(levels + 1);


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

	nDownAngle[3] = 45.0;
	nDownAngleV[3] = 10.0;
	nRotate[3] = 77.0;
	nRotateV[3] = 0.0;
	nBranches[3] = 10.0;
	nLength[3] = 0.0;
	nLengthV[3] = 0.0;
	nTaper[3] = 1.0;
	nSegSplits[3] = 0;
	nSplitAngle[3] = 0.0;
	nSplitAngleV[3] = 0.0;
	nCurveRes[3] = 1;
	nCurve[3] = 0;
	nCurveBack[3] = 0;
	nCurveV[3] = 0;

	leaves = 25;
	leafShape = 0;
	leafScale = 0.17;
	leafScaleX = 1;
	attractionUp = 0.5;
	pruneRatio = 0;
	pruneWidth = 0.5;
	pruneWidthPeak = 0.5;
	prunePowerLow = 0.5;
	prunePowerHigh = 0.5;

	colorStem = QColor(30, 162, 0);
	colorLeave = QColor(100, 255, 40);

	//generate();
}

/**
 * XMLファイルから、treeのパラメータを読み込み、モデルを生成する。
 *
 * @param filename		treeのXMLファイル
 */
void PMTree::load(const QString& filename) {
	QFile file(filename);

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();
 
    QDomNode species = root.firstChild();
	QDomNode param = species.firstChild();

	while (!param.isNull()) {
		if (param.nodeName() != "param") {
			param = param.nextSibling();
			continue;
		}

		QString name = param.toElement().attribute("name");
		float value = param.toElement().attribute("value").toFloat();

		if (name == "Shape") {
			shape = value;
		} else if (name == "Levels") {
			levels = value;

			// 配列のサイズを確保
			nScale.resize(levels + 1, 0);
			nScaleV.resize(levels + 1, 0);
			nLength.resize(levels + 1, 0);
			nLengthV.resize(levels + 1, 0);
			nTaper.resize(levels + 1, 0);
			nBaseSplits.resize(levels + 1, 0);
			nSegSplits.resize(levels + 1, 0);
			nSplitAngle.resize(levels + 1, 0);
			nSplitAngleV.resize(levels + 1, 0);
			nCurveRes.resize(levels + 1, 0);
			nCurve.resize(levels + 1, 0);
			nCurveBack.resize(levels + 1, 0);
			nCurveV.resize(levels + 1, 0);
			nDownAngle.resize(levels + 1, 0);
			nDownAngleV.resize(levels + 1, 0);
			nRotate.resize(levels + 1, 0);
			nRotateV.resize(levels + 1, 0);
			nBranches.resize(levels + 1, 0);
		} else if (name == "Scale") {
			scale = value;
		} else if (name == "ScaleV") {
			scaleV = value;
		} else if (name == "BaseSize") {
			baseSize = value;
		} else if (name == "Ratio") {
			ratio = value;
		} else if (name == "RatioPower") {
			ratioPower = value;
		} else if (name == "Flare") {
			flare = value;
		} else if (name == "Lobes") {
			lobes = value;
		} else if (name == "LobeDepth") {
			lobeDepth = value;
		} else if (name == "Leaves") {
			leaves = value;
		} else if (name == "LeafShape") {
			leafShape = value;
		} else if (name == "LeafScale") {
			leafScale= value;
		} else if (name == "LeafScaleX") {
			leafScaleX = value;
		} else if (name == "LeafQuality") {
			leafQuality = value;
		} else if (name == "AttractionUp") {
			attractionUp = value;
		} else if (name == "PruneRatio") {
			pruneRatio = value;
		} else if (name == "PrunePowerLow") {
			prunePowerLow = value;
		} else if (name == "PrunePowerHigh") {
			prunePowerHigh = value;
		} else if (name == "PruneWidth") {
			pruneWidth = value;
		} else if (name == "PruneWidthPeak") {
			pruneWidthPeak = value;
		} else if (name.mid(1) == "Scale") {
			int level = name.mid(0, 1).toInt();
			nScale[level] = value;
		} else if (name.mid(1) == "ScaleV") {
			int level = name.mid(0, 1).toInt();
			nScaleV[level] = value;
		} else if (name.mid(1) == "BaseSplits") {
			int level = name.mid(0, 1).toInt();
			nBaseSplits[level] = value;
		} else if (name.mid(1) == "DownAngle") {
			int level = name.mid(0, 1).toInt();
			nDownAngle[level] = value;
		} else if (name.mid(1) == "DownAngleV") {
			int level = name.mid(0, 1).toInt();
			nDownAngleV[level] = value;
		} else if (name.mid(1) == "Rotate") {
			int level = name.mid(0, 1).toInt();
			nRotate[level] = value;
		} else if (name.mid(1) == "RotateV") {
			int level = name.mid(0, 1).toInt();
			nRotateV[level] = value;
		} else if (name.mid(1) == "Branches") {
			int level = name.mid(0, 1).toInt();
			nBranches[level] = value;
		} else if (name.mid(1) == "Length") {
			int level = name.mid(0, 1).toInt();
			nLength[level] = value;
		} else if (name.mid(1) == "LengthV") {
			int level = name.mid(0, 1).toInt();
			nLengthV[level] = value;
		} else if (name.mid(1) == "Taper") {
			int level = name.mid(0, 1).toInt();
			nTaper[level] = value;
		} else if (name.mid(1) == "SegSplits") {
			int level = name.mid(0, 1).toInt();
			nSegSplits[level] = value;
		} else if (name.mid(1) == "SplitAngle") {
			int level = name.mid(0, 1).toInt();
			nSplitAngle[level] = value;
		} else if (name.mid(1) == "SplitAngleV") {
			int level = name.mid(0, 1).toInt();
			nSplitAngleV[level] = value;
		} else if (name.mid(1) == "CurveRes") {
			int level = name.mid(0, 1).toInt();
			nCurveRes[level] = value;
		} else if (name.mid(1) == "Curve") {
			int level = name.mid(0, 1).toInt();
			nCurve[level] = value;
		} else if (name.mid(1) == "CurveBack") {
			int level = name.mid(0, 1).toInt();
			nCurveBack[level] = value;
		} else if (name.mid(1) == "CurveV") {
			int level = name.mid(0, 1).toInt();
			nCurveV[level] = value;
		}

		param = param.nextSibling();
	}

	generate();
}

/**
 * パラメータに基づいて、treeモデルを生成する。
 */
void PMTree::generate() {
	rendManager->removeStaticGeometry("tree");

	//scale_tree = genRand(scale, scaleV);
	scale_tree = genRand(scale, 0.0f);
	float length0 = genRand(nLength[0], nLengthV[0]) * scale_tree;
	float radius0 = length0 * ratio * genRand(nScale[0], nScaleV[0]);

	length_base = baseSize * scale_tree;

	mat4 modelMat;
	generateStem(0, modelMat, radius0, length0);

	cout << "generation done." << endl;
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
	for (int i = 0; i < nCurveRes[level]; ++i) {
		float segment_length = length / nCurveRes[level];

		float r1 = computeRadius(nTaper[level], radius, length, (float)i / nCurveRes[level]);
		float r2 = computeRadius(nTaper[level], radius, length, (float)(i + 1) / nCurveRes[level]);
		
		float rot = generateSegment(level, i, modelMat, r1, r2, length, segment_length);

		// Z軸方向に移動
		modelMat = translate(modelMat, vec3(0, 0, segment_length));

		// Z軸周りに回転
		modelMat = rotate(modelMat, rot, vec3(0, 0, 1));

		// X軸周りに回転
		if (nCurveBack[level] == 0.0f) {
			modelMat = rotate(modelMat, deg2rad(genRand(nCurve[level] / nCurveRes[level], nCurveV[level] / nCurveRes[level])), vec3(1, 0, 0));
		} else {
			if (i < nCurveRes[level] / 2) {
				modelMat = rotate(modelMat, deg2rad(genRand(nCurve[level] / nCurveRes[level] * 2.0f, nCurveV[level] / nCurveRes[level] * 2.0f)), vec3(1, 0, 0));
			} else {
				modelMat = rotate(modelMat, deg2rad(genRand(nCurveBack[level] / nCurveRes[level] * 2.0f, nCurveV[level] / nCurveRes[level] * 2.0f)), vec3(1, 0, 0));
			}
		}
	}
}

/**
 * Stemを構成する各segmentを生成する。
 *
 * @param level				レベル
 * @param index				インデックス
 * @param modelMat			モデル行列
 * @param radius1
 * @param radius2
 * @param stem_length
 * @param segment_length	
 * @return					Z軸まわりに回転した角度
 */
float PMTree::generateSegment(int level, int index, mat4 modelMat, float radius1, float radius2, float stem_length, float segment_length) {
	// 各segmentを、25cmで区切ってcylinderとして描画する
	int nstacks = ceil(segment_length / 0.25f);
	for (int i = 0; i < nstacks; ++i) {
		float r1 = (radius2 - radius1) / (float)nstacks * (float)i + radius1;
		float r2 = (radius2 - radius1) / (float)nstacks * (float)(i + 1) + radius1;
		if (level == 0) {
			float Z1 = ((float)index * segment_length + segment_length * (float)i / (float)nstacks) / stem_length;
			r1 *= computeFlare(Z1);
			float Z2 = ((float)index * segment_length + segment_length * (float)(i + 1) / (float)nstacks) / stem_length;
			r2 *= computeFlare(Z2);
		}
		rendManager->addCylinder("tree", translate(modelMat, vec3(0, 0, segment_length / (float)nstacks * i)), r1, r2, segment_length / (float)nstacks, colorStem, 12, 10);
	}

	if (level >= levels - 1) {
		// 葉を生成する
		float offset_child = segment_length * index; 
		int leaves_per_branch = leaves * shapeRatio(4, offset_child / stem_length) * leafQuality;
		
		float interval = segment_length / (leaves_per_branch + 1); // 葉の間の距離

		return generateLeaves(level + 1, modelMat, leaves_per_branch, interval);
	}

	float substem_z = 0.0f;	// substemの位置
		
	if (level == 0) {
		if (segment_length * (index + 1) < length_base) {
			// segmentが全てbaseの一部なので、substemは生成しない
			return 0.0f;
		} else if (segment_length * index >= length_base) {
			// segmentが、完全にbaseの外なので、一番下からsubstemを生成出来る
		} else {
			substem_z = length_base - segment_length * (int)(length_base / segment_length);
		}
	}

	float subst_per_segm = nBranches[level + 1] / nCurveRes[level];
	int substems_eff = subst_per_segm / segment_length * (segment_length - substem_z) + 0.5f; // substemの数
	float dist = (segment_length - substem_z) / (substems_eff + 1);	// substem間の距離

	// z軸まわりに移動
	substem_z += dist;
	modelMat = translate(modelMat, vec3(0, 0, substem_z));

	float total_rot = 0.0f;

	for (int s = 0; s < substems_eff; ++s) {
		float offset_child = segment_length * index + substem_z;

		// x軸まわりに回転
		mat4 modelMat2 = rotate(modelMat, deg2rad(genRand(nDownAngle[level + 1], nDownAngleV[level + 1])), vec3(1, 0, 0));

		float length_max = genRand(nLength[level + 1], nLengthV[level + 1]);
		float length_child;
		if (level == 0) {
			length_child = stem_length * length_max * shapeRatio(shape, (stem_length - offset_child) / (stem_length - length_base));
		} else {
			length_child = length_max * (stem_length - 0.6f * offset_child);
		}
		float radius_child = radius1 * (length_child / stem_length);

		generateStem(level + 1, modelMat2, radius_child, length_child);

		// z軸まわりに回転
		float rot = deg2rad(genRand(nRotate[level + 1], nRotate[level + 1]));
		modelMat = rotate(modelMat, rot, vec3(0, 0, 1));
		total_rot += rot;

		// z軸まわりに移動
		modelMat = translate(modelMat, vec3(0, 0, dist));
		substem_z += dist;
	}

	return total_rot;
}

/**
 * 葉を生成する。
 *
 * @param level					レベル
 * @param modelMat				モデル行列
 * @param leaves_per_branch		生成する葉の数
 * @param interval				生成する葉の間隔
 * @return						Z軸周りに、回転した角度
 */
float PMTree::generateLeaves(int level, mat4 modelMat, int leaves_per_branch, float interval) {
	float total_rot = 0.0f;

	// z軸まわりに移動
	float z = interval;
	modelMat = translate(modelMat, vec3(0, 0, z));

	for (int s = 0; s < leaves_per_branch; ++s) {
		// x軸まわりに回転
		mat4 modelMat2 = rotate(modelMat, deg2rad(genRand(nDownAngle[level], nDownAngleV[level])), vec3(1, 0, 0));

		float length = leafScale / sqrtf(leafQuality);
		float width = leafScale * leafScaleX / sqrtf(leafQuality);
		
		// 少しZ軸方向に移動
		modelMat2 = translate(modelMat2, vec3(0, 0, length));

		// X軸まわりに90度回転
		modelMat2 = rotate(modelMat2, (float)M_PI * 0.5f, vec3(1, 0, 0));

		// 葉を描画
		rendManager->addCircle("tree", modelMat2, width, length, colorLeave, 12);

		// z軸まわりに回転
		float rot = deg2rad(genRand(nRotate[level], nRotate[level]));
		modelMat = rotate(modelMat, rot, vec3(0, 0, 1));
		total_rot += rot;

		// z軸方向に移動
		modelMat = translate(modelMat, vec3(0, 0, interval));
		z += interval;
	}

	return total_rot;
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

float PMTree::computeFlare(float Z) {
	float y = max(0.0f, 1.0f - 8.0 * Z);
	return flare * (pow(100.0f, y) - 1.0f) / 100.0f + 1.0f;
}

float PMTree::computeRadius(float nTaper, float radius, float length, float Z) {
	float unit_taper;
	if (nTaper < 1) {
		unit_taper = nTaper;
	} else if (nTaper < 2) {
		unit_taper = 2.0 - nTaper;
	} else {
		unit_taper = 0.0;
	}

	float taper_z = radius * (1.0 - unit_taper * Z);

	if (nTaper < 1) {
		return taper_z;
	} else {
		float Z2 = (1.0f - Z) * length;
		
		float depth;
		if (nTaper < 2 || Z2 < taper_z) {
			depth = 1.0f;
		} else {
			depth = nTaper - 2.0f;
		}

		float Z3;
		if (nTaper < 2) {
			Z3 = Z2;
		} else {
			Z3 = fabs(Z2 - 2.0f * taper_z * (int)(Z2 / (2.0f * taper_z) + 0.5));
		}

		if (nTaper < 2 && Z3 >= taper_z) {
			return taper_z;
		} else {
			return (1.0f - depth) * taper_z + depth * sqrtf(taper_z * taper_z - (Z3 - taper_z) * (Z3 - taper_z));
		}
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
}

/**
 * degreeをradianに変換する。
 *
 * @param deg		degree
 * @return			radian
 */
float PMTree::deg2rad(float deg) {
	return deg * M_PI / 180.0;
}
