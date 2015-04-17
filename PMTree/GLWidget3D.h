#pragma once

#include "glew.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include "Camera.h"
#include <QVector3D>
#include <vector>
#include "PMTree.h"
#include "VBORenderManager.h"

class MainWindow;

class GLWidget3D : public QGLWidget {
public:
	MainWindow* mainWin;
	VBORenderManager vboRenderManager;
	Camera camera;
	QPoint lastPos;
	PMTree tree;

public:
	GLWidget3D(MainWindow *parent);
	void drawScene();
	void updateCamera();

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();    
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

};

