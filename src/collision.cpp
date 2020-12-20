#include "main.h"

struct tBoundingBox
{
      WVector max;
      WVector min;
};

int boxes;
int walls;
tBoundingBox *boxArray;
WALL *wallArray;

void boundCreate(OBJECT* objs, int num) {
    boxArray = new tBoundingBox[num];
    boxes=num;
    for (int j=0; j<num; j++) {
        float minX = objs[j].Vertices[0].x;
        float maxX = objs[j].Vertices[0].x;
        float minZ = objs[j].Vertices[0].z;
        float maxZ = objs[j].Vertices[0].z;
        for (int i=1; i<objs[j].numVertices; i++) {
            if (objs[j].Vertices[i].x < minX) minX = objs[j].Vertices[i].x;
            if (objs[j].Vertices[i].x > maxX) maxX = objs[j].Vertices[i].x;
            if (objs[j].Vertices[i].z < minX) minX = objs[j].Vertices[i].z;
            if (objs[j].Vertices[i].z > maxX) maxX = objs[j].Vertices[i].z;
        }
        boxArray[j].min.x  = minX + objs[j].pos.x;
        boxArray[j].max.x  = maxX + objs[j].pos.x;
        boxArray[j].min.z  = minZ + objs[j].pos.z;
        boxArray[j].max.z  = maxZ + objs[j].pos.z;
    }
}

int boundCheck(float minX, float maxX, float minZ, float maxZ) {
    for (int i=0; i<boxes; i++) {
        // todo find out why the difference in x and z
        if (    (maxX+1) > boxArray[i].min.x &&
                (minX-1) < boxArray[i].max.x &&
                (maxZ+2.5f) > boxArray[i].min.z &&
                (minZ-2.5f) < boxArray[i].max.z ) {
            if (i==6)
                return 12;
            else
                return false;
        }
    }
    return wallCheck(minX-2, maxX+2, minZ-2, maxZ+2);
}

void wallsCreate(WALL *tmpWall, int numWalls) {   
    wallArray = tmpWall;
    walls = numWalls;
}

bool wallCheck(float minX, float maxX, float minZ, float maxZ) {
    for (int i=0; i<walls; i++) {
        if (maxX > wallArray[i].min.x && minX < wallArray[i].max.x
            && maxZ > wallArray[i].min.z && minZ < wallArray[i].max.z)
            return false;
    }
    return true;
}
