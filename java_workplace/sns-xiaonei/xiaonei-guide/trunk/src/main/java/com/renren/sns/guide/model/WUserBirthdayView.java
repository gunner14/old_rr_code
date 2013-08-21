/**
 * $Id: WUserBirthdayView.java 15927 2010-01-26 02:56:35Z yan.liu@XIAONEI.OPI.COM $
 * Copyright 2009-2010 Oak Pacific Interactive. All right reserved.
 */
package com.renren.sns.guide.model;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Scanner;

/**
 * @author wei.xiang(wei.xiang@opi-corp.com)
 * @since 2010-5-26
 */
public class WUserBirthdayView implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 2206777312719239548L;

	protected int id;

	protected short daymonth;

	public WUserBirthdayView(int paramInt, short paramShort1, short paramShort2) {
		this(paramInt, getDaymonth(paramShort1, paramShort2));
	}

	public WUserBirthdayView(int paramInt, short paramShort) {
		this.id = paramInt;
		this.daymonth = paramShort;
	}

	public WUserBirthdayView() {
	}

	public void setDaymonth(short paramShort) {
		this.daymonth = paramShort;
	}

	public void setDaymonth(short paramShort1, short paramShort2) {
		this.daymonth = getDaymonth(paramShort1, paramShort2);
	}

	public static short getDaymonth(short paramShort1, short paramShort2) {
		return (short) (paramShort1 + paramShort2 * 100);
	}

	public int getId() {
		return this.id;
	}

	public short getDay() {
		return (short) (this.daymonth % 100);
	}

	public short getMonth() {
		return (short) (this.daymonth / 100);
	}

	public static List<WUserBirthdayView> parseUserBirthView(String paramString) {
		Scanner localScanner = new Scanner(paramString).useDelimiter("\t|\n");
		ArrayList<WUserBirthdayView> localArrayList = new ArrayList<WUserBirthdayView>();
		while (localScanner.hasNext()) {
			localArrayList.add(new WUserBirthdayView(localScanner.nextInt(),localScanner.nextShort(), localScanner.nextShort()));
		}

		Collections.sort(localArrayList, new c());

		return localArrayList;
	}

	public String toString() {
		return String.format("%d\t%d\t%d\n", new Object[] {
				Integer.valueOf(getId()), Short.valueOf(getDay()),
				Short.valueOf(getMonth()) });
	}

	static class c implements Comparator<WUserBirthdayView>, Serializable {

		/**
		 * 
		 */
		private static final long serialVersionUID = 8664183674703718842L;

		public int compare(WUserBirthdayView paramWUserBirthdayView1,WUserBirthdayView paramWUserBirthdayView2) {
			return ((WUserBirthdayView.getDaymonth(paramWUserBirthdayView1.getDay(), 
					paramWUserBirthdayView1.getMonth()) < WUserBirthdayView.getDaymonth(paramWUserBirthdayView2.getDay(),
					paramWUserBirthdayView2.getMonth())) ? -1 : 1);
		}
	}
}
