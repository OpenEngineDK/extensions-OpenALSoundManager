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

namespace OpenEngine {
namespace Sound {

//init static event queue
QueuedEvent<PlaybackEventArg>* OpenALSoundManager::playback = new QueuedEvent<PlaybackEventArg>();


OpenALSoundManager::OpenALSoundManager(ISceneNode* root): theroot(root) {
    playback->Attach(*this);
}

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

    // process the event queue
    OpenALSoundManager::playback->Release();

	//init to assumed startposition
	pos[0] = 0.0;
	pos[1] = 0.0;
	pos[2] = 0.0;

	//init to assumed direction
	dir = new Quaternion<float>();

	theroot->Accept(*this);

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

/*
bool OpenALSoundManager::CheckID(int id) {

	map<int, ALuint*>::iterator iter = loadedsoundfiles.find(id);
	return (iter != loadedsoundfiles.end());

}
*/

void OpenALSoundManager::VisitSoundNode(SoundNode* node) {

	//make nessesary variables
	ALuint* source;
	ALCenum error;
	
	//check if loaded
/*	map<int, ALuint*>::iterator iter = loadedsoundfiles.find(node->GetID());
	
	if (iter != loadedsoundfiles.end()) {

		source = iter->second;
*/
	if (node->GetID() != -1) {
		//setup the source settings
		alSource3f(*source, AL_POSITION, pos[0], pos[1], pos[2]);
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to set position but got: " << error << logger.end;

		alSourcef(*source, AL_GAIN, (ALfloat)node->GetGain());
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to set position but got: " << error << logger.end;

	}
	//we have to load it :(((
	else {
				
		//generate the source
		alGenSources(1, source);
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to gen source but got: " << error << logger.end;

		//setup the source settings
		alSource3f(*source, AL_POSITION, pos[0], pos[1], pos[2]);
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to set position but got: " << error << logger.end;

		alSourcef(*source, AL_GAIN, (ALfloat)node->GetGain());
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to set position but got: " << error << logger.end;

		//TODO BIND THIS TO Chistains ACTUAL DATA
		ALuint *data;
		
		//attach the buffer
		alSourcei(*source, AL_BUFFER, *data);
		if ((error = alGetError()) != AL_NO_ERROR) 
			logger.info << "tried to bind source and buffer together but got: " << error << logger.end;

		node->SetID(*source);

/*		//remember to add to loaded files
		loadedsoundfiles[node->GetID()] = source; */
	}

	node->VisitSubNodes(*this);

}

void OpenALSoundManager::Handle(PlaybackEventArg e) {
    switch (e.action) {
    case PlaybackEventArg::PLAY: 
        logger.info << "lets play!" << logger.end;
        if (e.id == -1) {
            logger.warning << "OpenAL sound not initialized" << logger.end;
            return;
        }
        alSourcePlay(e.id);
    break;
    case PlaybackEventArg::STOP: 
        logger.info << "lets stop!" << logger.end;
        if (e.id == -1) {
            logger.warning << "OpenAL sound not initialized" << logger.end;
            return;
        }
        alSourceStop(e.id);
        break;
    case PlaybackEventArg::PAUSE: 
        logger.info << "lets pause!" << logger.end;
        if (e.id == -1) {
            logger.warning << "OpenAL sound not initialized" << logger.end;
            return;
        }
        alSourcePause(e.id);
        break;
    default: 
        logger.warning << "Unknown playback event type" << logger.end;
    }
}


} // NS Sound
} // NS OpenEngine
