package com.example.dlnamediademo

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.dlnamediademo.center.MediaRenderProxy
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity(), DeviceUpdateBroadcastFactory.IDevUpdateListener {

    private lateinit var mRenderProxy:MediaRenderProxy
    private lateinit var mBrocastFactory: DeviceUpdateBroadcastFactory

    private var status = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        initData()

        tv_start.setOnClickListener {
            if (status){
                stop()
                tv_start.text = "开启投屏"
            }else{
                start()
                tv_start.text = "关闭投屏"
            }
        }
    }

    fun initData(){
        mRenderProxy = MediaRenderProxy.getInstance()
        mBrocastFactory =
            DeviceUpdateBroadcastFactory(this)

        updateDevInfo(DLNAMediaApplication.instance.getDevInfo())
        mBrocastFactory.register(this)

    }

    private fun updateDevInfo(info: DeviceInfo) {
        status = info.status
        val status = if (info.status) "open" else "close"
        val text = """
            投屏状态: $status
            投屏设备: ${info.dev_name}
            uuid: ${info.uuid}
            """.trimIndent()
        tv_info.text = text
    }

    fun start(){
        mRenderProxy.startEngine()
    }
    fun reset(){
        mRenderProxy.restartEngine()
    }
    fun stop(){
        mRenderProxy.stopEngine()
    }

    override fun onUpdate() {
        updateDevInfo(DLNAMediaApplication.instance.getDevInfo())
    }
}