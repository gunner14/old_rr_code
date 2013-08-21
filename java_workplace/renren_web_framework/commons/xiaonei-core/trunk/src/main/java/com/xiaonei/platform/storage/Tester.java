package com.xiaonei.platform.storage;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;

public class Tester {

	public static void main(String[] args) {
		try {
			//Allocator.initialize(new DatabaseHelperImpl());
			//File source = new File(args[0]);
			for (int i = 0; ; ++i) {
//				String pattern = "storage_test/" + i + args[0];
//				FileHelper helper = new SimpleFileHelperImpl();
//				String file = Allocator.getInstance().allocate(source, pattern,
//						helper);
//				helper.destroy();
//				System.out.println("Source: " + args[0]);
//				System.out.println("Pattern: " + pattern);
//				if (file != null) {
//					System.out.println("Result:  " + file);
//					String url = Allocator.getInstance().locate(file);
//					System.out.println("URL: " + url);
//				} else {
//					System.out.println("Error:");
//				}
				//System.out.println("##################\n");
				try {
					Thread.sleep(1000);
				} catch (Exception e) {

				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}


class SimpleFileHelperImpl extends FileHelper {

	public boolean saveAs(File source, File target) {
		BufferedReader reader = null;
		PrintWriter writer = null;
		try {
			reader = new BufferedReader(new FileReader(source));
			writer = new PrintWriter(new FileWriter(target));
			String line = null;
			while ((line = reader.readLine()) != null) {
				writer.println(line);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				reader.close();
			} catch (Exception e) {

			}
			try {
				writer.close();
			} catch (Exception e) {

			}
		}
		return true;
	}

	public void destroy() {

	}
}