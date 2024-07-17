#include "SOIL2/SOIL2.h"
#include <cstdio>
#include <cmath>
#include <glut.h>

// Global rotation angles for the planets and their own rotation
float revolutionAngle = 0.0f;  // Angle for the revolution around the Sun
float mercuryRotationAngle = 0.0f;
float venusRotationAngle = 0.0f;
float earthRotationAngle = 0.0f;
float marsRotationAngle = 0.0f;

// Frame rate parameters
const int TARGET_FPS = 60;
const float FRAME_DURATION = 1.0f / TARGET_FPS;

// Function to load texture
void loadTexture(GLuint* texture, const char* path) {
    *texture = SOIL_load_OGL_texture(
        path,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS
    );

    if (*texture == 0) {
        printf("[Texture loader] \"%s\" failed to load! SOIL error: '%s'\n", path, SOIL_last_result());
    }
}

// Planet class
class Planet {
public:
    float radius;
    float distanceFromSun;
    GLuint textureID;       // Texture ID
    float rotationSpeed;    // Rotation speed for the planet's own axis

    Planet(float radius, float distance, float rotationSpeed)
        : radius(radius), distanceFromSun(distance), textureID(0), rotationSpeed(rotationSpeed) {}

    void loadTexture(const char* filename) {
        ::loadTexture(&textureID, filename);
    }

    virtual void draw() {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glEnable(GL_TEXTURE_2D);
        GLUquadric* quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE);
        gluSphere(quad, radius, 32, 32);
        gluDeleteQuadric(quad);
        glDisable(GL_TEXTURE_2D);
    }

    void drawAtPosition(float revolutionAngle, float rotationAngle) {
        glPushMatrix();
        glRotatef(revolutionAngle, 0.0, 1.0, 0.0);   // Rotate around the Sun
        glTranslatef(distanceFromSun, 0.0, 0.0);    // Position relative to the Sun
        glRotatef(rotationAngle, 0.0, 1.0, 0.0);    // Rotate around own axis
        draw();
        glPopMatrix();
    }

    void updateRotation() {
        rotationSpeed += 0.1f;  // Increment rotation speed
    }
};

// Sun class, derived from Planet
class Sun : public Planet {
public:
    Sun(float radius, float distance) : Planet(radius, distance, 0.0f) {}  

    // Override draw method to customize appearance
    void draw() override {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glEnable(GL_TEXTURE_2D);
        GLUquadric* quad = gluNewQuadric();
        GLfloat emission[] = { 1.0, 1.0, 0.0, 1.0 };  // Yellowish emission for the Sun
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        gluQuadricTexture(quad, GL_TRUE);
        gluSphere(quad, radius, 32, 32);
        gluDeleteQuadric(quad);
        glDisable(GL_TEXTURE_2D);
    }

    void drawAtPosition(float rotationAngle) {
        glPushMatrix();
        glRotatef(rotationAngle, 0.0, 1.0, 0.0);
        draw();
        glPopMatrix();
    }
};

// Planets initialization
Sun sun(2.0f, 0.0f);        // The Sun is a special case
Planet mercury(0.5f, 3.0f, 0.13f);
Planet venus(0.7f, 5.0f, 0.12f);
Planet earth(0.8f, 7.0f, 1.0f);
Planet mars(0.9f, 8.0f, 0.1f);

void initialize() {
    glClearColor(0.0, 0.0, 0.0, 1.0);       // Set clear color to black
    glEnable(GL_DEPTH_TEST);                // Enable depth testing
    glEnable(GL_LIGHTING);                  // Enable lighting
    glEnable(GL_LIGHT0);                    // Enable light source 0
    glEnable(GL_COLOR_MATERIAL);            // Enable material coloring based on glColor

    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };  // Light position (directional)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glViewport(0, 0, 1024, 768);            // Set viewport size
    glMatrixMode(GL_PROJECTION);            // Switch to projection matrix mode
    glLoadIdentity();                       // Load identity matrix
    gluPerspective(45.0, 1024.0 / 768.0, 1.0, 100.0);  // Set perspective projection

    glMatrixMode(GL_MODELVIEW);             // Switch to modelview matrix mode

    // Load textures after initializing OpenGL context
    sun.loadTexture("textures/sun.jpg");
    mercury.loadTexture("textures/mercury.jpg");
    venus.loadTexture("textures/venus.jpg");
    earth.loadTexture("textures/earth.jpg");
    mars.loadTexture("textures/mars.jpg");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers
    glLoadIdentity();                                   // Load identity matrix
    gluLookAt(10.0, 8.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // Set camera position and orientation

    // Draw each planet at its current position and rotation
    sun.drawAtPosition(revolutionAngle);
    mercury.drawAtPosition(revolutionAngle * 4.15f, mercuryRotationAngle);
    venus.drawAtPosition(revolutionAngle * 1.62f, venusRotationAngle);
    earth.drawAtPosition(revolutionAngle * 1.0f, earthRotationAngle);
    mars.drawAtPosition(revolutionAngle * 0.53f, marsRotationAngle);

    glutSwapBuffers();  // Swap the front and back buffers (double buffering)
}

void timer(int value) {
    revolutionAngle += 1.0f;                            // Increment revolution angle for all planets
    mercuryRotationAngle += mercury.rotationSpeed ;  // Update rotation angle for Mercury
    venusRotationAngle += venus.rotationSpeed ;      // Update rotation angle for Venus
    earthRotationAngle += earth.rotationSpeed ;      // Update rotation angle for Earth
    marsRotationAngle += mars.rotationSpeed ;         // Update rotation angle for Mars
    
    printf("%f", earthRotationAngle);


    glutPostRedisplay();                                // Request a redraw
    glutTimerFunc(1000 / TARGET_FPS, timer, 0);         // Schedule next update
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                              // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Set display mode
    glutInitWindowSize(1024, 768);                      // Set window size
    glutCreateWindow("Solar System");                   // Create window with title

    initialize();                                       // Initialize OpenGL context and textures

    glutDisplayFunc(display);                           // Register display function
    glutTimerFunc(0, timer, 0);                          // Start timer function for animation

    glutMainLoop();                                     // Enter GLUT main loop

    return 0;
}
