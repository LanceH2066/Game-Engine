#ifndef _MODEL_H
#define _MODEL_H

#include<_common.h>

class _model
{
    public:
        _model();
        virtual ~_model();

        vec3 rotation; // model rotation value
        vec3 pos; // model position value
        vec3 scale;    // model scale value

        void drawModel();

    protected:

    private:
};

#endif // _MODEL_H
