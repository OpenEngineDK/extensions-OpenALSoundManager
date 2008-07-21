// OpenGL renderer implementation.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OPENAL_SOUND_MANAGER_H_
#define _OPENAL_SOUND_MANAGER_H_

#include <Core/IModule.h>
#include <Core/Event.h>
#include <vector>

namespace OpenEngine {

namespace Sound {

using namespace std;
using OpenEngine::Core::IModule;

/**
 * Renderer using OpenGL
 *
 * @class Renderer Renderer.h Renderers/OpenGL/IRenderer.h
 */
class OpenALSoundManager : public IModule {
private:

public:
    OpenALSoundManager();
    ~OpenALSoundManager();

    void Initialize();
    void Process(const float deltaTime, const float percent);
    void Deinitialize();
    bool IsTypeOf(const std::type_info& inf);

};

} // NS Sound
} // NS OpenEngine

#endif
