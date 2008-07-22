// OpenAL sound manager implementation.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Sound/OpenALSoundManager.h>

#include <AL/al.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Sound {

QueuedEvent<SoundEventArg*>* OpenALSoundManager::process = new QueuedEvent<SoundEventArg*>();


OpenALSoundManager::OpenALSoundManager(ISceneNode* root) {
	//init the tree root
	theroot = root;
    process->Attach(*this);
}

/**
 * OpenALSoundManager destructor.
 * Performs no clean up.
 */
OpenALSoundManager::~OpenALSoundManager() {

}

void OpenALSoundManager::Initialize() {

	ALCdevice* thedevice = alcOpenDevice(NULL);

	if (thedevice) {

		ALCcontext* thecontext = alcCreateContext(thedevice, NULL);
		alcMakeContextCurrent(thecontext);

	}
	else
		logger.info << "this should never happen and if it those report to u040437 :)" << logger.end;

}

/**
 * @note The processing function assumes that the scene has not been
 *       replaced by null since the initialization face. 
 */
void OpenALSoundManager::Process(const float deltaTime, const float percent) {

    OpenALSoundManager::process->Release();

	//init to assumed startposition
	pos[0] = 0.0;
	pos[1] = 0.0;
	pos[2] = 0.0;

	//init to assumed direction
	dir = new Quaternion<float>();

	theroot->VisitSubNodes(*this);

	delete(dir);
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

bool OpenALSoundManager::CheckID(int id) {

	return false;	

}

void OpenALSoundManager::VisitSoundNode(SoundNode* node) {

	//check if loaded
	if (CheckID(node->GetID())) {

	}
	//we have to load it :(((
	else {
		//make nessesary variables
		ALuint buffer[1], source[1];
		ALCenum error;

		//generate the buffer
		alGenBuffers(1, &(buffer[0]));
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to gen buffer but got: " << error << logger.end;

		//TODO BIND THIS TO Chistains ACTUAL DATA
		ALvoid *data;
		//fill the buffer
		alBufferData(buffer[0], AL_FORMAT_MONO8, data, 10, 200);
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to load the buffer but got: " << error << logger.end;

		//generate the source
		alGenSources(1, &(source[0]));
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to gen source but got: " << error << logger.end;

		//setup the source settings
		alSource3f(source[0], AL_POSITION, pos[0], pos[1], pos[2]);
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to set position but got: " << error << logger.end;

		alSourcef(source[0], AL_GAIN, (ALfloat)node->GetGain());
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to set position but got: " << error << logger.end;

		//attach the buffer
		alSourcei(source[0], AL_BUFFER, buffer[0]);
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to bind source and buffer together but got: " << error << logger.end;
	}

	node->VisitSubNodes(*this);

}


void OpenALSoundManager::HandleEvent(PlayEventArg* e) {
    logger.info << "lets play!" << logger.end;
    if (e->id == -1) {
        logger.warning << "OpenAL sound not initialized" << logger.end;
        return;
    }
    
    alSourcePlay(e->id);
}

void OpenALSoundManager::HandleEvent(StopEventArg* e) {
    logger.info << "lets stop!" << logger.end;
    if (e->id == -1) {
        logger.warning << "OpenAL sound not initialized" << logger.end;
        return;
    }
    alSourceStop(e->id);
}

void OpenALSoundManager::HandleEvent(PauseEventArg* e) {
    logger.info << "lets pause!" << logger.end;
    if (e->id == -1) {
        logger.warning << "OpenAL sound not initialized" << logger.end;
        return;
    }
    alSourcePause(e->id);
}

void OpenALSoundManager::HandleEvent(GainEventArg* e) {
    logger.info << "lets gain!" << logger.end;
    if (e->id == -1) {
        logger.warning << "OpenAL sound not initialized" << logger.end;
        return;
    }
    alSourcef(e->id, AL_GAIN, e->gain);
}

void OpenALSoundManager::HandleEvent(SoundEventArg* e) {
    
    logger.warning << "Unknown event type" << logger.end;
}

void OpenALSoundManager::Handle(SoundEventArg* e) {
    e->Apply(this);
    delete e;
}


} // NS Sound
} // NS OpenEngine
