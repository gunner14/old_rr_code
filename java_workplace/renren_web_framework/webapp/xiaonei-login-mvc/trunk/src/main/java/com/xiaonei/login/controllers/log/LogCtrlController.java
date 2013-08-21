package com.xiaonei.login.controllers.log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.HashMap;
import java.util.Map;

import net.paoding.rose.web.Invocation;
import net.paoding.rose.web.annotation.rest.Get;
import net.paoding.rose.web.annotation.rest.Post;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import com.xiaonei.login.logic.logger.PassportLoggerFactory;
import com.xiaonei.login.logic.logger.PpLogger;

/**
 * LogCtrlController <br>
 * 
 * @author tai.wang@opi-corp.com Jan 8, 2010 - 12:12:26 PM
 */
public class LogCtrlController {

	@Get
	@Post
	public String _jspService(Invocation inv) {
		String cmsg = "";
		inv.getResponse().setContentType("text/html");
		if ("i".equals(inv.getRequest().getParameter("g"))) {
			JSONObject jo = new JSONObject();
			for (Map.Entry<String, PpLogger> enLogger : PassportLoggerFactory
					.getLoggerMap().entrySet()) {
				jo.put(enLogger.getKey(), enLogger.getValue().isEnable());
			}
			InetAddress addr;
			String ip = "127.0.0.1";
			try {
				addr = InetAddress.getLocalHost();
				ip = addr.getHostAddress();
			} catch (UnknownHostException e) {
				e.printStackTrace();
			}
			jo.put("ip", ip);

			cmsg += output(jo.toString());
			return "@" + cmsg;
		}

		cmsg += outwrite(_jsp_string0, 0, _jsp_string0.length);
		if ("l".equals(inv.getRequest().getParameter("s"))) {
			JSONArray ja = JSONArray.fromObject(inv.getRequest().getParameter(
					"t"));
			for (Map.Entry<String, PpLogger> enLogger : PassportLoggerFactory
					.getLoggerMap().entrySet()) {
				if (ja.contains(enLogger.getKey())) {
					enLogger.getValue().setEnable(true);
				} else {
					enLogger.getValue().setEnable(false);
				}
			}
			return "@" + cmsg;
		}

		cmsg += outwrite(_jsp_string1, 0, _jsp_string1.length);

		cmsg += outwrite(_jsp_string2, 0, _jsp_string2.length);

		if ("POST".equals(inv.getRequest().getMethod())) {
			for (String ip : ipList) {
				JSONArray logType = new JSONArray();
				for (String lt : inv.getRequest().getParameterValues(
						"loggerType")) {
					if (lt.startsWith(ip + "_")) {
						logType.add(lt.replaceFirst(ip + "_", ""));
					}
				}
				HttpURLConnection uc = null;
				BufferedReader in = null;
				try {
					URL url = new URL("http://" + ip + "/log/logCtrl?s=l&t="
							+ logType.toString());
					uc = (HttpURLConnection) url.openConnection();
					uc.connect();
					in = new BufferedReader(new InputStreamReader(uc
							.getInputStream()));
					in.readLine();
				} catch (Throwable e) {
				} finally {
					if (in != null) {
						try {
							in.close();
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
				}
			}
		}

		cmsg += outwrite(_jsp_string2, 0, _jsp_string2.length);

		Map<String, JSONObject> loggerMap = new HashMap<String, JSONObject>();
		for (String ip : ipList) {
			HttpURLConnection uc = null;
			BufferedReader in = null;
			String msg = null;
			try {
				URL url = new URL("http://" + ip + "/log/logCtrl?g=i");
				uc = (HttpURLConnection) url.openConnection();
				uc.connect();
				in = new BufferedReader(new InputStreamReader(uc
						.getInputStream()));
				msg = in.readLine();
			} catch (Throwable e) {
			} finally {
				if (in != null) {
					try {
						in.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
			if (!"".equals(msg) && null != msg) {
				JSONObject jo = JSONObject.fromObject(msg);
				loggerMap.put(ip, jo);
			}
		}

		cmsg += outwrite(_jsp_string3, 0, _jsp_string3.length);
		StringBuffer buf = new StringBuffer();
		for (Map.Entry<String, PpLogger> enLogger : PassportLoggerFactory
				.getLoggerMap().entrySet()) {
			buf.append(outwrite(_jsp_string4, 0, _jsp_string4.length));
			buf.append(output((enLogger.getKey().replaceFirst("passportLog-",
					""))));
			buf.append(outwrite(_jsp_string5, 0, _jsp_string5.length));
		}
		cmsg += buf.toString();
		cmsg += outwrite(_jsp_string6, 0, _jsp_string6.length);
		for (Map.Entry<String, PpLogger> enLogger : PassportLoggerFactory
				.getLoggerMap().entrySet()) {
			cmsg += outwrite(_jsp_string7, 0, _jsp_string7.length);
			cmsg += output((enLogger.getKey()));
			cmsg += outwrite(_jsp_string8, 0, _jsp_string8.length);
		}
		cmsg += outwrite(_jsp_string9, 0, _jsp_string9.length);
		for (Map.Entry<String, JSONObject> jologger : loggerMap.entrySet()) {
			cmsg += outwrite(_jsp_string10, 0, _jsp_string10.length);
			cmsg += output((jologger.getKey()));
			cmsg += outwrite(_jsp_string11, 0, _jsp_string11.length);
			cmsg += output((jologger.getKey()));
			cmsg += outwrite(_jsp_string12, 0, _jsp_string12.length);
			for (Map.Entry<String, PpLogger> enLogger : PassportLoggerFactory
					.getLoggerMap().entrySet()) {
				cmsg += outwrite(_jsp_string13, 0, _jsp_string13.length);
				cmsg += output((jologger.getKey()));
				cmsg += outwrite('_');
				cmsg += output((enLogger.getKey()));
				cmsg += outwrite(_jsp_string14, 0, _jsp_string14.length);
				cmsg += output((jologger.getKey()));
				cmsg += outwrite('_');
				cmsg += output((enLogger.getKey()));
				cmsg += outwrite(_jsp_string15, 0, _jsp_string15.length);
				cmsg += output((jologger.getValue().getBoolean(
						enLogger.getKey()) ? "checked" : ""));
				cmsg += outwrite(_jsp_string16, 0, _jsp_string16.length);
			}
			cmsg += outwrite(_jsp_string17, 0, _jsp_string17.length);
		}
		cmsg += outwrite(_jsp_string18, 0, _jsp_string18.length);
		return "@" + cmsg;
	}

	private final static char[] _jsp_string7;

	private final static char[] _jsp_string16;

	private final static char[] _jsp_string9;

	private final static char[] _jsp_string10;

	private final static char[] _jsp_string8;

	private final static char[] _jsp_string14;

	private final static char[] _jsp_string6;

	private final static char[] _jsp_string11;

	private final static char[] _jsp_string4;

	private final static char[] _jsp_string12;

	private final static char[] _jsp_string13;

	private final static char[] _jsp_string0;

	private final static char[] _jsp_string5;

	private final static char[] _jsp_string3;

	private final static char[] _jsp_string18;

	private final static char[] _jsp_string1;

	private final static char[] _jsp_string15;

	private final static char[] _jsp_string2;

	private final static char[] _jsp_string17;
	static {
		_jsp_string7 = "\r\n                                    <td align=\"center\">\r\n                                       <input type=\"checkbox\" onclick=\"seType('"
				.toCharArray();
		_jsp_string16 = " />\r\n                                        </td>\r\n                               "
				.toCharArray();
		_jsp_string9 = "\r\n                        </tr>\r\n                       "
				.toCharArray();
		_jsp_string10 = "\r\n                           <tr>\r\n                                <td>\r\n                                    "
				.toCharArray();
		_jsp_string8 = "',this.checked);\">\r\n                                   </td>\r\n                           "
				.toCharArray();
		_jsp_string14 = "\" \r\n                                            id=\""
				.toCharArray();
		_jsp_string6 = "\r\n                        </tr>\r\n                       <tr>\r\n                            <td align=\"center\">\r\n                               &nbsp;\r\n                          </td>\r\n                               <td>\r\n                                    <input type=\"checkbox\" onclick=\"seAll(this.checked);\">\r\n                              </td>\r\n                           "
				.toCharArray();
		_jsp_string11 = "\r\n                               </td>\r\n                               <td>\r\n                                    <input type=\"checkbox\" onclick=\"seIp('"
				.toCharArray();
		_jsp_string4 = "\r\n                                    <td>\r\n                                        "
				.toCharArray();
		_jsp_string12 = "',this.checked)\">\r\n                              </td>\r\n                               "
				.toCharArray();
		_jsp_string13 = "\r\n                                       <td align=\"center\">\r\n                                           <input type=\"checkbox\" \r\n                                           name=\"loggerType\" \r\n                                            value=\""
				.toCharArray();
		_jsp_string0 = "\r\n".toCharArray();
		_jsp_string5 = "\r\n                                    </td>\r\n                           "
				.toCharArray();
		_jsp_string3 = "\r\n<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\r\n\r\n<html>\r\n <head>\r\n      <title>Logger Setup</title>\r\n     <meta http-equiv=\"pragma\" content=\"no-cache\">\r\n       <meta http-equiv=\"cache-control\" content=\"no-cache\">\r\n        <meta http-equiv=\"expires\" content=\"0\">\r\n </head>\r\n \r\n    <body>\r\n      Please choose the logger and the ip/host:\r\n       <FORM METHOD=\"Post\">\r\n      <TABLE CELLPADDING=\"5\" CELLSPACING=\"0\" BORDER=\"1\">\r\n            <TR>\r\n                <TD COLSPAN=\"2\">\r\n                  <H2>Enable Disable Passport Logger</H2>\r\n             </TD>\r\n           </TR>\r\n           <TR>\r\n                <TD>\r\n                    <table border=\"1\">\r\n                        <tr>\r\n                            <td>\r\n                                &nbsp;\r\n                          </td>\r\n                           <td>\r\n                                &nbsp;\r\n                          </td>\r\n                           "
				.toCharArray();
		_jsp_string18 = "\r\n                   </table>\r\n                </TD>\r\n           </TR>\r\n           <TR>\r\n                <TD COLSPAN=\"2\">\r\n                  <INPUT TYPE=\"Submit\" NAME='Submit' VALUE='Apply Changes'><BR>\r\n             </TD>\r\n           </TR>\r\n       </TABLE>\r\n        </FORM>\r\n </body>\r\n</html>\r\n<script type=\"text/javascript\">\r\nfunction seAll(checked){\r\n  var boxes = document.getElementsByName(\"loggerType\");\r\n    for(var i =0; i<boxes.length; i++){\r\n         boxes[i].checked=checked;\r\n       }  \r\n}\r\nfunction seIp(ip,checked){\r\n var boxes = document.getElementsByName(\"loggerType\");\r\n  for(var i =0; i<boxes.length; i++){\r\n   if(boxes[i].id.indexOf(ip) != -1){\r\n        boxes[i].checked=checked;\r\n     }\r\n }  \r\n}\nfunction seType(type,checked){\r\n var boxes = document.getElementsByName(\"loggerType\");\r\n    for(var i =0; i<boxes.length; i++){\r\n   if(boxes[i].id.lastIndexOf(type) != -1){\r\n      boxes[i].checked=checked;\r\n     }\r\n }  \r\n}\n</script>\r\n"
				.toCharArray();
		_jsp_string1 = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n".toCharArray();
		_jsp_string15 = "\" \r\n                                            "
				.toCharArray();
		_jsp_string2 = "\r\n\r\n".toCharArray();
		_jsp_string17 = "\r\n                           </tr>\r\n                       "
				.toCharArray();
	}

	private static final String[] ipList = // ip list
	{// start
	"127.0.0.1", "10.3.16.102", "10.3.16.103", "10.3.16.104", "10.3.16.15",
			"10.3.16.26", "10.3.16.27", "10.3.16.28", "10.3.16.29",
			"10.3.16.30", "10.3.16.42", "10.3.16.44", "10.3.16.45",
			"10.3.16.46", "10.3.16.47", "10.3.16.48", "10.3.16.49",
			"10.3.16.50", "10.3.16.81", "10.3.16.82", "10.3.16.84",
			"10.3.16.85", "10.3.16.86", "10.3.16.87", "10.3.16.88",
			"10.3.16.89", "10.3.16.90", "10.3.17.23", "10.3.17.24",
			"10.3.17.25", "10.3.17.26", "10.3.17.27", "10.3.17.28",
			"10.3.17.29", "10.3.17.30", "10.3.17.31", "10.3.17.32",
			"10.3.17.33", "10.3.17.34", "10.3.18.170", "10.3.18.178",
			"10.3.18.179", "10.3.18.189", "10.3.18.206", "10.3.18.205", };// end

	protected String output(String msg) {
		return msg;
	}

	private String outwrite(char[] jspString9, int i, int length) {
		return String.valueOf(jspString9);
	}

	private String outwrite(char c) {
		return String.valueOf(c);

	}
}
