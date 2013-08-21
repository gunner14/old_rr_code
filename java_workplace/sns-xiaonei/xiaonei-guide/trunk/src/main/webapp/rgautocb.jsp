
<%@page import="org.apache.commons.httpclient.protocol.SecureProtocolSocketFactory"%>
<%@page import="org.apache.commons.httpclient.protocol.Protocol"%>
<%@page import="org.apache.commons.httpclient.methods.PostMethod"%>
<%@page import="org.apache.commons.httpclient.params.HttpMethodParams"%>
<%@page import="org.apache.commons.httpclient.methods.GetMethod"%>
<%@page import="org.apache.commons.httpclient.HttpMethod"%>
<%@page import="org.apache.commons.httpclient.HttpClient"%>
<%@page import="java.security.KeyManagementException"%>
<%@page import="java.security.NoSuchAlgorithmException"%>
<%@page import="java.security.cert.CertificateException"%>
<%@page import="java.security.cert.X509Certificate"%>

<%@page import="javax.net.SocketFactory"%>
<%@page import="javax.net.ssl.SSLContext"%>
<%@page import="javax.net.ssl.TrustManager"%>
<%@page import="javax.net.ssl.X509TrustManager"%>

<%@page import="org.apache.commons.httpclient.ConnectTimeoutException"%>

<%@page import="org.apache.commons.httpclient.params.HttpConnectionParams"%>
<%@page import="org.apache.commons.httpclient.protocol.SecureProtocolSocketFactory"%>
<%@page import="java.io.IOException"%>
<%@page import="java.net.InetAddress"%>
<%@page import="java.net.InetSocketAddress"%>
<%@page import="java.net.Socket"%>
<%@page import="java.net.SocketAddress"%>
<%@page import="java.net.UnknownHostException"%>

<%!public class MySSLSocketFactory implements SecureProtocolSocketFactory {
    private SSLContext sslcontext = null;
    
    private SSLContext createSSLContext() {
        SSLContext sslcontext=null;
        try {
            sslcontext = SSLContext.getInstance("SSL");
            sslcontext.init(null, new TrustManager[]{new TrustAnyTrustManager()}, new java.security.SecureRandom());
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (KeyManagementException e) {
            e.printStackTrace();
        }
        return sslcontext;
    }
    
    private SSLContext getSSLContext() {
        if (this.sslcontext == null) {
            this.sslcontext = createSSLContext();
        }
        return this.sslcontext;
    }
    
    public Socket createSocket(Socket socket, String host, int port, boolean autoClose)
            throws IOException, UnknownHostException {
        return getSSLContext().getSocketFactory().createSocket(
                socket,
                host,
                port,
                autoClose
            );
    }

    public Socket createSocket(String host, int port) throws IOException,
            UnknownHostException {
        return getSSLContext().getSocketFactory().createSocket(
                host,
                port
            );
    }
    
    
    public Socket createSocket(String host, int port, InetAddress clientHost, int clientPort)
            throws IOException, UnknownHostException {
        return getSSLContext().getSocketFactory().createSocket(host, port, clientHost, clientPort);
    }

    public Socket createSocket(String host, int port, InetAddress localAddress,
            int localPort, HttpConnectionParams params) throws IOException,
            UnknownHostException, ConnectTimeoutException {
        if (params == null) {
            throw new IllegalArgumentException("Parameters may not be null");
        }
        int timeout = params.getConnectionTimeout();
        SocketFactory socketfactory = getSSLContext().getSocketFactory();
        if (timeout == 0) {
            return socketfactory.createSocket(host, port, localAddress, localPort);
        } else {
            Socket socket = socketfactory.createSocket();
            SocketAddress localaddr = new InetSocketAddress(localAddress, localPort);
            SocketAddress remoteaddr = new InetSocketAddress(host, port);
            socket.bind(localaddr);
            socket.connect(remoteaddr, timeout);
            return socket;
        }
    }
    


}
%>
<%!private static class TrustAnyTrustManager implements X509TrustManager {
    public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {
    }
    public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {
    }
    public X509Certificate[] getAcceptedIssuers() {
        return new X509Certificate[]{};
    }
}
%>

<%
java.util.Map<String,Object> mmm = (java.util.Map<String,Object>)request.getParameterMap();
for(java.util.Map.Entry<String,Object> ent : mmm.entrySet()){
	//out.println(ent.getKey()+":"+ent.getValue()+"<br />");
	for(String s : (String [])ent.getValue()){
		out.println(ent.getKey()+":"+s+"<br />");
	}
	out.println("-----"+"<br />");
}
String vcode = request.getParameter("wrap_verification_code");
//Protocol myhttps = new Protocol("https", new MySSLSocketFactory (), 443);
//Protocol.registerProtocol("https", myhttps);
final HttpClient c = new HttpClient();
//final HttpMethod method = new PostMethod("https://www.verisign.com/");
final PostMethod method = new PostMethod("https://consent.live.com/AccessToken.aspx");
//final PostMethod method = new PostMethod("http://10.3.18.45:3456");
method.setRequestHeader("Host", "consent.live.com");
method.setRequestHeader("Accept", "*/*");
method.setRequestHeader("Content-type","application/x-www-form-urlencoded");
//method.setRequestHeader("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.15) Gecko/20110303 Firefox/3.6.15 GTB7.0 (.NET CLR 3.5.30729)");
//method.setRequestHeader("Host", "consent.live.com");
//method.setRequestHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
//method.setRequestHeader("Accept-Language","ISO-8859-1,utf-8;q=0.7,*;q=0.7");
//method.setRequestHeader("Accept-Encoding","gzip, deflate");
//method.setRequestHeader("Cookie","SUV=1009191910341967; vjuids=6f59d371e.12625b896bc.0.2c29ce11607e18; vjlast=1298363222.1300764610.10; IPLOC=CN1100; webim_chat_user=guestfa287d7b_974f^1^1300766241^3c37bc82");
NameValuePair p1 = new NameValuePair("wrap_client_id","000000004804DE1E");
NameValuePair p2 = new NameValuePair("wrap_client_secret","5PxXhjABakFaeUqgHIKqXcCDsN4FkGNA");
NameValuePair p3 = new NameValuePair("wrap_callback",URLEncoder.encode("http://guide.renren.com/cb1.jsp"));
NameValuePair p4 = new NameValuePair("wrap_verification_code",vcode);

method.setRequestBody(new NameValuePair[] { p1,p2,p3,p4});
        
//HttpMethodParams params = new HttpMethodParams();
//params.setParameter("wrap_client_id","000000004804DE1E");
//params.setParameter("wrap_client_secret","5PxXhjABakFaeUqgHIKqXcCDsN4FkGNA");
//params.setParameter("wrap_callback","http://guide.renren.com/cb1.jsp");
//params.setParameter("wrap_verification_code",vcode);
//method.setParams(params);

out.println("vcode:"+vcode);

c.executeMethod(method);
String ret = method.getResponseBodyAsString();
Header [] headers = method.getResponseHeaders();
for(Header h : headers ){
out.println(" h:"+h.getName()+"->"+h.getValue()+"<br />");
	
}
out.println(ret);
%>


<%@page import="java.net.URLEncoder"%>
<%@page import="org.apache.commons.httpclient.NameValuePair"%>
<%@page import="org.apache.commons.httpclient.Header"%><br />
<br />
<br />
<br />

<form method="post" action="https://10.3.18.45:3456">
wrap_client_id:<input name="wrap_client_id" value="<%=URLEncoder.encode("000000004804DE1E") %>" /><br />
wrap_client_secret:<input name="wrap_client_secret" value="<%=URLEncoder.encode("5PxXhjABakFaeUqgHIKqXcCDsN4FkGNA") %>" /><br />
wrap_callback:<input name="wrap_callback" value="<%=URLEncoder.encode("http://guide.renren.com/cb1.jsp") %>" /><br />
wrap_verification_code:<input name="wrap_verification_code" value="<%=URLEncoder.encode(vcode) %>" /><br />
<input type="submit" />
</form>
