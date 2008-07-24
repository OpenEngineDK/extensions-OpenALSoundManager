
# Create the extension library
ADD_LIBRARY(Extensions_OpenALSoundManager
  Scene/SoundNode.cpp
  Sound/OpenALSoundManager.cpp
  Sound/SoundNodeVisitor.cpp
)

TARGET_LINK_LIBRARIES(Extensions_OpenALSoundManager
  Extensions_OpenALSoundResource
  ${OPENAL_LIBRARY}
)
