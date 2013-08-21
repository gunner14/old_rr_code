package com.xiaonei.reg.guide.root.miscs;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Random;

public class MyEnDeCoder {
	public static void main(String[] args) {

		Date date = new Date();
		SimpleDateFormat matter=new SimpleDateFormat("yyyyMMddhhmm");
		String time = matter.format(date);
		String src = "inshion"+time;
		String e = "";

		
		//encode001()得到双倍于原串长度的目标串
		e = encode001(src);
		System.out.println("after encode : "+e);
		System.out.println("after decode : "+decode001(e));
		
		decode001(null);
		
	}

	private final static char[] chars = { 
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H','I', 'J', 'K', 'L', 'M', 
			'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U','V', 'W', 'X', 'Y', 'Z',

			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
			'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
			
			'0','1','2','3','4','5','6','7','8','9',
			
			'_',

	};
	
	private final static int N_ALL = chars.length;
	
	private final static int N_NULL = 1;
	
	private final static int N = chars.length - N_NULL;
	
	private final static int N_NULL_OFFSET = N;

	public static String getCurrentTime(String s) {
		Date date = new Date();
		SimpleDateFormat matter=new SimpleDateFormat("yyyyMMddhhmm");
		String time = matter.format(date);
		return time;
	}

	public static String encode001(String s) {
		StringBuilder sb = new StringBuilder();
		char[] ss = s.toCharArray();
		for (int i = 0; i < s.length(); i++) {

			int index = genIndex(N);
			int curIndex = getIndex(ss[i]);
			int toIndex = curIndex + index;
			if (toIndex >= N)
				toIndex -= N;
			sb.append(chars[index]);
			sb.append(chars[toIndex]);

		}

		return sb.toString();
	}
	
	public static String decode001(String s) {
		if(MyStringUtils.isNullOrEmpty(s))return "";
		StringBuilder sb = new StringBuilder();
		char[] ss = s.toCharArray();
		for (int i = 0; i < s.length() - 1; i++) {
			int key = -1;
			if (i % 2 == 0) {
				key = getIndex(ss[i]);
				char evalue = ss[i + 1];
				int eindex = getIndex(evalue);
				eindex = eindex - key;
				if (eindex < 0)
					eindex += N;
				sb.append(chars[eindex]);
			}
		}
	
		return sb.toString();
	
	}

	public static String encode002(String s) {
		StringBuilder sb = new StringBuilder();
		
		sb.append(s);
		while(sb.length()%10 != 0){
			sb.append("_");
		}
		
		char[] ss = sb.toString().toCharArray();
		sb = new StringBuilder();
		
		for (int i = 0; i < ss.length; i++) {
				int index = genIndex(N_ALL);
				int curIndex = getIndex(ss[i]);
				int toIndex = curIndex + index;
				if (toIndex >= N_ALL)
					toIndex -= N_ALL;
				sb.append(chars[index]);
				sb.append(chars[toIndex]);

		}
		
		return sb.toString();
	}

	public static String decode002(String s) {
		StringBuilder sb = new StringBuilder();
		char[] ss = s.toCharArray();
		for (int i = 0; i < s.length() - 1; i++) {
			int key = -1;
			if (i % 2 == 0) {
				key = getIndex(ss[i]);
				char evalue = ss[i + 1];
				int eindex = getIndex(evalue);
				eindex = eindex - key;
				if (eindex < 0)
					eindex += N_ALL;
				if(eindex == N_NULL_OFFSET)break;
				sb.append(chars[eindex]);
			}
			
		}
	
		return sb.toString();
	
	}
	


	private static int getIndex(char c) {
		for (int i = 0; i < chars.length; i++) {
			if (chars[i] == c) {
				return i;
			}
		}
		return -1;
	}

	private static int genIndex(int Max) {
		Random r = new Random();

		return r.nextInt(Max);
	}

	@SuppressWarnings("unused")
	private static char genValidCharacter() {
		return chars[genIndex(N)];
	}
}
