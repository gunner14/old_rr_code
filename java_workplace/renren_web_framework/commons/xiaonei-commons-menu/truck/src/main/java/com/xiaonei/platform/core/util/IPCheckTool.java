/*
 * 这个是为应对一个菜单需求而开发的类
 * 需求内容是：对于北京的用户左侧菜单显示应用A而不显示应用B，非北京用户显示B而不显示A
 * 实现方法：同过纯真ip数据库获取北京的ip地址段，并将ip转为long型，合并连续的ip段，然后将
 * 每个段的开始与结尾ip存入bjip.txt
 * 使用时将bjip.txt读入内存，将传入的ip转型为long，通过二分查找判断ip是否属于北京地区
 */
package com.xiaonei.platform.core.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

public class IPCheckTool {

	private static final String IP_FILE = "C:\\Program Files\\cz88.net\\ip\\qqwry.dat";

	private static List<Long> bjip = null;

	private static IPCheckTool instance = new IPCheckTool();

	/**
	 * @return 单一实例
	 */
	public static IPCheckTool getInstance() {
		return instance;
	}

	private IPCheckTool() {
		try {
			getIPList();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	/**
	 * 从ip的字符串形式得到字节数组形式
	 * 
	 * @param ip
	 *            字符串形式的ip
	 * @return 字节数组形式的ip
	 */
	private static int[] getIpByteArrayFromString(String ip) throws Exception {
		int[] ret = new int[4];
		StringTokenizer st = new StringTokenizer(ip, ".");
		ret[0] = (int) (Integer.parseInt(st.nextToken()));
		ret[1] = (int) (Integer.parseInt(st.nextToken()));
		ret[2] = (int) (Integer.parseInt(st.nextToken()));
		ret[3] = (int) (Integer.parseInt(st.nextToken()));
		return ret;
	}

	/**
	 * 将ip字符串转换为long型
	 * 
	 * @param ip
	 * @return
	 * @throws Exception
	 */
	public static long ipTpLong(String ip) throws Exception {
		int[] a = getIpByteArrayFromString(ip);
		long endip = 0;
		for (int i = a.length - 1; i >= 0; i--) {
			long tmp = Long.valueOf(a[i]);
			endip += tmp << ((3 - i) * 8);
		}
		return endip;

	}

	/**
	 * 读取有有纯真ip数据库(qqwry.dat文件)转换过来的ip文本，并将北京地区的ip段取出
	 * 
	 * @param path
	 * @return
	 * @throws Exception
	 */
	public static List<Long> BufferedReaderDemo(String path) throws Exception {
		path = "C:\\Program Files\\cz88.net\\ip\\tt.txt";
		ArrayList<Long> all = new ArrayList<Long>();
		File file = new File(path);
		if (!file.exists() || file.isDirectory())
			throw new FileNotFoundException();
		InputStreamReader read = new InputStreamReader(
				new FileInputStream(file), "GBK");
		BufferedReader br = new BufferedReader(read);
		String temp = null;
		temp = br.readLine();
		int i = 0;
		while (temp != null) {
			if (temp.indexOf("北京") >= 0 || temp.indexOf("清华大学") >= 0) {
				if (temp.substring(32, 34).equals("北京")
						|| temp.substring(32, 36).equals("清华大学")) {
					String ipBegin = temp.substring(0, 16).trim();
					String ipEnd = temp.substring(16, 32).trim();
					long ipBeginLong = ipTpLong(ipBegin);
					long ipEndLong = ipTpLong(ipEnd);
					if (all.size() == 0) {
						all.add(ipBeginLong);
						all.add(ipEndLong);
					} else {
						long last = all.get(all.size() - 1);
						if (last == ipBeginLong - 1) {
							all.remove(all.size() - 1);
							all.add(ipEndLong);
						} else {
							all.add(ipBeginLong);
							all.add(ipEndLong);
						}
					}
				}
				System.out.println(temp.substring(32, 34));
				i++;
			}
			temp = br.readLine();
		}
		return all;
	}

	/**
	 * 二分查找某个ip是否在ip列表中 这个ip列表每两个数字为一组，前一个为开始ip地址，后一个为结尾ip地址
	 * 
	 * @param list
	 * @param ip
	 * @param begin
	 * @param end
	 * @return
	 */
	public static boolean binarySerch(List<Long> list, long ip, int begin,
			int end) {
		if (list == null || list.size() == 0 || ip > list.get(end)
				|| ip < list.get(begin)) {
			return false;
		}
		boolean flag = false;
		while (begin < end) {
			if (begin == end - 1) {
				// 如果ip在begin~end的ip段中返回true，否则false
				if (begin % 2 == 0) {
					return true;
				}
				return false;
			}

			int middle = (begin + end) / 2;
			if (ip > list.get(middle)) {
				begin = middle;
			} else if (ip < list.get(middle)) {
				end = middle;
			} else {
				return true;
			}
		}
		return flag;
	}

	/**
	 * 这个函数用来生成一个ip（ip被转换为long型）列表
	 * 
	 * @param list
	 * @throws IOException
	 */
	public static void generateIPFile(List<Long> list) throws IOException {
		String path = "C:\\Program Files\\cz88.net\\ip\\bjip.txt";
		PrintStream out = new PrintStream(new FileOutputStream(path));

		if (list != null) {
			for (Long tmp : list) {
				out.println(tmp);
			}
		}
	}

	/**
	 * 读取bjip.txt获取北京地区的ip列表
	 * 
	 * @throws Exception
	 */
	public void getIPList() throws Exception {
		ArrayList<Long> ips = new ArrayList<Long>();
		InputStreamReader r = new InputStreamReader(IPCheckTool.class
				.getResourceAsStream("bjip.txt"));
		BufferedReader br = new BufferedReader(r);
		String temp;
		while ((temp = br.readLine()) != null) {
			ips.add(Long.valueOf(temp));
		}
		br.close();
		bjip = ips;
	}

	/**
	 * 根据ip判断是否属于北京地区
	 * 
	 * @param ip
	 * @return
	 */
	public boolean isBJIP(String ip) {
		boolean isbjIP = false;
		if (ip == null || "".equals(ip)) {
			return false;
		}
		try {
			long ipLong = ipTpLong(ip);
			isbjIP = binarySerch(bjip, ipLong, 0, bjip.size() - 1);
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		return isbjIP;
	}

	public static void main(String arg[]) {
		System.out.println(5 / 2);
		System.out.println(IPCheckTool.getInstance().isBJIP("1.4.4.255"));
		// String demo = "222.35.26.0 222.35.26.255   北京市顺义区 劳保网吧(高丽营镇东马各庄村)";
		// String test[] = demo.split(" ");
		// List<Long> all = new ArrayList<Long>();
		// try {
		// all = BufferedReaderDemo("");
		// generateIPFile(all);
		// getIPList();
		// } catch (IOException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// } catch (Exception e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		//		
		// System.out.println("all.size()============" + all.size() + "   " +
		// all.get(all.size() - 1));
		// System.out.println("is ip in bj=================" + binarySerch(all,
		// ipTpLong("140.114.24.68"), 0, all.size()-1));
		// // IPLocation ipLocation =
		// IPSeeker.getInstance().getIPLocation("61.161.78.42");
		// ipTpLong("255.255.255.255");
		// ipTpLong("221.218.64.0");
		// System.out.println(ipTpLong("61.161.78.42"));
		// System.out.println(ipLocation.country);
	}
}
