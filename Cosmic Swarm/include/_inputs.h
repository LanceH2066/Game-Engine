#ifndef _INPUTS_H
#define _INPUTS_H

#include<_common.h>
#include<_model.h>
#include<_parallax.h>
#include<_player.h>
#include<_sounds.h>

class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        void processRawInput(LPARAM lParam, _player* player);

        void keyPressed(_model *);  // Handling given model by key strokes
        void keyPressedPRLX(_parallax*);
        void keyPressedPlayer(_player*);
        void keyPressedSounds(_sounds*,char*);
        void keyUpSounds(_sounds*,char*);
        void keyUpPlayer(_player* player);
        void keyUP();

        void mouseEventDown(_model *, double, double); // Handling given model by mouse inputs
        void mouseEventUp();
        void mouseMove(_model *, double, double);
        void mouseWheel(_model *, double);

        double prev_msX;
        double prev_msY;

        bool isMsTranslate;
        bool isMsRotation;

        WPARAM wParam;
        map<int, bool> keyStates;

    protected:

    private:
};

#endif // _INPUTS_H
