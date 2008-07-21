// OpenAL sound manager implementation.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Sound/OpenALSoundManager.h>

namespace OpenEngine {
namespace Sound {

OpenALSoundManager::OpenALSoundManager() {

}

/**
 * OpenALSoundManager destructor.
 * Performs no clean up.
 */
OpenALSoundManager::~OpenALSoundManager() {

}

void OpenALSoundManager::Initialize() {
}

/**
 * @note The processing function assumes that the scene has not been
 *       replaced by null since the initialization face. 
 */
void OpenALSoundManager::Process(const float deltaTime, const float percent) {
}

void OpenALSoundManager::Deinitialize() {
}

bool OpenALSoundManager::IsTypeOf(const std::type_info& inf) {
    return ((typeid(OpenALSoundManager) == inf));
}


} // NS Sound
} // NS OpenEngine
