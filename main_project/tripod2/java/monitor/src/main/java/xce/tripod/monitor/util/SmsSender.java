package xce.tripod.monitor.util;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.NameValuePair;
import org.apache.commons.httpclient.methods.PostMethod;

public class SmsSender {

	private static String send(String number, String message) throws IOException {
		HttpClient hc = new HttpClient();
		PostMethod method = new PostMethod("http://sms.notify.d.xiaonei.com:2000/receiver");
		NameValuePair numberValue = new NameValuePair("number", number);
		NameValuePair messageValue = new NameValuePair("message", message);
		method.setRequestBody(new NameValuePair[] { numberValue, messageValue });
		hc.executeMethod(method);
		String reply = method.getStatusLine().toString();
		reply += method.getResponseBodyAsString();
		method.releaseConnection();
		return reply;
	}

	public static synchronized boolean[] sendGroup(String[] numberArray, String message) {
		boolean[] result = new boolean[numberArray.length];

		for (int i = 0; i < numberArray.length; ++i) {
			result[i] = true;
			try {
				send(numberArray[i], message);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				result[i] = false;
			}
		}

		return result;
	}

	public static void main(String[] args) throws Throwable {
		if (args.length < 2) {
			System.out.println("Usage: SendMessage PhoneNumberFile Message");
			System.exit(-1);
		}

		String numberFile = args[0];

		String messge = args[1];

		try {
			BufferedReader br = new BufferedReader(new FileReader(numberFile));
			String numberStr = null;
			List<String> numberList = new ArrayList<String>();
			while ((numberStr = br.readLine()) != null) {
				numberList.add(numberStr);
			}

			if (numberList.size() != 0) {
				String[] numberArray = new String[numberList.size()];
				for (int i = 0; i < numberArray.length; ++i) {
					numberArray[i] = numberList.get(i);
				}
				boolean[] result = null;
				result = sendGroup(numberArray, messge);
				for (int i = 0; i < numberArray.length; ++i) {
					System.out.println(numberArray[i] + " " + result[i]);
				}
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
}
