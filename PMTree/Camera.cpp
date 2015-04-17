#include "Camera.h"

Camera::Camera() {
	xrot = 0.0f;//-75.0;
	yrot = 0.0;
	zrot = 0.0f;//-45.0;
	dx = 0.0;
	dy = 0.0;
	dz = 100.0;
	lookAtX = 0.0f;
	lookAtY = 0.0f;
	lookAtZ = 0.0f;
	fovy = 60.0f;
}

QVector4D Camera::getCamPos() {
	/*GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	QMatrix4x4 mvMat(m[0], m[1], m[2], m[3],
	m[4], m[5], m[6], m[7],
	m[8], m[9], m[10], m[11],
	m[12], m[13], m[14], m[15]);

	QVector4D eye(0.0f, 0.0f, 0.0f, 1.0f);
	return ((mvMat.transposed()).inverted())*eye;*/
	QVector4D eye(0.0f, 0.0f, 0.0f, 1.0f);
	return ((mvMatrix.transposed()).inverted())*eye;
}

void Camera::updatePerspective(int width,int height){
	float aspect=(float)width/(float)height;
	float zfar=30000.0f;//90000.0f;
	float znear=5.0f;

	float f = 1.0f / tan (fovy * (0.00872664625f));//PI/360

	double m[16]=
	{	 f/aspect,	0,								0,									0,
				0,	f,								0,						 			0,
			    0,	0,		(zfar+znear)/(znear-zfar),		(2.0f*zfar*znear)/(znear-zfar),
			    0,	0,		    				   -1,									0

	};
	pMatrix=QMatrix4x4(m);
}

void Camera::updateCamMatrix() {
	/*glLoadIdentity();
	glTranslatef(-dx, -dy, -dz);
	glRotatef(xrot, 1.0, 0.0, 0.0);		
	glRotatef(yrot, 0.0, 1.0, 0.0);
	glRotatef(zrot, 0.0, 0.0, 1.0);
	glTranslatef(-lookAtX, -lookAtY, -lookAtZ);*/
	// modelview matrix
	mvMatrix.setToIdentity();
	mvMatrix.translate(-dx, -dy, -dz);
	mvMatrix.rotate(xrot, 1.0, 0.0, 0.0);		
	mvMatrix.rotate(yrot, 0.0, 1.0, 0.0);
	mvMatrix.rotate(zrot, 0.0, 0.0, 1.0);
	mvMatrix.translate(-lookAtX, -lookAtY, -lookAtZ);
	// normal matrix
	normalMatrix=mvMatrix.normalMatrix();
	// mvp
	mvpMatrix=pMatrix*mvMatrix;
}

void Camera::qNormalizeAngle(float &angle) {
	while (angle < 0)
		angle += 360.0;
	while (angle >= 360.0)
		angle -= 360.0;
}

void Camera::setRotation(float x, float y, float z) {
	setXRotation(x);
	setYRotation(y);
	setZRotation(z);		
}

void Camera::setXRotation(float angle) {
	qNormalizeAngle(angle);
	xrot = angle;			
}

void Camera::setYRotation(float angle) {
	qNormalizeAngle(angle);
	yrot = angle;			
}

void Camera::setZRotation(float angle) {
	qNormalizeAngle(angle);
	zrot = angle;			
}

void Camera::changeXRotation(float angle) {
	setXRotation(xrot+angle);
}

void Camera::changeYRotation(float angle) {
	setYRotation(yrot+angle);
}

void Camera::changeZRotation(float angle) {
	setZRotation(zrot+angle);
}

void Camera::setTranslation(float x, float y, float z) {
	dx = x;
	dy = y;
	dz = z;
}

void Camera::changeXYZTranslation(float x, float y, float z) {
	dx += x;
	dy += y;
	dz += z;
	//printf("dx: %f,dy: %f, dz: %f\n",dx,dy,dz);
}

void Camera::setLookAt(float x, float y, float z) {
	lookAtX = x;
	lookAtY = y;
	lookAtZ = z;
}

void Camera::resetCamera() {
	setLookAt(0.0f, 0.0f, 0.0f);
	xrot = 0.0f;
	yrot = 0.0;
	zrot = 0.0f;
	setTranslation(0.0f, 0.0f, 100.0f);
}//
