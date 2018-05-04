LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := rcscontroller
LOCAL_SRC_FILES :=  rcscontroller.cpp ../rcsagent/softagent.cpp ../rcsagent/SoftdaNew_RCSCtl.cpp\
                     ../rcsagent/IMSCallState.cpp ../rcsagent/global.cpp   \
					 ../rcsagent/uuid.cpp ../rcsagent/xcapparser.cpp  ../rcsagent/MediaInteractive.cpp\
					 ../rcsagent/fireevent.cpp ../rcsagent/Log.cpp ../rcsagent/checkNet.cpp
					 
LOCAL_SRC_FILES += 	../rcsagent/CallCtrl/Call.cpp ../rcsagent/CallCtrl/CallCtrl.cpp \
                         ../rcsagent/CallCtrl/CallStatus.cpp ../rcsagent/CallCtrl/IMS_SIP.cpp ../rcsagent/CallCtrl/IMS_Event_Conf.cpp\
						 ../rcsagent/CallCtrl/IMS_Event_CTD.cpp  ../rcsagent/CallCtrl/IMS_Method_Conf.cpp ../rcsagent/CallCtrl/SDP.cpp	\
			../rcsagent/CallCtrl/IMSDataConfEventHttpMode.cpp ../rcsagent/CallCtrl/IMSDataConfMethodHttpMode.cpp ../rcsagent/CallCtrl/ZXSock.cpp \
			../rcsagent/CallCtrl/xcomm.c ../rcsagent/CallCtrl/ConferenceBridge.cpp ../rcsagent/MOAControl.cpp 
			
LOCAL_SRC_FILES += 	../rcsagent/filetrans_new.cpp ../rcsagent/OfflineMsg.cpp ../rcsagent/BroadcastMsg.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include						 
LOCAL_LDLIBS := -lc -ldl -lm -lz -llog
#LOCAL_SHARED_LIBRARIES := zxinos BasicFunc xcap sip zteexpat msrp MediaModule zip http
LOCAL_STATIC_LIBRARIES := zip
LOCAL_SHARED_LIBRARIES := zxinos BasicFunc xcap sip msrp MediaModule http zteexpat
LOCAL_CFLAGS := -Wno-write-strings -g -DRCS

LOCAL_ARM_MODE := arm
LOCAL_LDFLAGS := -fPIC

include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))