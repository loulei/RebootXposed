package com.example.rebootxposed;

import de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam;

public class HookWeixin implements IXposedModuleLaunch {

	@Override
	public void handleHookApp(LoadPackageParam lpparam) throws Throwable {
		// TODO Auto-generated method stub
		System.out.println("88888888888811111"+lpparam.processName);
	}

}
