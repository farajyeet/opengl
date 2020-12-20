#include "main.h"

float you_angle = 0.0f;
float you_x = 0;
float you_z = 0;
float you_vel_angle = 0.0f;
float you_vel_x = 0.0f;
float you_vel_z = 0.0f;
float you_velocity = 10;
float you_turn = 0;
float you_dir = 0;

float piover180 = 0.01745329251994329577;

bool light = true;
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };

static GLint T0     = 0;
static GLint Frames = 0;
GLfloat fps = 500;

bool debug = false;

void quit_app( int code ) {
    SDL_Quit( );
    exit( code );
}

void handle_keydown( SDL_keysym* keysym ) {
    switch (keysym->sym) {
        case SDLK_UP:
            you_dir = -1;
            break;
        case SDLK_DOWN:
            you_dir = 1;
            break;
        case SDLK_LEFT:
            you_turn = -1;
            break;
        case SDLK_RIGHT:
            you_turn = 1;
            break;
        case SDLK_KP_PLUS:
            you_velocity++;
            break;
        case SDLK_KP_MINUS:
            you_velocity--;
            break;
        case SDLK_q:
        case SDLK_ESCAPE:
            quit_app( 0 );
            break;
        case SDLK_l:
            light = !light;
            if ( !light )
            glDisable( GL_LIGHTING );
            else
            glEnable( GL_LIGHTING );
            break;
        case SDLK_d:
            debug = !debug;
            break;
        default:
            break;
    }
}

void handle_keyup( SDL_keysym* keysym ) {
    switch (keysym->sym) {
        case SDLK_UP:
        case SDLK_DOWN:
            you_dir = 0;
            break;
        case SDLK_LEFT:
        case SDLK_RIGHT:
            you_turn = 0;
            break;
    }
}


void process_events( void ) {
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
        case SDL_KEYDOWN:
            handle_keydown( &event.key.keysym );
            break;
        case SDL_KEYUP:
            handle_keyup( &event.key.keysym );
            break;
        case SDL_QUIT:
            quit_app( 0 );
            break;
        }

    }
}

void you_compensate( void ) {
    glRotatef( you_angle, 0.0f, 1.0f, 0.0f );
    glTranslatef( -you_x, -1, -you_z);
}

void draw_screen( void ) {
    // step
    you_angle += ( 10 * you_velocity * you_turn ) / fps;
    
    float you_x_new = you_x - ( (float)sin(you_angle*piover180) * you_velocity * you_dir ) / fps;
    float you_z_new = you_z + ( (float)cos(you_angle*piover180) * you_velocity * you_dir ) / fps;
    int joel = boundCheck(you_x_new, you_x_new, you_z_new, you_z_new); // todo change this to avoid repetition
    if (joel == 1) {
        you_x = you_x_new;
        you_z = you_z_new;
    }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity(); you_compensate();
    glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
    glEnable ( GL_COLOR_MATERIAL ) ;   
    glLoadIdentity(); you_compensate();
    glTranslatef( 0.0f, 3.0f, 0.0f );
    draw_cube();
    
    DrawWorld();
    
    if (debug) {
        glLoadIdentity(); glTranslatef(0, 0, -1);
        glColor3f(1, 1, 1);
        glRasterPos2f(-0.7f, -0.5f);
        glPrint("X: %f   Z: %f   Angle: %f   Velocity: %f", you_x, you_z, you_angle, you_velocity);
        glRasterPos2f(-0.7f, 0.5f);
        glPrint("FPS: %f", fps);
    }
    
    if (joel == 12) {
        glLoadIdentity(); glTranslatef(0, 0, -1);
        glColor3f(1, 1, 1);
        glRasterPos2f(-0.7f, 0.5f);
        glPrint("the cake,");
        glRasterPos2f(-0.7f, 0.4f);
        glPrint("is a lie!!");
    }
    
    Frames++;
    GLint t = SDL_GetTicks();
    if (t - T0 >= 100) {
        GLfloat seconds = (t - T0) / 1000.0;
        fps = Frames / seconds;
        T0 = t;
        Frames = 0;
    }
    
    SDL_GL_SwapBuffers( );
}

static void setup_opengl( int width, int height ) {
    float ratio = (float) width / (float) height;
    glShadeModel( GL_SMOOTH );
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0, 0, 0, 0 );
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
    glEnable(GL_LIGHT1);
    glLoadIdentity( );
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );
    buildFont();
    return;
}

int main(int argc, char* argv[]) {
    bool fullscreen = false;
    char* world = "world";
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-win") == 0) fullscreen = false;
        if (strcmp(argv[i], "-f") == 0) fullscreen = true;
        if (strcmp(argv[i], "-w") == 0)
            world = argv[i + 1];
        if (strcmp(argv[i], "-d") == 0) debug = true;
    }
    SetupWorld(world);
    you_x = getWorldX();
    you_z = getWorldZ();
    you_angle = getWorldAngle();

    const SDL_VideoInfo* info = NULL;
    int width = 0;
    int height = 0;
    int bpp = 0;
    int flags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Video initialization failed: %s\n",
            SDL_GetError());
        quit_app(1);
    }

    info = SDL_GetVideoInfo();
    if (!info) {
        fprintf(stderr, "Video query failed: %s\n",
            SDL_GetError());
        quit_app(1);
    }

    width = 640;
    height = 480;
    bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    flags = SDL_OPENGL;
    if (fullscreen)
        flags |= SDL_FULLSCREEN;
    if (SDL_SetVideoMode(width, height, bpp, flags) == 0) {
        fprintf(stderr, "Video mode set failed: %s\n",
            SDL_GetError());
        quit_app(1);
    }

    setup_opengl(width, height);
    bool init = true;
    while (1) {
        process_events();
        draw_screen();
        if (init) { glEnable(GL_LIGHTING); init = false; }
    }

    return 0;
};
