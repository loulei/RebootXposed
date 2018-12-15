package com.example.rebootxposed;

import de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam;

public interface IXposedModuleLaunch {

	String getHookAppPackageName();
	
	void handleHookApp(LoadPackageParam lpparam) throws Throwable;
}
