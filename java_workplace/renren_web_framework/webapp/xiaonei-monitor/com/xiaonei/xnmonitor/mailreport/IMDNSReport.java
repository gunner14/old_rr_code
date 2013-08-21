package com.xiaonei.xnmonitor.mailreport;

import java.io.UnsupportedEncodingException;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.Collections;
import java.util.Date;
import java.util.List;

import javax.mail.MessagingException;
import javax.mail.internet.MimeMessage;

import kce.adapter.ipseek.IpSeekAdapter;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.mail.javamail.JavaMailSenderImpl;
import org.springframework.mail.javamail.MimeMessageHelper;

import com.xiaonei.xnmonitor.dto.IMInfo;
import com.xiaonei.xnmonitor.mailreport.classpack.IMDNSPackage;
import com.xiaonei.xnmonitor.utils.MyAr;
import com.xiaonei.xnmonitor.utils.UtilTools;

public class IMDNSReport {

	JavaMailSenderImpl mailSenderImpl = new JavaMailSenderImpl();

	@Autowired
	private JavaMailSender mailSender = null;
	@Autowired
	private Email email;

	public IMDNSReport() {
	}

	public void sendMail() throws MessagingException,
			UnsupportedEncodingException {
		MimeMessage mimeMessage = mailSenderImpl.createMimeMessage();
		MimeMessageHelper mimeMessageHelper = new MimeMessageHelper(
				mimeMessage, true, "utf-8");
		// MimeMessageHelper messageHelper = new
		// MimeMessageHelper(mailMessage,true,"utf-8");
		// Email email = new Email();
		mimeMessageHelper.setTo(email.getEmailTo());
		mimeMessageHelper.setFrom(email.getEmailFrom());
		mimeMessageHelper.setSubject(email.getSubject());
		String message = getMessage();
		mimeMessageHelper.setText("<html><head></head><body>" + message + "a"
				+ " </body></html>", true);
		// mimeMessageHelper.addInline("a", new
		// File("/home/lightning/personal/照片/DSC00381.JPG"));
		// mimeMessageHelper.addInline("2", new
		// File("/home/lightning/personal/照片/DSC00393.JPG"));
		// File pdfFile = new
		// File("/home/lightning/Desktop/Python_Core_Zh.pdf");
		// mimeMessageHelper.addAttachment(MimeUtility.encodeText(pdfFile.getName()),
		// pdfFile);
		mailSender.send(mimeMessage);
	}

	public String getMessage() {

		Calendar ca = Calendar.getInstance();
		ca.setTime(new Date());
		Timestamp begin = UtilTools.getTimeStamp(ca.get(Calendar.YEAR), ca
				.get(Calendar.MONTH) + 1, ca.get(Calendar.DAY_OF_MONTH) - 1);
		Timestamp end = UtilTools.getTimeStamp(ca.get(Calendar.YEAR), ca
				.get(Calendar.MONTH) + 1, ca.get(Calendar.DAY_OF_MONTH));
		// Timestamp begin = new Timestamp(109, 8, 7, 0, 0, 0, 0);
		// Timestamp end = new Timestamp(109, 8, 8, 0, 0, 0, 0);
//		System.out.println(begin + ":" + end);
		String sql = "select distinct url,type,avg(cost_time) as average_cost ,count(id) as total ,"
				+ " sum(cost_time ) as tc from im where type='dns' and happen_time between '"
				+ begin
				+ "' and '"
				+ end
				+ "' "
				+ " group by url,type order by tc desc , average_cost desc,total desc limit 50";
//		System.out.println(sql);
		List<Object[]> result = MyAr.sql(sql);
		StringBuffer msgBuilder = new StringBuffer();
		msgBuilder.append("<html><head>IM DNS解析最长耗费时间TOP50 " + begin + "~"
				+ end + "</head><body>");

		msgBuilder
				.append("<table><tr><td>DNS</td><td>平均解析时间</td><td>解析次数</td><td>类型</td></tr><br/>");
		for (Object[] record : result) {
			msgBuilder.append("<tr><td>" + record[0] + "</td><td>" + record[2]
					+ "</td><td>" + record[3] + "</td><td>" + record[1]
					+ "</td></tr>");
		}
		msgBuilder.append("</table> ");
		msgBuilder.append("<br/>XNMonitor says：<br/>");
		String suggestion = getMostSlowDns(begin, end);
		msgBuilder.append(suggestion);
		msgBuilder.append(" </body>");

		return msgBuilder.toString();
		// select distinct url,type,avg(cost_time) as average_cost ,count(id) as
		// total , sum(cost_time ) as tc from im where type='dns' and
		// happen_time between '2009-09-07 00:00:00' and '2009-09-08 00:00:00'
		// group by url,type order by tc desc , average_cost desc,total desc
		// limit 50
	}

	public String getMostSlowDns(Timestamp begin, Timestamp end) {
		// StringBuilder advice = new StringBuilder();
		String sql0 = "select distinct url,IP,count(id) as total from im where type='dns' "
				+ " and happen_time between '"
				+ begin
				+ "' and '"
				+ end
				+ "' group by " + " url ,IP order by total desc ;";
		List<Object[]> result0 = MyAr.sql(sql0);
		List<IMInfo> sql0List = IMDNSPackage.getIMDNSReportList(result0);

		String sql50 = "select distinct url,IP,count(id) as total from im where type='dns' "
				+ " and happen_time between '"
				+ begin
				+ "' and '"
				+ end
				+ "' and cost_time > 50   group by "
				+ " url ,IP having total>10 order by total desc ;";
		List<Object[]> result50 = MyAr.sql(sql50);
		List<IMInfo> sql50List = IMDNSPackage.getIMDNSReportList(result50);
		setPercent(sql0List, sql50List);
		setLocation(sql50List);
		String sql500 = "select distinct url,IP,count(id) as total from im where type='dns' "
				+ " and happen_time between '"
				+ begin
				+ "' and '"
				+ end
				+ "' and cost_time > 500   group "
				+ " by url ,IP having total>5 order by total desc ;";
		List<Object[]> result500 = MyAr.sql(sql500);
		List<IMInfo> sql500List = IMDNSPackage.getIMDNSReportList(result500);
		setPercent(sql0List, sql500List);
		setLocation(sql500List);
		String sql2000 = "select distinct url,IP,count(id) as total from im where type='dns' "
				+ " and happen_time between '"
				+ begin
				+ "' and '"
				+ end
				+ "' and cost_time > 2000   group "
				+ " by url ,IP having total>2 order by total desc ;";
		List<Object[]> result2000 = MyAr.sql(sql2000);
		List<IMInfo> sql2000List = IMDNSPackage.getIMDNSReportList(result2000);
		setPercent(sql500List, sql2000List);
		setLocation(sql2000List);
		// System.out.println(sql0 + result0.size());
		// System.out.println(sql50 + result50.size());
		// System.out.println(sql2000 + result2000.size());
		StringBuilder sugesstion = new StringBuilder();
		sugesstion
				.append("<br/><tr>经过我们的分析发现在过去的一天中，解析时间超过理想接受状态50ms,并且发生次数超过50次的DNS HOST 和IP所占的全天的查询的次数的比例是：</tr><br/>");
		sugesstion
				.append("<table><tr><td>DNS</td><td>IP</td><td>次数</td><td>占全部次数的百分比</td><td>地址</td></tr><br/>");
		for (IMInfo iminfo50 : sql50List) {
			sugesstion.append("<tr><td>" + iminfo50.getUrlOrHost()
					+ "</td><td>" + iminfo50.getIP() + "</td><td>"
					+ iminfo50.getTotal() + "</td><td>" + iminfo50.getPercent()
					+ "</td><td>" + iminfo50.getLocation() + "</tr>");
			// System.out.println(iminfo50.getIP());
		}
		sugesstion.append("</table>");
		sugesstion
				.append("<br/> <tr>我们还发现在过去的一天中解析时间超过500ms,并且这样的长时间的解析次数超过5次的DNS HOST 和IP所占全天的查询次数是：</tr><br/>");
		sugesstion
				.append("<table><tr><td>DNS</td><td>IP</td><td>次数</td><td>占全部次数的百分比</td><td>地址</td></tr>");
		for (IMInfo iminfo500 : sql500List) {
			sugesstion.append("<tr><td>" + iminfo500.getUrlOrHost()
					+ "</td><td>" + iminfo500.getIP() + "</td><td>"
					+ iminfo500.getTotal() + "</td><td>"
					+ iminfo500.getPercent() + "</td><td>"
					+ iminfo500.getLocation() + "</tr>");
		}
		sugesstion.append("</table>");
		sugesstion
				.append("<br/><tr>最后我们还发现在超过500ms的结果中，解析时间超过2000ms的次数所占超过500ms的次数的比例有：</tr><br/>");
		sugesstion
				.append("<table><tr><td>DNS</td><td>IP</td><td>次数</td><td>占超过500ms的次数的百分比</td><td>地址</td></tr>");
		for (IMInfo iminfo2000 : sql2000List) {
			sugesstion.append("<tr><td>" + iminfo2000.getUrlOrHost()
					+ "</td><td>" + iminfo2000.getIP() + "</td><td>"
					+ iminfo2000.getTotal() + "</td><td>"
					+ iminfo2000.getPercent() + "</td><td>"
					+ iminfo2000.getLocation() + "</tr>");
		}
		sugesstion.append("</table>");
		sugesstion.append("  按照百分比排序的结果是：  ");
		sugesstion
				.append("<table><tr><td>DNS</td><td>IP</td><td>次数</td><td>占超过500ms的次数的百分比</td><td>地址</td></tr>");
		Collections.sort(sql2000List, new MyComparator());
		for (IMInfo iminfo2000 : sql2000List) {
			sugesstion.append("<tr><td>" + iminfo2000.getUrlOrHost()
					+ "</td><td>" + iminfo2000.getIP() + "</td><td>"
					+ iminfo2000.getTotal() + "</td><td>"
					+ iminfo2000.getPercent() + "</td><td>"
					+ iminfo2000.getLocation() + "</tr>");
		}
		sugesstion.append("</table>");
		return sugesstion.toString();
	}

	public void setPercent(List<IMInfo> baseIMInfoList,
			List<IMInfo> conditionIMInfoList) {
		for (IMInfo conditionIminfo : conditionIMInfoList) {
			for (IMInfo baseIminfo : baseIMInfoList) {
				if (conditionIminfo.getUrlOrHost().equals(
						baseIminfo.getUrlOrHost())
						&& conditionIminfo.getUrlOrHost() != null
						&& !conditionIminfo.getUrlOrHost().equals("")) {
					conditionIminfo.setPercent(conditionIminfo.getTotal() * 1.0
							/ baseIminfo.getTotal() * 100);
					// System.out.println("conditionTotal" +
					// conditionIminfo.getIP() + ":"
					// + conditionIminfo.getTotal() + ":" +
					// baseIminfo.getTotal()
					// + baseIminfo.getIP() + ":" + "baseContiditonTotal");
					break;
				}
			}
		}

	}

	public void setLocation(List<IMInfo> imInfoList) {
		for (IMInfo iminfo : imInfoList) {
			iminfo.setLocation(IpSeekAdapter.getInstance().getAddress(
					iminfo.getIP().trim()));
//			System.out.println(IpSeekAdapter.getInstance().getAddress(
//					iminfo.getIP().trim()));
		}
	}

	public String getMostFrequence() {
		return null;
	}

	public static void main(String... args)
			throws UnsupportedEncodingException, MessagingException {
		IMDNSReport imDNSReport = new IMDNSReport();
		imDNSReport.sendMail();
	}
	// public String

}
