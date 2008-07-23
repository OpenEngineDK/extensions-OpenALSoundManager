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

using OpenEngine::Core::Exception;

namespace OpenEngine {
namespace Sound {

//init static event queue
QueuedEvent<PlaybackEventArg>* OpenALSoundManager::playback = new QueuedEvent<PlaybackEventArg>();


  OpenALSoundManager::OpenALSoundManager(ISceneNode* root, IViewingVolume* vv): theroot(root), vv(vv) {
    playback->Attach(*this);
}

OpenALSoundManager::~OpenALSoundManager() {}

void OpenALSoundManager::Initialize() {
    const ALCchar* defaultdevice = alcGetString( NULL, ALC_DEFAULT_DEVICE_SPECIFIER );
    logger.info << "default audio device: " << defaultdevice << logger.end;

    ALCdevice* thedevice = alcOpenDevice(NULL);
    if (thedevice) {
        ALCcontext* thecontext = alcCreateContext(thedevice, NULL);
	alcMakeContextCurrent(thecontext);
	logger.info << "openal has been initialize" << logger.end;
    }
    else
      throw new Exception("could not initalize sound module");
}

/**
 * @note The processing function assumes that the scene has not been
 *       replaced by null since the initialization face. 
 */
void OpenALSoundManager::Process(const float deltaTime, const float percent) {
    Vector<3,float> vvpos = vv->GetPosition();
    logger.info << "viewing from: " << vvpos << logger.end;
    alListener3f(AL_POSITION, vvpos[0], vvpos[1], vvpos[2]);

    //init to assumed startposition
    pos = Vector<3,float>(0.0, 0.0, 0.0);

    //init to assumed direction
    dir = new Quaternion<float>();
    theroot->Accept(*this);
    delete(dir);

    // process the event queue
    OpenALSoundManager::playback->Release();
}

void OpenALSoundManager::Deinitialize() {
    //@todo clean up
    //for all buffer ids in buffers
    //alDeleteBuffers(1, &bufferID);

    //for all source ids in sources
    //alDeleteSources(1, &sourceID);

    ALCcontext* thecontext = alcGetCurrentContext();
    ALCdevice* thedevice = alcGetContextsDevice(thecontext);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(thecontext);
    alcCloseDevice(thedevice);
}

bool OpenALSoundManager::IsTypeOf(const std::type_info& inf) {
    return ((typeid(OpenALSoundManager) == inf));
}

void OpenALSoundManager::VisitTransformationNode(TransformationNode* node) {
	//get the change in position from the transformation and apply to current
	Vector<3, float> transpos = node->GetPosition();
	pos = pos + transpos;

	//get the change in dir from the transformation and apply to current
	Quaternion<float> transdir = node->GetRotation();
	(*dir) = (*dir) * transdir;

	node->VisitSubNodes(*this);

	//take of transformation again
	pos = pos - transpos;

	//take of turn again
	(*dir) = (*dir) * (transdir.GetInverse());
}

void OpenALSoundManager::VisitSoundNode(SoundNode* node) {
    ALuint source = 0;
    ALCenum error;
	
    //check if loaded
    map<SoundNode*,unsigned int>::iterator iter =
      sourceIDs.find(node);

    if (iter == sourceIDs.end()) {
        //generate the source
        alGenSources(1, &source);

	if ((error = alGetError()) != AL_NO_ERROR) 
	    logger.info << "tried to gen source but got: " << error << logger.end;

	//attach the buffer
	ALuint bufferID;
	ISoundResourcePtr resource = node->GetResource();
	alGenBuffers(1, &bufferID);
	alBufferData(bufferID, resource->GetFormat(), resource->GetBuffer(),
		     resource->GetBufferSize(), resource->GetFrequency());
	alSourcei(source, AL_BUFFER, bufferID);

	if ((error = alGetError()) != AL_NO_ERROR) 
	    throw Exception("tried to bind source and buffer together but got: " + error);


	//remember to add to loaded files
	sourceIDs[node] = source;
    }
    else
        source = iter->second;

    //setup the source settings
    alSource3f(source, AL_POSITION, pos[0], pos[1], pos[2]);
    if ((error = alGetError()) != AL_NO_ERROR) 
        throw Exception("tried to set position but got: " + error);

    /*
    alSourcef(source, AL_GAIN, (ALfloat)node->GetGain());
    if ((error = alGetError()) != AL_NO_ERROR) 
      throw Exception("tried to set gain but got: " + error);
    */

    node->VisitSubNodes(*this);
}

void OpenALSoundManager::Handle(PlaybackEventArg e) {
    map<SoundNode*,unsigned int>::iterator iter =
      sourceIDs.find(e.node);
    if (iter == sourceIDs.end()) {
      logger.warning << "OpenAL sound not initialized" << logger.end;
      return;
    }
    ALuint source = iter->second;

    switch (e.action) {
    case PlaybackEventArg::PLAY: 
        logger.info << "play" << logger.end;
        alSourcePlay(source);
	break;
    case PlaybackEventArg::STOP: 
        logger.info << "stop" << logger.end;
        alSourceStop(source);
        break;
    case PlaybackEventArg::PAUSE: 
        logger.info << "pause" << logger.end;
        alSourcePause(source);
        break;
    default: 
        logger.warning << "Unknown playback event type" << logger.end;
    }
}

} // NS Sound
} // NS OpenEngine
