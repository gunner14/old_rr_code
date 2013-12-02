package com.renren.disk.http;

import java.util.Date;
import java.util.Properties;

import javax.mail.Message.RecipientType;
import javax.mail.MessagingException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.AddressException;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

public class Mail {
	
	private static String host = "smtp.renren-inc.com";
	private static String username = "chuanjiang.peng@renren-inc.com";
	private static String password = "2008061727.pcJ";
	private static String title = "人人桌面网盘分享";
	
	private static Properties props = new Properties();
	private Session session;
	private MimeMessage msg;
	private InternetAddress[] to;
	private InternetAddress from;
	private Transport transport;
	
	/*public static void main(String[] args){
		new Mail().SendMessage(host, host, host, "471524839@qq.com", host);
	}*/
	public Mail(){
		props.put("mail.smtp.host", host);
		props.put("mail.smtp.auth", "true");
		session = Session.getInstance(props);
		msg = new MimeMessage(session);
	}
	
	/**
	 * 发送邮件
	 * 
	 * @param sender
	 *            发送者姓名
	 * @param filename
	 *            分享文件的文件名
	 * @param link
	 *            分享链接
	 * @param receiver
	 *            文件接收者
	 * @param content
	 *            文件内容
	 * @return 邮件是否发送成功
	 */
	public String SendMessage(String sender, String receiver, String content){
		String[] receivers = receiver.split(";");
		int num = receivers.length;
		to = new InternetAddress[num];
		for(int i = 0, j = 0; i < num; ++i){
			try {
				to[j] = new InternetAddress(receivers[i]);
				++j;
			} catch (AddressException e) {
				e.printStackTrace();
				return "fail";
			}
	    }
		try {
			from = new InternetAddress(username);
		} catch (AddressException e1) {
			e1.printStackTrace();
			return "fail";
		}
		try {
			msg.setFrom(from);
			msg.setRecipients(RecipientType.TO, to);
			msg.setSubject(title);
			msg.setSentDate(new Date());
			msg.setText("你的好友" + sender + "：\n" + content);
			msg.saveChanges();
			transport = session.getTransport("smtp");
			transport.connect(host, username, password);
			transport.sendMessage(msg, msg.getAllRecipients());
			return "success";
		} catch (MessagingException e) {
			e.printStackTrace();
			return "fail";
		}
	}
	
}
