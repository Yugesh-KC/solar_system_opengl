#pragma once
#include "SOIL2/SOIL2.h"
#include <cstdio>
#include <cmath>
#include <glut.h>

int rotation_multiplier = 3;

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
    float x, y, z;

    Planet(float radius, float distance, float rotationSpeed)
        : radius(radius), distanceFromSun(distance), textureID(0), rotationSpeed(rotationSpeed) {
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


    void drawAtPosition(float revolutionAngle, float rotationAngle) {
        glPushMatrix();
        x = distanceFromSun * cos(revolutionAngle * 3.14159f / 180.0f);
        z = distanceFromSun * sin(revolutionAngle * 3.14159f / 180.0f);
        //glRotatef(revolutionAngle, 0.0, 1.0, 0.0);   // Rotate around the Sun
        glTranslatef(x, 0.0, z);    // Position relative to the Sun
        glRotatef(rotationAngle*rotation_multiplier, 0.0, 1.0, 0.0);    // Rotate around own axis
       
        draw();
        glPopMatrix();
    }

    void updateRotation() {
        rotationSpeed += 0.1f;  // Increment rotation speed
    }
};

class RingPlanet : public Planet {
public:
    float ringInnerRadius;
    float ringOuterRadius;
    GLuint ringTextureID;  // Texture ID for the rings

    RingPlanet(float radius, float distance, float rotationSpeed, float ringInnerRadius, float ringOuterRadius)
        : Planet(radius, distance, rotationSpeed),
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

// Sun class, derived from Planet
class Sun : public Planet {
public:
    Sun(float radius, float distance, float rotationSpeed)
        : Planet(radius, distance, rotationSpeed) {}
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
        glRotatef(rotationAngle*rotation_multiplier, 0.0, 1.0, 0.0);
        draw();
        glPopMatrix();
    }
};

class Moon : public Planet {
public:
    Moon(float radius, float distance, float rotationSpeed)
        : Planet(radius, distance, rotationSpeed) {}

    void drawAtPosition(float planetX, float planetY, float planetZ, float revolutionAngle, float rotationAngle) {
        glPushMatrix();

        // Translate to the parent planet's position
        glTranslatef(planetX, planetY, planetZ);

        // Calculate moon's position relative to parent planet
        x = distanceFromSun * cos(revolutionAngle * 3.14159f /180.0f);
        z = distanceFromSun * sin(revolutionAngle * 3.14159f / 180.0f);

        // Translate to moon's position relative to the parent planet
        glTranslatef(x, 0.0, z);
        printf("moon %f %f \n", planetX + x,planetZ+z);
        // Rotate around own axis
        glRotatef(rotationAngle*rotation_multiplier, 0.0, 1.0, 0.0);

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
};
