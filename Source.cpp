#include <glut.h>

// Rotation angle for the revolving planets
float angle = 0.0f;

// Planet class
class Planet {
public:
    float radius;
    float distance;
    float r, g, b; // Color components

    Planet(float rad, float dist, float red, float green, float blue)
        : radius(rad), distance(dist), r(red), g(green), b(blue) {}

    void draw() {
        glColor3f(r, g, b);
        glutSolidSphere(radius, 32, 32);
    }

    void drawAtPosition(float rotationAngle) {
        glPushMatrix();
        glRotatef(rotationAngle, 0.0, 1.0, 0.0);
        glTranslatef(distance, 0.0, 0.0);
        draw();
        glPopMatrix();
    }
};

// Planets
Planet centralPlanet(1.0f, 0.0f, 1.0f, 0.0f, 0.0f); // Red
Planet revolvingPlanet1(0.5f, 3.0f, 0.0f, 1.0f, 0.0f); // Green
Planet revolvingPlanet2(0.5f, 5.0f, 0.0f, 0.0f, 1.0f); // Blue

// Initialization function
void initialize() {
    // Set up the clearing color (black)
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Enable color material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Set light properties
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Set viewport
    glViewport(0, 0, 1024, 768);

    // Set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1024.0 / 768.0, 1.0, 100.0);

    // Set modelview matrix
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera
    gluLookAt(10.0, 8.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Draw the central planet
    centralPlanet.draw();

    // Draw the revolving planets
    revolvingPlanet1.drawAtPosition(angle);
    revolvingPlanet2.drawAtPosition(-angle);

    // Swap buffers
    glutSwapBuffers();
}

void idle() {
    // Update the rotation angle
    angle += 0.1f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }

    // Request display update
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set display mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Set window size
    glutInitWindowSize(1024, 768);

    // Create the window
    glutCreateWindow("Planet Simulation");

    // Initialize OpenGL settings
    initialize();

    // Set callback functions
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    // Start the main loop
    glutMainLoop();

    return 0;
}
