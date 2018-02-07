#include <jni.h>
#include <string>



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