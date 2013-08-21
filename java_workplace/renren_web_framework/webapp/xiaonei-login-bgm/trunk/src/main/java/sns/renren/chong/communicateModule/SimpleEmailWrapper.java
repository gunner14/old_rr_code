package sns.renren.chong.communicateModule;

import java.util.List;
import java.util.ListIterator;

import org.apache.commons.mail.EmailException;
import org.apache.commons.mail.SimpleEmail;

/**
 *对SimpleEmail的包装，使用者只需初始化EmailInformation，再调用sendMail即可完成发送 
 * 
 * @author wenhong.mo@renren-inc.com
 */
public class SimpleEmailWrapper {
	private EmailInformation info = null;
	public SimpleEmailWrapper(EmailInformation info) {
		this.info = info;
	}
	
	
	public EmailInformation getInfo() {
		return info;
	}


	public void setInfo(EmailInformation info) {
		this.info = info;
	}


	public int sendMail(EmailInformation info) {	
		
		SimpleEmail email = new SimpleEmail();
		//email.setTLS(true);
		email.setHostName(info.getHostName());
		email.setAuthentication(info.getSender(), info.getPassword());
		
		try {
			email.setFrom(info.getSender());
			
			addTo(email, info.getReciver());
			addToBcc(email, info.getBcc());
			addToCc(email, info.getCc());
			
			email.setCharset(info.getCharset());
			email.setSubject(info.getSubject());
			email.setMsg(info.getMsg());
			email.send();
			
		} catch (EmailException e) {

		}				
		return 0;
	}
	
	private void addTo(SimpleEmail email, List<String> users) throws EmailException {
		if (users == null)
			return ;
		
		ListIterator<String> ito = users.listIterator();
		while (ito.hasNext()) {
			String user = ito.next();
			email.addTo(user);
		}
	}
	
	private void addToBcc(SimpleEmail email, List<String> users) throws EmailException {
		if (users == null)
			return ;
		
		ListIterator<String> ito = users.listIterator();
		while (ito.hasNext()) {
			String user = ito.next();
			email.addBcc(user);
		}
	}
	
	private void addToCc(SimpleEmail email, List<String> users) throws EmailException {
		if (users == null)
			return ;
		
		ListIterator<String> ito = users.listIterator();
		while (ito.hasNext()) {
			String user = ito.next();
			email.addCc(user);
		}
	}
	
}
