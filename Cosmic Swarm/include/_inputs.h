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

        void keyPressed(_player*, _sounds* sounds);
        void keyUp(_player* player, _sounds* sounds);

    protected:

    private:
};

#endif // _INPUTS_H
