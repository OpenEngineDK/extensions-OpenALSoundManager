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

#include <Scene/ISceneNode.h>
#include <Scene/ISceneNodeVisitor.h> 
#include <Scene/TransformationNode.h>
#include <Core/Event.h>
#include <Logging/Logger.h>
#include <AL/alc.h>
#include <AL/al.h>
#include <Scene/SoundNode.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>
#include <Sound/SoundFileTuple.h>

#include <Core/IModule.h>
#include <Core/QueuedEvent.h>
#include <Core/IListener.h>
#include <vector>

namespace OpenEngine {
namespace Sound {

using namespace std;

using namespace OpenEngine::Scene;
using OpenEngine::Core::IModule;
using OpenEngine::Core::QueuedEvent;
using OpenEngine::Core::IListener;

// Audio playback events
// Forward references
class SoundEventArg;
class PlayEventArg;
class PauseEventArg;
class StopEventArg;
class SeekEventArg;
class GainEventArg;

/**
 * Renderer using OpenGL
 *
 * @class Renderer Renderer.h Renderers/OpenGL/IRenderer.h
 */
class OpenALSoundManager : public IModule, IListener<SoundEventArg*>, public ISceneNodeVisitor {
private:
	//the current position and rotation
	Vector<3, float> pos;
	Quaternion<float>* dir;
	
	//the root of the tree
	ISceneNode* theroot;

	//the buffers

public:
    static QueuedEvent<SoundEventArg*>* process;

    OpenALSoundManager(ISceneNode* root);
    ~OpenALSoundManager();

    void Initialize();
    void Process(const float deltaTime, const float percent);
    void Deinitialize();
    bool IsTypeOf(const std::type_info& inf);

    void HandleEvent(SoundEventArg* e);
    void HandleEvent(PlayEventArg* e);
    void HandleEvent(StopEventArg* e);
    void HandleEvent(PauseEventArg* e);
    void HandleEvent(GainEventArg* e);
    void Handle(SoundEventArg* e);

    void VisitTransformationNode(TransformationNode* node);
	void VisitSoundNode(SoundNode* node);

	bool CheckID(int id);


};


class SoundEventArg {
public:
    SoundEventArg() {}
    virtual ~SoundEventArg() {}
    //OpenALSoundResource* sound
    int id;
    virtual void Apply(OpenALSoundManager* sm) = 0;
};

class PlayEventArg : public SoundEventArg {
public:
    PlayEventArg() {}
    virtual ~PlayEventArg() {}
    void Apply(OpenALSoundManager* sm) {
        sm->HandleEvent(this);
    };
};

class StopEventArg : public SoundEventArg {
public:
    StopEventArg() {}
    virtual ~StopEventArg() {}
    void Apply(OpenALSoundManager* sm) {
        sm->HandleEvent(this);
    };
};

class PauseEventArg : public SoundEventArg {
public:
    PauseEventArg() {}
    virtual ~PauseEventArg() {}    
    void Apply(OpenALSoundManager* sm) {
        sm->HandleEvent(this);
    };
};

class SeekEventArg : public SoundEventArg {
public:
    SeekEventArg() {}
    virtual ~SeekEventArg() {}
    int pos;
    void Apply(OpenALSoundManager* sm) {
        sm->HandleEvent(this);
    };
};

class GainEventArg : public SoundEventArg {
public:
    GainEventArg() {}
    virtual ~GainEventArg() {}
    float gain;
    void Apply(OpenALSoundManager* sm) {
        sm->HandleEvent(this);
    };
};

} // NS Sound
} // NS OpenEngine

#endif
