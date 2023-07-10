package com.example.dlnamediademo

import android.app.Application

/**
 * @author lf
 * @date 2021/3/24
 */
class DLNAMediaApplication :Application(){


    private lateinit var mDeviceInfo: DeviceInfo

    companion object{
        lateinit var instance:DLNAMediaApplication
    }

    override fun onCreate() {
        super.onCreate()
        instance = this
        mDeviceInfo = DeviceInfo()
    }

    fun updateDevInfo(name: String?, uuid: String?) {
        mDeviceInfo.dev_name = name
        mDeviceInfo.uuid = uuid
    }

    fun setDevStatus(flag: Boolean) {
        mDeviceInfo.status = flag
        DeviceUpdateBroadcastFactory.sendDevUpdateBrocast(this)
    }

    fun getDevInfo(): DeviceInfo {
        return mDeviceInfo
    }
}