// OpenAL sound manager implementation.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OPENAL_SOUND_MANAGER_H_
#define _OPENAL_SOUND_MANAGER_H_

#include <Display/IViewingVolume.h>
#include <Scene/ISceneNode.h>
#include <Core/Event.h>
#include <Logging/Logger.h>

#include <Meta/OpenAL.h>
#include <Math/Quaternion.h>
#include <Math/Vector.h>

#include <Core/IModule.h>
#include <Core/QueuedEvent.h>
#include <Core/IListener.h>
#include <Scene/SoundNode.h>
#include <Sound/ISound.h>
#include <Resources/ISoundResource.h>

namespace OpenEngine {
namespace Sound {

using OpenEngine::Core::IModule;
using OpenEngine::Core::QueuedEvent;
using OpenEngine::Core::IListener;
using OpenEngine::Display::IViewingVolume;
using OpenEngine::Math::Vector;
using OpenEngine::Math::Quaternion;
using OpenEngine::Scene::ISceneNode;
using OpenEngine::Scene::SoundNode;
using OpenEngine::Resources::ISoundResourcePtr;

class OpenALSoundManager : public IModule {
private:
    ISceneNode* theroot;
    IViewingVolume* vv;
    
    void Init();

    class OpenALSound: public ISound {
    private:
        ALuint sourceID;
	ALuint bufferID;
	Vector<3,float> pos;
	Quaternion<float> dir;
	ISoundResourcePtr resource;

	void PrintAttribute(ALenum e);
	string EnumToString(ALenum e);
    public:
	OpenALSound(ISoundResourcePtr resource);
	~OpenALSound();
	void Initialize();
	void Play();
	void Stop();
	void Pause();
	Vector<3,float> GetPosition();
	void SetPosition(Vector<3,float> pos);
	Quaternion<float> GetRotation();
	void SetRotation(Quaternion<float> rotation);
	unsigned int GetID();
	void SetID(unsigned int);
	void SetGain(float gain);
	ISoundResourcePtr GetResource();
    };

public:
    OpenALSoundManager(ISceneNode* root, IViewingVolume* vv);
    ~OpenALSoundManager();

    ISound* CreateSound(ISoundResourcePtr resource);

    void Initialize();
    void Process(const float deltaTime, const float percent);
    void Deinitialize();
    bool IsTypeOf(const std::type_info& inf);
};

} // NS Sound
} // NS OpenEngine

#endif
