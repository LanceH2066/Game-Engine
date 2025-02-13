#include "_model.h"

_model::_model()
{
    // Initialization
    rotation.x = rotation.y = rotation.z = 0.0;
    pos.x = pos.y;
    pos.z = -8.0;
    scale.x = scale.y = scale.z = 1.0;
}

_model::~_model()
{

}

void _model::drawModel()
{
    glPushMatrix();
        glColor3f(1.0,0.6,0.6);             // Set Color
        glTranslatef(pos.x,pos.y,pos.z);    // Set Position
        glRotatef(rotation.x,1,0,0);        // Set Rotation X
        glRotatef(rotation.y,0,1,0);        // Set Rotation Y
        glRotatef(rotation.z,0,0,1);        // Set Rotation Z
        glScalef(scale.x,scale.y,scale.z);  // Set Scale
        glutSolidTorus(0.5,1.5,40,40);      // Draw Shape
    glPopMatrix();
}
