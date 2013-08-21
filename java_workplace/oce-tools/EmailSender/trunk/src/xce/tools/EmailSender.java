package xce.tools;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Date;
import java.util.Properties;

import javax.activation.DataHandler;
import javax.activation.FileDataSource;
import javax.mail.Message;
import javax.mail.Multipart;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeBodyPart;
import javax.mail.internet.MimeMessage;
import javax.mail.internet.MimeMultipart;

public class EmailSender {

	private static String getFileContent(String filename) throws IOException {

		InputStream stream = new BufferedInputStream(new FileInputStream(new File(filename)));
		StringBuffer buffer = new StringBuffer();
		/*
		 * BufferedReader reader = new BufferedReader(new FileReader(new
		 * File(filename))); String line = null; while((line =
		 * reader.readLine()) != null) { buffer.append(line).append("\n"); }
		 */

		int x;
		while (-1 != (x = stream.read())) {
			if ('\n' == x) {
				buffer.append("<br>");
				continue;
			}
			buffer.append((char) x);
		}

		System.out.println("body: " + buffer.toString());

		return buffer.toString();
	}

	public static void main(String[] args) throws Throwable {
		if (args.length < 1) {
			System.out.println("Usage: EmailSender <config> [body]");
			return;
		}

		Properties config = new Properties();

		config.load(new BufferedInputStream(new FileInputStream(args[0])));
		String from = config.getProperty("email.from");
		String to = config.getProperty("email.to");
		String cc = config.getProperty("email.cc");
		String subject = new String(config.getProperty("email.subject").getBytes("ISO8859-1"), "UTF-8");
		String contentType = config.getProperty("content.type", "text/plain");
		String styleFile = config.getProperty("content.style");
		String bodyFile = config.getProperty("content.body");
		String attachment = config.getProperty("content.attachment");

		Properties props = new Properties();
		props.put("mail.smtp.host", "mail.renren-inc.com");
		props.put("mail.smtp.auth", "false");

		if (from == null || to == null || subject == null) {
			System.err.println("Please check the config file.");
			return;
		}

		Session mailSession = Session.getDefaultInstance(props);

		mailSession.setDebug(true);

		Message message = new MimeMessage(mailSession);
		message.setFrom(new InternetAddress(from));
		for (InternetAddress toAddress : InternetAddress.parse(to)) {
			message.addRecipient(Message.RecipientType.TO, toAddress);
		}
		if (cc != null) {
			for (InternetAddress toAddress : InternetAddress.parse(cc)) {
				message.addRecipient(Message.RecipientType.CC, toAddress);
			}
		}
		message.setSentDate(new Date());
		message.setSubject(subject);

		MimeBodyPart textBodyPart = new MimeBodyPart();
		StringBuffer contentBuffer = new StringBuffer();
		if (styleFile != null) {
			contentBuffer.append(getFileContent(styleFile));
		}
		if (bodyFile != null) {
			contentBuffer.append(getFileContent(bodyFile));
		} else {
			if (args.length < 2) {
				System.out.println("body is not specified in property file or in parameters");
				return;
			}
			String body = args[1];
			contentBuffer.append(body);
		}

		textBodyPart.setContent(contentBuffer.toString(), contentType);

		Multipart container = new MimeMultipart();
		container.addBodyPart(textBodyPart);
		if (attachment != null) {
			MimeBodyPart fileBodyPart = new MimeBodyPart();
			FileDataSource fds = new FileDataSource(attachment);
			fileBodyPart.setDataHandler(new DataHandler(fds));
			fileBodyPart.setFileName(fds.getName());
			container.addBodyPart(fileBodyPart);
		}

		message.setContent(container);
		message.saveChanges();

		Transport.send(message);
	}
}
