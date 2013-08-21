package com.xiaonei.platform.core.utility;

import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MobileUserAgent {

	public static boolean parserUserAgentStandard(String userAgent) {
		Pattern p = Pattern.compile("^((Dopod|ACER|Alcatel|AUDIOVOX|BlackBerry|CDM|Ericsson|LG\b|LGE|Motorola|MOT|NEC|Nokia|Panasonic|QCI|SAGEM|SAMSUNG|SEC|Sanyo|Sendo|SHARP|SIE|SonyEricsson|Telit|Telit_Mobile_Terminals|TSM)[- ]?([^\\/\\s\\_]+))(\\/(\\S+))?");
		Matcher m = p.matcher(userAgent);
		if (!m.find()) {
			return false;
		}

		String vendor = m.group(2);
		String model = m.group(3);

		if (vendor.equalsIgnoreCase("ACER")) {
			vendor = "Acer";
		} else if (vendor.equalsIgnoreCase("AUDIOVOX")) {
			vendor = "Audiovox";
		} else if (vendor.equalsIgnoreCase("CDM")) {
			vendor = "Audiovox";
		} else if (vendor.substring(0, 2).equalsIgnoreCase("LG")) {
			vendor = "LG";
		} else if (vendor.equalsIgnoreCase("MOT")) {
			vendor = "Motorola";
		} else if (vendor.equalsIgnoreCase("SAGEM")) {
			if (model.equals("-")) {
				return false;
			}
		} else if (vendor.equalsIgnoreCase("SEC")) {
			vendor = "SAMSUNG";
		} else if (vendor.equalsIgnoreCase("SIE")) {
			vendor = "Siemens";
		} else if (vendor.equalsIgnoreCase("Telit_Mobile_Terminals")) {
			vendor = "Telit";
		} else if (vendor.equalsIgnoreCase("TSM")) {
			vendor = "Vitelcom";
		}
		return MobileUserAgent.isVendor(vendor);
	}

	public static boolean parserUserAgentMozilla(String userAgent) {
	  // SAMSUNG browsers
	  String vendor = null;
	  String model = null;

	  Pattern p = Pattern.compile("^Mozilla\\/SMB3\\((Z105)\\)\\/(Samsung)");
	  Matcher m = p.matcher(userAgent);
	  if (m.find()) {
		return true;
	  }

	  // Extract the string between the brackets.
	  p = Pattern.compile("^Mozilla\\/\\d+\\.\\d+\\s*\\(([^\\)]+)\\)", Pattern.CASE_INSENSITIVE);
	  m = p.matcher(userAgent);
	  if (!m.find()) {
		  return false;
	  }

	  String[] parts = m.group(1).split(";"); // split string between brackets on ';' seperator.

	  // Micro$oft PPC and Smartphone browsers. Unfortunately, one day, if history repeats itself, this will probably be the only user-agent check necessary.
	  if ((parts.length >= 4) && (parts[0].equalsIgnoreCase("compatible")) && (parts[2].trim().equalsIgnoreCase("Windows CE"))) {
		  vendor = "Microsoft";

		  p = Pattern.compile("^\\d{1,4}x\\d{1,4}$", Pattern.CASE_INSENSITIVE);
		  if (parts[3].trim().equalsIgnoreCase("PPC") || parts[3].trim().equalsIgnoreCase("smartphone")) {
			  model = "SmartPhone";
			  if (parts.length >= 5) {
				  m = p.matcher(parts[4].trim());
			  }
		  } else if ((parts.length >= 5) && (parts[4].trim().equalsIgnoreCase("PPC") || (parts[4].trim().equalsIgnoreCase("smartphone")))) {
			  model = "SmartPhone";
		  }

		  if (model != null) {
			  return true;
		  }
	  }

	  // Nokia's with Opera browsers or SonyEricssons.
	  if (parts.length >= 4) {
		  p = Pattern.compile("^(Nokia|Sony)\\s*(\\S+)$");
		  m = p.matcher(parts[3].trim());
		  if (parts[0].trim().equalsIgnoreCase("compatible") && m.find()) {
			  if (m.group(1).equalsIgnoreCase("Sony")) {
				  vendor = "SonyEricsson";
			  }
			  model = m.group(2);
			  return true;
		  }
	  }

	  // SANYO browsers
	  if (parts.length >= 1) {
		  p = Pattern.compile("^MobilePhone ([^\\/]+)\\/([A-Z]+\\/)?(\\d+\\.\\d+)$");
		  m = p.matcher(parts[0].trim());
		  if (m.find()) { // MobilePhone PM-8200/US/1.0
			  return true;
		  }
	  }

	  // Nokias with ReqwirelessWeb browser
	  if (parts.length >= 3) {
		  p = Pattern.compile("^(Nokia)\\s*(\\S+)$");
		  m = p.matcher(parts[1].trim());
		  if (parts[0].trim().equalsIgnoreCase("compatible") && m.find()) {
			  return true;
		  }
	  }

	  return false;
	}

	public static boolean parserUserAgentImode(String userAgent) {
		HashMap<String, String> hm = new HashMap<String, String>();
		hm.put("D", "Mitsubishi");
		hm.put("ER", "Ericsson");
		hm.put("F", "Fujitsu");
		hm.put("KO", "Kokusai"); // Hitachi
		hm.put("M", "Mitsubishi");
		hm.put("P", "Panasonic"); // Matsushita
		hm.put("N", "NEC");
		hm.put("NM", "Nokia");
		hm.put("R", "Japan Radio");
		hm.put("S", "SAMSUNG");
		hm.put("SH", "Sharp");
		hm.put("SO", "Sony");
		hm.put("TS", "Toshiba");

		String keys = hm.keySet().toString();
		keys = keys.substring(1, keys.length() - 1).replaceAll(", ", "|");
		
		// Standard i-mode user agents
		Pattern p = Pattern.compile("^(portalmmm|DoCoMo)\\/(\\d+\\.\\d+) ((" + keys + ")[\\w\\-]+)\\((c(\\d+))?", Pattern.CASE_INSENSITIVE);
		Matcher m = p.matcher(userAgent);

		if (m.find()) {
			return MobileUserAgent.isVendor(hm.get(m.group(4).toUpperCase()));
		} else {
			p = Pattern.compile("^DoCoMo\\/(\\d+\\.\\d+)\\/((" + keys + ")[\\w\\.\\-\\_]+)(\\/c(\\d+))?", Pattern.CASE_INSENSITIVE);
			m = p.matcher(userAgent);

		    // DoCoMo HTML i-mode user agents
			if (m.find()) {
				// HTML 1.0: DoCoMo/1.0/modelname
				// HTML 2.0: DoCoMo/1.0/modelname/cache
				// HTML 3.0: DoCoMo/1.0/modelname/cache/unique_id_information
				return MobileUserAgent.isVendor(hm.get(m.group(3).toUpperCase()));
			}
		}

	    return false;
	}

	public static boolean parserUserAgentRubbish(String userAgent) {
		// Old ReqwirelessWeb browsers for Nokia. ReqwirelessWeb/2.0.0 MIDP-1.0 CLDC-1.0 Nokia3650
		String model = null;
		Pattern p = Pattern.compile("(Nokia)\\s*(N-Gage|\\d+)$");
		Matcher m = p.matcher(userAgent);
		if (m.find()) {
			return MobileUserAgent.isVendor(m.group(1));
		}

		// LG Electronics
		p = Pattern.compile("^(LG)E?\\/(\\w+)(\\/(\\d+\\.\\d+))?");
		m = p.matcher(userAgent);
		if (m.find()) {		// LGE/U8150/1.0 Profile/MIDP-2.0 Configuration/CLDC-1.0
			return MobileUserAgent.isVendor(m.group(1));
		}

	    // And now for the worst of all user agents, those that start with the text 'PHILIPS'.
		p = Pattern.compile("^(PHILIPS)(.+)");
		m = p.matcher(userAgent);
		if (m.find()) {
			String garbage = m.group(2).trim().toUpperCase(); // everything after the word PHILIPS in uppercase.
			p = Pattern.compile("^-?(\\d+)");
			m = p.matcher(garbage);
			if (m.find()) {
				model = m.group(1);
		      	// PHILIPS855 ObigoInternetBrowser/2.0
		        // PHILIPS 535 / Obigo Internet Browser 2.0
			}
			
			p = Pattern.compile("^-?(FISIO)\\s*(\\d+)");
			m = p.matcher(garbage);
			if (m.find()) {				// match the FISIO model names.
				model = m.group(1) + m.group(2);
		      	// PHILIPS-FISIO 620/3
		        // PHILIPS-Fisio311/2.1
		        // PHILIPS-FISIO311/2.1
			}
			
			p = Pattern.compile("^-?(XENIUM)");
			m = p.matcher(garbage);
			if (m.find()) {				// match the XENIUM model names.
				model = m.group(1);
		      	// PHILIPS-Xenium9@9 UP/4.1.16r
		        // PHILIPS-XENIUM 9@9/2.1
		        // PHILIPS-Xenium 9@9++/3.14
			}
			
			p = Pattern.compile("^-?([^\\s\\/]+)");
			m = p.matcher(garbage);
			if (m.find()) {				// match all other model names that contain no spaces and no slashes.
				model = m.group(1);
		        // PHILIPS-Ozeo UP/4
		        // PHILIPS-V21WAP UP/4
		        // PHILIPS-Az@lis288 UP/4.1.19m
		        // PHILIPS-SYSOL2/3.11 UP.Browser/5.0.1.11
			}

			return MobileUserAgent.isVendor(model);
		}

	    // Vitelcom user-agents (used in Spain)
		p = Pattern.compile("^(Vitelcom)-(Feature Phone)(\\d+\\.\\d+)");
		m = p.matcher(userAgent);
		if (m.find()) {
			// Vitelcom-Feature Phone1.0 UP.Browser/5.0.2.2(GUI)  -- this is a TSM 3 or a TSM 4.
			return MobileUserAgent.isVendor(m.group(1));
		}

	    return false;
	}

	public static boolean parserUserAgent(String userAgent) {
	  if (parserUserAgentStandard(userAgent)) {
		  return true;
	  }
	  if (parserUserAgentMozilla(userAgent)) {
		  return true;
	  }
	  if (parserUserAgentImode(userAgent)) {
		  return true;
	  }
	  if (parserUserAgentRubbish(userAgent)) {
		  return true;
	  }

	  return false;
	}

	public static boolean isVendor(String vendor) {
		return (vendor != null && vendor.length() > 0);
	}

	/**
	 * http://www.useragentstring.com/pages/Opera/
	 */
	public static boolean isOpera(String userAgent) {
		Pattern p = Pattern.compile(".*opera.*", Pattern.CASE_INSENSITIVE);
		Matcher m = p.matcher(userAgent);
		if (m.find()) {
			return true;
		}

		return false;
	}

	public static boolean isNotMobile(String userAgent) {
		Pattern p = Pattern.compile("(m3gate|winwap|openwave)", Pattern.CASE_INSENSITIVE);
		Matcher m = p.matcher(userAgent);
		if (m.find()) {
			return true;
		}

		if (MobileUserAgent.isOpera(userAgent)) {
			p = Pattern.compile("(Windows 2000|Windows XP|Windows NT|Windows ME|Windows 98)", Pattern.CASE_INSENSITIVE);
			m = p.matcher(userAgent);
			if (m.find()) {	// if web opera
				return true;
			}
		}

		return false;
	}

	public static boolean isMobile(String userAgent) {
		if (userAgent == null || userAgent.length() <= 0)
			return false;

		return parserUserAgent(userAgent);
	}

	public static void main(String args[]) {
		/*
		// 自动识别手机/web browser
		try {
			String ua = request.getHeader("User-Agent");

			if (BaseAction.isFromWap(response, request)) {
				if (MobileUserAgent.isNotMobile(ua)) {
					response.sendRedirect("http://www.xiaonei.com");
				}
			} else {
				if (MobileUserAgent.isMobile(ua)) {
					response.sendRedirect("http://m.xiaonei.com");
				}
			}

		} catch (Exception e) {}
		*/

		String ua = "Dopod575 Mozilla/4.0 (compatible; MSIE 4.01; Windows CE; Smartphone; 176x220)";
//		ua = "SAMSUNG-SGH-A300/1.0 UP/4.1.19k";
//		ua = "PHILIPS-FISIO 620/3";
//		ua = "PHILIPS855 ObigoInternetBrowser/2.0";
//		ua = "DoCoMo/2.0 modelname(cache;individual_identification_information)";
//		ua = "portalmmm/1.0 m21i-10(c10)";
//		ua = "Opera/9.23 (Windows NT 5.1; U; en)";
//		ua = "Opera/9.50 (J2ME/MIDP; Opera Mini/4.0.8462/8; U; en)";
//		ua = "Mozilla/4.0 (compatible; MSIE 5.0; Windows 98) Opera 5.12 [en]";

/*
Mozilla/5.0 (Windows 2000; U) Opera 6.01 [de]
Mozilla/4.78 (Windows 2000; U) Opera 6.01 [en]
Mozilla/4.0 (compatible; MSIE 5.0; Windows XP) Opera 6.01 [it]
Mozilla/4.0 (compatible; MSIE 5.0; Windows XP) Opera 6.01 [et]
Mozilla/4.0 (compatible; MSIE 5.0; Windows XP) Opera 6.01 [de]
Mozilla/4.0 (compatible; MSIE 5.0; Windows NT 4.0) Opera 6.01 [en]
Mozilla/4.0 (compatible; MSIE 5.0; Windows NT 4.0) Opera 6.01 [de]
Mozilla/4.0 (compatible; MSIE 5.0; Windows ME) Opera 6.01 [en]
Mozilla/4.0 (compatible; MSIE 5.0; Windows ME) Opera 6.01 [de]
Mozilla/4.0 (compatible; MSIE 5.0; Windows 98) Opera 6.01 [it]
Mozilla/4.0 (compatible; MSIE 5.0; Windows 98) Opera 6.01 [fr]
Mozilla/4.0 (compatible; MSIE 5.0; Windows 98) Opera 6.01 [en]
Mozilla/4.0 (compatible; MSIE 5.0; Windows 98) Opera 6.01 [de]
Mozilla/4.0 (compatible; MSIE 5.0; Windows 95) Opera 6.01 [de]
*/
		/*if (MobileUserAgent.isNotMobile(ua)) {
			System.out.println("is not mobile");
		} else {
			System.out.println("is mobile");
		}*/

		if (MobileUserAgent.isMobile(ua)) {
			System.out.println("yes");
		} else {
			System.out.println("no");
		}

	}
}
