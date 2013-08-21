package com.xiaonei.tools.monitor;

public class Root {

	public static void main(String... args) {
		System.out.println("New Edition new 4 MailStone 4th");
		Runnable http = new HttpAnalyze();
		Thread httpThread = new Thread(http);
		httpThread.start();

		Runnable db = new SqlAnalyze();
		Thread dbThread = new Thread(db);
		dbThread.start();

		Runnable httpErr = new HttpErrAnalyze();
		Thread httpErrThread = new Thread(httpErr);
		httpErrThread.start();

		Runnable im = new IMAnalyze();
		Thread imThread = new Thread(im);
		imThread.start();

		Runnable imdns = new IMOnlyDNSAnalyze();
		Thread imdnsThread = new Thread(imdns);
		imdnsThread.start();

	}

}
