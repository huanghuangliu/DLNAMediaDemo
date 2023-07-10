package com.example.dlnamediademo.center;

import android.content.Context;
import android.content.Intent;

import com.example.dlnamediademo.DLNAMediaApplication;
import com.example.dlnamediademo.service.MediaRenderService;
import com.example.dlnamediademo.util.CommonLog;
import com.example.dlnamediademo.util.LogFactory;

/**
 * 投屏操作
 */
public class MediaRenderProxy implements IBaseEngine{

	private static final CommonLog log = LogFactory.createLog();
	
	private static MediaRenderProxy mInstance;
	private Context mContext;
	
	private MediaRenderProxy(Context context) {
		mContext = context;
	}

	public static synchronized MediaRenderProxy getInstance() {
		if (mInstance == null){
			mInstance  = new MediaRenderProxy(DLNAMediaApplication.instance);
		}
		return mInstance;
	}

	@Override
	public boolean startEngine() {
		Intent intent = new Intent(MediaRenderService.START_RENDER_ENGINE);
		intent.setPackage(mContext.getPackageName());
		mContext.startService(intent);
		return true;
	}

	@Override
	public boolean stopEngine() {
		mContext.stopService(new Intent(mContext, MediaRenderService.class));
		return true;
	}

	@Override
	public boolean restartEngine() {
		Intent intent = new Intent(MediaRenderService.RESTART_RENDER_ENGINE);
		intent.setPackage(mContext.getPackageName());
		mContext.startService(intent);
		return true;
	}

}
