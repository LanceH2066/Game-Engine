#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>

class _scene
{
    public:

        _scene();                               // Constructor
        virtual ~_scene();                      // Destructor

        GLint initGL();                         // Initialize Game Objects
        void drawScene();                       // Render The Final Scene
        void reSize(GLint width, GLint height); // Resize Window

    protected:

    private:
};

#endif // _SCENE_H
