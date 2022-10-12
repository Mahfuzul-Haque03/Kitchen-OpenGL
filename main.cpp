#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>

#include <iostream>

using namespace std;

double const pi = acos(-1), d=0.05;
double const sn = sin(pi/180), cs=cos(pi/180);
double Txval=0,Tyval=0,Tzval=0;
double windowHeight=1000, windowWidth=1000;
double wallLen=20, wallWidth=0.5, wallHeight = 10;
GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0, eye_x = wallLen/2, eye_y = wallHeight/2, eye_z = 1.5*wallLen, s=1;
GLfloat  look_x = wallLen/2, look_y = wallHeight/2, look_z = wallLen/2, dx, dy, dz, dw, upx=0, upy=1, upz=0, tx, ty, nearplane = 4;
GLboolean bRotate = false, uRotate = true;

int c; //coloring variable
double rackheight = 3.0;
int theta_x=0, theta_y=0, theta_z=0;
GLfloat sx, cx;

bool l0=0, l0a=1, l0s=1, l0d=1, l1=1, l1a=1, l1s=1, l1d=1;



static GLfloat colors[][3] =
{
    {0.8,0.22,0.59},//left and right wall
    {0.91,0.9,0.91},//front wall
    {0.8,0.54,0.22},//bottom wall- floor-2
    {0.87,0.61,0.8},//shelf-3
    {0.49,0.96,0.93},//window glass-4
    {0.53,0.49,0.22},//window wood-5
    {0.38039, 0.36078, 0.38431},// for box under self-6
    {0.5,0.45,0.44},// fridge box-7
    {0.74510, 0.72549, 0.71373},// fridge door-8
    {0.11,0.12,0.12},// fridge door handle-9
    {0.20000, 0.00000, 0.20000},// oven box-10
    {0.39,0.77,0.12},// oven glass-11
    {0.00000, 0.00000, 0.00000},// oven display-12
    {0.07,0.05,0.38},// stove box-13
    {0.7, 0.3, 0.0},// stove box-14
    {0.7, 0.3, 0.0},// stove box-15
    {0.75,0.06,0.07},// fan center-16
    {0.4,0.14,0.05},// fan blade-17
    {0.03,0.01,0},// table wood-18
    {0.76,0.65,0.26},// Kettle brass-19
    {1,1,1},// white-20
    {0.25,0.23,0.06},// light rope-21
    {0.04,0.04,0.01},// eye-22
    //{0.9,0.9,0.88},// body-23
    {0.14,0.29,0.56},// body-23
    {0.93,0.64,0.08},// nose-24
    {0.15,0.1,0.01},// hand-25

    {0,0,0}// black
};

void tric(double deg)
{
    deg = deg * pi / 180;
    sx=sin(deg);
    cx=cos(deg);
}

void wcsAlign()
{
    look_x=look_x-eye_x;
    look_y=look_y-eye_y;
    look_z=look_z-eye_z;

    tric(-theta_x);//X axix
    dy = look_y*cx-look_z*sx;
    dz = look_y*sx+look_z*cx;

    look_y=dy;
    look_z=dz;

    tric(-theta_y);//Y axix
    dx = look_x*cx+look_z*sx;
    dz = -look_x*sx+look_z*cx;

    look_x = dx;
    look_z = dz;

    tric(-theta_z);//Z axix

    dx = look_x*cx-look_y*sx;
    dy = look_x*sx+look_y*cx;

    look_x = dx;
    look_y = dy;

//    cout << "WCS:" << endl;
//    cout << look_x << " " << look_y << " " << look_z << endl;
}

void retransform()
{
    tric(theta_z);//Z axix

    dx = look_x*cx-look_y*sx;
    dy = look_x*sx+look_y*cx;

    look_x = dx;
    look_y = dy;

    tric(theta_y);//Y axix
    dx = look_x*cx+look_z*sx;
    dz = -look_x*sx+look_z*cx;

    look_x = dx;
    look_z = dz;

    tric(theta_x);//X axix
    dy = look_y*cx-look_z*sx;
    dz = look_y*sx+look_z*cx;

    look_y=dy;
    look_z=dz;


//    cout << "VCS:" << endl;
//    cout << look_x << " " << look_y << " " << look_z << endl;

    look_x=look_x+eye_x;
    look_y=look_y+eye_y;
    look_z=look_z+eye_z;

}

void matprop(GLfloat a0, GLfloat a1, GLfloat a2, GLfloat a3, GLfloat a4, GLfloat a5, GLfloat a6, GLfloat a7, GLfloat a8, GLfloat a9)
{
    //materialistic property
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { a0,a1,a2, 1.0 };
    GLfloat mat_diffuse[] = { a3,a4,a5, 1.0 };
    GLfloat mat_specular[] = { a6,a7,a8, 1.0 };
    GLfloat mat_shininess[] = {a9};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
}

void drawaxes()
{
    glPushMatrix();// axis drawing
        c = 9;

        glColor3f(colors[c][0],colors[c][1],colors[c][2]);
        glBegin(GL_LINES);
        glVertex3f(-wallLen,0,0);
        glVertex3f(wallLen, 0, 0);//x axis
        glEnd();

        c = 8;
        glColor3f(colors[c][0],colors[c][1],colors[c][2]);

        glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0, wallLen, 0);//y axis
        glEnd();

        c = 1;
        glColor3f(colors[c][0],colors[c][1],colors[c][2]);

        glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0, 0, wallLen);//z axis
        glEnd();


    glPopMatrix();

}

void drawwall()
{
    glPushMatrix();//left wall
        glTranslatef(0,wallHeight/2,wallLen/2);
        glScalef(wallWidth,wallHeight,wallLen);

        glutSolidCube(1.0);
    glPopMatrix();

}

void drawboundary()
{
    c = 0;
    matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);
     glPushMatrix();//left and right wall


        drawwall();

        glTranslatef(wallLen,0,0);
        drawwall();
    glPopMatrix();

    glPushMatrix();//front wall
        c = 1;
        glTranslatef(0,0,0.25);
        glRotatef(90, 0,1,0);
        drawwall();
    glPopMatrix();
    c = 2;
    matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

    glPushMatrix();//floor wall
        c = 2;
        glTranslatef(wallLen/2,0.25,wallLen/2);
        glScalef(wallLen,wallWidth,wallLen);

        glutSolidCube(1.0);
    glPopMatrix();

}


void drawshelf()
{
    glPushMatrix();//shelf left
        c = 3;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    90);


        glTranslatef(1.5, rackheight,wallLen/2);
        glScalef(3-d,0.3,wallLen-d);

        glutSolidCube(1.0);

    glPopMatrix();
    glPushMatrix();//shelf front
        c = 3;

        glTranslatef(wallLen/2, rackheight,1.5);
        glRotatef(90, 0,1,0);
        glScalef(3-d,0.3,wallLen-d);

        glutSolidCube(1.0);

    glPopMatrix();

}


void drawboxes()
{
    glPushMatrix();//left box under rack
        c = 6;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);


        glTranslatef(1.5,1.4,wallLen/2);
        glScalef(3-d, 2.8, wallLen-d);

        glutSolidCube(1.0);

    glPopMatrix();

    glPushMatrix();//front box under rack
        //c = 5;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);


        glTranslatef(wallLen/2-d,1.4,1.5);
        glRotated(90, 0, 1, 0);
        glScalef(3-d, 3, wallLen-d);

        glutSolidCube(1.0);

    glPopMatrix();


    // boxes above rack

    glPushMatrix();//left box above shelf
        c = 3;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);


        glTranslatef(1.5,7,wallLen/2);
        glScalef(2-d, 3.5, wallLen-d);

        glutSolidCube(1.0);

    glPopMatrix();

}


void drawwindow()
{

    glPushMatrix();//window glass
    c = 4;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

        glTranslatef(8,6,0.25);
        glScalef(4,4,0.7);

        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();//window vertical woods
    c = 5;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

        glTranslatef(10,6,0.25);
        glScalef(.2,4,0.8);

        glutSolidCube(1.0);

        glTranslatef(-6.67,0,0);
        glutSolidCube(1.0);

        glTranslatef(-6.67,0,0);
        glutSolidCube(1.0);

        glTranslatef(-6.67,0,0);
        glutSolidCube(1.0);

    glPopMatrix();

    glPushMatrix();//window vertical woods
    c = 5;
        glTranslatef(8,7.9,0.25);
        glScalef(4,.2,0.8);

        glutSolidCube(1.0);

        glTranslatef(0,-19,0);
        glutSolidCube(1.0);

    glPopMatrix();
}

void fridge()
{
    c = 7;
    matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

    glPushMatrix();//fridge box
        glTranslatef(8.9,3.5,5);
        glScalef(1.8,6.7,2.5);

        glutSolidCube(1.0);

    glPopMatrix();

    c = 8;
    matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

    glPushMatrix();//fridge door
        glTranslatef(7.9,3.7,5.63);
        glScalef(0.1,6.4,1.24);

        glutSolidCube(1.0);

        glTranslatef(0,0,-1.05);
        glutSolidCube(1.0);

    glPopMatrix();

    c = 9;
    matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

    glPushMatrix();//fridge door handle
        glTranslatef(7.9,3.7,5.25);
        glScalef(.3,4,.1);

        glutSolidCube(1.0);

        glTranslatef(0,0,-6);
        glutSolidCube(1.0);

    glPopMatrix();
}

void drawfridge()
{
    matprop(0.0215,0.0745,0.0215,   0.07568,0.01424,0.07568,    0.633,0.727811,0.633,    60);


    glPushMatrix();

    glTranslatef(11.7,0, 13);
    glScalef(0.8,0.9,0.9);
    fridge();
    glPopMatrix();
}

void drawsphere()
{

        glPushMatrix();
        c=21;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    100);

            glTranslatef(4.0, 9.2, 8.0);//light rope
            glScalef(0.15, 4, 0.15);
            glutSolidCube(0.5);
        glPopMatrix();

        glPushMatrix();
        c=20;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    100);

            glTranslatef(4.0, 8.0, 8.0);//light1
            glutSolidCube(0.5);
        glPopMatrix();

        glPushMatrix();
        c=20;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    100);

            glTranslatef(15.0, 8.0, 15.0);///light0
            //glutSolidCube(1);
        glPopMatrix();

}



void oven()
{
    glPushMatrix();// Oven box
        c = 10;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);


        glScalef(2.5, 1.5, 1.5);

        glutSolidCube(1.0);

    glPopMatrix();

    glPushMatrix();//oven glass
        c = 11;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

        glTranslatef(-0.3,0,0.75);
        glScalef(1.5, 1.2, 0.1);

        glutSolidCube(1.0);

    glPopMatrix();

    glPushMatrix();//oven handle
        c = 9;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

        glTranslatef(0.6,0,0.75);
        glScalef(0.15, 1, .3);

        glutSolidCube(1.0);

    glPopMatrix();

    glPushMatrix();//oven display
        c = 12;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);


        glTranslatef(1,0.3,0.75);
        glScalef(.4, .3, .1);

        glutSolidCube(1.0);

    glPopMatrix();

        glPushMatrix();//oven buttons
        c = 12;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);


        glTranslatef(0.9,0.0,0.75);
        glScalef(.06, .06, .1);

        glutSolidCube(1.0);

        glTranslatef(2,0,0);
        glutSolidCube(1.0);

        glTranslatef(2,0,0);
        glutSolidCube(1.0);

        glTranslatef(0,-2,0);
        glutSolidCube(1.0);

        glTranslatef(-2,0,0);
        glutSolidCube(1.0);

        glTranslatef(-2,0,0);
        glutSolidCube(1.0);

    glPopMatrix();
}

void drawoven()
{
    glPushMatrix();

        glTranslatef(1.6,3.85+d,5);
        oven();

    glPopMatrix();
}

void stove()
{
    glPushMatrix();// Oven box
        c = 13;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);


        glTranslatef(5,3.3,1.5);
        glScalef(1.8, 0.2, 1);

        glutSolidCube(1.0);

    glPopMatrix();

    glPushMatrix();// Oven knob
        c = 9;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);


        glTranslatef(4.5,3.3,2);
        glScalef(0.1, 0.02, 0.1);

        glutSolidCube(1.0);

        glTranslatef(10,0,0);
        glutSolidCube(1.0);

    glPopMatrix();//fire
        c = 14;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);


        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();

        glTranslatef(4.5,3.5,1.5);
        glRotatef(90.0f, 1, 0, 0);
        gluCylinder(quadratic,0.15,0.1,0.08,32,32);// obj, rad top, rad bottom, height, slice, stack
        gluDeleteQuadric(quadratic);

    glPopMatrix();

}

void drawstove()
{
    glPushMatrix();

        glTranslatef(0,-1.7,0);
        glScalef(1.5,1.5,1.1);
        stove();

    glPopMatrix();
}

void fan()
{
    glPushMatrix();//fan center
        c=16;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

        glutSolidSphere(.2, 30, 10);

    glPopMatrix();

    glPushMatrix();//fan blade1
        c=15;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

        glScalef(2, 0.4, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();//fan blade2
        c=15;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

        glScalef(0.4, 2, 0.1);
        glutSolidCube(1.0);
    glPopMatrix();
}

void drawFan()
{

    glPushMatrix();//fan box
        c=4;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

        glTranslatef(17, 8, 0.25);
        glScalef(2.5, 2.5, 0.6);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(17, 8, 0.6);
        glRotatef( theta,0, 0, 1.0 );
        fan();
    glPopMatrix();
}

void table()
{

    glPushMatrix();
        glScalef(6, 0.2, 4);
        glutSolidCube(1.0);

    glPopMatrix();

    glPushMatrix();
        glTranslatef(-2.5, -1.5, -1.5);
        glScalef(0.4, 3, 0.4);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-2.5, -1.5, 1.5);
        glScalef(0.4, 3, 0.4);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(2.5, -1.5, -1.5);
        glScalef(0.4, 3, 0.4);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(2.5, -1.5, 1.5);
        glScalef(0.4, 3, 0.4);
        glutSolidCube(1.0);
    glPopMatrix();
}

void drawtable()
{
     c=17;
    matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    50);

    glPushMatrix();
        glTranslatef(16,3.5,10);
        glRotatef(90, 0,1,0);
        table();
    glPopMatrix();
}

void drawkettle()
{
    c = 19;
    matprop(0.329412,0.223529,0.027451,   0.780392,0.568627,0.113725,    0.992157,0.941176,0.807843,    27.8974);

    glPushMatrix();
        glTranslatef(16, 4.0, 10);
        glutSolidTeapot(0.5);
    glPopMatrix();
}

void snowman()
{
        glPushMatrix();//body
        c=23;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    1,1,1,    80);

        glutSolidSphere(0.6, 30, 40);

        glTranslatef(0,-.9,0);
        glutSolidSphere(0.8, 30, 40);

        glPopMatrix();

        glPushMatrix();//eye
        c=22;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    60);

        glTranslatef(-.25,.25,.5);
        glutSolidSphere(0.1, 30, 40);

        glTranslatef(.5,0,0);
        glutSolidSphere(0.1, 30, 40);

        glPopMatrix();

        glPushMatrix();//nose
        c=24;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    100);

        glTranslatef(0,.1,.3);
        glutSolidCone(.2, 0.9, 100, 100);


        glPopMatrix();

        glPushMatrix();//hand
        c=25;
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    100);

        glTranslatef(-.9,-.2,0);
        glRotatef(50, 0,0,1);
        glScalef(.3,.8,.3);
        glutSolidCube(1);

        glPopMatrix();

        glPushMatrix();
        matprop(colors[c][0],colors[c][1],colors[c][2],   colors[c][0],colors[c][1],colors[c][2],    colors[c][0],colors[c][1],colors[c][2],    100);

        glTranslatef(.9,-.2,0);
        glRotatef(-50, 0,0,1);
        glScalef(.3,.8,.3);
        glutSolidCube(1);

        glPopMatrix();

}

void drawsnowman()
{
    glPushMatrix();
    glTranslatef(6,3,8);

    glRotatef(alpha, 0,1,0 );
    snowman();
    glPopMatrix();
}
void drawkitchen()
{

    drawaxes();
/**/
    drawsphere();
    drawboundary();
    drawshelf();
    drawboxes();
    drawwindow();
    drawfridge();
    drawoven();
    drawstove();
    drawFan();
    drawtable();
    drawkettle();
    drawsnowman();/*
    */

}

void light0()
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.6, 0.6, 0.6, 1.0};
    GLfloat light_diffuse[]  = {1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = {15.0, 8.0, 15.0, 1.0};

    if(l0) glEnable( GL_LIGHT0);

    if(l0a) glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    else glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);

    if(l0d) glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    else glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);

    if(l0s) glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    else glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);

    glLightfv( GL_LIGHT0, GL_POSITION, light_position);

  /*  GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 10.0); */
}

void light1()//spot light
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.8, 0.8, 0.8, 1.0};
    GLfloat light_diffuse[]  = {1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = {4.0, 8.0, 8.0, 1.0};

    if(l1) glEnable(GL_LIGHT1);

    if(l1a) glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient);
    else glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);

    if(l1d) glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    else glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);

    if(l1s) glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
    else glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);

    glLightfv( GL_LIGHT1, GL_POSITION, light_position);

    GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
}



void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5, 5, -5, 5, nearplane, 250);//left, right, bottom,top,
     //gluPerspective(60,1,5,100);
    glMatrixMode(GL_MODELVIEW);//select matrix for operation: projection, selection, model view
    glLoadIdentity();
    gluLookAt(eye_x,eye_y,eye_z, look_x,look_y,look_z, upx,upy,upz);//eye, ref, up

    glViewport(0, 0, windowHeight, windowWidth);

    // glRotatef(45, 0, 0, 1 );
    // triangle();
//        glRotatef( alpha,axis_x, axis_y, 0.0 );
//        glRotatef( theta, axis_x, axis_y, 0.0 );
    glScalef(s,s,s);
     drawkitchen();
   // glTranslatef(2,2,2);
     //drawkitchen();
 /*
    glPushMatrix();
    glTranslatef(0,0,Tzval);

    glRotatef( alpha,axis_x, axis_y, 0.0 );
    glRotatef( theta, axis_x, axis_y, 0.0 );
    drawkitchen();
    glPopMatrix();
*/
   /*
     glPushMatrix();
        glTranslatef(0,0,Tzval);
        glScalef(3,3,3);
        glutSolidCube(1);
    glPopMatrix();
  */
    glFlush();
    glutSwapBuffers();
}

/*

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 's':
    case 'S':
        bRotate = !bRotate;
        uRotate = false;
        axis_x=0.0;
        axis_y=1.0;
        break;

    case 'r':
    case 'R':
        uRotate = !uRotate;
        bRotate = false;
        axis_x=1.0;
        axis_y=0.0;
        break;
    case '+':
        Tzval+=0.2;
        break;

    case '-':
        Tzval-=0.2;
        break;

    case 27:	// Escape key
        exit(1);
    }
}
*/

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'w':
        dx=look_x-eye_x;
        dy=look_y-eye_y;
        dz=look_z-eye_z;
        dw = sqrt(dx*dx + dy*dy + dz*dz);

        eye_x += 0.5*dx/dw;
        eye_y += 0.5*dy/dw;
        eye_z += 0.5*dz/dw;

        look_x += 0.5*dx/dw;
        look_y += 0.5*dy/dw;
        look_z += 0.5*dz/dw;
//
//        eye_z -= 0.1;
//        look_z -= 0.1;
        break;
    case 's':
        dx=look_x-eye_x;
        dy=look_y-eye_y;
        dz=look_z-eye_z;
        dw = sqrt(dx*dx + dy*dy + dz*dz);

        eye_x -= 0.5*dx/dw;
        eye_y -= 0.5*dy/dw;
        eye_z -= 0.5*dz/dw;

        look_x -= 0.5*dx/dw;
        look_y -= 0.5*dy/dw;
        look_z -= 0.5*dz/dw;
        break;

    case 'd':
        eye_x += 0.5;
        look_x += 0.5;
        break;
    case 'a':
        eye_x -= 0.5;
        look_x-= 0.5;
        break;

    case 'q':
        eye_y += 0.5;
        look_y += 0.5;
        break;
    case 'z':
        eye_y -= 0.5;
        look_y-= 0.5;
        break;

    case '4'://yaw left
//        look_x=look_x-eye_x;
//        //look_y=look_y-eye_y;
//        look_z=look_z-eye_z;
//
//        dx = look_x*cs+look_z*sn;
//        dz = -look_x*sn+look_z*cs;
//
//        look_x = dx+eye_x;
//        //look_y = dy+eye_y;
//        look_z = dz+eye_z;theta_y += 1;

        wcsAlign();
        theta_y += 1;
        retransform();

        break;
    case '6':
//        look_x=look_x-eye_x;
//        //look_y=look_y-eye_y;
//        look_z=look_z-eye_z;
//
//        dx = look_x*cs-look_z*sn;
//        dz = look_x*sn+look_z*cs;
//
//        look_x = dx+eye_x;
//        //look_y = dy+eye_y;
//        look_z = dz+eye_z;
//
//        theta_y -= 1;
            wcsAlign();
            theta_y -= 1;
            retransform();

        break;
    case '8'://pitch left
//        //look_x=look_x-eye_x;
//        look_y=look_y-eye_y;
//        look_z=look_z-eye_z;
//
//        dy = look_y*cs-look_z*sn;
//        dz = look_y*sn+look_z*cs;
//
//        //look_x = dx+eye_x;
//        look_y = dy+eye_y;
//        look_z = dz+eye_z;
//
//        theta_x += 1;

        wcsAlign();
        theta_x += 1;
        retransform();

        break;
    case '2':
//        //look_x=look_x-eye_x;
//        look_y=look_y-eye_y;
//        look_z=look_z-eye_z;
//
//        dy = look_y*cs+look_z*sn;
//        dz = -look_y*sn+look_z*cs;
//
//        //look_x = dx+eye_x;
//        look_y = dy+eye_y;
//        look_z = dz+eye_z;
//
//        theta_x -= 1;

        wcsAlign();
        theta_x -= 1;
        retransform();
        break;

        case '7'://roll

        dx = upx*cs-upy*sn;
        dy = upx*sn+upy*cs;

        upx=dx;
        upy=dy;

        theta_z += 1;


        break;

        case '9'://roll
        dx = upx*cs+upy*sn;
        dy = -upx*sn+upy*cs;

        upx=dx;
        upy=dy;

        theta_z -= 1;

        break;

        case '+'://zoom in by lens
        nearplane+=0.1;

        break;
        case '-'://zoom in by lens
        nearplane-=0.1;

        break;

        case '*'://zoom in by scaling
        s+=.01;

        break;

        case '/'://zoom out by scaling
        s-=.01;

        break;

        case '1'://zoom out by scaling
        eye_x = 0;
        eye_y=0;
        eye_z=5;
        look_x=0;
        look_y=0;
        look_z=0;
        break;

        case 'l':
        if(l0==true){
            glDisable(GL_LIGHT0);
            l0=0;
        }
        else{
            glEnable( GL_LIGHT0 );
            l0=1;
        }
        break;

        case 'h':
        if(l0a==true){
            l0a=0;
            //light0();
        }
        else{
            l0a=1;
            //light0();
        }
        break;

        case 'j':
        if(l0d==true){
            l0d=0;
            //light0();
        }
        else{
            l0d=1;
            //light0();
        }
        break;

        case 'k':
        if(l0s==true){
            l0s=0;
            //light0();
        }
        else{
            l0s=1;
            //light0();
        }
        break;

        case ',':
        if(l1==true){
            glDisable(GL_LIGHT1);
            l1=0;
        }
        else{
            glEnable( GL_LIGHT1 );
            l1=1;
        }
        break;

        case 'b':
            l1a = !l1a;

        break;

        case 'n':
            l1d = !l1d;

        break;

        case 'm':
            l1s = !l1s;

        break;

        case 'r':
        bRotate = !bRotate;
        break;


    case 27:
        exit(1);
    }
//    cout << "Main:" << endl;
//    cout << look_x << " " << look_y << " " << look_z <<" " << eye_x << " " << eye_y << " " << eye_z << endl;

    theta_x %= 360;
    theta_y %= 360;
    theta_z %= 360;
    //cout << theta_x << " " << theta_y << " " << theta_z << endl;
}



void animate()
{

    light0();
    light1();

    if (bRotate == true)
    {
        theta += 10;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 1;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }


    glutPostRedisplay();

}

void printMenue()
{
    cout << "Basic Functions:" << endl;
    cout << "\t w : Move Forward" << endl;
    cout << "\t s : Move Backword" << endl;
    cout << "\t a : Move Left by X " << endl;
    cout << "\t d : Move Right by X" << endl;
    cout << "\t q : Move Up by Z" << endl;
    cout << "\t z : Move Up by Z" << endl;
    cout << "\t * : Zoom in by Scaling" << endl;
    cout << "\t / : Zoom out by Scaling" << endl;
    cout << "\t 8 : Pitch Up" << endl;
    cout << "\t 2 : Pitch Down" << endl;
    cout << "\t 4 : Yaw Left" << endl;
    cout << "\t 6 : Yaw Right" << endl;
    cout << "\t 7 : Roll Right" << endl;
    cout << "\t 9 : Roll Left" << endl;

    cout << "\nPoint Light Operation:" << endl;
    cout << "\t l : Point Light(Light 0) On/Off" << endl;
    cout << "\t h : Ambient Light On/Off" << endl;
    cout << "\t j : Diffusion Light On/Off" << endl;
    cout << "\t k : Specular Light On/Off" << endl;

    cout << "\nSpot Light Operation:" << endl;
    cout << "\t , : Point Light(Light 1) On/Off" << endl;
    cout << "\t b : Ambient Light On/Off" << endl;
    cout << "\t n : Diffusion Light On/Off" << endl;
    cout << "\t m : Specular Light On/Off" << endl;

    cout << "\nExhaust Fan Operation:" << endl;
    cout << "\t r : Exhaust Fan On/Off" << endl;
}

int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Kitchen Room");


    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    printMenue();

    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutMainLoop();
    return 0;
}




