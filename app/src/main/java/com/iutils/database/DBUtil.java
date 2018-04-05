package com.iutils.database;

import android.content.Context;

import com.iutils.utils.FileUtil;
import com.iutils.utils.SystemUtil;

import net.sqlcipher.database.SQLiteDatabase;

import java.io.File;

public class DBUtil {

    public static void init(Context context) {
        SQLiteDatabase.loadLibs(context);


        File databaseFile = getDatabasePath("demo.db");
        databaseFile.mkdirs();
        databaseFile.delete();
        SQLiteDatabase database = SQLiteDatabase.openOrCreateDatabase(databaseFile, "test123", null);
        database.execSQL("create table t1(a, b)");
        database.execSQL("insert into t1(a, b) values(?, ?)", new Object[]

                {
                        "one for the money",
                        "two for the show"
                });
    }

    private static File getDatabasePath(String dbName)
    {
        return new File(FileUtil.getWorkDir()+dbName);
    }
}
