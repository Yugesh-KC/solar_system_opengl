#include "SOIL2/SOIL2.h"
#include <cstdio>
#include <cmath>
#include <glut.h>
#include "planets.h"
bool showOrbits = true;
// Global rotation angles for the planets and their own rotation
float revolutionAngle = 0.0f;
float sunRotationAngle = 0.0f;
float mercuryRotationAngle = 0.0f;
float venusRotationAngle = 0.0f;
float earthRotationAngle = 0.0f;
float marsRotationAngle = 0.0f;
float moonRotationAngle = 0.0f;

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^no need to change this block^^^^^^^^^^^^^^^^^^^^^^^^^^



// Revolution speeds
float mercuryRevolutionSpeed = 3.0f;
float venusRevolutionSpeed = 2.0f;
float earthRevolutionSpeed = 1.0f;
float marsRevolutionSpeed = 4.0f;
float moonRevolutionSpeed = 7.0f;
float revolutionSpeed = 2.2f;

// Rotation speeds
float sunRotationSpeed = 1.0f;
float mercuryRotationSpeed = 2.13f;
float venusRotationSpeed = 2.12f;
float earthRotationSpeed = 1.0f;
float marsRotationSpeed = 1.1f;
float moonRotationSpeed = 1.3f;

// Sizes and distances
float sunRadius = 1.0f;
float mercuryRadius = 0.1f;
float venusRadius = 0.2f;
float earthRadius = 0.3f;
float marsRadius = 0.3f;
float moonRadius = 0.05f;

float mercuryDistance = 3.0f;
float venusDistance = 5.0f;
float earthDistance = 7.0f;
float marsDistance = 8.0f;
float moonDistance = 2.0f;  // Distance from Earth

// Frame rate parameters
const int TARGET_FPS = 60;
const float FRAME_DURATION = 1.0f / TARGET_FPS;

// Planets initialization
Sun sun(sunRadius, 0.0f, sunRotationSpeed);
Planet mercury(mercuryRadius, mercuryDistance, mercuryRotationSpeed);
Planet venus(venusRadius, venusDistance, venusRotationSpeed);
Planet earth(earthRadius, earthDistance, earthRotationSpeed);
Planet mars(marsRadius, marsDistance, marsRotationSpeed);
Moon moon(moonRadius, moonDistance, moonRotationSpeed);


void toggleOrbitsVisibility() {
    showOrbits = !showOrbits; // Toggle the state
}
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'o':
    case 'O':
        toggleOrbitsVisibility();
        glutPostRedisplay(); // Request a redraw to reflect the change
        break;
    }
}

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
    moon.loadTexture("textures/mercury.jpg");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers
    glLoadIdentity();                                   // Load identity matrix
    gluLookAt(10.0, 8.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  // Set camera position and orientation

    // Draw each planet at its current position and rotation
    GLfloat noEmission[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    if (showOrbits) {
        // Draw orbits with specific material properties
        mercury.drawOrbit();
        venus.drawOrbit();
        earth.drawOrbit();
        mars.drawOrbit();
        moon.drawOrbit(earth.distanceFromSun, earth.x, earth.z);
    }
    
    sun.drawAtPosition(sunRotationAngle);
    mercury.drawAtPosition(revolutionAngle * mercuryRevolutionSpeed, mercuryRotationAngle);
    venus.drawAtPosition(revolutionAngle * venusRevolutionSpeed, venusRotationAngle);
    earth.drawAtPosition(revolutionAngle * earthRevolutionSpeed, earthRotationAngle);
    mars.drawAtPosition(revolutionAngle * marsRevolutionSpeed, marsRotationAngle);
    moon.drawAtPosition(earth.x, earth.y, earth.z, revolutionAngle * moonRevolutionSpeed, moonRotationAngle);

    glutSwapBuffers();  // Swap the front and back buffers (double buffering)
}

void timer(int value) {
    revolutionAngle += 1.0f;                          // Increment revolution angle for all planets
    sunRotationAngle += sunRotationSpeed;
    mercuryRotationAngle += mercuryRotationSpeed;    // Update rotation angle for Mercury
    venusRotationAngle += venusRotationSpeed;        // Update rotation angle for Venus
    earthRotationAngle += earthRotationSpeed;        // Update rotation angle for Earth
    marsRotationAngle += marsRotationSpeed;          // Update rotation angle for Mars
    moonRotationAngle += moonRotationSpeed;          // Update rotation angle for Moon

    printf("earth %f %f %f \n", earth.x, earth.y, earth.z);

    glutPostRedisplay();                              // Request a redraw
    glutTimerFunc(1000 / TARGET_FPS, timer, 0);       // Schedule next update
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                              // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Set display mode
    glutInitWindowSize(1024, 768);                      // Set window size
    glutCreateWindow("Solar System");                   // Create window with title

    initialize();                                       // Initialize OpenGL context and textures

    glutDisplayFunc(display);                           // Register display function
    glutTimerFunc(0, timer, 0);                         // Start timer function for animation
    glutKeyboardFunc(keyboard);
    glutMainLoop();                                     // Enter GLUT main loop

    return 0;
}