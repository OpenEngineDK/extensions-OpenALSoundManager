// OpenAL implementation of a sound source 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Sound/OpenALSound.h>

namespace OpenEngine {
namespace Sound {

using namespace OpenEngine::Sound;

OpenALSound::OpenALSound(ISoundResourcePtr resource) : resource(resource) { }
OpenALSound::~OpenALSound() {}

void OpenALSound::Play() {

}

void OpenALSound::Stop() {

}

void OpenALSound::Pause() {

}

void OpenALSound::SetPos(Vector<3,float> pos) {
    this->pos = pos;
}

ISoundResourcePtr OpenALSound::GetResource() {
    return resource;
}
    
} //NS Sound
} //NS OpenEngine
