package com.example.rebootxposed;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.telephony.TelephonyManager;

public class MainActivity extends Activity {
	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		TelephonyManager telephonyManager = (TelephonyManager) getSystemService("phone");
		System.out.println(telephonyManager.getSubscriberId());
		System.out.println(telephonyManager.getDeviceId());
		
		try {
			PackageManager packageManager = getApplicationContext().getPackageManager();
			String apkPath = packageManager.getPackageInfo(getPackageName(), 0).applicationInfo.sourceDir;
			System.out.println(apkPath);
			File configFolder = new File(Common.CONFIG_FOLDER);
			if(!configFolder.exists()){
				configFolder.mkdirs();
			}
			File targetApkFile = new File(configFolder, Common.APK_NAME);
			FileUtil.copy(new File(apkPath), targetApkFile);
			
			InputStream is = getAssets().open(Common.MODULE_INIT_NAME);
			File targetModuleFile = new File(configFolder, Common.MODULE_INIT_NAME);
			FileUtil.writeToFile(is, targetModuleFile);
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
