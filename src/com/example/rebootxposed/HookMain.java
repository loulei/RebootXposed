package com.example.rebootxposed;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.lang.reflect.Method;

import android.text.TextUtils;
import dalvik.system.DexClassLoader;
import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam;

public class HookMain implements IXposedHookLoadPackage {
	
	@Override
	public void handleLoadPackage(final LoadPackageParam lpparam) throws Throwable {
		// TODO Auto-generated method stub
		File moduleFile = new File(Common.CONFIG_FOLDER, Common.MODULE_INIT_NAME);
		if(moduleFile.exists()){
			BufferedReader bufferedReader = new BufferedReader(new FileReader(moduleFile));
			String moduleName = bufferedReader.readLine();
			String packageName = bufferedReader.readLine();
			bufferedReader.close();
			if(!TextUtils.isEmpty(moduleName) && !TextUtils.isEmpty(packageName)){
				if(lpparam.packageName.equals(packageName)){
					File apkFile = new File(Common.CONFIG_FOLDER, Common.APK_NAME);
					if(apkFile.exists()){
						DexClassLoader dexClassLoader = new DexClassLoader(apkFile.getPath(), "/data/data/"+lpparam.packageName + "/", null, ClassLoader.getSystemClassLoader());
						
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
								Method handleHookAppMd = moduleClass.getDeclaredMethod("handleHookApp", LoadPackageParam.class);
								handleHookAppMd.setAccessible(true);
								handleHookAppMd.invoke(module, lpparam);
							}
						}
					}
				}
			}else{
				throw new RuntimeException("module init config error");
			}
		}else{
			throw new RuntimeException("module init config not found");
		}

		

	}

}
