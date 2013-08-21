package com.xiaonei.reg.thread.timelimit;

import java.util.Calendar;

public class Test {
	private Test() {
	};

	public static void main(String[] adfas) throws InterruptedException {
		Calendar beginDate = Calendar.getInstance();
		beginDate.set(1983, 6, 26);
		System.out.println(beginDate.getTime());
		Calendar endDate = Calendar.getInstance();
		endDate.set(endDate.get(Calendar.YEAR), endDate.get(Calendar.MONTH),endDate.get(Calendar.DATE));
		System.out.println(endDate.getTime());
		System.out.println(betweenDays(beginDate, endDate));
	}

	public static int betweenDays(Calendar beginDate, Calendar endDate) {
		if (beginDate.get(Calendar.YEAR) == endDate.get(Calendar.YEAR)) {
			return endDate.get(Calendar.DAY_OF_YEAR) - beginDate.get(Calendar.DAY_OF_YEAR);
		} else {
			if (beginDate.getTimeInMillis() < endDate.getTimeInMillis()) {
				int days = beginDate.getActualMaximum(Calendar.DAY_OF_YEAR) - beginDate.get(Calendar.DAY_OF_YEAR) + endDate.get(Calendar.DAY_OF_YEAR);
				for (int i = beginDate.get(Calendar.YEAR) + 1; i < endDate.get(Calendar.YEAR); i++) {
					Calendar c = Calendar.getInstance();
					c.set(Calendar.YEAR, i);
					days += c.getActualMaximum(Calendar.DAY_OF_YEAR);
				}
				return days;
			} else {
				int days = endDate.getActualMaximum(Calendar.DAY_OF_YEAR) - endDate.get(Calendar.DAY_OF_YEAR) + beginDate.get(Calendar.DAY_OF_YEAR);
				for (int i = endDate.get(Calendar.YEAR) + 1; i < beginDate.get(Calendar.YEAR); i++) {
					Calendar c = Calendar.getInstance();
					c.set(Calendar.YEAR, i);
					days += c.getActualMaximum(Calendar.DAY_OF_YEAR);
				}
				return days;
			}
		}
	}

}
