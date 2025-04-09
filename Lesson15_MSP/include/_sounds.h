#ifndef _SOUNDS_H
#define _SOUNDS_H

#include <SNDS/irrKlang.h>
#include<_common.h>
using namespace irrklang;


class _sounds
{
    public:
        _sounds();
        virtual ~_sounds();

        void playMusic(char *);  // play background music
        void playSounds(char *); // play sounds
        void pauseSounds(char*); // pause or stop music
        int initSounds();

        ISoundEngine *sndEng = createIrrKlangDevice(); // create sound instatnce
    protected:

    private:
};

#endif // _SOUNDS_H
