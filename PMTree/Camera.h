#pragma once

#include "glew.h"

#include "qvector4d.h"
#include "qmatrix4x4.h"
//#include "global.h"
#include "Camera.h"
#include <QFile>

class Camera {
public:
	static enum { TYPE_2D = 0, TYPE_CAR };

public:
	float xrot;
	float yrot;
	float zrot;

	float dx;
	float dy;
	float dz;

	float lookAtX;
	float lookAtY;
	float lookAtZ;

	float fovy;

	QMatrix4x4 mvMatrix;
	QMatrix4x4 mvpMatrix;
	QMatrix4x4 pMatrix;
	QMatrix3x3 normalMatrix;

	int type;

public:
	Camera();
	QVector4D getCamPos();

	void updatePerspective(int width,int height);
	void updateCamMatrix();

	static void qNormalizeAngle(float &angle);
	void setRotation(float x, float y, float z);

	void setXRotation(float angle);
	void setYRotation(float angle);
	void setZRotation(float angle);

	void changeXRotation(float angle);
	void changeYRotation(float angle);
	void changeZRotation(float angle);
	void setTranslation(float x, float y, float z);
	void changeXYZTranslation(float x, float y, float z);
	void setLookAt(float x, float y, float z);
	void resetCamera();
};

