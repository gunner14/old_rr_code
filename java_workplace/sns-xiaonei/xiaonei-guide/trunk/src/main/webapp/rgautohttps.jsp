<%@ page contentType="text/html;charset=UTF-8" session="false"%>
<%@page import="java.io.IOException"%>
<%@page import="java.net.InetAddress"%>
<%@page import="java.net.InetSocketAddress"%>
<%@page import="java.net.Socket"%>
<%@page import="java.net.SocketAddress"%>
<%@page import="java.net.UnknownHostException"%>
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

<%out.println(" <br />"); %>

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
<%out.println("To get an user's current state!" +" <br /><br />"); %>



<%@page import="com.xiaonei.msg.home.MessageHome"%>
<%@page import="com.xiaonei.msg.view.MessageIn"%>
<%@page import="java.nio.charset.Charset"%>
<%@page import="org.apache.commons.httpclient.protocol.Protocol"%>
<%@page import="org.apache.commons.httpclient.HttpClient"%>
<%@page import="org.apache.commons.httpclient.params.HttpMethodParams"%>
<%@page import="org.apache.commons.httpclient.methods.GetMethod"%>
<%@page import="org.apache.commons.httpclient.HttpHost"%>
<%@page import="java.util.List"%>
<%@page import="org.apache.commons.httpclient.Header"%>
<%@page import="java.util.ArrayList"%><style>
	input {
		border:1px solid blue;
	}
	
	body {
		color:blue;
	}
</style>
<form action="">

id:<input name="id" value="81120" />
start:<input name="start" value="0" />

<input type="hidden" name="run" value="true">
<input type="submit" />

</form>
<%
String ticket = request.getParameter("ticket");
String url = "https://passport.no.opi-corp.com/verify.php?t="+ticket;
 //注册https
 Protocol myhttps = new Protocol("https", new MySSLSocketFactory (), 443);
 Protocol.registerProtocol("https", myhttps);

HttpClient httpClient = new HttpClient();
//httpClient.getParams().setParameter(HttpMethodParams.HTTP_CONTENT_CHARSET, "UTF-8");
httpClient.getHostConfiguration().setHost(new HttpHost("passport.no.opi-corp.com", 443, myhttps));
List <Header> headers = new ArrayList <Header>(); 
headers.add(new Header("User-Agent", "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)"));
headers.add(new Header("Referer", " http://www.renren.com/"));
httpClient.getHostConfiguration().getParams().setParameter("http.default-headers", headers);
        
GetMethod method = new GetMethod();

httpClient.executeMethod(method);
String ret = method.getResponseBodyAsString();
out.println(ret);
%>

<%

%>

<%out.println(" <br />"); %>
<%out.println("~The End~" +" <br />"); %>