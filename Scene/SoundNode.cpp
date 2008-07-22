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

SoundNode::SoundNode() {
    id = -1;
}

/**
* Copy constructor.
* Performs a shallow copy.
*
* @param node Sound node to copy.
*/
SoundNode::SoundNode(SoundNode& node) {
    id = -1;
}

SoundNode::~SoundNode() {

}

ISceneNode* SoundNode::CloneSelf() {
  SoundNode* clone = new SoundNode(*this);
  return clone;
}


void SoundNode::Accept(ISceneNodeVisitor& v) {
  v.VisitSoundNode(this);
}

void SoundNode::Play() {
    PlayEventArg* e = new PlayEventArg();
    e->id = id;
    OpenALSoundManager::process->Notify(e);
}

void SoundNode::Stop() {
    StopEventArg* e = new StopEventArg();
    e->id = id;
    OpenALSoundManager::process->Notify(e);
}

void SoundNode::Pause() {
    PauseEventArg* e = new PauseEventArg();
    e->id = id;
    OpenALSoundManager::process->Notify(e);
}

float SoundNode::GetGain() {
	return gain;
}

void SoundNode::SetGain(float gain) {
    this->gain = gain;
    GainEventArg* e = new GainEventArg();
    e->id = id;
    e->gain = gain;
    OpenALSoundManager::process->Notify(e);
}

int SoundNode::GetID() {
	return id;
}

void SoundNode::SetID(int val) {
	id = val;
}

    
} //NS Scene
} //NS OpenEngine
