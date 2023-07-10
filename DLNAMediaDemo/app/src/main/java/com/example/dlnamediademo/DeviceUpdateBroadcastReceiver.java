package com.example.dlnamediademo;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.example.dlnamediademo.util.CommonLog;
import com.example.dlnamediademo.util.LogFactory;

public class DeviceUpdateBroadcastReceiver extends BroadcastReceiver {

	private static final CommonLog log = LogFactory.createLog();
	private DeviceUpdateBroadcastFactory.IDevUpdateListener mListener;
	
	public void setListener(DeviceUpdateBroadcastFactory.IDevUpdateListener listener){
		mListener = listener;
	}
	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		if (action == null){
			return ;
		}
		
		if (DeviceUpdateBroadcastFactory.PARAM_DEV_UPDATE.equalsIgnoreCase(action)){
			if (mListener != null){
				mListener.onUpdate();
			}
		}
	}
	
	
}
