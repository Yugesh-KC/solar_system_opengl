#define PI 3.14159265358979323846
#pragma once
#include "SOIL2/SOIL2.h"
#include <cstdio>
#include <cmath>
#include <glut.h>

// Constants
const float EARTH_DAYS_IN_YEAR = 365.25;
const float DEGREES_IN_CIRCLE = 360.0;
float rotation_multiplier = 0.001f ;

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
    float rotationalPeriod;
    //float orbitalPeriod;
    GLuint textureID;       // Texture ID
    float rotationSpeed;    // in degrees per day
    float revolutionSpeed;  // in degrees per day
    float currentRotationAngle;
    float axialtilt;
    float currentRevolutionAngle;
    float x, y, z;

    Planet(float radius, float distance, float rotationPeriod,float axialtilt=0.0f,bool retrograde=false)
        : radius(radius), distanceFromSun(distance), textureID(0), currentRotationAngle(0), currentRevolutionAngle(0),rotationalPeriod(rotationalPeriod),axialtilt(axialtilt) {

        //Calculate rotationSpeed based on rotationPeriod i.e earth days per day of a planet
        //rotationSpeed = (2 * PI * radius) / (rotationalPeriod);
        // into degree per day
       //rotationSpeed = (rotationSpeed / 2 * PI * radius)* DEGREES_IN_CIRCLE;
        
        // Calculate the orbital period in Earth years
        float orbitalPeriodYears = pow(distanceFromSun, 1.5);

        // Convert the orbital period to days
        float orbitalPeriodDays = orbitalPeriodYears * EARTH_DAYS_IN_YEAR;

        // Calculate revolution speed in degrees per day
        revolutionSpeed = DEGREES_IN_CIRCLE / orbitalPeriodDays;

        // Calculate rotation speed in degrees per day
      rotationSpeed = DEGREES_IN_CIRCLE / rotationPeriod;

      if (retrograde) {
          rotationSpeed = -rotationSpeed;
      }
    }

    void drawOrbit() {
        glDisable(GL_TEXTURE_2D);
        const int segments = 100;

        // Set material properties for the orbits
        GLfloat orbitColor[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // Light gray color
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orbitColor);

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) {
            float theta = 2.0f * 3.14159f * float(i) / float(segments);
            float ox = distanceFromSun * cos(theta);
            float oz = distanceFromSun * sin(theta);
            glVertex3f(ox, 0.0f, oz);
        }
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

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

    void drawAtPosition() {
        currentRevolutionAngle += revolutionSpeed;
        currentRotationAngle += rotationSpeed;

        glPushMatrix();
        x = distanceFromSun * cos(currentRevolutionAngle * 3.14159f / 180.0f);
        z = distanceFromSun * sin(currentRevolutionAngle * 3.14159f / 180.0f);
        glTranslatef(x, 0.0, z);    // Position relative to the Sun


        // Apply axial tilt
        glRotatef(axialtilt, 0.0, 0.0, 1.0);
        glRotatef(currentRotationAngle * rotation_multiplier, 0.0, 1.0, 0.0);    // Rotate around own axis

        draw();
        glPopMatrix();
    }
};

// Sun class, derived from Planet
class Sun : public Planet {
public:
    Sun(float radius, float distance, float rotationPeriod)
        : Planet(radius, distance, rotationPeriod) {}
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

    void drawAtPosition() {
        currentRotationAngle += rotationSpeed;

        glPushMatrix();
        glRotatef(currentRotationAngle * rotation_multiplier, 0.0, 1.0, 0.0);
        draw();
        glPopMatrix();
    }
};

class Moon : public Planet {
public:
    Moon(float radius, float distance, float rotationPeriod)
        : Planet(radius, distance, rotationPeriod) {}

    void drawAtPosition(float planetX, float planetY, float planetZ) {
        currentRevolutionAngle += revolutionSpeed;
        currentRotationAngle += rotationSpeed;

        glPushMatrix();

        // Translate to the parent planet's position
        glTranslatef(planetX, planetY, planetZ);

        // Calculate moon's position relative to parent planet
        x = distanceFromSun * cos(currentRevolutionAngle * 3.14159f / 180.0f);
        z = distanceFromSun * sin(currentRevolutionAngle * 3.14159f / 180.0f);

        // Translate to moon's position relative to the parent planet
        glTranslatef(x, 0.0, z);

       

        // Rotate around own axis
        glRotatef(currentRotationAngle * rotation_multiplier, 0.0, 1.0, 0.0);// Rotate around own tilted axis

        // Draw the moon
        draw();

        glPopMatrix();
    }

    void draw() override {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glEnable(GL_TEXTURE_2D);
        GLUquadric* quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE);
        gluSphere(quad, radius, 32, 32);
        gluDeleteQuadric(quad);
        glDisable(GL_TEXTURE_2D);
    }

    void drawOrbit(float parentDistanceFromSun, float parentX, float parentZ) {
        glDisable(GL_TEXTURE_2D);
        const int segments = 100;

        // Set material properties for the orbit
        GLfloat orbitColor[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // Light gray color
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orbitColor);

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) {
            float theta = 2.0f * 3.14159f * float(i) / float(segments);
            float ox = distanceFromSun * cos(theta);
            float oz = distanceFromSun * sin(theta);
            glVertex3f(parentX + ox, 0.0f, parentZ + oz);
        }
        glEnd();

        glEnable(GL_TEXTURE_2D);
    }

};

class RingPlanet : public Planet {
public:
    float ringInnerRadius;
    float ringOuterRadius;
    GLuint ringTextureID;  // Texture ID for the rings

    RingPlanet(float radius, float distance, float rotationPeriod, float ringInnerRadius, float ringOuterRadius,float axialtile=0.0f)
        : Planet(radius, distance, rotationPeriod,axialtilt),
        ringInnerRadius(ringInnerRadius), ringOuterRadius(ringOuterRadius), ringTextureID(0) {
    }

    void loadRingTexture(const char* filename) {
        ::loadTexture(&ringTextureID, filename);
    }

    void drawRings() {
        glBindTexture(GL_TEXTURE_2D, ringTextureID);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_TRIANGLE_STRIP);
        for (float angle = 0.0f; angle <= 2.0f * 3.14159f + 0.1f; angle += 0.1f) { // added 0.1f to ensure closure
            glTexCoord2f(0.0f, 0.0f); glVertex3f(ringInnerRadius * cos(angle), 0.0f, ringInnerRadius * sin(angle));
            glTexCoord2f(1.0f, 1.0f); glVertex3f(ringOuterRadius * cos(angle), 0.0f, ringOuterRadius * sin(angle));
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }


    void draw() override {
        Planet::draw();  // Draw the planet
        drawRings();     // Draw the rings
    }
};


