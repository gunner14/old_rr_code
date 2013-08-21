package sns.renren.chong.communicateModule;

//import java.io.IOException;
//import java.io.InputStream;
import java.util.HashMap;
import java.util.Iterator;
//import java.util.List;
import java.util.Map;

//import org.jdom2.Document;
//import org.jdom2.Element;
//import org.jdom2.JDOMException;
//import org.jdom2.input.SAXBuilder;

/**
 *邮件服务器地址集合，通过邮箱名，可查询到响应的邮件服务器的域名。辅助SimpleEmailWrapper完成邮件发送
 *通过配置文件进行初始化，初始化后若查询的邮箱名没有对应的服务器域名，默认在邮箱后缀前加smtp.返回，
 *例如：wenhong.mo@renren-inc.com， 若没配置，则默认返回 smtp.renren-inc.com
 *
 *@author wenhong.mo@renren-inc.com
 */
public class ServerDnsNameSet {
	
	private Map<String, String> map = new HashMap<String, String>();
	private String fileName = null;
	//ServerDnsNameSet.class.getClassLoader().getResource("pro-inf/dnsNameSet.xml").getPath();
	
	public ServerDnsNameSet() {	
		this.fileName = "dnsNameSet.xml";
		this.initMap(this.fileName);
	}
	
	public ServerDnsNameSet(String fileName) {
		this.initMap(fileName);
	}
	public ServerDnsNameSet(Map<String, String> map) {
		this.fileName = "dnsNameSet.xml";
		this.map = map;
	}
	
	/**
	 * 估计是因为包冲突，new SAXBuilder()会爆异常，暂时注销掉
	 */
	private void initMap(String fileName) {
//		this.fileName = fileName;
//		Document doc = null;
//		Element root = null;
//		SAXBuilder builder = new SAXBuilder();
//		
//		try {
//			//File in = new File(this.fileName);
//			InputStream in = ServerDnsNameSet.class.getClassLoader().getResourceAsStream(this.fileName);
//			try {
//				doc = builder.build(in);
//				root = doc.getRootElement();
//			} catch (JDOMException e) {
//				e.printStackTrace();
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
//			
//			List<Element> chdList = root.getChildren("element");
//			Iterator<Element> ito = chdList.iterator();
//			while (ito.hasNext()) {
//				Element ele = ito.next();
//				Element key = ele.getChild("key");
//				Element value = ele.getChild("value");
//				map.put(key.getText(), value.getText());
//			}
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
	}
	
	
	private String getSuffix(String mailAddress) {
		String suffix = mailAddress.substring(mailAddress.indexOf("@") + 1);
		return suffix;
	}
	
	public String getServerDnsName(String mailAddress) {
		String suffix = this.getSuffix(mailAddress);
		String serverName = this.getDnsName(suffix);
		if(serverName == null) {
			serverName = "smtp." + suffix;
		}
		return serverName;
	}
	
	private String getDnsName(String key) {
		return map.get(key);
	}
	
	
	public String getAllPairs() {
		String content = "";
		Iterator<String> ito = map.keySet().iterator();
		
		while(ito.hasNext()) {
			String key = ito.next();
			content += key + " : " + map.get(key) + "\n"; 
		}
		return content;
	}
	
//	public static void main(String[] args) {
//		System.out.println(ServerDnsNameSet.class.getResource("").getPath());
//		System.out.println(ServerDnsNameSet.class.getClassLoader().getResource("").getPath());
//		
//		File file = new File("WebContent/dnsNameSet.xml");
//		try {
//			InputStream in = new FileInputStream(file);
//		} catch (FileNotFoundException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
//		
//		ServerDnsNameSet test = new ServerDnsNameSet("dnsNameSet.xml");
//		System.out.println(test.getAllPairs());
//		
//	}
}
