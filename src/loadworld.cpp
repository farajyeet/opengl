#include "main.h"

OBJECT *Objects;
OBJECT *wallObject;
WALL *Walls;
int numObjects;
int numWalls;

float world_x = 0;
float world_z = 0;
float world_angle = 0;

typedef struct tagVERTEX                        
{
    float x, y, z;
} VERTEX;

typedef struct tagFACE {
    int v;                          // 3D vertices
} FACE;

float getWorldX() {
    return world_x;
}
float getWorldZ() {
    return world_z;
}
float getWorldAngle() {
    return world_angle;
}

short readstr(FILE *f, char *string) {                 
    do
    {
        fgets(string, 255, f);
        if (feof(f)) return 1;
    } while ((string[0] == '#') || (string[0] == '\n'));
    return 0;
}

OBJECT LoadMtl(char* mtlfile, OBJECT tmpobj) {

    FILE *filein;                          
    filein = fopen(mtlfile, "rt");                
    tmpobj.mtl = new WMaterial[tmpobj.numMtl];
    bool content = false;
    for (int i=0; i<tmpobj.numMtl; i++) {
        float ar=0, ag=0, ab=0, dr=0, dg=0, db=0, sr=0, sg=0, sb=0;
        float ns=0;
        while (true) {
            char oneline[255];
            char t1[255], t2[255], t3[255], t4[255];
            float f2=0, f3=0, f4=0;
            if (readstr(filein,oneline)) break;
            content = true;
            int num = sscanf(oneline, "%s %f %f %f", t1, &f2, &f3, &f4);
            if (strcmp(t1,"newmtl") == 0) {
                sscanf(oneline, "%s %s", t2, t3);
                tmpobj.mtl[i].name = new char[255];
                strcpy(tmpobj.mtl[i].name, t3);
            }
            if (strcmp(t1,"Ka") == 0) {
                ar = f2;
                ag = f3;
                ab = f4;
            }
            if (strcmp(t1,"Kd") == 0) {
                dr = f2;
                dg = f3;
                db = f4;
            }
            if (strcmp(t1,"Ks") == 0) {
                sr = f2;
                sg = f3;
                sb = f4;
            }
            if (strcmp(t1,"Ns") == 0) {
                ns = f2;
            }
            if (strcmp(t1,"illum") == 0) {
                break;
            }
        }
        tmpobj.mtl[i].ambient[0] = ar;
        tmpobj.mtl[i].ambient[1] = ag;
        tmpobj.mtl[i].ambient[2] = ab;
        tmpobj.mtl[i].ambient[3] = 1;
        tmpobj.mtl[i].diffuse[0] = dr;
        tmpobj.mtl[i].diffuse[1] = dg;
        tmpobj.mtl[i].diffuse[2] = db;
        tmpobj.mtl[i].diffuse[3] = 1;
        tmpobj.mtl[i].specular[0] = sr;
        tmpobj.mtl[i].specular[1] = sg;
        tmpobj.mtl[i].specular[2] = sb;
        tmpobj.mtl[i].specular[3] = 1;
        tmpobj.mtl[i].shininess[0] = ns;
        tmpobj.ismtl = content;
    }
    fclose(filein);
    return tmpobj;
}

OBJECT LoadObj(char* objfile, int bits) {

    FILE *filein;                           
    filein = fopen(objfile, "rt");                
    
    char* mtllib;
    char oneline[255];
    char tmp1[255];
    char tmp2[255];
    readstr(filein,oneline);
    sscanf(oneline, "%s %s", tmp1, tmp2);
    if (strcmp(tmp1, "mtllib") == 0) {
        mtllib = tmp2;
    }
    
    int numvert = 10000;
    char x[10], y[10], z[10];
    int numface = 4*10000;

    VERTEX* vertex = new VERTEX[numvert];
    int vertloop = 0;
    while (vertloop < numvert)        
    {
        if (readstr(filein,oneline)) break;
        char tmp[255];
        sscanf(oneline, "%s ", tmp);
        if (strcmp(tmp, "v") == 0) {
            sscanf(oneline, "%s %s %s %s", tmp, &x, &y, &z);

            vertex[vertloop].x = atof(x);
            vertex[vertloop].y = atof(y);
            vertex[vertloop].z = atof(z);
            vertloop++;
        }

    }

    fclose(filein);
    filein = fopen(objfile, "rt"); 

    VERTEX* normal = new VERTEX[numvert];
    int normloop = 0;
    while (normloop < numvert) {
        if (readstr(filein,oneline)) break;
        char tmp[255];
        sscanf(oneline, "%s ", tmp);
        if (strcmp(tmp, "vn") == 0) {
            sscanf(oneline, "%s %s %s %s", tmp, &x, &y, &z);
            normal[normloop].x = atof(x);
            normal[normloop].y = atof(y);
            normal[normloop].z = atof(z);
            normloop++;
        }
    }
    
    fclose(filein);
    filein = fopen(objfile, "rt"); 
    
    int numgroup = 100; 
    FACE** face;
    face = new FACE*[100];
    for (int i=0; i<100; i++) face[i] = new FACE[40000]; 
    int grouploop = 0;
    int faceloop[numgroup];
    bool loop = true;
    char** mtls;
    mtls = new char*[100]; 
    for (int i=0; i<100; i++) mtls[i] = new char[256];
    bool skip = false;
    while (grouploop < numgroup && loop) {
        faceloop[grouploop] = 0;
        
        char v1[10], v2[10], v3[10], v4[10];
        if (skip) skip = false;
        else {
            if (readstr(filein,oneline)) { break; }
        }
        char tmp[255];
        char tmp2[255];
        sscanf(oneline, "%s %s", tmp, tmp2);
        if (strcmp(tmp, "usemtl") == 0) {
            strcpy(mtls[grouploop], tmp2);
        }
        else if (strcmp(tmp, "f") == 0) {
            skip = true;
            while (faceloop[grouploop] < numface) {
                if (skip) skip = false;
                else {
                    if (readstr(filein,oneline)) { loop=false; break; }
                }
                sscanf(oneline, "%s %s", tmp, tmp2);
                if (strcmp(tmp, "f") == 0) {
                    if (sscanf(oneline, "%s %s %s %s %s", tmp, &v1, &v2, &v3, &v4) == 4) {
                        strcpy (v4, v3);
                    }
                    int test1, test2;
                    if (sscanf(v1, "%d//%d", &test1, &test2) == 2) {
                        face[grouploop][faceloop[grouploop]].v = test1;
                    }
                    else {
                        face[grouploop][faceloop[grouploop]].v = atoi(v1);
                    }
                    faceloop[grouploop]++;
                    if (sscanf(v2, "%d//%d", &test1, &test2) == 2) {
                        face[grouploop][faceloop[grouploop]].v = test1;
                    }
                    else {
                        face[grouploop][faceloop[grouploop]].v = atoi(v2);
                    }
                    faceloop[grouploop]++;
                    if (sscanf(v3, "%d//%d", &test1, &test2) == 2) {
                        face[grouploop][faceloop[grouploop]].v = test1;
                    }
                    else {
                        face[grouploop][faceloop[grouploop]].v = atoi(v3);
                    }
                    faceloop[grouploop]++;
                    if (sscanf(v4, "%d//%d", &test1, &test2) == 2) {
                        face[grouploop][faceloop[grouploop]].v = test1;
                    }
                    else {
                        face[grouploop][faceloop[grouploop]].v = atoi(v4);
                    }
                    faceloop[grouploop]++;
                    for (int i=0; i<9; i++) {
                        v4[i] = '0';
                    }
                    v4[9] = '\0'; 
                }
                else if (faceloop[grouploop] != 0) {
                    if (strcmp(tmp, "usemtl") == 0) {                                   skip = true;
                    }
                    break;
                }
            }
            grouploop++;
        }
    }
    fclose(filein);

    OBJECT tmpobj;
    tmpobj.numMtl = bits;
    tmpobj.numGroups = grouploop;
    tmpobj.numVertices = vertloop;
    tmpobj.numNormals = normloop;
    printf("*********** Groups: %d Vertices: %d Normals: %d\n", tmpobj.numGroups, tmpobj.numVertices, tmpobj.numNormals);
    tmpobj.Vertices = new WVector[tmpobj.numVertices];
    tmpobj.Normals = new WVector[tmpobj.numNormals];
    
    for (int i=0; i<tmpobj.numVertices; i++) {
        tmpobj.Vertices[i].x = vertex[i].x;
        tmpobj.Vertices[i].y = vertex[i].y;
        tmpobj.Vertices[i].z = vertex[i].z; 
    }
    for (int i=0; i<tmpobj.numNormals; i++) {
        tmpobj.Normals[i].x = normal[i].x;
        tmpobj.Normals[i].y = normal[i].y;
        tmpobj.Normals[i].z = normal[i].z;
    }
    
    
    tmpobj.groups = new WGroup[tmpobj.numGroups];
    for (int j=0; j<tmpobj.numGroups; j++) {
        tmpobj.groups[j].numIndices = faceloop[j];
        tmpobj.groups[j].Indices = new GLuint[tmpobj.groups[j].numIndices];
        tmpobj.groups[j].mtlname = mtls[j];

        for (int i=0; i<tmpobj.groups[j].numIndices; i++) {
            tmpobj.groups[j].Indices[i] = face[j][i].v - 1;
        }
    }
    
    tmpobj = LoadMtl(tmp2, tmpobj);

    if (tmpobj.ismtl) {
        for (int j=0; j<tmpobj.numGroups; j++) {
            tmpobj.groups[j].mtl = -1;
            for (int c=0; c<tmpobj.numMtl; c++) {
                if (strcmp(tmpobj.groups[j].mtlname, tmpobj.mtl[c].name) == 0) {
                    tmpobj.groups[j].mtl = c;
                }
            }
        }
    }


    return tmpobj;
}

OBJECT CreateWall(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
    OBJECT tmpObj;
    tmpObj.numGroups = 1;
    tmpObj.groups = new WGroup[1];
    tmpObj.groups[0].numIndices = 8;
    tmpObj.numVertices = 4;
    tmpObj.Vertices = new WVector[tmpObj.numVertices];
    tmpObj.Vertices[0].x = maxX;
    tmpObj.Vertices[0].y = maxY;
    tmpObj.Vertices[0].z = maxZ;
    tmpObj.Vertices[1].x = minX;
    tmpObj.Vertices[1].y = maxY;
    tmpObj.Vertices[1].z = minZ;
    tmpObj.Vertices[2].x = minX;
    tmpObj.Vertices[2].y = minY;
    tmpObj.Vertices[2].z = minZ;
    tmpObj.Vertices[3].x = maxX;
    tmpObj.Vertices[3].y = minY;
    tmpObj.Vertices[3].z = maxZ;
//    tmpObj.Normals = new GLfloat[tmpObj.numVertices*3];
//     tmpObj.Normals[0] = 0;
//     tmpObj.Normals[1] = -1;
//     tmpObj.Normals[2] = 0;
//     tmpObj.Normals[3] = 0;
//     tmpObj.Normals[4] = -1;
//     tmpObj.Normals[5] = 0;
//     tmpObj.Normals[6] = 0;
//     tmpObj.Normals[7] = -1;
//     tmpObj.Normals[8] = 0;
//     tmpObj.Normals[9] = 0;
//     tmpObj.Normals[10] = -1;
//     tmpObj.Normals[11] = 0;
    tmpObj.groups[0].Indices = new GLuint[tmpObj.groups[0].numIndices];
    tmpObj.groups[0].Indices[0] = 0;
    tmpObj.groups[0].Indices[1] = 1;
    tmpObj.groups[0].Indices[2] = 2;
    tmpObj.groups[0].Indices[3] = 3;
    tmpObj.groups[0].Indices[4] = 3;
    tmpObj.groups[0].Indices[5] = 2;
    tmpObj.groups[0].Indices[6] = 1;
    tmpObj.groups[0].Indices[7] = 0;
    return tmpObj;
}

void DrawWorld() {
    glLoadIdentity(); you_compensate();
    glBegin( GL_QUADS );
    glColor3fv( green );
    glVertex3f( 10, 0, -10);
    glColor3fv( red );
    glVertex3f(-10, 0, -10);
    glColor3fv( blue );
    glVertex3f( -10, 0, 10);
    glColor3fv( yellow );
    glVertex3f( 10, 0, 10);
    glEnd( );
    for (int i=0; i<numObjects; i++) {
        glLoadIdentity(); you_compensate();
        glTranslatef( Objects[i].pos.x, Objects[i].pos.y, Objects[i].pos.z );
        glRotatef(Objects[i].rot.x, 1, 0, 0);
        glRotatef(Objects[i].rot.y, 0, 1, 0);
        glRotatef(Objects[i].rot.z, 0, 0, 1);
        
        if (!Objects[i].ismtl) {
            GLfloat a[] = {0,0,0,1};
            GLfloat d[] = {0.8f,0.8f,0.8f,1};
            GLfloat s[] = {0.5f,0.5f,0.5f};
            GLfloat sh[] = {100};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, d);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, sh);
            glColor3fv( blue );
        }

        glVertexPointer( 3, GL_FLOAT, sizeof(WVector), Objects[i].Vertices);
        glNormalPointer( GL_FLOAT, sizeof(WVector), Objects[i].Normals );
        
        for (int j=0; j<Objects[i].numGroups; j++) {
            if (Objects[i].ismtl) {
                if (Objects[i].groups[j].mtl != -1) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Objects[i].mtl[Objects[i].groups[j].mtl].ambient);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Objects[i].mtl[Objects[i].groups[j].mtl].diffuse);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Objects[i].mtl[Objects[i].groups[j].mtl].specular);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Objects[i].mtl[Objects[i].groups[j].mtl].shininess);
                    glColor3fv(Objects[i].mtl[Objects[i].groups[j].mtl].diffuse);
                }
            }
            glDrawElements( GL_QUADS, Objects[i].groups[j].numIndices, GL_UNSIGNED_INT, Objects[i].groups[j].Indices );
        }
    }
    
    glLoadIdentity(); you_compensate();
    for (int i=0; i<numWalls; i++) {
        glColor3fv( green );
        glVertexPointer( 3, GL_FLOAT, sizeof(WVector), wallObject[i].Vertices);
        glDrawElements( GL_QUADS, wallObject[i].groups[0].numIndices, GL_UNSIGNED_INT, wallObject[i].groups[0].Indices );
    }
    return;
}

void SetupWorld(char* worldfile) {
    FILE *filein;
    filein = fopen(worldfile, "rt");
    
    char oneline[255];
    char tmp1[255];
    char tmp2[255];
    readstr(filein,oneline);
    sscanf(oneline, "%s %s", tmp1, tmp2);
    if (!strcmp(tmp1, "Var") == 0) {
        printf("Sorry, this filetype/version is not supported.");
        exit(1);
    }
    while (true) {
        readstr(filein,oneline);
        sscanf(oneline, "%s %s", tmp1, tmp2);
        if (strcmp(tmp1, "numWalls") == 0) numWalls = atoi(tmp2);
        else if (strcmp(tmp1, "numObjects") == 0) numObjects = atoi(tmp2);
        else if (strcmp(tmp1, "X") == 0) world_x = atoi(tmp2);
        else if (strcmp(tmp1, "Z") == 0) world_z = atoi(tmp2);
        else if (strcmp(tmp1, "Angle") == 0) world_angle = atoi(tmp2);
        else if (strcmp(tmp1, "endVar") == 0) break;
    }
    
    while (true) {
        if (readstr(filein,oneline)) break;
        sscanf(oneline, "%s", tmp1);
        if (strcmp(tmp1, "Objects") == 0) {
            Objects = new OBJECT[numObjects];
            int i = 0;
            while (i < numObjects) {
                if (readstr(filein,oneline)) break;
                sscanf(oneline, "%s", tmp1);
                if (strcmp(tmp1, "Object") == 0) {
                    float x=0,y=0,z=0,rx=0,ry=0,rz=0;
                    int bits=0;
                    sscanf(oneline, "%s %s %f %f %f %f %f %f %d", tmp1, tmp2, &x, &y, &z, &rx, &ry, &rz, &bits);
                    Objects[i] = LoadObj(tmp2, bits);
                    Objects[i].pos.x = x;
                    Objects[i].pos.y = y;
                    Objects[i].pos.z = z;
                    Objects[i].rot.x = rx;
                    Objects[i].rot.y = ry;
                    Objects[i].rot.z = rz;
                    i++;
                }
                else if (strcmp(tmp1, "endObjects") == 0) { break; }
            }
        }
        else if (strcmp(tmp1, "Walls") == 0) {
            Walls = new WALL[numWalls];
            int i = 0;
            printf("numWalls: %d\n", numWalls);
            while (i < numWalls) {
                if (readstr(filein,oneline)) break;
                sscanf(oneline, "%s", tmp1);
                if (strcmp(tmp1, "Wall") == 0) {
                    float lx=0,ly=0,lz=0,bx=0,by=0,bz=0;
                    sscanf(oneline, "%s %f %f %f %f %f %f", tmp1, &lx, &ly, &lz, &bx, &by, &bz);
                    Walls[i].min.x = lx;
                    Walls[i].min.y = ly;
                    Walls[i].min.z = lz;
                    Walls[i].max.x = bx;
                    Walls[i].max.y = by;
                    Walls[i].max.z = bz;
                    printf("Wall %d: %f %f %f - %f %f %f \n", i, Walls[i].min.x, Walls[i].min.y, Walls[i].min.z, Walls[i].max.x, Walls[i].max.y, Walls[i].max.z);
                    i++;
                }
                else if (strcmp(tmp1, "endWalls") == 0) { break; }
            }
        }
    }

    wallObject = new OBJECT[numWalls];
    for (int i=0; i<numWalls; i++) {
        wallObject[i] = CreateWall(Walls[i].min.x, Walls[i].min.y, Walls[i].min.z, Walls[i].max.x, Walls[i].max.y, Walls[i].max.z);
    }

    boundCreate(Objects,numObjects);
    wallsCreate(Walls,numWalls);
}

