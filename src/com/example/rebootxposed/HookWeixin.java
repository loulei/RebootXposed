package com.example.rebootxposed;

import de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam;

public class HookWeixin implements IXposedModuleLaunch {

	@Override
	public String getHookAppPackageName() {
		// TODO Auto-generated method stub
		return "com.tencent.mm";
	}

	@Override
	public void handleHookApp(LoadPackageParam lpparam) throws Throwable {
		// TODO Auto-generated method stub
		System.out.println("9999999999999"+lpparam.processName);
	}

}
