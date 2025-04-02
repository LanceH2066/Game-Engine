#include "_model.h"

_model::_model()
{
    //ctor

    rotation.x = rotation.y = rotation.z= 0.0; // init rotations
    pos.x = -2.15;
    pos.y=-1.3;                         // init placement
    pos.z = -4.0;
    scale.x=scale.y=scale.z = 1.0;

    xMin = yMin = 0;
    xMax = yMax = 1.0;
}

_model::~_model()
{
    //dtor
}
void _model::initModel(char* fileName)
{
    mytex->loadTexture(fileName);
}


void _model::drawModel()
{
    glPushMatrix();
        mytex->textureBinder();

        glTranslatef(pos.x,pos.y,pos.z);
        glRotatef(rotation.x, 1,0,0);
        glRotatef(rotation.y, 0,1,0);
        glRotatef(rotation.z, 0,0,1);
        glScalef(scale.x,scale.y,scale.z);

        glBegin(GL_QUADS);
            glTexCoord2f(xMin,yMax);
            glVertex3f(-1,-1,0);

            glTexCoord2f(xMax,yMax);
            glVertex3f(1,-1,0);

            glTexCoord2f(xMax,yMin);
            glVertex3f(1,1,0);

            glTexCoord2f(xMin,yMin);
            glVertex3f(-1,1,0);
        glEnd();
    glPopMatrix();

}
