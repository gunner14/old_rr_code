package com.xiaonei.reg.register.controllers.ws;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.MalformedURLException;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

import javax.net.SocketFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

import org.apache.commons.httpclient.ConnectTimeoutException;
import org.apache.commons.httpclient.params.HttpConnectionParams;
import org.apache.commons.httpclient.protocol.SecureProtocolSocketFactory;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.JSONValue;
import org.json.simple.parser.ParseException;

import com.xiaonei.platform.core.utility.Base64;

class MySSLSocketFactory implements SecureProtocolSocketFactory {
	private SSLContext sslcontext = null;

	private SSLContext createSSLContext() {
		SSLContext sslcontext = null;
		try {
			sslcontext = SSLContext.getInstance("SSL");
			sslcontext.init(null,
					new TrustManager[] { new TrustAnyTrustManager() },
					new java.security.SecureRandom());
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

	public Socket createSocket(Socket socket, String host, int port,
			boolean autoClose) throws IOException, UnknownHostException {
		return getSSLContext().getSocketFactory().createSocket(socket, host,
				port, autoClose);
	}

	public Socket createSocket(String host, int port) throws IOException,
			UnknownHostException {
		return getSSLContext().getSocketFactory().createSocket(host, port);
	}

	public Socket createSocket(String host, int port, InetAddress clientHost,
			int clientPort) throws IOException, UnknownHostException {
		return getSSLContext().getSocketFactory().createSocket(host, port,
				clientHost, clientPort);
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
			return socketfactory.createSocket(host, port, localAddress,
					localPort);
		} else {
			Socket socket = socketfactory.createSocket();
			SocketAddress localaddr = new InetSocketAddress(localAddress,
					localPort);
			SocketAddress remoteaddr = new InetSocketAddress(host, port);
			socket.bind(localaddr);
			socket.connect(remoteaddr, timeout);
			return socket;
		}
	}

}

class TrustAnyTrustManager implements X509TrustManager {
	public void checkClientTrusted(X509Certificate[] chain, String authType)
			throws CertificateException {
	}

	public void checkServerTrusted(X509Certificate[] chain, String authType)
			throws CertificateException {
	}

	public X509Certificate[] getAcceptedIssuers() {
		return new X509Certificate[] {};
	}
}

public class RRMSNConnect {
	
	private static String GetAllContacts(String accessToken,String uid,String payload) throws IOException {
		StringBuilder sb = new StringBuilder();
		try{
			URL url = new URL("http://apis.live.net/V4.1/cid-"+uid+"/Contacts/AllContacts?payload="+payload);
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			//conn.setRequestProperty("Accept", "application/json");
			//conn.setRequestProperty("Content-Type", "application/json");
			conn.setRequestProperty("Accept", "application/json");
			conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
			conn.setRequestProperty("Authorization", "WRAP access_token=" + accessToken);
			//conn.setRequestProperty("Accept-Charset", "utf-8");
			//conn.setRequestProperty("Content-Type", "utf-8");
			conn.setAllowUserInteraction(true);
			conn.setDoInput(true);
			conn.setDoOutput(true);
			conn.setUseCaches(false);
			conn.setDefaultUseCaches(false);
			conn.setRequestMethod("GET");
			conn.getInputStream();
			BufferedReader rd = new BufferedReader(new InputStreamReader(conn.getInputStream(),"UTF-8"));
			String line;
			while ((line = rd.readLine()) != null) {
				sb.append(line);
			}
			//System.out.println(sb.toString());
			rd.close();
		} catch (MalformedURLException e){
			e.printStackTrace();
			throw e;
		} catch (IOException e){
			e.printStackTrace();
			throw e;
		}
		return sb.toString();
	}
	
	private static String GetWindowsLiveID(String accessToken,String uid) throws IOException {
		StringBuilder sb = new StringBuilder();
		try{
			URL url = new URL("http://apis.live.net/V4.1/cid-"+uid+"/Profiles");
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			conn.setRequestProperty("Accept", "application/json");
			conn.setRequestProperty("Content-Type", "application/json");
			//conn.setRequestProperty("Accept", "atom/xml");
			//conn.setRequestProperty("Content-Type", "atom/xml");
			conn.setRequestProperty("Authorization", "WRAP access_token=" + accessToken);
			//conn.setRequestProperty("Accept-Charset", "utf-8");
			//conn.setRequestProperty("Content-Type", "utf-8");
			conn.setAllowUserInteraction(true);
			conn.setDoInput(true);
			conn.setDoOutput(true);
			conn.setUseCaches(false);
			conn.setDefaultUseCaches(false);
			conn.setRequestMethod("GET");
			conn.getInputStream();
			BufferedReader rd = new BufferedReader(new InputStreamReader(conn.getInputStream(),"UTF-8"));
			String line;
			while ((line = rd.readLine()) != null) {
				sb.append(line);
			}
			//System.out.println(sb.toString());
			rd.close();
		} catch (MalformedURLException e){
			e.printStackTrace();
			throw e;
		} catch (IOException e){
			e.printStackTrace();
			throw e;
		}
		String address = "";
		try {
			JSONObject json = (JSONObject) JSONValue.parseWithException(sb.toString());
			JSONArray entrys = (JSONArray)json.get("Entries");
			JSONObject entry =(JSONObject) entrys.get(0);
			JSONArray emails = (JSONArray)entry.get("Emails");
			JSONObject email = (JSONObject)emails.get(0);
			address = (String)email.get("Address");
		} catch (ParseException e) {
			e.printStackTrace();
		}
		return address;
	}
	
	public static void main(String[] args) {
		String Access_Token = "EwC4ARAnAAAUWkziSC7RbDJKS1VkhugDegv7L0eAAFNI3kRBMkchUqAGsBBuudEbv1L3%2bGeUm73f94NPcSb9cdQRGcq1L5k47L3gZ18WzOQlHXzLSzxCxVHOoPoWLIQZRnP3SwW7blYyGwn9wOxntQ47OZWjA0ptOXSwTjlStlCNtKNjfquXyXZsGbb9hnxNwqAv%2bpeM0MjS5Mfc9XYkA2YAAAht2nf14%2fY1rQgBU28XlPtFe%2f3v6aVSQg8v5YwYqpYJYjh3dDvsf40Fly3FUxJhVgpGl%2fq3k8BdQN5DvNHjaRiAcDTsdAQocEXf0i3mLh78qmWTBt4VrNB9%2fBEcJhw0S2llPrnQakP%2f47GI6YJ%2fckfc5Jt%2f%2fmgGjcazpecQ2iatnfayFoOIZGHbjcgi%2bNB2BvZQcOMCaT8DBdA0ZPvVn0Y0eWccO2jJYl4HvOTx%2bkSHy0wgJiHDpuC8nCJ44xZq0U0cDRYxsdylix01YmHMqkcJaGvzvFN4xMx6keo6YwsA21ndLJo8UX%2bJfDd8x%2b5BK3l7QrrTd9WIAgykiu8acAcbPvRBdj9uKfiUDr9xA04DIREFAAA%3d";
		String Cid = "3e21cdffe9bd5ba8";
		try {
			System.out.println(GetWindowsLiveID(Access_Token,Cid));
			/*String Wlid = 当前用户的WindowsLiveID
			String Cid=当前用户的Cid
			String Access_Token=当前用户Connect之后获得的AccessToken，如果过期了将不能获得数据
			String AppId=应用申请获得的AppID
			String payloadSource = Wild+ "\r\n" +Cid+ "\r\n" +Access_Token+ "\r\n" +AppId
			byte[] payloadSourceBytes = 用UTF8编码转换成字节(payloadSource);
			byte[] payloadIV = 随机产生的16字节数据;
			byte[] payloadCipher = AES(payloadSourceBytes, AESKey, IV);
			byte[] payloadBytes = payloadIV + payloadCipher;
			string payload = BASE64(payloadBytes);*/
			String Wlid = GetWindowsLiveID(Access_Token,Cid);
			String AppId="000000004804DE1E";
			String payloadSource = Wlid+ "\r\n" +Cid+ "\r\n" +Access_Token+ "\r\n" +AppId;
			byte[] payloadSourceBytes = payloadSource.getBytes("UTF-8");
			byte[] payloadIV = new byte[]{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
			byte[] payloadCipher = AES.encrypt(new String(payloadSourceBytes), new String(payloadIV));
			byte[] payloadBytes = new byte[payloadIV.length+payloadCipher.length];
			System.arraycopy(payloadIV,0,payloadBytes,0,payloadIV.length); 
			System.arraycopy(payloadCipher,0,payloadBytes,payloadIV.length,payloadCipher.length); 
			String payload = new String(Base64.encode(payloadBytes));
			System.out.println(GetAllContacts(Access_Token,Cid,payload));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}
