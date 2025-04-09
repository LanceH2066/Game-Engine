#include "_sounds.h"

_sounds::_sounds()
{
    //ctor
}

_sounds::~_sounds()
{
    //dtor
    sndEng->drop();
}
void _sounds::playMusic(char* fileName)
{
    sndEng->play2D(fileName,true);
}

void _sounds::playSounds(char* fileName)
{
  //   if(!sndEng->isCurrentlyPlaying(fileName))
        sndEng->play2D(fileName,false,false);
}

void _sounds::pauseSounds(char* fileName)
{
    sndEng->play2D(fileName,true,false);
}

int _sounds::initSounds()
{
    if(!sndEng)
       {
           cout<< "ERROR: ** Sound Engine could not start"<<endl;
           //return 0;
       }
     return 1;
}
