#include "_sounds.h"
using namespace irrklang;

_sounds::_sounds()
{

}
_sounds::~_sounds()
{
    soundEngine->drop();
}
int _sounds::initSounds()
{
    if(!soundEngine)
    {
        cout << "ERROR: Sound Engine Could Not Start" << endl;
        //return 0;
    }

    return 1;
}
void _sounds::playMusic(char* fileName)
{
    soundEngine->play2D(fileName,true);
}
void _sounds::playSounds(char* fileName)
{
    if(!soundEngine->isCurrentlyPlaying(fileName))
    {
        soundEngine->play2D(fileName,false,false);
    }
}
void _sounds::pauseSounds(char* fileName)
{
    soundEngine->play2D(fileName,true,false);
}
