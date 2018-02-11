#include <jni.h>
#include <string>
#include <dlfcn.h>
#include "VLog.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


extern "C"
JNIEXPORT jstring JNICALL
Java_vincent_loaddex_study_MainActivity_stringFromJNI(JNIEnv *env, jobject instance) {

    return env->NewStringUTF("Hello");
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_vincent_loaddex_study_VTDexLoader_loadDexNative
        (JNIEnv *env, jclass jcls, jobject jcontext, jstring jpath) {
    jclass contextCls = env->GetObjectClass(jcontext);

    // ClassLoader cl = context.getClassLoader();
    jmethodID getClassLoaderMethod = env->GetMethodID(contextCls, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject classLoaderObj = env->CallObjectMethod(jcontext, getClassLoaderMethod);


    // ClassLoader parent = cl.getParent();
    jclass classLoaderCls = env->GetObjectClass(classLoaderObj);
    jmethodID getParentMethod = env->GetMethodID(classLoaderCls, "getParent", "()Ljava/lang/ClassLoader;");
    jobject classLoaderParentObj = env->CallObjectMethod(classLoaderObj, getParentMethod);


    // ApplicationInfo info = context.getApplicationInfo();
    jmethodID getApplicationInfoMethod = env->GetMethodID(contextCls, "getApplicationInfo", "()Landroid/content/pm/ApplicationInfo;");
    jobject applicationInfoObj = env->CallObjectMethod(jcontext, getApplicationInfoMethod);

    // info.nativeLibraryDir
    jclass applicationInfoCls = env->GetObjectClass(applicationInfoObj);
    jfieldID nativeLibraryDirField = env->GetFieldID(applicationInfoCls, "nativeLibraryDir", "Ljava/lang/String;");
    jstring nativeLibraryDirStr = (jstring) env->GetObjectField(applicationInfoObj, nativeLibraryDirField);

    // context.getCacheDir()
    jmethodID getCacheDirMethod = env->GetMethodID(contextCls, "getCacheDir", "()Ljava/io/File;");
    jobject cacheDirFileObj = env->CallObjectMethod(jcontext, getCacheDirMethod);

    // File.getAbsolutePath()
    jclass fileCls = env->GetObjectClass(cacheDirFileObj);
    jmethodID getAbsolutePathMethod = env->GetMethodID(fileCls, "getAbsolutePath", "()Ljava/lang/String;");
    jstring cacheDirPath = (jstring) env->CallObjectMethod(cacheDirFileObj, getAbsolutePathMethod);

    // DexClassLoader dcl = new DexClassLoader();
    jclass dexClassLoaderCls = env->FindClass("dalvik/system/DexClassLoader");
    jmethodID contr = env->GetMethodID(dexClassLoaderCls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
    jobject dexClassLoaderObj = env->NewObject(dexClassLoaderCls, contr, jpath, cacheDirPath, nativeLibraryDirStr, classLoaderParentObj);

    jfieldID parentField = env->GetFieldID(classLoaderCls, "parent", "Ljava/lang/ClassLoader;");
    env->SetObjectField(classLoaderObj, parentField, dexClassLoaderObj);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return false;
    }

    return true;
}




extern "C"
JNIEXPORT jboolean JNICALL
Java_vincent_loaddex_study_VTDexLoader_loadDexUsedLibrary  (JNIEnv *env, jclass jcls, jobject jclassLoader, jobject jdexDir, jobject jfiles) {
    jclass multiDexCls = env->FindClass("android/support/multidex/MultiDex");
    jmethodID installSecondaryDexMethod = env->GetStaticMethodID(multiDexCls, "installSecondaryDexes", "(Ljava/lang/ClassLoader;Ljava/io/File;Ljava/util/List;)V");
    env->CallStaticVoidMethod(multiDexCls, installSecondaryDexMethod, jclassLoader, jdexDir, jfiles);
    env->DeleteLocalRef(multiDexCls);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return false;
    }

    return true;
}



extern "C"

JNIEXPORT jint JNICALL
Java_vincent_loaddex_study_VTDexLoader_dlOpenTest(JNIEnv *env, jclass type) {



    /**
     * 4.3是可以的
     */
    LOGD("Java_vincent_loaddex_study_VTDexLoader_dlOpenTest");
   void* handle =  dlopen("/system/lib/libdvm.so", RTLD_NOW);

    if(!handle){
        LOGE("dlopen option failed.");
        return 0;
    }
    unsigned int (*dexRoundUp2) (unsigned int);
    dexRoundUp2 = (unsigned int (*) (unsigned int))dlsym(handle,"_Z16dexRoundUpPower2j");
    if(dexRoundUp2){

        unsigned  int result = dexRoundUp2(10);
        LOGD("dexRound2(10):%d", result);
        return result;
    }
    return 0;


}



/**
 * native asset 文件操作
 */
extern "C"
JNIEXPORT void JNICALL
Java_vincent_loaddex_study_VTDexLoader_assetCopyNative(JNIEnv *env, jobject instance,
                                                       jobject jcontext, jstring fileName_) {
    const char *fileName = env->GetStringUTFChars(fileName_, 0);


    env->PushLocalFrame(16);
    // 通过调用Context.getAssets()获取java层中的AssetManger对象
    jclass contextCls = env->GetObjectClass(jcontext);
    jmethodID getAssetsMethod = env->GetMethodID(contextCls, "getAssets", "()Landroid/content/res/AssetManager;");
    jobject assetManagerObj = env->CallObjectMethod(jcontext, getAssetsMethod);

     AAssetManager *am = AAssetManager_fromJava(env, assetManagerObj);
    AAsset *asset = AAssetManager_open(am, fileName, 0);

    off_t len = AAsset_getLength(asset);
    const void *assetBuf = AAsset_getBuffer(asset);


    env->PopLocalFrame(NULL);
    env->ReleaseStringUTFChars(fileName_, fileName);
}