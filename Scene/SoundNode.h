// Sound node.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _SOUND_NODE_H_
#define _SOUND_NODE_H_

#include <Scene/SceneNode.h>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

namespace OpenEngine {
namespace Scene {

/**
 * Sound node.
 * Acts as a simple node wrapping a sound source.
 *
 * @class SoundNode SoundNode.h Scene/SoundNode.h
 */
class SoundNode : public SceneNode {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        // serialize base class information
        ar & boost::serialization::base_object<SceneNode>(*this);
    }

protected:
    ISceneNode* CloneSelf();

public:
    /**
     * Default constructor.
     */
    SoundNode();

    SoundNode(SoundNode& node);

    /**
     * Destructor.
     */
    ~SoundNode();

    /**
     * Accept a visitor.
     *
     * @see ISceneNode::Accept
     */
    void Accept(ISceneNodeVisitor& visitor);

};

} // NS Scene
} // NS OpenEngine

BOOST_CLASS_EXPORT(OpenEngine::Scene::SoundNode)


#endif // _SOUND_NODE_H_
