package vincent.loaddex.study.activity;

import android.content.Intent;
import android.content.res.AssetManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;


import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import vincent.loaddex.study.R;
import vincent.loaddex.study.VTDexLoader;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static {
        System.loadLibrary("native-lib");
    }

    private static final String TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.sample_btn).setOnClickListener(this);


        try {
            File file = copyAsset(DEX_FILE_NAME);
            if (null != file) {

                List<File> files = new ArrayList<File>(1);
                files.add(file);

                VTDexLoader.loadDexNative(this, file.getAbsolutePath());
//                boolean result = VTDexLoader.loadDexUsedLibrary(getClassLoader(), getCacheDir(), files);
//                Log.d(TAG, "load dex result: " + result);

            } else {
                Log.d(TAG, "load dex failed");
            }
        } catch (IOException e) {
            e.printStackTrace();
            Log.d(TAG, "load dex failed");
        }
    }


    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.sample_btn) {

            ClassLoader classLoader = getClassLoader();

            Class<?> clazz = null;
            try {
                clazz = Class.forName("vincent.loaddex.study.demo.DemoActivity");
                startActivity(new Intent(this,clazz));
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            }


        }
    }

    //要加载的dex项目
    private static final String DEX_FILE_NAME = "classes.dex";

    private File copyAsset(String name) throws IOException {
        BufferedInputStream bis = null;
        BufferedOutputStream bos = null;
        AssetManager am = getAssets();
        File file = new File(getCacheDir(), name);
        if(file.exists())file.delete();
        file.createNewFile();
        try {
            bis = new BufferedInputStream(am.open(name));
            bos = new BufferedOutputStream(new FileOutputStream(file));
            byte[] buf = new byte[4096];
            int len;
            while ((len = bis.read(buf)) != -1) {
                bos.write(buf, 0, len);
            }

            return file;
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (bis != null) {
                try {
                    bis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (bos != null) {
                try {
                    bos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return null;
    }


}
