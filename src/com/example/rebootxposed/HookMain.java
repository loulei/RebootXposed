package com.example.rebootxposed;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.lang.reflect.Method;

import android.app.Application;
import android.content.Context;
import dalvik.system.DexClassLoader;
import dalvik.system.PathClassLoader;
import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam;

public class HookMain implements IXposedHookLoadPackage {
	
	@Override
	public void handleLoadPackage(final LoadPackageParam lpparam) throws Throwable {
		// TODO Auto-generated method stub
		
		File apkFile = new File(Common.CONFIG_FOLDER, Common.APK_NAME);
		if(apkFile.exists()){
//			File localApk = new File("/data/data/"+lpparam.packageName + "/rebootapp.apk");
//			FileUtil.copy(apkFile, localApk);
//			File dexOutDir = new File("/data/data/"+lpparam.packageName + "/rebootapp_dex/");
//			if(!dexOutDir.exists()){
//				dexOutDir.mkdirs();
//			}
			DexClassLoader dexClassLoader = new DexClassLoader(apkFile.getPath(), "/data/data/"+lpparam.packageName + "/", null, ClassLoader.getSystemClassLoader());
			File moduleFile = new File(Common.CONFIG_FOLDER, Common.MODULE_INIT_NAME);
			BufferedReader bufferedReader = new BufferedReader(new FileReader(moduleFile));
			String moduleName = bufferedReader.readLine();
			bufferedReader.close();
			Class<?> moduleClass = Class.forName(moduleName, true, dexClassLoader);
			if(moduleClass == null){
				return;
			}
			Object module = moduleClass.newInstance();
			boolean isModule = false;
			Class<?>[] interfaces = moduleClass.getInterfaces();
			if(interfaces != null && interfaces.length > 0){
				for(Class<?> inter : interfaces){
					if("com.example.rebootxposed.IXposedModuleLaunch".equals(inter.getName())){
						isModule = true;
						break;
					}
				}
				if(isModule){
					Method getHookAppPackageNameMd = moduleClass.getDeclaredMethod("getHookAppPackageName");
					getHookAppPackageNameMd.setAccessible(true);
					String packageName = (String) getHookAppPackageNameMd.invoke(module);
					if(lpparam.packageName.equals(packageName)){
						Method handleHookAppMd = moduleClass.getDeclaredMethod("handleHookApp", LoadPackageParam.class);
						handleHookAppMd.setAccessible(true);
						handleHookAppMd.invoke(module, lpparam);
					}
				
				}
			}
		}
		

	}

}
