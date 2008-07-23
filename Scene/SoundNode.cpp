// Sound node.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Scene/SoundNode.h>

#include <Sound/OpenALSoundManager.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Scene {

using namespace OpenEngine::Sound;

SoundNode::SoundNode(ISoundResourcePtr resource) : resource(resource) { }

/**
* Copy constructor.
* Performs a shallow copy.
*
* @param node Sound node to copy.
*/
SoundNode::SoundNode(SoundNode& node) { }

SoundNode::~SoundNode() {}

ISceneNode* SoundNode::CloneSelf() {
  SoundNode* clone = new SoundNode(*this);
  return clone;
}


void SoundNode::Accept(ISceneNodeVisitor& v) {
  v.VisitSoundNode(this);
}

void SoundNode::Play() {
    PlaybackEventArg e;
    e.action = PlaybackEventArg::PLAY;
    OpenALSoundManager::playback->Notify(e);
}

void SoundNode::Stop() {
    PlaybackEventArg e;
    e.action = PlaybackEventArg::STOP;
    OpenALSoundManager::playback->Notify(e);
}

void SoundNode::Pause() {
    PlaybackEventArg e;
    e.action = PlaybackEventArg::PAUSE;
    OpenALSoundManager::playback->Notify(e);
}

float SoundNode::GetGain() {
	return gain;
}

void SoundNode::SetGain(float gain) {
    this->gain = gain;
//     GainEventArg e;
//     e.id = id;
//     e.gain = gain;
//     OpenALSoundManager::process->Notify(e);
}

ISoundResourcePtr SoundNode::GetResource() {
    return resource;
}
    
} //NS Scene
} //NS OpenEngine
