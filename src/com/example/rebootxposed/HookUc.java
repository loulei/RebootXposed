package com.example.rebootxposed;

import de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam;

public class HookUc implements IXposedModuleLaunch {

	@Override
	public String getHookAppPackageName() {
		// TODO Auto-generated method stub
		return "com.UCMobile.intl";
	}

	@Override
	public void handleHookApp(LoadPackageParam lpparam) throws Throwable {
		// TODO Auto-generated method stub
		System.out.println(lpparam.packageName+","+lpparam.processName);
	}

}
