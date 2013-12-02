package xce.tripod.monitor.util;

import xce.tools.EmailSender;

public class WarningSender {
	public static String propertyFile = "./email_property";
	public static String numberFile = "./phone_numbers";
	public static boolean isSendEmail = true;
	public static boolean isSendSms = true;

	private static final String html = "<html> <head> <style> " + "table { border-collapse:collapse; border:none; }"
			+ "th { border-top:solid #F79646 1.0pt; border-left:solid #F79646 1.0pt; background: #F79646; }"
			+ "td { border:solid #F79646 1.0pt; }" + "</style> </head>" + "<body>%s</body></html>";

	public static void sendEmail(String message) {
		String emailHtml = String.format(html, message);

		String[] args = { propertyFile, emailHtml };

		try {
			EmailSender.main(args);
		} catch (Throwable e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private static void sendSms(String message) {
		String[] args = { numberFile, message };
		try {
			SmsSender.main(args);
		} catch (Throwable e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public static void sendWarning(String message) {
		if (isSendEmail) {
			sendEmail(message);
		}

		if (isSendSms) {
			sendSms(message);
		}
	}
}
