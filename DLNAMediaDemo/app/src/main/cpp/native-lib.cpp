#include <jni.h>
#include <string>
#include "Platinum.h"
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MediaRenderer", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "MediaRenderer", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "MediaRenderer", __VA_ARGS__))

static const int MEDIA_RENDER_CTL_MSG_BASE = 0x100;
static const int MEDIA_RENDER_CTL_MSG_SET_AV_URL = (MEDIA_RENDER_CTL_MSG_BASE + 0);
static const int MEDIA_RENDER_CTL_MSG_STOP = (MEDIA_RENDER_CTL_MSG_BASE + 1);
static const int MEDIA_RENDER_CTL_MSG_PLAY = (MEDIA_RENDER_CTL_MSG_BASE + 2);
static const int MEDIA_RENDER_CTL_MSG_PAUSE = (MEDIA_RENDER_CTL_MSG_BASE + 3);
static const int MEDIA_RENDER_CTL_MSG_SEEK = (MEDIA_RENDER_CTL_MSG_BASE + 4);
static const int MEDIA_RENDER_CTL_MSG_SETVOLUME = (MEDIA_RENDER_CTL_MSG_BASE + 5);
static const int MEDIA_RENDER_CTL_MSG_SETMUTE = (MEDIA_RENDER_CTL_MSG_BASE + 6);
static const int MEDIA_RENDER_CTL_MSG_SETPLAYMODE = (MEDIA_RENDER_CTL_MSG_BASE + 7);
static const int MEDIA_RENDER_CTL_MSG_PRE = (MEDIA_RENDER_CTL_MSG_BASE + 8);
static const int MEDIA_RENDER_CTL_MSG_NEXT = (MEDIA_RENDER_CTL_MSG_BASE + 9);

static JavaVM *jvm;

static PLT_MediaRenderer *mediaRenderer;
static jclass platinumReflectionClass;
static jmethodID platinumReflectionMethodId;

static char *GetCharArrFromByteArray(JNIEnv *env, jbyteArray buf) {
    char *chars = NULL;
    jbyte *bytes;
    bytes = env->GetByteArrayElements(buf, 0);
    int chars_len = env->GetArrayLength(buf);
    chars = new char[chars_len + 1];
    memset(chars, 0, chars_len + 1);
    memcpy(chars, bytes, chars_len);
    chars[chars_len] = 0;
    env->ReleaseByteArrayElements(buf, bytes, 0);
    return chars;
}

/**
 * MediaRenderDelegate
 */
class DelegateImpl : public PLT_MediaRendererDelegate {

private:

public:

    explicit DelegateImpl() {
    }

    /**
     * 回调到 Java 中
     * @param cmd
     * @param value
     * @param data
     */
    void onActionReflection(jint cmd, char *value, char *data) {
        JNIEnv *env;
        jint res = jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
        if (res != JNI_OK) {
            res = jvm->AttachCurrentThread(&env, NULL);
            if (JNI_OK != res) {
                LOGE("Failed to AttachCurrentThread, ErrorCode = %d", res);
                return;
            }
        }


        jstring valueStr = env->NewStringUTF(value);
        jstring dataStr = env->NewStringUTF(data);
        env->CallStaticVoidMethod(platinumReflectionClass,
                                  platinumReflectionMethodId,
                                  (jint) cmd,
                                  valueStr,
                                  dataStr);
        env->DeleteLocalRef(valueStr);
        env->DeleteLocalRef(dataStr);
        jvm->DetachCurrentThread();
    }


    ~DelegateImpl() override = default;

    NPT_Result OnGetCurrentConnectionInfo(PLT_ActionReference &action) override {
        if (NPT_FAILED(action->VerifyArgumentValue("ConnectionID", "0"))) {
            action->SetError(706, "No Such Connection.");
            return NPT_FAILURE;
        }

        if (NPT_FAILED(action->SetArgumentValue("RcsID", "0"))) {
            return NPT_FAILURE;
        }
        if (NPT_FAILED(action->SetArgumentValue("AVTransportID", "0"))) {
            return NPT_FAILURE;
        }
        if (NPT_FAILED(action->SetArgumentOutFromStateVariable("ProtocolInfo"))) {
            return NPT_FAILURE;
        }
        if (NPT_FAILED(action->SetArgumentValue("PeerConnectionManager", "/"))) {
            return NPT_FAILURE;
        }
        if (NPT_FAILED(action->SetArgumentValue("PeerConnectionID", "-1"))) {
            return NPT_FAILURE;
        }
        if (NPT_FAILED(action->SetArgumentValue("Direction", "Input"))) {
            return NPT_FAILURE;
        }
        if (NPT_FAILED(action->SetArgumentValue("Status", "Unknown"))) {
            return NPT_FAILURE;
        }

        return NPT_SUCCESS;
    }

    NPT_Result OnNext(PLT_ActionReference &action) override {
        return NPT_ERROR_NOT_IMPLEMENTED;
    }

    NPT_Result OnPause(PLT_ActionReference &action) override {
        onActionReflection(MEDIA_RENDER_CTL_MSG_PAUSE, "", "");
        return NPT_SUCCESS;

    }

    NPT_Result OnPlay(PLT_ActionReference &action) override {
        onActionReflection(MEDIA_RENDER_CTL_MSG_PLAY, "", "");
        return NPT_SUCCESS;

    }

    NPT_Result OnPrevious(PLT_ActionReference &action) override {
        return NPT_ERROR_NOT_IMPLEMENTED;
    }

    NPT_Result OnSeek(PLT_ActionReference &action) override {
        return NPT_ERROR_NOT_IMPLEMENTED;
    }

    NPT_Result OnStop(PLT_ActionReference &action) override {
        onActionReflection(MEDIA_RENDER_CTL_MSG_STOP, "", "");
        return NPT_SUCCESS;
    }

    NPT_Result OnSetAVTransportURI(PLT_ActionReference &action) override {
        NPT_String metaData;
        NPT_String uri;

        if (action->GetArgumentValue("CurrentURIMetaData", metaData) == NPT_SUCCESS &&
            action->GetArgumentValue("CurrentURI", uri) == NPT_SUCCESS) {
            onActionReflection(MEDIA_RENDER_CTL_MSG_SET_AV_URL, uri,
                               metaData);

           //  PLT_Service *serviceAVT;
                    //  if (mediaRenderer->FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1",
                    //                                       serviceAVT) == NPT_SUCCESS) {
                    //      serviceAVT->SetStateVariable("AVTransportURI", uri);
                      //    serviceAVT->SetStateVariable("AVTransportURIMetaData", metaData);
            }
            return NPT_SUCCESS;
        }
        return NPT_FAILURE;
    }

    NPT_Result OnSetPlayMode(PLT_ActionReference &action) override {
        return NPT_ERROR_NOT_IMPLEMENTED;
    }

    NPT_Result OnSetVolume(PLT_ActionReference &action) override {
        return NPT_ERROR_NOT_IMPLEMENTED;
    }

    NPT_Result OnSetVolumeDB(PLT_ActionReference &action) override {
        return NPT_ERROR_NOT_IMPLEMENTED;
    }

    NPT_Result OnGetVolumeDBRange(PLT_ActionReference &action) override {
        return NPT_ERROR_NOT_IMPLEMENTED;
    }

    NPT_Result OnSetMute(PLT_ActionReference &action) override {
        return NPT_ERROR_NOT_IMPLEMENTED;
    }
};


static PLT_UPnP *uPnP;
extern "C"
{

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
    }
    NPT_LogManager::GetDefault().Configure(
            "plist:.level=FINE;.handlers=ConsoleHandler;.ConsoleHandler.outputs=2;.ConsoleHandler.colors=false;.ConsoleHandler.filter=59");
    platinumReflectionClass =
            static_cast<jclass>(env->NewGlobalRef(
                    env->FindClass("com/geniusgithub/mediarender/jni/PlatinumReflection")));

    if (platinumReflectionClass == NULL) {
        LOGE("有点不对,类都没找到");
    }

    platinumReflectionMethodId =
            env->GetStaticMethodID(platinumReflectionClass, "onActionReflection",
                                   "(ILjava/lang/String;Ljava/lang/String;)V");

    return JNI_VERSION_1_6;
}
/**
 * 开启 MediaRender 服务
 * @param env
 * @param clazz
 * @param friendname
 * @param uuid
 * @return
 */
JNIEXPORT jint JNICALL
Java_com_geniusgithub_mediarender_jni_PlatinumJniProxy_startMediaRender(JNIEnv *env, jclass clazz,
                                                                        jbyteArray friendname,
                                                                        jbyteArray frienduuid) {
    if (uPnP != nullptr) {
        LOGI("还没停止呢");
        return -1;
    }
    char *name = GetCharArrFromByteArray(env, friendname);
    char *uuid = GetCharArrFromByteArray(env, frienduuid);
    mediaRenderer = new PLT_MediaRenderer(
            name,
            false,
            uuid);
    auto delegate = new DelegateImpl();
    mediaRenderer->SetDelegate(delegate);
    PLT_DeviceHostReference device(
            mediaRenderer
    );
    uPnP = new PLT_UPnP();
    uPnP->AddDevice(device);
    NPT_Result ret = uPnP->Start();

    return ret;
}

/**
 * 停止服务
 * @param env
 * @param clazz
 * @return
 */
JNIEXPORT jint JNICALL
Java_com_geniusgithub_mediarender_jni_PlatinumJniProxy_stopMediaRender(JNIEnv *env, jclass clazz) {
    if (uPnP != nullptr) {
        auto ret = uPnP->Stop();
        uPnP = nullptr;
        return ret;
    }
    return -1;
}

/**
 * 响应事件
 * @param env
 * @param clazz
 * @param cmd
 * @param value
 * @param data
 *
 * 暂不实现
 * @return
 */
JNIEXPORT jboolean JNICALL
Java_com_geniusgithub_mediarender_jni_PlatinumJniProxy_responseGenaEvent(JNIEnv *env, jclass clazz,
                                                                         jint cmd, jbyteArray value,
                                                                         jbyteArray data) {


    return true;
}

/**
 * 开启日志打印
 * @param env
 * @param clazz
 * @param flag
 * 暂不实现
 * @return
 */
JNIEXPORT jboolean JNICALL
Java_com_geniusgithub_mediarender_jni_PlatinumJniProxy_enableLogPrint(JNIEnv *env, jclass clazz,
                                                                      jboolean flag) {

    return false;
}

}