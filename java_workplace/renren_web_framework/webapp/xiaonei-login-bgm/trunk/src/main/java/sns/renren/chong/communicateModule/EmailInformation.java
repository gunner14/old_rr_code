package sns.renren.chong.communicateModule;

import java.util.LinkedList;
import java.util.List;

/**
 *包含一封邮件所有要设置的消息，辅助SimpleEmailWrapper，完成邮件邮件发送。
 *使用者设置邮件所需的所有字段到对应的EmailInformation对象后，传递该对象给SimpleEmailWrapper，
 *再调用其sendMail()方法即可完成发送
 *
 *@author wenhong.mo@renren-inc.com
 */
public class EmailInformation {
	
	private String hostName = "";
	private String sender = "";
	private String password = "";
	private String subject = "";
	private String msg = "";
	private String charset = "UTF-8";
	private List<String> reciver = null;
	private List<String> bcc = null;
	private List<String> cc = null;
	
	public List<String> getReciver() {
		return reciver;
	}

	public void setReciver(List<String> reciver) {
		this.reciver = reciver;
	}
	
	public String getCharset() {
		return charset;
	}

	public void setCharset(String charset) {
		this.charset = charset;
	}
	
	public String getHostName() {
		return hostName;
	}

	public void setHostName(String hostName) {
		this.hostName = hostName;
	}

	public String getSender() {
		return sender;
	}

	public void setSender(String sender) {
		this.sender = sender;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public String getSubject() {
		return subject;
	}

	public void setSubject(String subject) {
		this.subject = subject;
	}

	public String getMsg() {
		return msg;
	}

	public void setMsg(String msg) {
		this.msg = msg;
	}

	public List<String> getBcc() {
		return bcc;
	}

	public void setBcc(List<String> bcc) {
		this.bcc = bcc;
	}

	public List<String> getCc() {
		return cc;
	}

	public void setCc(List<String> cc) {
		this.cc = cc;
	}
	
	public void addBcc(String mail) {
		if (bcc == null)
			bcc = new LinkedList<String>();
		bcc.add(mail);
	}
	
	public void addcc(String mail) {
		if (cc == null)
			cc = new LinkedList<String>();
		cc.add(mail);
	}
	
	public void addReciver(String mail) {
		if (this.reciver == null)
			this.reciver = new LinkedList<String>();
		this.reciver.add(mail);
	}
	
}
