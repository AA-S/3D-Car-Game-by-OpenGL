#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include<stdio.h>
#include<bits/stdc++.h>
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#include<iostream>

using namespace std;
void checkCollision();
GLfloat alpha = .50, theta = 0.0, axis_x=0.0, axis_y=0.0;
GLboolean bRotate = false, uRotate = false;

float ex=2,ey=2,ez=50,lx=2,ly=2,lz=49;
int divx = 250, divy = 4, movd;
int delay = 40;
float dzx=-200;
float myCarMove = 2,othersCarMoveLeft=-100,othersCarMoveRight=-120,
      othersCarMoveMiddle=-60,othersCarMoveSide = -120,othersCarMoveSide2 = -90;

float collisionLeft=-00, collisionRight=-0,
      collisionMiddle=-0,collisionSide = -0,collisionSide2=0;

int totalScore = 0;
bool collision=false;

struct Image_Data
{
    unsigned long Size_X;
    unsigned long Size_Y;
    unsigned char *Data;
};

int Load_Bitmap(char* Filename, Image_Data* Out_Image_Data)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3

    FILE * file = fopen(Filename,"rb");
    if (!file)
    {
        return 0;
    }

    if ( fread(header, 1, 54, file)!=54 )  // If not 54 bytes read : problem
    {
        return 0;
    }

    if ( header[0]!='B' || header[1]!='M' )
    {
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    Out_Image_Data->Data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(Out_Image_Data->Data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    Out_Image_Data->Size_X = width;
    Out_Image_Data->Size_Y = height;

    return 1;
}

GLuint Textures[100];

void Add_Texture(int ID, Image_Data* Data)
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Data->Size_X,Data->Size_Y,0,GL_BGR,GL_UNSIGNED_BYTE,Data->Data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
}


void draw_solid_cube(GLfloat size)
{
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
    float x4,y4,z4;

    x1 = -size/2;
    y1 = -size/2;
    z1 = -size/2;

    x2 = size/2;
    y2 = -size/2;
    z2 = -size/2;

    x3 = size/2;
    y3 = -size/2;
    z3 = size/2;

    x4 = -size/2;
    y4 = -size/2;
    z4 = size/2;

    float x5,y5,z5;
    float x6,y6,z6;
    float x7,y7,z7;
    float x8,y8,z8;

    x5 = -size/2;
    y5 = size/2;
    z5 = -size/2;

    x6 = size/2;
    y6 = size/2;
    z6 = -size/2;

    x7 = size/2;
    y7 = size/2;
    z7 = size/2;

    x8 = -size/2;
    y8 = size/2;
    z8 = size/2;

    float n1_x = 0, n1_y = -1, n1_z = 0; //face that faces towards bottom
    float n2_x = 0, n2_y = 1, n2_z = 0; //up
    float n3_x = 0, n3_y = 0, n3_z = -1; //front
    float n4_x = 0, n4_y = 0, n4_z = 1; //back
    float n5_x = 1, n5_y = 0, n5_z = 0; //right
    float n6_x = -1, n6_y = 0, n6_z = 0; //left

    glBegin(GL_QUADS);
    //Quad 1

    glNormal3f(0,-1,0);

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x4,y4,z4);

    //Quad 2

    glNormal3f(0,1,0);

    glTexCoord2f(0,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);
    //Quad 3

    glNormal3f(n3_x,n3_y,n3_z);

    glTexCoord2f(0,0);
    glVertex3f(x5,y5,z5);

    glTexCoord2f(1,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,1);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(0,1);
    glVertex3f(x1,y1,z1);
    //Quad 4


    glNormal3f(n4_x,n4_y,n4_z);

    glTexCoord2f(0,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,1);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(0,1);
    glVertex3f(x3,y3,z3);
    //Quad 5

    glNormal3f(n5_x,n5_y,n5_z);

    glTexCoord2f(0,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x2,y2,z2);

    //Quad 6

    glNormal3f(n6_x,n6_y,n6_z);

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(1,1);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);

    glEnd();
}

void drawText(char ch[],int xpos, int ypos,int zpos)//draw the text for score and game over
{
    int numofchar = strlen(ch);
    glLoadIdentity ();
    glRasterPos3f( xpos , ypos, xpos);
    for (int i = 0; i <= numofchar - 1; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch[i]);//font used here, may use other font also

    }
}

void drawRoad(float x, float y , float z)
{
    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x,y,z);//+2.5+0.025+2.5
    glScalef(3,0.05,95);
    //glutSolidCube(1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

}
float zx=0;
void drawRoadSide(float x, float y , float z)
{
    //glClearColor(1,1,1,1);
    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x-2,y,z);//+2.5+0.025+2.5
    glScalef(1,0.05,95);
    //glutSolidCube(1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x-6,y+.01,z);//+2.5+0.025+2.5
    glScalef(8,0.05,95);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x+2,y,z);//+2.5+0.025+2.5
    glRotatef( 180, 0, 0, 1 );
    glScalef(1,0.05,95);
    //glutSolidCube(1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x+6,y+.01,z);//+2.5+0.025+2.5
    glRotatef( 180, 0, 0, 1 );
    glScalef(8,0.05,95);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x,y,-100);//+2.5+0.025+2.5
    glScalef(210,200.05,5);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void drawDivider(float x, float y , float z)//black patch drawn in middle of road
{
    for(int i=2; i<=300; i+=2)
    {
        glPushMatrix();
        //if(z+i<-30)glColor4f(0,0,0,1);else
        glColor4f(1,1,1,1);
        glTranslatef(x,y+.1,z+i);//+2.5+0.025+2.5
        glScalef(.1,0.05,.8);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        draw_solid_cube(1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

}

void drawMyCar(float x, float y , float z)
{
    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x,y+.6,z);//+2.5+0.025+2.5
    glScalef(.3,.5,1);
    //printf("%f %f %f\n",x,y,z);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x-.13,y+.3,z-.3);//+2.5+0.025+2.5
    glScalef(.3,1,1);
    //printf("%f %f %f\n",x,y,z);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glutSolidSphere(.1,100,100);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x-.13,y+.3,z+.3);//+2.5+0.025+2.5
    glScalef(.3,1,1);
    //printf("%f %f %f\n",x,y,z);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glutSolidSphere(.1,100,100);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x+.13,y+.3,z+.3);//+2.5+0.025+2.5
    glScalef(.3,1,1);
    //printf("%f %f %f\n",x,y,z);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glutSolidSphere(.1,100,100);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x+.13,y+.3,z-.3);//+2.5+0.025+2.5
    glScalef(.3,1,1);
    //printf("%f %f %f\n",x,y,z);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glutSolidSphere(.1,100,100);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawOthersCarLeft(float x, float y , float z,int ti)
{

    for(int i=0; i<=220; i+=25)
    {
       // int ti = rand()%3+5;
        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x,y+.6,z+i);//+2.5+0.025+2.5
        glScalef(.3,.5,1);
        collisionLeft = z+i;
        //printf("%f left\n",collisionLeft);
        checkCollision();
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,ti);
        draw_solid_cube(1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void drawOthersCarRight(float x, float y , float z,int ti)
{
    for(int i=0; i<=220; i+=25)
    {
        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x,y+.6,z+i);//+2.5+0.025+2.5
        glScalef(.3,.5,1);
        collisionRight = z+i;
        //printf("%f left\n",collisionLeft);
        checkCollision();
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,ti);
        draw_solid_cube(1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void drawOthersCarMiddle(float x, float y , float z,int ti)
{
    for(int i=0; i<=220; i+=25)
    {
        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x,y+.6,z+i);//+2.5+0.025+2.5
        glScalef(.3,.5,1);
        collisionMiddle = z+i;
        //printf("%f left\n",collisionLeft);
        checkCollision();
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,ti);
        draw_solid_cube(1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void drawOthersCarSide(float x, float y , float z)
{
//    if(rand())
    for(int i=0; i<=220; i+=25)
    {
        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x,y+.6,z+i);//+2.5+0.025+2.5
        glScalef(.3,.5,1);
        collisionSide = z+i;
        //printf("%f left\n",collisionLeft);
        checkCollision();
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,4);
        draw_solid_cube(1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void drawOthersCarSide2(float x, float y , float z)
{
//    if(rand())
    for(int i=0; i<=220; i+=25)
    {
        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x,y+.6,z+i);//+2.5+0.025+2.5
        glScalef(.3,.5,1);
        collisionSide2 = z+i;
        //printf("%f left\n",collisionLeft);
        checkCollision();
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,4);
        draw_solid_cube(1);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x-.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z+.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glColor4f(1,1,1,1);
        glTranslatef(x+.13,y+.3,z-.3+i);//+2.5+0.025+2.5
        glScalef(.3,1,1);
        //printf("%f %f %f\n",x,y,z);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,2);
        glutSolidSphere(.1,100,100);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}


void Write(char *string) //Write string on the screen
{
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}


void drawScore()
{
    char tmp_str[40];
    // Print the status of the game on the screen
    glColor3f(1, 0, 0);
    glRasterPos2f(3, 5);

    if(!collision)
    {
        sprintf(tmp_str, "Score: %d",totalScore++);
        Write(tmp_str);
    }
    else
    {
        glColor3f(1, 0, 0);
        sprintf(tmp_str, "Game over");
        Write(tmp_str);
        printf("Your score is = %d\n",totalScore);
        Sleep(3000);
        exit(0);
 //       Beep();
//        sound(500);
//        exit(0);

    }
}
void checkCollision()
{
    if((abs(collisionLeft-45.5)<=0.5001 && abs(1-myCarMove)<=.20001) ||
       (abs(collisionRight-45.5)<=0.5 && abs(myCarMove-2)<=.3)||
        (abs(collisionMiddle-45.5)<=0.5 && abs(myCarMove-1.5)<=.2)||
       (abs(collisionSide-45.5)<=0.5001 && abs(myCarMove-.5)<=.1001)||
       (abs(collisionSide2-45.5)<=0.5001 && abs(myCarMove-2.5)<=.1001)){
        collision = true;

        } //printf("m = %f  %f\n",abs(1-myCarMove),abs(collisionLeft-45.5));
//        exit(0);
}

int ti;
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective(65,.3,1,200);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    //set_light(0,true,ambient,diffuse,specular,light_position);
    gluLookAt(ex,ey,ez, lx,ly,lz, 0,1,0);

    //glViewport(20,20,700,700);
    glPushMatrix();
    glRotatef( theta, 1, 0, 0.0 );
    glRotatef( alpha, 0, 1, 0.0 );
    drawRoad(1.4,0,0);
    drawRoadSide(1.4,0,zx);
    drawDivider(1.4,0,dzx);
    drawMyCar(myCarMove,0,45.5);

    ti = (time(0)+rand())%3+5;
    drawOthersCarLeft(1,0,othersCarMoveLeft,5);
    drawOthersCarRight(2,0,othersCarMoveRight,6);
    drawOthersCarMiddle(1.5,0,othersCarMoveMiddle,7);
    drawOthersCarSide(.5,0,othersCarMoveSide);
    drawOthersCarSide2(2.5,0,othersCarMoveSide2);
    drawScore();
    checkCollision();
    if(othersCarMoveLeft>50)
    {
        othersCarMoveLeft = -120;

        Sleep(delay);
    }
    else
    {
        othersCarMoveLeft += 1;
        Sleep(delay);
    }
    if(othersCarMoveRight>50)
    {
        othersCarMoveRight = -120;
        Sleep(delay);
    }
    else
    {
        othersCarMoveRight += 1;
        Sleep(delay);
    }
    if(othersCarMoveMiddle>50)
    {
        othersCarMoveMiddle = -120;
        Sleep(delay);
    }
    else
    {
        othersCarMoveMiddle += 1;
        Sleep(delay);
    }

    if(othersCarMoveSide>50)
    {
        othersCarMoveSide = -120;
        Sleep(delay);
    }
    else
    {
        othersCarMoveSide += 1;
        Sleep(delay);
    }
    if(othersCarMoveSide2>50)
    {
        othersCarMoveSide2 = -120;
        Sleep(delay);
    }
    else
    {
        othersCarMoveSide2 += 1;
        Sleep(delay);
    }

    if(zx>22)
    {
        zx = 0;
        Sleep(delay);
    }
    else
    {
        zx += 1;
        Sleep(delay);
    }
    if(dzx>-30)
    {
        dzx=-100;
        Sleep(delay);
    }
    else
    {
        dzx += 1;
        Sleep(delay);
    }
    if(delay>=5)
    delay-=0.01;
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

float mm,xx,yy,zz;
float cam_angle = 270/180.0f * 3.1416;

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'j':
        cam_angle = cam_angle - 0.01f;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);

        break;
    case 'k':
        cam_angle = cam_angle + 0.01f;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);

        break;

    case 'z':
        ex = ex + cos(cam_angle) * 0.5f;
        ez = ez + sin(cam_angle) * 0.5f;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);

        break;
    case 'x':
        ex = ex - cos(cam_angle) * 0.5f;
        ez = ez - sin(cam_angle) * 0.5f;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);
        printf("%lf   %lf  %lf  %lf  %lf  %lf\n",ex,ey,ez,lx,ly,lz);
        break;

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
    case 'q':	// Escape key
        exit(1);
    }
}

void Specialkey(int key, int x, int y)//allow to use navigation key for movement of car
{
    switch(key)
    {
    case GLUT_KEY_UP:
        if(delay>=10)
            delay-=10;
printf("%d\n",delay);
        break;
    case GLUT_KEY_DOWN:
        delay+=10;

        break;
    case GLUT_KEY_LEFT:
        if(myCarMove>=.4)
            myCarMove -= .2;
        printf("%f left\n",myCarMove);
        break;
    case GLUT_KEY_RIGHT:
        if(myCarMove<=2.4)
            myCarMove += .2;
        printf("%f Right\n",myCarMove);
        break;
    }
    glutPostRedisplay();
}

void animate()
{
    if (bRotate == true)
    {
        theta += 0.2;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.2;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }
    glutPostRedisplay();
}



int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(1200,1200);
    glutCreateWindow("Car Game");

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);

    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(Specialkey);
    glutDisplayFunc(display);

    glutIdleFunc(animate);
    //set_light(0,true,ambient,diffuse,specular,light_position);

    glEnable(GL_NORMALIZE);
    // glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    glDepthFunc(GL_LESS);

    glGenTextures(10,Textures);
    Image_Data* Bitmap_1 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\CSE 42\\CSE 4208\\Program\\Game\\road3.bmp", Bitmap_1) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_2 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\CSE 42\\CSE 4208\\Program\\Game\\road5.bmp", Bitmap_2) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_3 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\CSE 42\\CSE 4208\\Program\\Game\\divider2.bmp", Bitmap_3) == 0)
    {
        return 0;
    }
    Image_Data* Bitmap_4 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\CSE 42\\CSE 4208\\Program\\Game\\crop.bmp", Bitmap_4) == 0)
    {
        return 0;
    }
    Image_Data* Bitmap_5 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\CSE 42\\CSE 4208\\Program\\Game\\sky.bmp", Bitmap_5) == 0)
    {
        return 0;
    }
    Image_Data* Bitmap_6 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\CSE 42\\CSE 4208\\Program\\Game\\car1.bmp", Bitmap_6) == 0)
    {
        return 0;
    }
    Image_Data* Bitmap_7 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\CSE 42\\CSE 4208\\Program\\Game\\car4.bmp", Bitmap_7) == 0)
    {
        return 0;
    }
    Image_Data* Bitmap_8 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\CSE 42\\CSE 4208\\Program\\Game\\car3.bmp", Bitmap_8) == 0)
    {
        return 0;
    }
    Image_Data* Bitmap_9 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\CSE 42\\CSE 4208\\Program\\Game\\car2.bmp", Bitmap_9) == 0)
    {
        return 0;
    }

    Add_Texture(0,Bitmap_1);
    Add_Texture(1,Bitmap_2);
    Add_Texture(2,Bitmap_3);
    Add_Texture(3,Bitmap_4);
    Add_Texture(4,Bitmap_5);
    Add_Texture(5,Bitmap_6);
    Add_Texture(6,Bitmap_7);
    Add_Texture(7,Bitmap_8);
    Add_Texture(8,Bitmap_9);

    glutMainLoop();

    return 0;
}
