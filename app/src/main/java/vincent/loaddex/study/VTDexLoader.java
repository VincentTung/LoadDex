package vincent.loaddex.study;

import android.content.Context;
import android.content.pm.ApplicationInfo;

import java.io.File;
import java.lang.reflect.Field;
import java.util.List;

import dalvik.system.DexClassLoader;

public class VTDexLoader {


    public static native boolean loadDexNative(Context context,String path);

    private static final String FILED_PARENT = "parent";

    //classLoader->dexClassLoader->parentClassloader(classLoader.getParent)
    public static  boolean loadDex(Context context, File file) {

        ClassLoader classLoader = context.getClassLoader();
        ClassLoader parentClassLoader = classLoader.getParent();

        ApplicationInfo info = context.getApplicationInfo();
        DexClassLoader dexClassLoader = new DexClassLoader(file.getPath(), context.getCacheDir().getAbsolutePath(), info.nativeLibraryDir, parentClassLoader);

        Field[] fields = classLoader.getClass().getDeclaredFields();

        for (Field field : fields) {
            if (FILED_PARENT.equals(field.getName())) {
                try {
                    field.set(classLoader, dexClassLoader);
                } catch (IllegalAccessException e) {
                    e.printStackTrace();
                }
                return true;
            }
        }
        return false;

    }

    public native static boolean loadDexUsedLibrary(ClassLoader cl, File dexDir, List<File> files);

}
