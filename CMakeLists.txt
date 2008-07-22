
# Create the extension library
ADD_LIBRARY(Extensions_OpenALSoundManager
  Scene/SoundNode.cpp
  Sound/OpenALSoundManager.cpp
#  Sound/SoundFileTuple.cpp
)

TARGET_LINK_LIBRARIES(Extensions_OpenALSoundManager
  ${OPENAL_LIBRARY}
)
