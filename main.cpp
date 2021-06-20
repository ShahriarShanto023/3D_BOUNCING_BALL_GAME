#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<mmsystem.h>
#include <math.h>
#include <cmath>
#include <bits/stdc++.h>
#include "include/BmpLoader.h"
#include<windows.h>
#define PI 3.4159265
#define rad (3.1416/180)
#define EN_SIZE 20

using namespace std;
static int slices = 16;
static int stacks = 16;
void *currentfont;
int score=0;
GLuint tex;
GLUquadric* sphere;
float xEye=0.0f,yEye=5.0f,zEye=30.0f;
float cenX=0,cenY=0,cenZ=0,roll=0;
float zoom=4;
float radius=0;
float theta=0,slope=0;
float speed = 0.3;
float angleBackFrac = 0.2;
float door_angle=.5;
int tola[5000][5000];
float tX=0,tY=-2,tZ=-8,rX=0,rY=0,rZ=4;
float tZ1=-20,tZ2=-40,tZ3=-60,tZ4=-80,tZ5=-100,tZ6=-120;
float rotX=0,rotY=0,rotZ=0;
float cosX=0,cosY=1,cosZ=0;
float angle=0;
int TIME=0;
bool START = false;
float torusPosX[7] = {1,-2,3,-4,-3,0,2};
float torusPosY[7] = {2,3,10,6,7,4,1};
bool Over = false;

bool rot = false;


unsigned int ID;

bool normalLight=false;
bool light0 = true;
bool no_ambient = false;
bool no_diffuse = false;
bool no_specular = false;
bool emmission_on = true;


int a1=0,a2=0,a3=0,xo=0,yo=0,zo=0;

float ze=5;
int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int window;
int wired=0;
int shcpt=1;
int animat = 0;
const int L=20;
const int dgre=3;
int ncpt=L+1;
int clikd=0;
const int nt = 40;				//number of slices along x-direction
const int ntheta = 20;
void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back=0, float ambFactor=1.0, float specFactor=1.0);

GLfloat ctrlpoints[L+1][3] =
{

    {3.83, 0.25, 0.0},{4, 0.43, 0.0},
    {4.4,0.73, 0.0},{4.73, 1.2, 0.0},
    {5, 1.5, 0.0},{5.2, 2, 0.0},
    {5, 2.63, 0.0},{4.55, 2.88, 0.0},
    {4.4, 3.1, 0.0},{4, 2.9, 0.0},
    {3.65, 2.7, 0.0},
    {3.3, 2.5, 0.0},{2.9, 2.25, 0.0},
    {2.5, 2, 0.0},{1.8, 1.6, 0.0},
    {1.1, 1.1, 0.0},{0.83, 0.83, 0.0},
    { 0.7, 0.7, 0.0},{ 0.5, 0.5, 0.0},
    { 0.4, 0.35, 0.0},{ 0.07, -0.03, 0.0}

};
float wcsClkDn[3],wcsClkUp[3];
void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back, float ambFactor, float specFactor)
{

    const GLfloat mat_ambient[]    = { kdr*ambFactor, kdg*ambFactor, kdb*ambFactor, 1.0f };
    const GLfloat mat_diffuse[]    = { kdr, kdg, kdb, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f*specFactor, 1.0f*specFactor, 1.0f*specFactor, 1.0f };
    const GLfloat high_shininess[] = { shiny };
    if(frnt_Back==0)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    }
    else if(frnt_Back==1)
    {
        glMaterialfv(GL_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_BACK, GL_SHININESS, high_shininess);
    }
    else if(frnt_Back==2)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
    }

}


///////////////////////////////
class point1
{
public:
    point1()
    {
        x=0;
        y=0;
    }
    int x;
    int y;
} clkpt[2];
int flag=0;

static GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},

    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

static GLubyte c_ind[6][4] =
{
    {3,1,5,7},
    {2,0,1,3},
    {7,5,4,6},
    {2,3,7,6},
    {1,0,4,5},
    {6,4,0,2}
};

static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1,
                        GLfloat x2, GLfloat y2, GLfloat z2,
                        GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void cube(float colR, float colG, float colB,int n=1)
{
    //set_mat_prop(colR,colG,colB,em,shine);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(n,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(n,n);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);
        glTexCoord2f(0,n);
    }
    glEnd();
}

void Cube(bool road)
{
    glColor3f(1,1,1);
    //GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        //glColor3f(colors[4][0],colors[4][1],colors[4][2]);
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);
        if(road)
        {
            glVertex3fv(&v_cube[c_ind[i][0]][0]);
            glTexCoord2f(10,10);
            glVertex3fv(&v_cube[c_ind[i][1]][0]);
            glTexCoord2f(10,0);
            glVertex3fv(&v_cube[c_ind[i][2]][0]);
            glTexCoord2f(0,0);
            glVertex3fv(&v_cube[c_ind[i][3]][0]);
            glTexCoord2f(0,10);
        }
        else
        {
            glVertex3fv(&v_cube[c_ind[i][0]][0]);
            glTexCoord2f(0,20);
            glVertex3fv(&v_cube[c_ind[i][1]][0]);
            glTexCoord2f(1,0);
            glVertex3fv(&v_cube[c_ind[i][2]][0]);
            glTexCoord2f(0,0);
            glVertex3fv(&v_cube[c_ind[i][3]][0]);
            glTexCoord2f(0,1);
        }

    }
    glEnd();
    //glutSolidSphere (3.0, 20, 16);

}


long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}
void BezierCurve ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}

///////////////////////
void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(-Nx,-Ny,-Nz);
}

void light()
{
    //light
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position0[] = {-4,4.53,-5};

    //GLfloat light_position1[] = {-(roomWidth/2-walldepth-0.25),3*(roomHeight/4),-4, 1.0 };

    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    //glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    // spot light
    /* GLfloat spot_direction[] = { -(roomWidth/2-walldepth-wearDropLength/2),-wearDropHeight-3,-(bedLength-roomLength/2+walldepth+1+wearDropWidth/2) };
     glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
     glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 90.0);*/

    if(light0)
    {
        if(no_ambient)
        {
            glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
        }
        else
        {
            glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
        }
        if(no_diffuse)
        {
            glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
        }
        else
        {
            glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        }
        if(no_specular)
        {
            glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
        }
        else
        {
            glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
        }
    }
    else
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
    }
}


void showControlPoints()
{
    glPointSize(5.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i <=L; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
}

void li8Src()
{

    glColor3f(1,0.7,0);

    glutSolidCube(0.3);

}
void hatBezier()
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;

    const float startx = 0, endx = ctrlpoints[L][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i

}


void curve(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 5000.0;
    const double a = t*90.0;

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(wired)
    {
        glPolygonMode( GL_FRONT, GL_LINE ) ;
        glPolygonMode( GL_BACK, GL_LINE ) ;

    }
    else
    {
        glPolygonMode( GL_FRONT,GL_FILL ) ;
        glPolygonMode( GL_BACK, GL_FILL ) ;
    }

    glPushMatrix();

    if(animat)
        glRotated(a,0,0,1);

    glRotatef( anglex, 1.0, 0.0, 0.0);
    glRotatef( angley, 0.0, 1.0, 0.0);         	//rotate about y-axis
    glRotatef( anglez, 0.0, 0.0, 1.0);

    glRotatef( 90, 0.0, 0.0, 1.0);
    glTranslated(-3.5,0,0);
    //glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info

    // void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back=0, float ambFactor=1.0, float specFactor=1.0);

    //  matColor(0.9,0.5,0.1,20);   // front face color
    // matColor(0.0,0.5,0.8,20,1);  // back face color

    glPushMatrix();



    // glRotatef(90,0,1,0);
    // glScalef(0.5,1,0.5);
    hatBezier();

    glPopMatrix();






    if(shcpt)
    {
        matColor(0.0,0.0,0.0,20);
        //showControlPoints();
    }

    glPopMatrix();




}


void scence()
{

    light();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);

    glPushMatrix();
//    glRotatef(90, 0,0,1);
    glTranslatef(-0.5,-2,0);
    glScalef(1,10,1);
    cube(1,1,1,1);
//    glScalef(0.4,1,1.4);
//    cylinder(1.5,1.5,4,10,1,1,1,false,128,true);
    glPopMatrix();
    glPushMatrix();
//    glRotatef(90, 0,0,1);
    glTranslatef(-0.7,-2,0);
    glScalef(1,10,1);
    cube(1,1,1,1);
//    glScalef(0.4,1,1.4);
//    cylinder(1.5,1.5,4,10,1,1,1,false,128,true);
    glPopMatrix();



    glDisable(GL_TEXTURE_2D);


}


void Baby_Doll()
{


    glPushMatrix();
//cloth
    glPushMatrix();
    glTranslated(0,-1.6,-6);
    glRotatef(-90,0.1,0,0);
    glColor3d(1, 0.373, 0);
    glScaled(1.5,1.7,2);
    glutSolidCone(1,2,slices,stacks);
    glPopMatrix();


//3black

    glPushMatrix();

    glTranslated(1,-.5,-4);
    glRotated(90,1,0,0);
    glScaled(.1,.1,.1);
    glColor3f(0,0,0);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();

    glPushMatrix();

    glTranslated(1,-.8,-4);
    glRotated(90,1,0,0);
    glScaled(.1,.1,.1);
    glColor3f(0,0,0);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();

    glPushMatrix();

    glTranslated(1,-1.1,-4);
    glRotated(90,1,0,0);
    glScaled(.1,.1,.1);
    glColor3f(0,0,0);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();


    //second

    glPushMatrix();
    glTranslated(0,-.35,-5.7);
    glRotated(-90,0.1,0,0);
    glColor3ub(0, 234, 0);
    glScaled(1,1,1);
    glutSolidCone(1,2,slices,stacks);
    glPopMatrix();

    //face

    glPushMatrix();
    glTranslated(0,.8,-5.8);
    glRotated(90,1,0,0);
    glScaled(1.5,1.5,1.5);
    glColor3ub(244, 224, 190);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();

//left leg

    glPushMatrix();

    glTranslated(-.74,-2.2,-6);

    glRotated(90,1,0,0);
    glScaled(.9,.9,.9);
    glColor3ub(128, 128, 128);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();


    glPushMatrix();
    glTranslated(-.74,-2.8,-6.1);
    glRotated(-90,0.1,0,0);
    glColor3ub(0,0,0);
    glScaled(.95,1,1);
    glutSolidCone(1,2,slices,stacks);
    glPopMatrix();

//right leg
    glPushMatrix();

    glTranslated(.74,-2.2,-6);
    glRotated(90,1,0,0);
    glScaled(.9,.9,.9);
    glColor3ub(128, 128, 128);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslated(.74,-2.8,-6.1);
    glRotated(-90,0.1,0,0);
    glColor3ub(0,0,0);
    glScaled(.95,1,1);
    glutSolidCone(1,2,slices,stacks);
    glPopMatrix();


    //hand

    glPushMatrix();
    glTranslated(1,-.55,-6);
    glRotated(60,0,1,0);
    glScaled(1.3,.2,.5);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-1,-.55,-6);
    glRotated(-60,0,1,0);
    glScaled(1.3,.2,.5);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();


    //mouse
    glPushMatrix();
    glTranslated(0,.3,-4.5);
    glRotated(180,0,.5,-1);
    glScaled(.2,-.2,.2);
    glutSolidCone(1,2,slices,stacks);
    glPopMatrix();

    //white
    glPushMatrix();
    glTranslated(0,0.1,-5);
    glRotated(180,0,.5,-1);
    glColor3f(1,1,1);
    glScaled(.1,.1,.1);
    glutSolidCube(.3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0.04,0.1,-5);
    glRotated(180,0,.5,-1);
    glColor3f(1,1,1);
    glScaled(.1,.1,.1);
    glutSolidCube(.3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.04,0.1,-5);
    glRotated(180,0,.5,-1);
    glColor3f(1,1,1);
    glScaled(.1,.1,.1);
    glutSolidCube(.3);
    glPopMatrix();

//eye
//right
    glPushMatrix();

    glTranslated(.32,.73,-4);
    glRotated(90,1,0,0);
    glScaled(.1,.1,.1);
    glColor3f(0,0,0);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();
//white
    glPushMatrix();

    glTranslated(.35,.8,-4.3);
    glRotated(90,0,1,0);
    glScaled(.2,.3,.2);
    glColor3f(1,1,1);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();


    //left
    glPushMatrix();

    glTranslated(-.32,.73,-4);
    glRotated(90,1,0,0);
    glScaled(.1,.1,.1);
    glColor3f(0,0,0);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();

//white
    glPushMatrix();

    glTranslated(-.35,.8,-4.3);
    glRotated(90,0,1,0);
    glScaled(.2,.3,.2);
    glColor3f(1,1,1);
    glutSolidSphere(1,slices,stacks);
    glPopMatrix();



    //upeye
    //left

    glPushMatrix();
    glTranslated(-.4,1.3,-4.3);
    glRotated(120,1,0,1);
    glScaled(.05,.2,.2);
    glColor3f(0,0,0);
    glutSolidCone(1,2,slices,stacks);
    glPopMatrix();

    //right

    glPushMatrix();
    glTranslated(.35,1.3,-4.3);
    glRotated(120,1,0,1);
    glScaled(.05,.2,.2);
    glColor3f(0,0,0);
    glutSolidCone(1,2,slices,stacks);
    glPopMatrix();

    glPopMatrix();


}


void Mirror_way(GLfloat x,GLfloat y,GLfloat z)
{


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    // glColor3d(1,1,1);
    glPushMatrix();
    glScalef(1,1.0,1);
    //glRotatef(40,0,0,1);
    glTranslatef(1,6,-.02);
    // cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);



    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glColor3d(0.9,0.9,0.9);
    glPushMatrix();
    //glColor3d(1,0,0);
    GLUquadric *qobj=gluNewQuadric();
    gluQuadricTexture(qobj,GL_TRUE);
    glTranslatef(-0.3,0,-0.8);
    gluSphere(qobj,1,50,50);
    gluDeleteQuadric(qobj);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);






}


static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}




void LoadTexture(const char*filename)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}


void kill_board()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
//    glTranslatef(0,-0.5,0);
    glScalef(1.5,0.5,0.2);
    glTranslatef(-.601,8.2,9.5);
    cube(1,0,0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);


}

void singlecircle(float r, float p,float q, float a,float b,float c)
{
    glBegin(GL_POLYGON);
    glColor3d(a,b,c);

    float x=0,y=0;
    for(int i=0; i<=180; i=i+5)
    {
        x = r * cos(((-3.1416)*i)/180);
        y = r * sin(((-3.1416)*i)/180);
        glVertex3f(x,y,p);
    }
//     glVertex3f(x,y-5,p);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3d(a,b,c);

    x=0,y=0;
    for(int i=360; i>=180; i=i-5)
    {
        x = r * cos(((3.1416)*i)/180);
        y = r * sin(((3.1416)*i)/180);
        glVertex3f(x,y,q);
    }
//     glVertex3f(x,y-5,q);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glColor3d(a,b,c);

    float m=0,n=0;
    for(int i=180; i<=360; i=i+5)
    {
        m = r * cos(((3.1416)*i)/180);
        n = r * sin(((3.1416)*i)/180);
        glVertex3f(m,n,q);
        glVertex3f(m,n,p);
    }
//     glVertex3f(x,y-5,q);
//     glVertex3f(x,y-5,p);
    glEnd();
}

void circle()
{
//far circle
    glBegin(GL_POLYGON);
    glColor3d(1,0,0);

    for(int i=0; i<=360; i=i+5)
    {
        float x=0,y=0;
        x = 1.2 * cos(((-3.1416)*i)/180);
        y = 1.2 * sin(((-3.1416)*i)/180);
        glVertex3f(x,y,0.6);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3d(1,0,0);

    for(int i=0; i<=360; i=i+5)
    {
        float x=0,y=0;
        x = 1.2 * cos(((3.1416)*i)/180);
        y = 1.2 * sin(((3.1416)*i)/180);
        glVertex3f(x,y,0.7);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glColor3d(1,0,0);

    for(int i=0; i<=360; i=i+5)
    {
        float m=0,n=0;
        m = 1.2 * cos(((3.1416)*i)/180);
        n = 1.2 * sin(((3.1416)*i)/180);
        glVertex3f(m,n,0.7);
        glVertex3f(m,n,0.6);
    }
    glEnd();

//middle circle

    glBegin(GL_POLYGON);
    glColor3d(1,0,0);

    for(int i=0; i<=360; i=i+5)
    {
        float x=0,y=0;
        x = .2 * cos(((-3.1416)*i)/180);
        y = .2 * sin(((-3.1416)*i)/180);
        glVertex3f(x,y,0.7);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3d(1,0,0);

    for(int i=0; i<=360; i=i+5)
    {
        float x=0,y=0;
        x = .2 * cos(((3.1416)*i)/180);
        y = .2 * sin(((3.1416)*i)/180);
        glVertex3f(x,y,.8);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glColor3d(1,0,0);

    for(int i=0; i<=360; i=i+5)
    {
        float m=0,n=0;
        m = .2 * cos(((3.1416)*i)/180);
        n = .2 * sin(((3.1416)*i)/180);
        glVertex3f(m,n,.8);
        glVertex3f(m,n,.7);
    }
    glEnd();

    //front circle
    glBegin(GL_POLYGON);
    glColor3d(0,0.3,0);

    for(int i=0; i<=360; i=i+5)
    {
        float x=0,y=0;
        x = .5 * cos(((-3.1416)*i)/180);
        y = .5 * sin(((-3.1416)*i)/180);
        glVertex3f(x,y,.8);
    }

    glEnd();

    glBegin(GL_POLYGON);
    glColor3d(0,0.3,0);

    for(int i=0; i<=360; i=i+5)
    {
        float x=0,y=0;
        x = .5 * cos(((3.1416)*i)/180);
        y = .5 * sin(((3.1416)*i)/180);
        glVertex3f(x,y,.9);
    }

    glEnd();

    glBegin(GL_QUAD_STRIP);
    glColor3d(0,0.3,0);

    for(int i=0; i<=360; i=i+5)
    {
        float m=0,n=0;
        m = .5 * cos(((3.1416)*i)/180);
        n = .5 * sin(((3.1416)*i)/180);
        glVertex3f(m,n,.9);
        glVertex3f(m,n,.8);
    }
    glEnd();


}

void quad(float a,float b,float c)
{
    glBegin(GL_QUADS);
    glColor3d(a+.3,b+.3,c+.3);
    glVertex3f(0,0,2);
    glVertex3f(2,0,2);
    glVertex3f(2,2,2);
    glVertex3f(0,2,2);

    glColor3d(a+.2,b+5,c+5);
    glVertex3f(2,0,2);
    glVertex3f(2,0,0);
    glVertex3f(2,2,0);
    glVertex3f(2,2,2);

    glColor3d(a,b+.2,c+.5);
    glVertex3f(0,0,0);
    glVertex3f(0,2,0);
    glVertex3f(2,2,0);
    glVertex3f(2,0,0);

    glColor3d(a-.2,b,c+.2);
    glVertex3f(0,2,0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,2);
    glVertex3f(0,2,2);

    glColor3d(a+.4,b,c);
    glVertex3f(0,2,0);
    glVertex3f(0,2,2);
    glVertex3f(2,2,2);
    glVertex3f(2,2,0);

    glColor3d(a,b,c+.5);
    glVertex3f(0,0,0);
    glVertex3f(2,0,0);
    glVertex3f(2,0,2);
    glVertex3f(0,0,2);


    glEnd();
}


void windMill()
{
    glPushMatrix();
    glTranslated(-4,-2,-5);
    glRotated(xo,0,1,0);
    glRotated(yo,1,0,0);
    glRotated(zo,0,0,1);
    glScaled(3,3,3);
    glPushMatrix();
    glTranslated(-1,-2,-6+ze);
    glRotated(10,1,0,0); //back
    glScaled(.05,2.7,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-1,-2,-4+ze);
    glRotated(-10,1,0,0); //front
    glScaled(.05,2.7,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0,-2,-5+ze);
    glRotated(10,0,0,1); //right
    glScaled(.05,2.7,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-2,-2,-5+ze);
    glRotated(-10,0,0,1); //left
    glScaled(.05,2.7,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-1.1,3.3,-5.1+ze);
    glScaled(.15,.25,.15); //up
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-.95,3.6,-5.1+ze);
    glScaled(.4,.4,.5); //circle
    circle();
    glPopMatrix();
    glPushMatrix();
    glTranslated(-1,3.6,-4.75+ze);
    glRotated(-40,0,0,1); //plate1
    glScaled(.1,.8,.03);
    quad(.6,.0,.0);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-1.1,3.5,-4.75+ze);
    glRotated(80,0,0,1); //plate2
    glScaled(.1,.8,.03);
    quad(.1,.1,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-.8,3.5,-4.75+ze);
    glRotated(200,0,0,1); //plate3
    glScaled(.1,.8,.03);
    quad(.0,.6,.6);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-2,-2,-5+ze);
    glRotated(-90,1,0,1); //left
    glScaled(.05,.73,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-1.93,-1.95,-5+ze);
    glRotated(-90,0,0,1); //left
    glRotated(45,1,0,0); //left
    glScaled(.05,.73,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-1,-2,-4+ze);
    glRotated(-90,1,0,1); //left
    glScaled(.05,.73,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-.0,-1.95,-4.9+ze);
    glRotated(-90,0,0,1); //left
    glRotated(225,1,0,0); //left
    glScaled(.05,.73,.05);
    quad(.5,.3,.1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-1.8,-1,-5+ze);
    glRotated(-90,1,0,1); //left
    glScaled(.05,.53,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-1.7,-.95,-4.9+ze);
    glRotated(-90,0,0,1); //left
    glRotated(45,1,0,0); //left
    glScaled(.05,.53,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-.95,-1,-4.2+ze);
    glRotated(-90,1,0,1); //left
    glScaled(.05,.53,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-.15,-.95,-4.98+ze);
    glRotated(-90,0,0,1); //2right_down
    glRotated(225,1,0,0);
    glScaled(.05,.53,.05);
    quad(.5,.3,.1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-1.65,-0,-5.0+ze);
    glRotated(-90,1,0,1); //3left_up
    glScaled(.05,.47,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-1.6,.05,-4.99+ze);
    glRotated(-90,0,0,1); //3left_down
    glRotated(45,1,0,0);
    glScaled(.05,.47,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-.99,-0,-4.38+ze);
    glRotated(-90,1,0,1); //3right_up
    glScaled(.05,.47,.05);
    quad(.5,.3,.1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(-.35,.05,-4.94+ze);
    glRotated(-90,0,0,1); //3right_down
    glRotated(225,1,0,0);
    glScaled(.05,.47,.05);
    quad(.5,.3,.1);
    glPopMatrix();


    glPopMatrix();



}


void Wind_Env()
{

    glPushMatrix();
    glTranslated(0,8.5,5);
    glScaled(1,1,1);
    windMill();
    glPopMatrix();



    glPushMatrix();
    glColor3f(0,0,0);
    glTranslated(4.8,11,-1.0);
    glScaled(1,1.2,1.2);
    glRotated(-55,0,0,1);
    curve();
    glPopMatrix();


    glPushMatrix();
    glTranslated(-2.5,6,2);
    glRotated(-60,0,1,0);
    glScaled(1.5,2,1.5);
    //glRotated(15,0,1,0);

    //Mirror_way(0,0,0);
    Baby_Doll();

    glPopMatrix();


}





void Find_Score()
{
    float ballPosX = -tX;
    float ballPosY = -tY;

    if(tZ>-8.2 && tZ<-7.8)
    {
        if((ballPosX>1.5 && ballPosX<4.5) && (ballPosY>8.5 && ballPosY<11.5) )
        {
            score++;
        }
    }
    else if(tZ2>-8.2 && tZ2<-7.8)
    {
        if((ballPosX>-5.5 && ballPosX<-2.5) && (ballPosY>4.5 && ballPosY<7.5) )
        {
            score++;;
        }
    }
    else if(tZ3>-8.2 && tZ3<-7.8)
    {
        if((ballPosX>-3.5 && ballPosX<-0.5) && (ballPosY>1.5 && ballPosY<4.5) )
        {
            score++;
        }
    }
    else if(tZ4>-8.2 && tZ4<-7.8)
    {
        if((ballPosX>-1.5 && ballPosX<1.5) && (ballPosY>2.5 && ballPosY<5.5) )
        {
            score++;
        }
    }
    else if(tZ5>-8.2 && tZ5<-7.8)
    {
        if((ballPosX>0.5 && ballPosX<3.5) && (ballPosY>-0.5 && ballPosY<2.5) )
        {
            score++;
        }
    }
    else if(tZ6>-8.2 && tZ6<-7.8)
    {
        if((ballPosX>-5.5 && ballPosX<-2.5) && (ballPosY>2.5 && ballPosY<5.5) )
        {
            score++;
        }
    }


    if(tZ>10 && tZ<10.5)
    {
        if((ballPosX>1.5 && ballPosX<4.5) && (ballPosY>8.5 && ballPosY<11.5) )
        {
            Over=true;
            START=false;
        }
    }
    else if(tZ2>10 && tZ2<10.5)
    {
        if((ballPosX>-5.5 && ballPosX<-2.5) && (ballPosY>4.5 && ballPosY<7.5) )
        {
            Over=true;
            START=false;
        }
    }
    else if(tZ3>10 && tZ3<10.5)
    {
        if((ballPosX>-3.5 && ballPosX<-0.5) && (ballPosY>1.5 && ballPosY<4.5) )
        {
            Over=true;
            START=false;
        }
    }
    else if(tZ4>10 && tZ4<10.5)
    {
        if((ballPosX>-1.5 && ballPosX<1.5) && (ballPosY>2.5 && ballPosY<5.5) )
        {
            Over=true;
            START=false;
        }
    }
    else if(tZ5>10 && tZ5<10.5)
    {
        if((ballPosX>0.5 && ballPosX<3.5) && (ballPosY>-0.5 && ballPosY<2.5) )
        {
            Over=true;
            START=false;
        }
    }
    else if(tZ6>10 && tZ6<10.5)
    {
        if((ballPosX>-5.5 && ballPosX<-2.5) && (ballPosY>2.5 && ballPosY<5.5) )
        {
            Over=true;
            START=false;
        }
    }
}

void environment(int n)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslated(-10,0,0);
    glScaled(EN_SIZE,0.3,EN_SIZE*2);
    Cube(true);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);



    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslated(torusPosX[n],torusPosY[n],-10);
    glScalef(1.5,1,0.12);
    glTranslatef(0,-0.5,-0.5);
    Cube(false);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);




    glColor3d(1.0,0.0,1.0);
    glPushMatrix();
    glTranslated(torusPosX[n],torusPosY[n],0);
    glScaled(0.3,0.3,0.3);
    glutSolidTorus(0.1,2,30,30);
    glPopMatrix();
    Find_Score();

    for(int i=-(EN_SIZE/2)+1; i<(EN_SIZE/2); i+=1)
    {
        for(int j=-(EN_SIZE/2)+1; j<(EN_SIZE/2); j+=1)
        {
            if(tola[i+(EN_SIZE/2)+1][j+(EN_SIZE/2)+1]!=0)
            {
                glColor3d(0.9,1.0,0.9);
                glPushMatrix();
                glTranslated(i,0,j);
                scence();
                glPopMatrix();

            }


            else if(i>=-5&&i<=5)
            {
                // glColor3d(1,1,1);

                //kill_board();

            }
            else
            {
                tola[i+(EN_SIZE/2)+1][j+(EN_SIZE/2)+1]=(rand()%5)+1;
                glPushMatrix();
                glTranslated(i,0,j);
                scence();
                glPopMatrix();


            }
        }
    }




}


void Ball()
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double a = t*90.0;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glColor3d(0.9,0.9,0.9);

    glPushMatrix();
    GLUquadric *qob=gluNewQuadric();
    gluQuadricTexture(qob,GL_TRUE);
    glRotatef(a,0,0,1);
    glScaled(0.70,0.7,0.7);
    gluSphere(qob,1,30,30);
    gluDeleteQuadric(qob);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


}

void draw()
{
    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double a = t*90.0;

    TIME = t;

    ///Ball
    if(rotX>11)rotX=11;
    if(rotX<-11)rotX=-11;
    if(rotZ>10)rotZ=10;
    if(rotZ<-15)rotZ=-15;

    glPushMatrix();
    glTranslated(-4,4.53,-5);
    li8Src();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0,1,0);
    glRotated(90,0,1,0);
    glRotated(5,0,0,1);
    glRotated(rotX,1,0,0);
    glRotated(rotY,0,1,0);
    glRotated(rotZ,0,0,1);

    glScaled(0.2,0.2,0.2);
    Ball();
    glPopMatrix();


    ///Environment
    if(tX>=4.1)tX=4.1;
    if(tX<=-4.1)tX=-4.1;
    if(tY>0.1)tY= 0.1;
    if(tY<-15)tY= -15;

    glPushMatrix();
    glTranslated(tX,tY,tZ);
    environment(2);
    glPopMatrix();

    glPushMatrix();
    glTranslated(tX,tY,tZ1);
    Wind_Env();



    glPopMatrix();

    glPushMatrix();
    glTranslated(tX,tY,tZ2);
    environment(3);
    glPopMatrix();

    glPushMatrix();
    glTranslated(tX,tY,tZ3);
    environment(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(tX,tY,tZ4);
    environment(5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(tX,tY,tZ5);
    environment(6);
    glPopMatrix();

    glPushMatrix();
    glTranslated(tX,tY,tZ6);
    environment(4);
    glPopMatrix();

    tZ+=speed;
    tZ1+=speed;
    tZ2+=speed;
    tZ3+=speed;
    tZ4+=speed;
    tZ5+=speed;
    tZ6+=speed;

    if(tZ>=20)tZ=-110;
    if(tZ1>=20)tZ1=-110;
    if(tZ2>=20)tZ2=-110;
    if(tZ3>=20)tZ3=-110;
    if(tZ4>=20)tZ4=-110;
    if(tZ5>=20)tZ5=-110;
    if(tZ6>=20)tZ6=-110;

    if(rotX>0)rotX-=angleBackFrac;
    if(rotX<0)rotX+=angleBackFrac;
    if(rotY>0)rotY-=angleBackFrac;
    if(rotY<0)rotY+=angleBackFrac;
    if(rotZ>0)rotZ-=angleBackFrac;
    if(rotZ<0)rotZ+=angleBackFrac;

    //cout<<tX<<" "<<tY<<" "<<tZ<<endl;
    //cout<<rotX<<" "<<rotY<<" "<<rotZ<<endl;

    speed += 0.0002;
    if(speed>=0.1)speed=0.1;
}




void setFont(void *font)
{
    currentfont=font;
}


void drawstring(float x,float y,float z,char *string)
{
    char *c;
    glRasterPos3f(x,y,z);

    for(c=string; *c!='\0'; c++)
    {
        //glColor3f(0.0,0.0,0.0);
        glutBitmapCharacter(currentfont,*c);
    }
}

void Front_Screen()
{
    glPushMatrix();
    setFont(GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1,1,1);
    drawstring(-1.0,8.0,0.0,"Bouncing Ball ");
    drawstring(-4,7.0,0.0,"UP: W, DOWN: S, LEFT: A, RIGHT: D, MAIN MENU: O");
    drawstring(-1.4,6.0,0.0,"Press Enter to Start ");
    glPopMatrix();
}

void GameOver()
{
    int final_Result = score/24;
    char num_string[10];
    itoa(final_Result,num_string,10);

    glPushMatrix();
    setFont(GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1,0,0);
    drawstring(-1.8,8.0,0.0,"Game Over ");
    drawstring(-2.4,7.0,0.0,"Total Score is : ");
    drawstring(0,7.0,0.0,num_string);
    drawstring(-2.4,6.0,0.0,"Press Enter to Restart ");
    // drawstring(-2.4,6.0,0.0,"Press \"O\" to go Home Screen ");
    glPopMatrix();

    speed = 0.0;
    zoom=4;
    tX=0;
    tY=-2;
    tZ=-8;
    tZ1=-20;
    tZ2=-40;
    tZ3=-60;
    tZ4=-80;
    tZ5=-100;
    tZ6=-120;
    rotX=0;
    rotY=0;
    rotZ=0;
    angleBackFrac = 0.2;
    START = false;
    rot = false;
    light0 = true;
    no_ambient = false;
    no_diffuse = false;
    no_specular = false;
    emmission_on = true;
    anglex= 0.0;
    angley= 0.0;
    anglez= 0.0;
    normalLight =false;
}

void Main_Screen()
{
    int final_Result = score/24;
    char num_string[10];
    itoa(final_Result,num_string,10);
    glPushMatrix();
    glColor3f(1,0,1);
    setFont(GLUT_BITMAP_TIMES_ROMAN_24);

    drawstring(-6,8.0,0.0,"Score : ");
    drawstring(-5,8.0,0.0, num_string);
    glPopMatrix();
}


/*

void drawInstrc(char* str,int x,int y,int z,float sx, float sy)
{
    char *c;
    glPushMatrix();
    glTranslatef(x, y+8,z);
    glScalef(sx,sy,z);

    for (c=str; *c != '\0'; c++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}


void drawInstrucChar(char c,float x,float y,float z)
{
    glPushMatrix();
    glTranslatef(x, y+8,z);
    glScalef(0.002f,0.002f,z);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    glPopMatrix();
}

*/
static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double a = t*90.0;
    double aa=a;

    if(!rot)
    {
        a=0;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(	0.0, 4.5, 10.0,
                0, 4, 0,
                0, 1.0f, 0.0f);
    glPushMatrix();
    light();
    glPopMatrix();

    if(START)
    {

        glPushMatrix();
        glTranslated(0,0,0);
        glScaled(zoom,zoom,zoom);
        glRotated(a,0,1,0);
        draw();
        glPopMatrix();
        Main_Screen();
    }
    else if(Over)
    {
        GameOver();
    }

    // drawStrokeText("UP: W, DOWN: S, LEFT: A, RIGHT: D, MAIN MENU: M",-5,0.9,0);
    // drawStrokeText("SCORE : ",-9,0,0);
    /*
    int mod,number=0;
    while(TIME)
    {
        mod=TIME%10;
        number=number*10+mod;
        TIME/=10;
    }
    float tmp=0;
    while(number)
    {
        mod=number%10;
        drawStrokeChar(mod+48,4+tmp,0,0);
        number/=10;
        tmp+=0.2;
    }
    */

    else
    {

        glPushMatrix();
        glTranslated(0,3,0);
        glRotated(aa,0,1,0);
        glScaled(1.5,1.5,1.5);
        Ball();
        glPopMatrix();
        Front_Screen();


        //drawStrokeText("Press G to Start",-1,-1,0);
        //drawStrokeText2("Bouncing Ball Game",-3,0,0);
        //drawStrokeText("UP: W, DOWN: S, LEFT: A, RIGHT: D, MAIN MENU: M",-3.9,-1,0);
    }

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    float frac = 0.3;
    float rotFrac = 1;
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;
    case 'r':
        rot=true;
        break;
    case 't':
        rot=false;
        break;
    case 'z':
        zoom+=0.05;
        break;
    case 'Z':
        zoom-=0.05;
    case 'w':
        tY-=frac;
        rotZ+=rotFrac;
        break;
    case 's':
        tY+=frac;
        rotZ-=rotFrac;
        break;
    case 'a':
        tX+=frac;
        rotX-=rotFrac*3;
        rotY+=rotFrac/2;
        break;
    case 'd':
        tX-=frac;
        rotX+=rotFrac*3;
        rotY-=rotFrac/2;
        break;
    case 'O':
    case 'o':
        score=0;
        START=false;
        Over = false;
        break;
//        case 'y':
//            rotX-=rotFrac;
//            break;
//        case 'h':
//            rotX+=rotFrac;
//            break;
//        case 'g':
//            rotY+=rotFrac;
//            break;
//        case 'j':
//            rotY-=rotFrac;
//            break;

    case '1':
        no_ambient = !no_ambient;
        break;
    case '2':
        no_diffuse = !no_diffuse;
        break;
    case '3':
        no_specular = !no_specular;
        break;
    case '4':
        light0 = !light0;
        break;
    case 13:
        score=0;
        START=true;
        break;
    case 'm':
        START=false;
        break;
    case 'V':
        anglex =((--anglex)%360);
        break;
    case 'v':
        anglex = (++anglex)%360;
        break;
    case 'B':
        angley = ( angley-1)%360;
        break;
    case 'b':
         angley = ( angley+1)%360;
        break;
    case 'N':
        anglez = (anglez-1)%360;
        break;
    case 'n':
        anglez = (anglez+1)%360;
        break;
//        case 'o':
//            cosX-=frac*cos(rotX*rad);
//            cosY+=frac*cos(rotY*rad);
//            cosZ-=frac*cos(rotZ*rad);
//            //cout<<"Front : "<<cosX<<" "<<cosY<<" "<<cosZ<<endl;
//            break;
//        case 'l':
//            cosX+=frac*cos(rotX*rad);
//            cosY-=frac*cos(rotY*rad);
//            cosZ+=frac*cos(rotZ*rad);
//            //cout<<"Back : "<<cosX<<" "<<cosY<<" "<<cosZ<<endl;
//            break;

    }

    glutPostRedisplay();
}


static void idle(void)
{
    glutPostRedisplay();
}







int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(1366,720);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    glutCreateWindow("Bouncing Ball Game");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    //init();

    PlaySound("game_sound.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);

    glClearColor(0.0,0.5,0.7,0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //
//glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
//glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);




    LoadTexture("E:\\DOWNLOAD\\computer_graphics_lab\\texture_try\\lastbuilding.bmp");
    LoadTexture("E:\\DOWNLOAD\\computer_graphics_lab\\texture_try\\ba.bmp");
    LoadTexture("E:\\DOWNLOAD\\computer_graphics_lab\\texture_try\\nv.bmp");
    LoadTexture("E:\\DOWNLOAD\\computer_graphics_lab\\texture_try\\brick.bmp");
    LoadTexture("E:\\DOWNLOAD\\computer_graphics_lab\\texture_try\\ball2.bmp");
    LoadTexture("E:\\DOWNLOAD\\computer_graphics_lab\\texture_try\\d.bmp");
    cout<<"Instruction :"<<endl;
    cout<<"Press w to go up"<<endl;
    cout<<"Press s to go down"<<endl;
    cout<<"Press a to go left"<<endl;
    cout<<"Press d to go right"<<endl;
    cout<<"Press r to start rotation"<<endl;
    cout<<"Press z to Zoom"<<endl;



    glutMainLoop();

    return EXIT_SUCCESS;
}

