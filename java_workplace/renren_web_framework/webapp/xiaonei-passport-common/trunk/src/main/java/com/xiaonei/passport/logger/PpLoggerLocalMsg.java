package com.xiaonei.passport.logger;


public class PpLoggerLocalMsg {
	private static class LocalMsg extends ThreadLocal<Object> {
		public final void initial() {
			this.set("");
		};

		@Override
		protected Object initialValue() {
			return false;
		};
	}

	private static LocalMsg localMsg = new LocalMsg();

	public static String get() {
		return localMsg.get().toString();
	}

	public static void initial() {
		localMsg.initial();
	}
	
	public static void stop(){
		LoginLoggerDef.login.info(localMsg.get().toString());
	}

	public static boolean isNotInitialValue() {
		return Boolean.valueOf(false).equals(localMsg.get());
	}

	public static void set(String msg) {
		localMsg.set(msg);
	}

}
