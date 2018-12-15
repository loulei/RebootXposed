package com.example.rebootxposed;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;

public class FileUtil {

	public static void copy(File sourceFile, File targetFile) throws Exception{
		byte[] buffer = new byte[4096];
		int len = 0;
		FileInputStream fis = new FileInputStream(sourceFile);
		FileOutputStream fos = new FileOutputStream(targetFile);
		while((len=fis.read(buffer)) != -1){
			fos.write(buffer, 0, len);
		}
		fos.flush();
		fis.close();
		fos.close();
	}
	
	public static void writeToFile(InputStream is, File targetFile) throws Exception{
		byte[] buffer = new byte[4096];
		int len = 0;
		FileOutputStream fos = new FileOutputStream(targetFile);
		while((len = is.read(buffer)) != -1){
			fos.write(buffer, 0, len);
		}
		fos.flush();
		fos.close();
		is.close();
	}
}
