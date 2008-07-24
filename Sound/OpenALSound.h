// OpenAL implementation of a sound source 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OPENAL_SOUND_H_
#define _OPENAL_SOUND_H_

#include <Sound/ISound.h>
#include <Math/Vector.h>
#include <Resources/ISoundResource.h>

namespace OpenEngine {
namespace Sound {

using OpenEngine::Math::Vector;
using namespace OpenEngine::Resources;

/**
 * OpenAL Sound source.
 * 
 *
 * @class OpenALSound OpenALSound.h Sound/OpenALSound.h
 */
class OpenALSound: public ISound {
private:
    Vector<3,float> pos;
    ISoundResourcePtr resource;
public:
    OpenALSound(ISoundResourcePtr resource);

    ~OpenALSound();

    // Play methods

    void Play();
    void Stop();
    void Pause();
    
    void SetPos(Vector<3,float> pos);

    ISoundResourcePtr GetResource();

};

} // NS Sound
} // NS OpenEngine


#endif // _OPENAL_SOUND_H_
