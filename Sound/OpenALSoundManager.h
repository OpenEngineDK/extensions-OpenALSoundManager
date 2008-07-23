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

#include <Meta/OpenAL.h>
#include <Scene/SoundNode.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>
//#include <Sound/SoundFileTuple.h>

#include <Core/IModule.h>
#include <Core/QueuedEvent.h>
#include <Core/IListener.h>
#include <vector>
#include <map>

namespace OpenEngine {
namespace Sound {

using namespace std;

using namespace OpenEngine::Scene;
using OpenEngine::Core::IModule;
using OpenEngine::Core::QueuedEvent;
using OpenEngine::Core::IListener;

// Audio playback events
struct PlaybackEventArg  {
    enum PlaybackAction {
        PLAY,
        STOP,
        PAUSE
    };
    
    int id;
    PlaybackAction action;
};

/**
 * Renderer using OpenGL
 *
 * @class Renderer Renderer.h Renderers/OpenGL/IRenderer.h
 */
class OpenALSoundManager : public IModule, IListener<PlaybackEventArg>, public ISceneNodeVisitor {
private:
	//the current position and rotation
	Vector<3, float> pos;
	Quaternion<float>* dir;
	
	//the root of the tree
	ISceneNode* theroot;

	//the map
//	map<int,ALuint*> loadedsoundfiles;

public:
    static QueuedEvent<PlaybackEventArg>* playback;

    OpenALSoundManager(ISceneNode* root);
    ~OpenALSoundManager();

    void Initialize();
    void Process(const float deltaTime, const float percent);
    void Deinitialize();
    bool IsTypeOf(const std::type_info& inf);

    void Handle(PlaybackEventArg e);

    void VisitTransformationNode(TransformationNode* node);
	void VisitSoundNode(SoundNode* node);

//	bool CheckID(int id);

};

} // NS Sound
} // NS OpenEngine

#endif
