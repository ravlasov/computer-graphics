#include "glwidget.h"

using namespace std;
GLWidget::GLWidget(QWidget *parent):QOpenGLWidget(parent)
{

}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor (0.5, 0.5, 0.5, 1.0);
    glViewport(0,0,800,800);
}

void GLWidget::wheelEvent(QWheelEvent *event){
    if(event->delta() < 0){
        CamDist*=1.1;
    }
    else{
        CamDist*=0.9;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    CamHeight += -dy / 3;
    CamAzimuth += -dx / 3;
    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

double GLWidget::RadFromDeg(double value) {
    return value / 57.2957795;
}

void GLWidget::drawRect(float p1[3], float p2[3], float p3[3], float p4[3], float normal[3]) {
    glBegin(GL_POLYGON);
    glNormal3fv(normal);
    glVertex3fv(p1);
    glNormal3fv(normal);
    glVertex3fv(p2);
    glNormal3fv(normal);
    glVertex3fv(p3);
    glNormal3fv(normal);
    glVertex3fv(p4);
    glEnd();
}

void GLWidget::drawVerticalCylinder(float bottomPos[3], float bottomRad, float topRad, float height, int sections) {
    glPushMatrix();
    glTranslatef(bottomPos[0], bottomPos[1], bottomPos[2]);
    gluCylinder(gluNewQuadric(), bottomRad, topRad, height, sections, sections);
    gluDisk(gluNewQuadric(), 0, bottomRad, sections, 1);
    glPushMatrix();
    glTranslatef(0, 0, height);
    gluDisk(gluNewQuadric(), 0, topRad, sections, 1);
    glPopMatrix();
    glPopMatrix();
}

void GLWidget::renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
    float vx = x2-x1;
    float vy = y2-y1;
    float vz = z2-z1;

    //handle the degenerate case of z1 == z2 with an approximation
    if(std::abs(vz) <= 0.01f)
        vz = .01f;

    float v = sqrt( vx*vx + vy*vy + vz*vz );
    float ax = 57.2957795*acos( vz/v );
    if ( vz < 0.0f )
        ax = -ax;
    float rx = -vy*vz;
    float ry = vx*vz;
    glPushMatrix();

    //draw the cylinder body
    glTranslatef( x1,y1,z1 );
    glRotatef(ax, rx, ry, 0.0);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, v, subdivisions, 1);

    glPopMatrix();
}

void GLWidget::drawCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
GLUquadricObj *quadric=gluNewQuadric();
gluQuadricNormals(quadric, GLU_SMOOTH);
renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
gluDeleteQuadric(quadric);
}

void GLWidget::drawHorizontalTorus(float cx, float cy, float cz, float r, float torusR, int num_segments)
{
    float theta = 2.0f * 3.1415926f * float(0) / float(num_segments);
    float prevX = r * cosf(theta);
    float prevY = r * sinf(theta);
    theta = 2.0f * 3.1415926f;
    for(float ii = 1; ii <= num_segments; ii++)
    {
        theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        drawCylinder(prevX + cx, prevY + cy, cz, x + cx, y + cy, cz, torusR, num_segments);
        prevX = x;
        prevY = y;
    }
}

void GLWidget::drawColumns(float cx, float cy, float cz, float r, float coluntsRad, float columnsHeight, int num_segments) {
    for(float ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        float c[3] = {x + cx, y + cy, cz};
        drawVerticalCylinder(c, coluntsRad, coluntsRad, columnsHeight, num_segments);

    }
}

void GLWidget::drawFrame(float rad, float freq, float radius, int num_segments) {
    float prevX = rad;
    float x;
    for (float z = freq; z <= 7.5f; z += freq) {
        x = std::log(z + 0.5) + rad;
        drawCylinder(prevX, 0, z - freq, x, 0, z, radius, num_segments);
        prevX = x;
    }
    float c[3] = {x, 0, 7.5f};
    drawVerticalCylinder(c, radius, radius, 25.5f, num_segments);
    rad = prevX - std::pow(12, 1.0 / 1.8);
    for (float z = 33 + freq; z <= 39; z += freq) {
        x = std::pow(2*(39 - z), 1.0 / 1.8) + rad;
        drawCylinder(prevX, 0, z - freq, x, 0, z, radius, num_segments);
        prevX = x;
    }
    for (float z = 39 + freq; z <= 46; z += freq) {
        x = -std::pow(2*(z - 39), 1.0 / 1.8) + rad;
        drawCylinder(prevX, 0, z - freq, x, 0, z, radius, num_segments);
        prevX = x;
    }
    c[0] = 3.4f;
    c[2] = 48;
    drawVerticalCylinder(c, radius, radius, 5, num_segments);
}

void GLWidget::drawBottle(float pos[3], int num_segments) {
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);

    glColor3ub(127,255,0);
    // bottom
    float coords[3] = {0,0,0};
    drawVerticalCylinder(coords, 10, 10, 6, num_segments);
    // bottom torus 1
    drawHorizontalTorus(0, 0, 7.5f, 10, 0.2f, num_segments);
    // bottom torus 2
    drawHorizontalTorus(0, 0, 9, 10, 0.2f, num_segments);
    // bottom columns 1
    drawColumns(0, 0, 6, 10, 0.2f, 1.5, 2);
    // bottom columns 2
    drawColumns(0, 0, 7.5f, 10, 0.2f, 1.5, 8);

    // first platform
    coords[2] = 20;
    drawVerticalCylinder(coords, 10, 10, 0.25, num_segments);
    // first platform torus
    drawHorizontalTorus(0, 0, 21.5f, 10, 0.2f, num_segments);
    // first platform columns
    drawColumns(0, 0, 20, 10, 0.2f, 1.5, 8);

    // second platform
    coords[2] = 33;
    drawVerticalCylinder(coords, 10, 10, 0.25, num_segments);
    // second platform torus
    drawHorizontalTorus(0, 0, 34.5f, 10, 0.2f, num_segments);
    // second platform columns
    drawColumns(0, 0, 33, 10, 0.2f, 1.5, 8);

    // bottle cap bottom
    coords[2] = 53;
    drawVerticalCylinder(coords, 4, 3.5, 6.5, num_segments);

    // center cylinders
        // first
    float coords1[3] = {0, 3.5, 6};
    drawVerticalCylinder(coords1, 0.4f, 0.4f, 38, num_segments);
        // second
    coords1[1] *= -1;
    drawVerticalCylinder(coords1, 0.4f, 0.4f, 38, num_segments);

    for (int fr = 0; fr < 4; fr++) {
        glPushMatrix();
        glRotatef(90 * fr, 0, 0, 1);
        drawFrame(10, 0.1f, 0.2f, num_segments);
        glPopMatrix();
    }

    glColor3ub(25,25,25);
    // bottle label
    coords[2] = 43;
    drawVerticalCylinder(coords, 5, 5, 5, 32);

    glColor3ub(255,255,255);
    // bottle cap top
    coords[2] = 59.5;
    drawVerticalCylinder(coords, 3.5, 3.5, 2.5, 32);

    glPopMatrix();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static float no_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightModelf(GL_LIGHT_MODEL_AMBIENT, GL_TRUE);
    float AmbientColor[4] = { red, green, blue, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AmbientColor);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    switch (matID) {
         case 0:{
            float chrome_spec[] = {0.77f, 0.77f, 0.77f, 1.00};
            float chrome_diff[] = {0.40f, 0.40f, 0.40f, 1.00};
            float chrome_amb[] = {0.25, 0.25, 0.25, 1.00};
            float chrome_shininess[] = {100.0};
            glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
            glMaterialfv(GL_FRONT, GL_SPECULAR, chrome_spec);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, chrome_diff);
            glMaterialfv(GL_FRONT, GL_AMBIENT, chrome_amb);
            glMaterialfv(GL_FRONT, GL_SHININESS, chrome_shininess);
            break;
         }
     }
    switch(lightMod)
    {
        case 0:
        {
            GLfloat light0_direction[] = {20.0, 0.0, 10.0, 0.0};
            glEnable(GL_LIGHT0);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, AmbientColor);
            glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
            break;
        }

    }
    glLoadIdentity();
    gluLookAt(CamDist * cos(RadFromDeg(CamHeight)) * cos(RadFromDeg(CamAzimuth)),
               CamDist * cos(RadFromDeg(CamHeight)) * sin(RadFromDeg(CamAzimuth)),
       CamDist * sin(RadFromDeg(CamHeight)),
       0,
       0,
       0,
       0,0,1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_COLOR_MATERIAL);

    float pos[3] = {0, 0, -25};
    drawBottle(pos, 60);
    glPushMatrix();
    pos[2] *= -1;
    glRotatef(180, 1, 1, 0);
    drawBottle(pos, 60);
    glPopMatrix();

    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
    glDisable(GL_LIGHTING);

    glColor4ub(250, 250, 250, 128);
    // floor
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float p1[3] = {-80, -80, -25};
    float p2[3] = {-80, 80, -25};
    float p3[3] = {80, 80, -25};
    float p4[3] = {80, -80, -25};
    float n[3] = {80, -80, -25};
    drawRect(p1, p2, p3, p4, n);
    glDisable(GL_BLEND);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, static_cast<GLdouble>(w) / static_cast<GLdouble>(h),0.1,150.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
