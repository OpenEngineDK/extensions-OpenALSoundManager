// Sound node.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <Scene/SoundNode.h>
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Scene {

SoundNode::SoundNode() {

}

/**
* Copy constructor.
* Performs a shallow copy.
*
* @param node Sound node to copy.
*/
SoundNode::SoundNode(SoundNode& node) {

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

} //NS Scene
} //NS OpenEngine
