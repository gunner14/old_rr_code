package com.renren.xoa2.demo.twitter;

/**
 * 作为一个压力器
 * 
 * @author jinchao.wen/yuan.liu
 * @email  jinchao.wen@opi-corp.com/yuan.liu1@renren-inc.com
 * @date   2011-12-21
 */
import java.util.ArrayList;
import java.util.List;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

import com.renren.xoa2.demo.twitter.TwitterService;

public class TwitterClient {

	public static class ExitHandler implements Runnable {  
		List<Thread> threadList;
		
		public ExitHandler(List<Thread> list) {  
			threadList = list;
		}  
		
		@SuppressWarnings("deprecation")
		@Override
		public void run() {  
			System.out.println("Terminating process... kill all thread!");
			for (Thread thread: threadList) {
				thread.stop(new Exception("kill!"));
				try {
					thread.join();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} 
		}  
	}
	
	private static int COUNT_ROUND = 10;
	private static int COUNT_THREAD = 100;
    private static int COUNT_PER_THREAD = 1000;

    private static String HOST_NAME;
    private static int PORT;

    private static int PING_SLEEP = 0;
    
    enum OPER_TYPE {
    	PING ,
    	POST_TWEET ,
    	TWEET_SEARCH_RESULT ,
    	ZIP
    }
    
    private static OPER_TYPE oper_type = OPER_TYPE.PING;
    
    static public void setServerInfo(String host, int port) {
        HOST_NAME = host;
        PORT = port;
    }
    
    static public void setOperType(int type) {
    	switch (type) {
		case 0:
			oper_type = OPER_TYPE.PING;
			break;
		case 1:
			oper_type = OPER_TYPE.POST_TWEET;
			break;
		case 2:
			oper_type = OPER_TYPE.TWEET_SEARCH_RESULT;
			break;
		case 3:
			oper_type = OPER_TYPE.ZIP;
		default:
			break;
		}
    }
    
    static public void setPingSleep(int time) {
    	PING_SLEEP = time;
    }

    private static Tweet getTweet(int id) {
        String curTime = Long.toBinaryString(System.currentTimeMillis());
        Tweet tweet = new Tweet(id, curTime, curTime);
    	return tweet;
    }

    static class LoadRunner implements Runnable {

        private TwitterService.Client client;
        private TTransport transport;

        @Override
        public void run() {
            try {
            	init(HOST_NAME, PORT);
            	
            	int i = 0;
            	while (++i <= COUNT_ROUND) {
            		invoke();
            	}
            } catch (Exception e) {
            	e.printStackTrace();
            } finally {
            	destroy();
            }
        }

        private void invoke() {
            try {
                long start = System.currentTimeMillis();
                for (int i = 0; i < COUNT_PER_THREAD; i++) {
                	switch (oper_type) {
					case PING:
	                	client.ping(PING_SLEEP);						
						break;
					case POST_TWEET:
						client.postTweet(getTweet(i));
						break;
					case TWEET_SEARCH_RESULT:
                        TweetSearchResult result = client.searchTweets("");
                        if (i % 100 == 0) {
                        	System.out.println("TweetSearchResult size : " + result.tweets.size());
                        }
                        break;
					case ZIP:
                        client.zip();
						break;
					default:
						break;
					}
                }
                System.out.println("total time count (ms): "+ (System.currentTimeMillis() - start));

            } catch (TTransportException e) {
                e.printStackTrace();
            } catch (TException e) {
                e.printStackTrace();
            } catch (InvalidException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        }

        private void init(String host, int port) {
            try {
                transport = new TFramedTransport(new TSocket(host, port));
                TProtocol protocol = new TBinaryProtocol(transport);
                client = new TwitterService.Client(protocol);
                transport.open();
            } catch (TTransportException e) {
                e.printStackTrace();
            }
        }

        private void destroy() {
            transport.close();
        }
    }

    public static void main(String[] args) {
    	if (args.length < 3) {
    		System.out.println("Usage : host port type[0|1|2|3] [thread_count]");
    		System.exit (-1);
    	}
    	
    	TwitterClient.setServerInfo(args[0], Integer.parseInt(args[1]));
    	TwitterClient.setOperType(Integer.parseInt(args[2]));
    	if (args.length > 3) {
    		COUNT_THREAD = Integer.parseInt(args[3]);
    	}
    	
    	if (TwitterClient.oper_type == OPER_TYPE.TWEET_SEARCH_RESULT) {
    		Thread thread = new Thread(new LoadRunner(), "AddTweet");

        	TwitterClient.setOperType(1);
    		
    		try {
    			thread.start();
				thread.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
    		
    		TwitterClient.setOperType(2);
    	} else if (TwitterClient.oper_type == OPER_TYPE.PING && 
    			args.length >= 5) {
    		setPingSleep(Integer.parseInt(args[4]));
    	}
    	
        List<Thread> threadList = new ArrayList<Thread>();
    	
        if (args.length >= 4) {
        	COUNT_THREAD = Integer.parseInt(args[3]);
        	System.out.println("Set thread number to : " + COUNT_THREAD);
        }
        
    	for( int i=0; i<COUNT_THREAD; i++){
    		Thread thread = new Thread( new LoadRunner(), "loadRunner" + String.valueOf(i));
    		thread.start();
    		threadList.add(thread);
        }
        
    	Thread exitThread = new Thread(new TwitterClient.ExitHandler(threadList));
    	
    	Runtime.getRuntime().addShutdownHook(exitThread);          
        
    	try {
        	for (Thread thread: threadList) {
                thread.join();        		
        	}
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
