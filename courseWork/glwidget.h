#ifndef QGLWIDGET_H
#define QGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include "mainwindow.h"
#include "include/glm/vec3.hpp"
#include "include/glm/glm.hpp"
#include <math.h>
#include <QGraphicsSceneEvent>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include <QGenericMatrix>
#include <QMouseEvent>
#include <QWheelEvent>


    class GLWidget : public QOpenGLWidget, public QOpenGLExtraFunctions //,public QOpenGLFunctions
    {
        Q_OBJECT

    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;

        void initializeGL() Q_DECL_OVERRIDE;
        void resizeGL(int w, int h)Q_DECL_OVERRIDE;
        void paintGL()Q_DECL_OVERRIDE;

    private:
        void drawRect(float p1[3], float p2[3], float p3[3], float p4[3], float normal[3]);
        void drawVerticalCylinder(float bottomPos[3], float bottomRad, float topRad, float height, int sections);
        void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric);
        void drawCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions);
        void drawHorizontalTorus(float cx, float cy, float cz, float r, float torusR, int num_segments);
        void drawColumns(float cx, float cy, float cz, float r, float coluntsRad, float columnsHeight, int num_segments);
        void drawFrame(float rad, float freq, float radius, int num_segments);
        void drawBottle(float pos[3], int num_segments);

    public:
        explicit GLWidget(QWidget *parent = nullptr);
        double RadFromDeg(double value);

        QPoint lastPos;
        int xRot;
        int yRot;
        int zRot;
        double CamAzimuth = 270;
        double CamHeight = 30; // угол над горизонтом
        double CamDist = 42.35;
        double count = 5;
        float lightIntensity=0.3f;
        float red = 0.5, green = 0.5, blue = 0.5;
        int lightMod = 0;
        int matID = 0;
        GLfloat angleX=90.0f,angleY=0.0f,angleZ=0.0f, vertexes[4][3], normal[3],zoom=-40.0f;

    };
#endif // QGLWIDGET_H
