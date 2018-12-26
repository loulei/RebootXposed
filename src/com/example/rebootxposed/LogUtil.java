package com.example.rebootxposed;

import android.util.Log;

public class LogUtil {

	private static int LOG_MAX_LENGTH = 2000;
	private static boolean isPrintLog = true;
	
	public static void d(String msg){
		d(LogUtil.class.getSimpleName(), msg);
	}
	
	public static void d(String tagName, String msg){
		if(isPrintLog){
			int strlength = msg.length();
			int start = 0;
			int end = LOG_MAX_LENGTH;
			for(int i=0; i<100; i++){
				if(strlength > end){
					Log.v(tagName, msg.substring(start, end));
					start = end;
					end = end + LOG_MAX_LENGTH;
				}else{
					Log.v(tagName, msg.substring(start, strlength));
					break;
				}
			}
		}
	}
}
