// Interface for a sound source.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _ISOUND_H_
#define _ISOUND_H_

#include <Math/Vector.h>
#include <Resources/ISoundResource.h>

namespace OpenEngine {
namespace Sound {

using OpenEngine::Math::Vector;
using namespace OpenEngine::Resources;

/**
 * ISound.
 * Interface for a sound source in OpenEngine.
 *
 * @class ISound ISound.h Sound/ISound.h
 */
class ISound {
private:

public:
    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    virtual void SetPos(Vector<3,float> pos) = 0;

    virtual ISoundResourcePtr GetResource();

};

} // NS Sound
} // NS OpenEngine

#endif // _ISOUND_H_
