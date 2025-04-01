#ifndef _INPUTS_H
#define _INPUTS_H

#include<_common.h>
#include<_model.h>
#include<_parallax.h>
#include<_sounds.h>
#include<_bullets.h>
#include<_player.h>
class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        void keyPressed(_model *); // handling given model by key strokes
        void keyPressedPRLX(_parallax *);
        void keyPressed(_bullets&);

        void keyUP();

        void mouseEventDown(_player *, _bullets&);
        void mouseEventUp(_player* player);
        void mouseMove(_player*, double,double);
        void mouseWheel(_model *, double);

        double prev_msX;
        double prev_msY;

        bool isMsTranslate;
        bool isMsRotation;

        WPARAM wParam;

    protected:

    private:
};

#endif // _INPUTS_H
