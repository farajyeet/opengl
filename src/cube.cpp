#include "main.h"

// TODO turn this into a special objects section

float cube_angle = 0.0f;

void draw_cube() {
    glRotatef( cube_angle, 1.0f, 0.0f, 0.0f );
     if( ++cube_angle > 360.0f ) {
         cube_angle = 0.0f;
     }

    GLfloat cube[] = {
        // front
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        // back
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        // left
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        // right
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        // top
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        // bottom
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f
    };
    
    glVertexPointer(3, GL_FLOAT, 0, cube);
   glColor3fv( red );
   glNormal3f(0, 0, 1);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glNormal3f(0, 0, -1);
   glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
   glColor3fv( green );
   glNormal3f(-1, 0, 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
   glNormal3f(1, 0, 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
   glColor3fv( blue );
   glNormal3f(0, 1, 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
   glNormal3f(0, -1, 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
}
