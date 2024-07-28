#include "SOIL2/SOIL2.h"
#include <cstdio>
#include <cmath>
#include <glut.h>
#include "planets.h"

bool showOrbits = true;
float addedDistance = 2.0f;

// Global rotation angles for the planets and their own rotation
float revolutionAngle = 0.0f;
float sunRotationAngle = 0.0f;
float mercuryRotationAngle = 0.0f;
float venusRotationAngle = 0.0f;
float earthRotationAngle = 0.0f;
float marsRotationAngle = 0.0f;
float moonRotationAngle = 0.0f;
float jupiterRotationAngle = 0.0f;
float uranusRotationAngle = 0.0f;
float neptuneRotationAngle = 0.0f;
float plutoRotationAngle = 0.0f;

// Camera parameters
float camX = 30.0f, camY = 10.0f, camZ = 5.0f;
float centreX = 0.0f, centreY = 0.0f, centreZ = 0.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

const float movementSpeed = 0.4f;
const float rotationSpeed = 0.04f; // for camera

// Revolution speeds
/*float mercuryRevolutionSpeed = 3.0f;
float venusRevolutionSpeed = 2.0f;
float earthRevolutionSpeed = 1.0f;
float marsRevolutionSpeed = 4.0f;
float moonRevolutionSpeed = 7.0f;
float jupiterRevolutionSpeed = 2.0f;
float uranusRevolutionSpeed = 1.0f;
float neptuneRevolutionSpeed = 0.6f;
float plutoRevolutionSpeed = 0.5f;*/

// Rotation speeds
/*float sunRotationSpeed = 1.0f;
float mercuryRotationSpeed = 2.13f;
float venusRotationSpeed = 2.12f;
float earthRotationSpeed = 1.0f;
float marsRotationSpeed = 1.1f;
float moonRotationSpeed = 1.3f;
float jupiterRotationSpeed = 1.0f;
float uranusRotationSpeed = 1.0f;
float neptuneRotationSpeed = 0.9f;
float plutoRotationSpeed = 0.8f;*/

// Sizes and distances
/*float sunRadius = 1.0f;
float mercuryRadius = 0.1f;
float venusRadius = 0.2f;
float earthRadius = 0.3f;
float marsRadius = 0.3f;
float moonRadius = 0.05f;
float jupiterRadius = 0.5f;
float uranusRadius = 0.3f;
float neptuneRadius = 0.2f;
float plutoRadius = 0.1f;*/

/*float sunRadius = 10.0f;
float mercuryRadius = 0.001631f;
float venusRadius = 0.004046f;
float earthRadius = 0.00004258f;
float marsRadius = 0.002266f;
float moonRadius = 0.001161f;
float jupiterRadius = 0.00046702f;
float uranusRadius = 0.00016949f;
float neptuneRadius = 0.00016457f;
float plutoRadius = 0.000794;*/

const float sunRadius =2.0f;
const float mercuryRadius = 0.035f;
const float venusRadius = 0.087f;
const float earthRadius = 0.091f;
const float moonRadius = 0.025f;
const float marsRadius = 0.049f;
const float jupiterRadius = 0.1004f;
const float saturnRadius = 0.0836f;
const float uranusRadius = 0.364f;
const float neptuneRadius = 0.353f;
const float plutoRadius = 0.017f;

float mercuryDistance = 1.0f;
float venusDistance = 2.0f;
float earthDistance = 3.0f;
float marsDistance = 4.0f;
float moonDistance = 0.1f;  // Distance from Earth
float jupiterDistance = 7.0f;
float saturnDistance = 10.0f;
float uranusDistance = 13.0f;
float neptuneDistance = 15.0f;
float plutoDistance = 18.0f;

// Frame rate parameters
const int TARGET_FPS = 60;
const float FRAME_DURATION = 1.0f / TARGET_FPS;

// Rotation periods
float mercuryPeriod = 58.6f;
float venusPeriod = 243.0f;
float earthPeriod = 1.0f;
float marsPeriod = 1.03f;
float jupitorPeriod = 0.42f;
float saturnPeriod = 0.46f;
float uranusPeriod = 0.71f;
float neptunePeriod = 0.67f;
float plutoPeriod = 6.0f;
float moonPeriod = 27.3f;
float sunPeriod = 25.0f;

/*float mercuryOrbitalPeriod = 88.0f;
float venusOrbitalPeriod = 225.0f;
float earthOrbitalPeriod = 365.0f;
float marsOrbitalPeriod = 687.0f;
float jupitorOrbitalPeriod = 4333.0f;
float saturnOrbitalPeriod = 10756.0f;
float uranusOrbitalPeriod = 30687.0f;
float neptuneOrbitalPeriod = 60190.0f;
float plutoOrbitalPeriod = 90520.0f;
float moonOrbitalPeriod = 27.3f;
float sunOrbitalPeriod = 25.0f; Orbital periods*/


//axialtilt
float mercuryaxialtilt = 0.03f;
float venusaxialtilt = 177.4f;
float earthaxialtilt = 23.44f;
float marsaxialtilt = 25.2f;
float jupitoraxialtilt = 3.1f;
float saturnaxialtilt = 26.7;
float uranusaxialtilt = 97.8f;
float neptuneaxialtilt = 28.3f;
float plutoaxialtilt = 122.53f;
float moonaxialtilt = 27.3f;
float sunaxialtilt = 25.0f;

// Saturn
//float saturnRadius = 0.00038927f;

float saturnringInnerRadius = saturnRadius * 2.3f;
float saturnringOuterRadius = saturnRadius*1.2f;
float saturnRevolutionSpeed = 1.0f;
float saturnRotationAngle = 1.0f;

// Planets initialization
Sun sun(sunRadius, 0.0f, sunPeriod);
Planet mercury(mercuryRadius, mercuryDistance, mercuryPeriod, mercuryaxialtilt);
Planet venus(venusRadius, venusDistance, venusPeriod, venusaxialtilt,true);
Planet earth(earthRadius, earthDistance, earthPeriod, earthaxialtilt);
Planet mars(marsRadius, marsDistance, marsPeriod, marsaxialtilt);
Moon moon(moonRadius, moonDistance, moonPeriod);
RingPlanet saturn(saturnRadius, saturnDistance, saturnPeriod, saturnringInnerRadius, saturnringOuterRadius, saturnaxialtilt);
Planet jupiter(jupiterRadius, jupiterDistance, jupitorPeriod, jupitoraxialtilt);
Planet uranus(uranusRadius, uranusDistance, uranusPeriod, uranusaxialtilt);
Planet neptune(neptuneRadius, neptuneDistance, neptunePeriod, neptuneaxialtilt);
Planet pluto(plutoRadius, plutoDistance, plutoPeriod, plutoaxialtilt);

/*Sun sun(sunRadius, 0.0f, sunPeriod, 0.0f);
Planet mercury(mercuryRadius, mercuryDistance, mercuryPeriod, mercuryOrbitalPeriod);
Planet venus(venusRadius, venusDistance, venusPeriod, venusOrbitalPeriod);
Planet earth(earthRadius, earthDistance, earthPeriod, earthOrbitalPeriod);
Planet mars(marsRadius, marsDistance, marsPeriod, marsOrbitalPeriod);
Moon moon(moonRadius, moonDistance, moonPeriod, 0.0f);
RingPlanet saturn(saturnRadius, saturnDistance, saturnPeriod, saturnringInnerRadius, saturnringOuterRadius);
Planet jupiter(jupiterRadius, jupiterDistance, jupiterRevolutionSpeed, jupitorOrbitalPeriod);
Planet uranus(uranusRadius, uranusDistance, uranusRevolutionSpeed, uranusOrbitalPeriod);
Planet neptune(neptuneRadius, neptuneDistance, neptuneRevolutionSpeed, neptuneOrbitalPeriod);
Planet pluto(plutoRadius, plutoDistance, plutoRevolutionSpeed, plutoOrbitalPeriod);
*/

void toggleOrbitsVisibility() {
    showOrbits = !showOrbits; // Toggle the state
}

void updateCamera(float angle, float x, float y, float z) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    float viewDirX = centreX - camX;
    float viewDirY = centreY - camY;
    float viewDirZ = centreZ - camZ;

    float newX = (cosAngle + (1 - cosAngle) * x * x) * viewDirX +
        ((1 - cosAngle) * x * y - z * sinAngle) * viewDirY +
        ((1 - cosAngle) * x * z + y * sinAngle) * viewDirZ;

    float newY = ((1 - cosAngle) * x * y + z * sinAngle) * viewDirX +
        (cosAngle + (1 - cosAngle) * y * y) * viewDirY +
        ((1 - cosAngle) * y * z - x * sinAngle) * viewDirZ;

    float newZ = ((1 - cosAngle) * x * z - y * sinAngle) * viewDirX +
        ((1 - cosAngle) * y * z + x * sinAngle) * viewDirY +
        (cosAngle + (1 - cosAngle) * z * z) * viewDirZ;

    centreX = camX + newX;
    centreY = camY + newY;
    centreZ = camZ + newZ;
}

// Special keys input callback
void special(int key, int x, int y) {
    float viewDirX = centreX - camX;
    float viewDirY = centreY - camY;
    float viewDirZ = centreZ - camZ;

    float viewDirLength = sqrt(viewDirX * viewDirX + viewDirY * viewDirY + viewDirZ * viewDirZ);
    viewDirX /= viewDirLength;
    viewDirY /= viewDirLength;
    viewDirZ /= viewDirLength;

    float rightX = upY * viewDirZ - upZ * viewDirY;
    float rightY = upZ * viewDirX - upX * viewDirZ;
    float rightZ = upX * viewDirY - upY * viewDirX;

    float rightLength = sqrt(rightX * rightX + rightY * rightY + rightZ * rightZ);
    rightX /= rightLength;
    rightY /= rightLength;
    rightZ /= rightLength;

    switch (key) {
    case GLUT_KEY_RIGHT:
        updateCamera(-rotationSpeed, upX, upY, upZ);
        break;
    case GLUT_KEY_LEFT:
        updateCamera(rotationSpeed, upX, upY, upZ);
        break;
    case GLUT_KEY_UP:
        updateCamera(-rotationSpeed, rightX, rightY, rightZ);
        break;
    case GLUT_KEY_DOWN:
        updateCamera(rotationSpeed, rightX, rightY, rightZ);
        break;
    }

    // Recalculate the up vector
    upX = viewDirY * rightZ - viewDirZ * rightY;
    upY = viewDirZ * rightX - viewDirX * rightZ;
    upZ = viewDirX * rightY - viewDirY * rightX;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    float viewDirX = centreX - camX;
    float viewDirY = centreY - camY;
    float viewDirZ = centreZ - camZ;

    // Normalize the view direction vector
    float viewDirLength = sqrt(viewDirX * viewDirX + viewDirY * viewDirY + viewDirZ * viewDirZ);
    viewDirX /= viewDirLength;
    viewDirY /= viewDirLength;
    viewDirZ /= viewDirLength;

    // Calculate the right vector (perpendicular to view direction and up vector)
    float rightX = upY * viewDirZ - upZ * viewDirY;
    float rightY = upZ * viewDirX - upX * viewDirZ;
    float rightZ = upX * viewDirY - upY * viewDirX;

    // Normalize the right vector
    float rightLength = sqrt(rightX * rightX + rightY * rightY + rightZ * rightZ);
    rightX /= rightLength;
    rightY /= rightLength;
    rightZ /= rightLength;

    switch (key) {

    case 'o':
        toggleOrbitsVisibility();
        break;

    case 'd':
        // Strafe left
        camX -= rightX * movementSpeed;
        camY -= rightY * movementSpeed;
        camZ -= rightZ * movementSpeed;
        centreX -= rightX * movementSpeed;
        centreY -= rightY * movementSpeed;
        centreZ -= rightZ * movementSpeed;
        break;

    case 'a':
        // Strafe right
        camX += rightX * movementSpeed;
        camY += rightY * movementSpeed;
        camZ += rightZ * movementSpeed;
        centreX += rightX * movementSpeed;
        centreY += rightY * movementSpeed;
        centreZ += rightZ * movementSpeed;
        break;

    case 'w':
        // Move forward
        camX += viewDirX * movementSpeed;
        camY += viewDirY * movementSpeed;
        camZ += viewDirZ * movementSpeed;
        centreX += viewDirX * movementSpeed;
        centreY += viewDirY * movementSpeed;
        centreZ += viewDirZ * movementSpeed;
        break;

    case 's':
        // Move backward
        camX -= viewDirX * movementSpeed;
        camY -= viewDirY * movementSpeed;
        camZ -= viewDirZ * movementSpeed;
        centreX -= viewDirX * movementSpeed;
        centreY -= viewDirY * movementSpeed;
        centreZ -= viewDirZ * movementSpeed;
        break;

    case 'r':
        // Reset camera
        camX = 10.0f;
        camY = 8.0f;
        camZ = 10.0f;
        centreX = 0;
        centreY = 0;
        centreZ = 0;
        upX = 0;
        upY = 1;
        upZ = 0;
        break;
    }

    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);                 // Set viewport to cover the new window
    glMatrixMode(GL_PROJECTION);            // Switch to projection matrix mode
    glLoadIdentity();                       // Load identity matrix
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);  // Set perspective projection
    glMatrixMode(GL_MODELVIEW);             // Switch back to modelview matrix mode
}

void initialize() {
    glClearColor(0.0, 0.0, 0.0, 1.0);       // Set clear color to black
    glEnable(GL_DEPTH_TEST);                // Enable depth testing
    glEnable(GL_LIGHTING);                  // Enable lighting
    glEnable(GL_LIGHT0);                    // Enable light source 0
    glEnable(GL_COLOR_MATERIAL);            // Enable material coloring based on glColor

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
    moon.loadTexture("textures/moon.jpg");
    saturn.loadTexture("textures/saturn.jpg");
    saturn.loadRingTexture("textures/saturnring.jpg");
    jupiter.loadTexture("textures/jupiter.jpg");
    uranus.loadTexture("textures/uranus.jpg");
    neptune.loadTexture("textures/neptune.jpg");
    pluto.loadTexture("textures/pluto.jpg");
}

void drawCameraTargetPoint() {
    // Disable depth testing only for the point
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();

    // Move to the target point
    glTranslatef(centreX, centreY, centreZ);

    // Set point size and color
    glPointSize(3.0f);          // Adjust size as needed

    // Draw the point
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f); // Point at the target position
    glEnd();

    glPopMatrix();

    // Re-enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void drawAxes(float length) {
    glBegin(GL_LINES);

    // X-axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);

    // Y-axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length, 0.0f);

    // Z-axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, length);

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers
    glLoadIdentity();                                   // Load identity matrix
    gluLookAt(camX, camY, camZ, centreX, centreY, centreZ, upX, upY, upZ);  // Set camera position and orientation

    // Draw each planet at its current position and rotation
    GLfloat noEmission[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
    if (showOrbits) {
        // Draw orbits with specific material properties
        mercury.drawOrbit();
        venus.drawOrbit();
        earth.drawOrbit();
        mars.drawOrbit();
        saturn.drawOrbit();
        moon.drawOrbit(earth.distanceFromSun, earth.x, earth.z);
        jupiter.drawOrbit();
        uranus.drawOrbit();
        neptune.drawOrbit();
        pluto.drawOrbit();
    }

    sun.drawAtPosition();
    mercury.drawAtPosition();
    venus.drawAtPosition();
    earth.drawAtPosition();
    mars.drawAtPosition();
    moon.drawAtPosition(earth.x, earth.y, earth.z);
    saturn.drawAtPosition();
    jupiter.drawAtPosition();
    uranus.drawAtPosition();
    neptune.drawAtPosition();
    pluto.drawAtPosition();

    // Draw the camera target point
    drawCameraTargetPoint();

    printf("%f %f %f %f %f %f \n \n", camX, camY, camZ, centreX, centreY, centreZ);

    glutSwapBuffers();  // Swap the front and back buffers (double buffering)
}

void timer(int value) {
    revolutionAngle += 1.0f;                          // Increment revolution angle for all planets
   /* sunRotationAngle += sunRotationSpeed;
    mercuryRotationAngle += mercuryRotationSpeed;    // Update rotation angle for Mercury
    venusRotationAngle += venusRotationSpeed;        // Update rotation angle for Venus
    earthRotationAngle += earthRotationSpeed;        // Update rotation angle for Earth
    marsRotationAngle += marsRotationSpeed;          // Update rotation angle for Mars
    moonRotationAngle += moonRotationSpeed;          // Update rotation angle for Moon
    jupiterRotationAngle += jupiterRotationSpeed;
    uranusRotationAngle += uranusRotationSpeed;
    neptuneRotationAngle += neptuneRotationSpeed;
    plutoRotationAngle += plutoRotationSpeed;*/

    glutPostRedisplay();                              // Request a redraw
    glutTimerFunc(1000 / TARGET_FPS, timer, 0);       // Schedule next update
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                              // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Set display mode
    glutInitWindowSize(1024, 768);                      // Set window size
    glutCreateWindow("Solar System");                   // Create window with title

    initialize();                                       // Initialize OpenGL context and textures
    glutReshapeFunc(reshape);

    glutDisplayFunc(display);                           // Register display function
    glutTimerFunc(0, timer, 0);                         // Start timer function for animation
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();                                     // Enter GLUT main loop

    return 0;
}
