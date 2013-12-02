package com.renren.disk.config;

public class DConstant {
	
	public static String client_id = "6UmoiUeSBcwMd7UMGRmyikD9";
	public static String client_secret = "Z61P71cNZkOd3tPlZEMCUjhsIt9TF5GR";
	public static String renren_base_path = "http://disk.renren.com/";
	public static String baidu_base_path = "https://openapi.baidu.com/oauth/2.0/";
	private static String[] picType={"jpg","jpeg","bmp","gif","png","ico","tif","pcx","tga"};
	private static String[] zipType={"zip","rar","jar"};
	private static String[] musicType={"mp3","wma","flac","aac","mmf","amr","ogg","mp2","wav","wavpack"};
	private static String[] movieType={"mp4","avi","3gp","rmvb","gif","wmv","mkv","mpg","vob","mov","flv","swf"};
	private static String[] docType={"doc","pdf","txt","docx"};
	
	public static String getFileTypeCss(String fname){
		int st = fname.indexOf(".");
		if (st == -1) {
			return "unknown.png";
		}
		String flag = fname.substring(st);
		if (contain(picType, flag)) {
			return "pic.png";
		} else if (contain(zipType, flag)) {
			return "zip.png";
		} else if (contain(musicType, flag)) {
			return "music.png";
		} else if (contain(movieType, flag)) {
			return "movie.png";
		} else if (contain(docType, flag)) {
			return "doc.png";
		} else {
			return "unknown.png";
		}
		
	}
	
	private static boolean contain(String[] arr, String str){
		for (int i = arr.length - 1; i >= 0; --i){
			if(arr[i].equals(str)) {
				return true;
			}
		}
		return false;
	}

}
