LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Game/Board.cpp \
                   ../../Classes/Game/CardDeck.cpp \
                   ../../Classes/Game/CardShuffler.cpp \
                   ../../Classes/Game/BoardState.cpp \
                   ../../Classes/Game/Player.cpp \
                   ../../Classes/Game/AIPlayer.cpp \
                   ../../Classes/Game/Card.cpp \
                   ../../Classes/Game/Menu.cpp \
                   ../../Classes/Game/FileOperation.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../Classes/Game

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
