// OpenAL sound manager implementation.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Sound/OpenALSoundManager.h>

#include <Logging/Logger.h>
#include <Core/Exceptions.h>
#include <Sound/SoundNodeVisitor.h>
#include <Sound/ISound.h>
#include <Utils/Convert.h>

namespace OpenEngine {
namespace Sound {

using OpenEngine::Core::Exception;
using OpenEngine::Utils::Convert;

OpenALSoundManager::OpenALSoundManager(ISceneNode* root, IViewingVolume* vv): theroot(root), vv(vv) {
    Init();
}

OpenALSoundManager::~OpenALSoundManager() {}

ISound* OpenALSoundManager::CreateSound(ISoundResourcePtr resource) {
    ISound* res = new OpenALSound(resource);
    res->Initialize();
    return res;
}

void OpenALSoundManager::Initialize() {}

void OpenALSoundManager::Init() {
    const ALCchar* defaultdevice = 
      alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER );
    logger.info << "default audio device: " << defaultdevice << logger.end;

    ALCdevice* thedevice = alcOpenDevice(NULL);
    if (thedevice) {
        ALCcontext* thecontext = alcCreateContext(thedevice, NULL);
	alcMakeContextCurrent(thecontext);
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	logger.info << "OpenAL has been initialized" << logger.end;
    }
    else
      throw new Exception("Could not initalize sound module");
}

/**
 * @note The processing function assumes that the scene has not been
 *       replaced by null since the initialization face. 
 */
void OpenALSoundManager::Process(const float deltaTime, const float percent) {
    if (vv != NULL) {
        Vector<3,float> vvpos = vv->GetPosition();
        alListener3f(AL_POSITION, vvpos[0], vvpos[1], vvpos[2]);
	//logger.info << "viewing from: " << vvpos << logger.end;
    }

    //@todo optimize this by saving the ref, and reinit pos in visitor
    SoundNodeVisitor* snv = new SoundNodeVisitor();
    theroot->Accept(*snv);
    delete snv;
}

void OpenALSoundManager::Deinitialize() {
    ALCcontext* thecontext = alcGetCurrentContext();
    ALCdevice* thedevice = alcGetContextsDevice(thecontext);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(thecontext);
    alcCloseDevice(thedevice);
}

bool OpenALSoundManager::IsTypeOf(const std::type_info& inf) {
    return ((typeid(OpenALSoundManager) == inf));
}

OpenALSoundManager::OpenALSound::OpenALSound(ISoundResourcePtr resource) : resource(resource) {
}

void OpenALSoundManager::OpenALSound::Initialize() {
    //generate the source
    ALuint source;
    alGenSources(1, &source);

    ALCenum error;
    if ((error = alGetError()) != AL_NO_ERROR) {
        throw Exception("tried to gen source but got: "
			+ Convert::ToString(error));
    }

    //attach the buffer
    alGenBuffers(1, &bufferID);
    alBufferData(bufferID, resource->GetFormat(), resource->GetBuffer(),
		 resource->GetBufferSize(), resource->GetFrequency());
    alSourcei(source, AL_BUFFER, bufferID);
    
    if ((error = alGetError()) != AL_NO_ERROR) {
        throw Exception("tried to bind source and buffer together but got: "
		      + Convert::ToString(error));
    }
    sourceID = source;
}

OpenALSoundManager::OpenALSound::~OpenALSound() {
    alDeleteBuffers(1, &bufferID);
    alDeleteSources(1, &sourceID);
}

void OpenALSoundManager::OpenALSound::Play() {
    alSourcePlay(sourceID);
    //alSourcef(src, AL_REFERENCE_DISTANCE, 1.0 );
    PrintAttribute(AL_REFERENCE_DISTANCE);
    PrintAttribute(AL_CONE_INNER_ANGLE);
    PrintAttribute(AL_ROLLOFF_FACTOR);
    PrintAttribute(AL_MAX_DISTANCE);
}

void OpenALSoundManager::OpenALSound::PrintAttribute(ALenum e) {
    float* where = new float[3];
    alGetSourcefv(sourceID, e, where);
    Vector<3,float> vec = Vector<3,float>(where[0],where[1],where[2]);
    delete where;
    logger.info << "(" << e << "): " << vec << logger.end;
}

void OpenALSoundManager::OpenALSound::Stop() {
    alSourceStop(sourceID);
}

void OpenALSoundManager::OpenALSound::Pause() {
    alSourcePause(sourceID);
}

void OpenALSoundManager::OpenALSound::SetRotation(Quaternion<float> dir) {
  //@todo apply rotation via openal
    this->dir = dir;
}

Quaternion<float> OpenALSoundManager::OpenALSound::GetRotation() {
    return dir;
}

void OpenALSoundManager::OpenALSound::SetPosition(Vector<3,float> pos) {
    alSource3f(sourceID, AL_POSITION, pos[0], pos[1], pos[2]);

    ALCenum error;
    if ((error = alGetError()) != AL_NO_ERROR) {
      throw Exception("tried to set position but got: "
		      + Convert::ToString(error));
    }
    this->pos = pos;
}

Vector<3,float> OpenALSoundManager::OpenALSound::GetPosition() {
    return pos;
}

ISoundResourcePtr OpenALSoundManager::OpenALSound::GetResource() {
    return resource;
}

unsigned int OpenALSoundManager::OpenALSound::GetID() {
    return sourceID;
}

void OpenALSoundManager::OpenALSound::SetID(unsigned int id) {
    sourceID = id;
}

void OpenALSoundManager::OpenALSound::SetGain(float gain) {
    ALCenum error;
    alSourcef(sourceID, AL_GAIN, (ALfloat)gain);
    if ((error = alGetError()) != AL_NO_ERROR) {
      throw Exception("tried to set gain but got: "
		      + Convert::ToString(error));
    }
}

} // NS Sound
} // NS OpenEngine
