#include "_xpOrb.h"

_xpOrb::_xpOrb()
{
    //ctor
    position = vec3(0.0f, 0.0f, 0.0f);
    scale = vec3(0.5f, 0.5f, 1.0f);
    isActive = true;

    xMin = 0.0f;
    xMax = 1.0f;
    yMin = 0.0f;
    yMax = 1.0f;
}

_xpOrb::~_xpOrb()
{
    //dtor
}

void _xpOrb::initOrb(char* fileName)
{
    xpTextureLoader->loadTexture(fileName);
}

void _xpOrb::drawOrb()
{
    if (!isActive) return;

    glPushMatrix();

        glTranslatef(position.x, position.y, position.z);
        glScalef(scale.x, scale.y, 1.0f);
        glColor3f(1.0f, 1.0f, 1.0f);

        xpTextureLoader->textureBinder();

        glBegin(GL_QUADS);
            glTexCoord2f(xMin, yMin); glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(xMax, yMin); glVertex3f(1.0f, -1.0f, 0.0f);
            glTexCoord2f(xMax, yMax); glVertex3f(1.0f, 1.0f, 0.0f);
            glTexCoord2f(xMin, yMax); glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();

    glPopMatrix();
}

void _xpOrb::placeOrb(vec3 pos)
{
     position = pos;
}

vec3 _xpOrb::getCollisionBoxMin() const
{
        return {
        position.x - collisionBoxSize.x,
        position.y - collisionBoxSize.y,
        position.z - collisionBoxSize.z
    };
}

vec3 _xpOrb::getCollisionBoxMax() const
{
        return {
        position.x + collisionBoxSize.x,
        position.y + collisionBoxSize.y,
        position.z + collisionBoxSize.z
    };
}

