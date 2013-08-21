package com.renren.xoa2.demo.twitter;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import com.renren.xoa2.demo.twitter.TwitterService;

public class SimpleTwitterClient {	

	    public static void main(String[] args) {

	        System.out.println("build txt start.");
	        
	        StringBuffer sb = new StringBuffer(4 * 1024 * 1024); // reserve buffer
	        
	        for (int i = 0; i < 400 * 1024; ++i) {
	        	sb.append("XOA 2.0.");
	        }
	        String txt = sb.toString();
	        
	        System.out.println("build txt ok : " + txt.length());
	        Tweet tweet = new Tweet();
	        tweet.setUserName("Jeo");
	        tweet.setUserId(12345);
	        tweet.setText(txt);
	        
	        TTransport transport = null;
	        TwitterService.Client client = null;

            try {
                transport = new TFramedTransport(new TSocket("127.0.0.1", 9090));
                TProtocol protocol = new TBinaryProtocol(transport);
                client = new TwitterService.Client(protocol);
                transport.open();
            } catch (TTransportException e) {
                e.printStackTrace();
            }
            try {
				//client.ping(0);
				//client.zip();
            	// client.postTweet(tweet);
            	
            	client.searchTweets("hello");
			} catch (TException e) {
				e.printStackTrace();
			} catch (InvalidException e) {
				System.out.println("---------------");
				e.printStackTrace();
			}
            
            transport.close();
	    }
	}
