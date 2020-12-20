#include "sdl/SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

struct WVector
{
    GLfloat x,y,z;
};

struct WMaterial {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];
    char* name;
};

struct WGroup {
    GLuint* Indices;
    int numIndices;
    char* mtlname;
    int mtl;
};

typedef struct tagOBJECT {
    WVector pos;
    WVector rot;
    float angle;

    int numGroups;
    WGroup* groups;
    int numMtl;
    WMaterial* mtl;
    bool ismtl;

    WVector* Vertices;
    WVector* Normals;
    int numVertices, numNormals;
} OBJECT;

struct WALL
{
      WVector max;
      WVector min;
};

void you_compensate();
void draw_cube();

void SetupWorld(char* worldfile);
float getWorldX();
float getWorldZ();
float getWorldAngle();
void DrawWorld();

void boundCreate(OBJECT* objs, int num);
int boundCheck(float minX, float maxX, float minZ, float maxZ);
void wallsCreate(WALL* tmpWall, int numWalls);
bool wallCheck(float minX, float maxX, float minZ, float maxZ);

static GLfloat red[]    = { 255,   0,   0 };
static GLfloat green[]  = {   0, 255,   0 };
static GLfloat blue[]   = {   0,   0, 255 };
static GLfloat white[]  = { 255, 255, 255 };
static GLfloat black[]  = {   0,   0,   0 };
static GLfloat yellow[] = { 255, 255,   0 };
static GLfloat purple[] = { 255,   0, 255 };

void buildFont();
void glPrint(const char *fmt, ...);
