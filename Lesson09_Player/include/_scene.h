#ifndef _SCENE_H
#define _SCENE_H

#include<_lightSetting.h>
#include<_model.h>
#include<_inputs.h>
#include<_parallax.h>
#include<_player.h>

class _scene
{
    public:

        _scene();                               // Constructor
        virtual ~_scene();                      // Destructor

        GLint initGL();                         // Initialize Game Objects
        void drawScene();                       // Render The Final Scene
        void reSize(GLint width, GLint height); // Resize Window

        int winMsg(	HWND hWnd, UINT	uMsg, WPARAM wParam, LPARAM	lParam);

        vec2 dim; // for screen width/height

    protected:

    private:
};

#endif // _SCENE_H
