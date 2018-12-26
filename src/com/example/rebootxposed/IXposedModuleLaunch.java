package com.example.rebootxposed;

import de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam;

public interface IXposedModuleLaunch {

	void handleHookApp(LoadPackageParam lpparam) throws Throwable;
}
