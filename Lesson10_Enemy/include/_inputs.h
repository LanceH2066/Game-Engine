#ifndef _INPUTS_H
#define _INPUTS_H

#include<_model.h>
#include<_parallax.h>

class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        void keyPressed(_model *);  // Handling given model by key strokes
        void keyPressedPRLX(_parallax*);
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

    protected:

    private:
};

#endif // _INPUTS_H
