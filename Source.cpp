#include "SOIL2/SOIL2/SOIL2.h"  // Ensure this path is correct for your setup
#include <cstdio>         // For printf
#include <glut.h>

// Rotation angles for the revolving planets and their own rotation
float angle = 0.0f;
float mercuryRotation = 0.0f;
float venusRotation = 0.0f;
float earthRotation = 0.0f;
float marsRotation = 0.0f;

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
    float distance;
    GLuint textureID;  // Texture ID
    float rotationSpeed;  // Rotation speed for the planet's own axis
    bool isCentral;

    Planet(float rad, float dist, float speed, bool central = false) : radius(rad), distance(dist), textureID(0), rotationSpeed(speed), isCentral(central) {}

    void loadTexture(const char* filename) {
        ::loadTexture(&textureID, filename);
    }

    void draw() {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glEnable(GL_TEXTURE_2D);
        GLUquadric* quad = gluNewQuadric();
        if (isCentral)
        {
            GLfloat emission[] =  { 0.5, 0.5, 0.5, 1.0 };
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);

        }
        gluQuadricTexture(quad, GL_TRUE);
        gluSphere(quad, radius, 32, 32);
        gluDeleteQuadric(quad);
        glDisable(GL_TEXTURE_2D);
    }

    void drawAtPosition(float revolutionAngle, float rotationAngle) {
        glPushMatrix();
        glRotatef(revolutionAngle, 0.0, 1.0, 0.0);
        glTranslatef(distance, 0.0, 0.0);
        glRotatef(rotationAngle, 0.0, 1.0, 0.0);  // Rotate around own axis
        draw();
        glPopMatrix();
    }

    void updateRotation() {
        rotationSpeed += 0.1f;  // Increment rotation angle
    }
};

// Planets
Planet centralPlanet(2.0f, 0.0f, 0.0f,true);  // The central planet does not have its own rotation
Planet Mercury(0.5f, 3.0f, 0.13f);
Planet Venus(0.7f, 5.0f, 0.12f);
Planet Earth(0.8f, 7.0f, 0.1f);
Planet Mars(0.9f, 8.0f, 0.1f);

void initialize() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glViewport(0, 0, 1024, 768);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1024.0 / 768.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    // Load textures after initializing OpenGL context
    centralPlanet.loadTexture("textures/sun.jpg");
    Mercury.loadTexture("textures/mercury.jpg");
    Venus.loadTexture("textures/venus.jpg");
    Earth.loadTexture("textures/earth.jpg");
    Mars.loadTexture("textures/mars.jpg");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(10.0, 8.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    centralPlanet.draw();
    Mercury.drawAtPosition(angle * 4.15, mercuryRotation);
    Venus.drawAtPosition(angle * 1.62, venusRotation);
    Earth.drawAtPosition(angle * 1.0, earthRotation);
    Mars.drawAtPosition(angle * 0.53, marsRotation);

    glutSwapBuffers();
}

void idle() {
    angle += 0.1f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }

    mercuryRotation += Mercury.rotationSpeed;
    venusRotation += Venus.rotationSpeed;
    earthRotation += Earth.rotationSpeed;
    marsRotation += Mars.rotationSpeed;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Planet Simulation");

    initialize();  // Initialize OpenGL context before loading textures

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
