package com.example.dlnamediademo;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;


public class DeviceUpdateBroadcastFactory {

	public static final String PARAM_DEV_UPDATE="com.geniusgithub.PARAM_DEV_UPDATE";
	
	public static interface IDevUpdateListener {
		public void onUpdate();
	}
	
	
	private DeviceUpdateBroadcastReceiver mReceiver;
	private Context mContext;
	
	public DeviceUpdateBroadcastFactory(Context context){
		mContext = context;
	}
	
	
	public void register(IDevUpdateListener listener){
		if (mReceiver == null){
			mReceiver = new DeviceUpdateBroadcastReceiver();
			mReceiver.setListener(listener);	
			mContext.registerReceiver(mReceiver, new IntentFilter(PARAM_DEV_UPDATE));
		}
	}

	public void unregister()
	{
		if (mReceiver != null){
			mContext.unregisterReceiver(mReceiver);
			mReceiver = null;
		}
	}
	
	public static void sendDevUpdateBrocast(Context context){
		Intent intent = new Intent();
		intent.setAction(PARAM_DEV_UPDATE);
		context.sendBroadcast(intent);
	}
	
}
