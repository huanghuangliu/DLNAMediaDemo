package com.example.dlnamediademo.util;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Build;

import com.example.dlnamediademo.BuildConfig;

/**
 * 缓存
 */
public class LocalConfigSharePreference {

	public static final String preference_name = "LocalConfigSharePreference";
	public static final String dev_name = "dev_name";
	
	public static boolean commitDevName(Context context, String devName){
	
		SharedPreferences sharedPreferences = context.getSharedPreferences(preference_name, 0);
		Editor editor = sharedPreferences.edit();
		editor.putString(dev_name, devName);
		editor.commit();
		return true;
	}
	
	public static String getDevName(Context context){
		SharedPreferences sharedPreferences = context.getSharedPreferences(preference_name, 0);
		String value = sharedPreferences.getString(dev_name, "你的设备:"+ Build.BRAND);
		return value;
	}
	
}
