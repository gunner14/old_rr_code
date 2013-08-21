package com.renren.xoa.proxy.client;

import java.net.ConnectException;
import java.net.InetSocketAddress;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.jboss.netty.bootstrap.ClientBootstrap;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelFutureListener;
import org.jboss.netty.channel.ChannelHandler;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelPipelineCoverage;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;
import org.jboss.netty.channel.socket.DefaultSocketChannelConfig;
import org.jboss.netty.util.Timeout;
import org.jboss.netty.util.Timer;
import org.jboss.netty.util.TimerTask;

import com.renren.xoa.commons.exception.XoaConnectException;
import com.renren.xoa.commons.spdy.datastructure.ExpireWheel;
import com.renren.xoa.commons.spdy.frame.Frame;
import com.renren.xoa.commons.spdy.frame.StreamIdentifiableFrame;
import com.renren.xoa.proxy.FrameProxy;

/**
 * 
 * 
 * @author Li Weibo (weibo.leo@gmail.com) //I believe spring-brother
 * @since 2010-6-10 下午06:13:44
 */
@ChannelPipelineCoverage("one")
public class SpdyFrameClient extends SimpleChannelUpstreamHandler implements FrameProxy{

	protected static final int[] fibonacci = new int[] { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610 };
	
	protected Log logger = LogFactory.getLog(this.getClass());
	
	private ClientBootstrap bootstrap;
    
	private final Timer timer = HashedWheelTimerManager.getInstance().getTimer();
	
    private Channel channel = null;

	private ExpireWheel<ResponseCallback> obs = new ExpireWheel<ResponseCallback>(
			ExpireWheel.CAPACITY_2P18, 2);
	
    /**
     * +2自增的stream id
     */
    private AtomicInteger streamId = new AtomicInteger(1);

    /**
     * 重连次数
     */
    private int reconnectCount = 0;
    
    /**
     * stream id大过一定数值时需要重置回1，为了防止重复reset，需要上锁
     */
    private Object streamIdResetLock = new Object();
    
    /**
     * streamId的重置阈值，超过这个值就要进行reset了。
     * StreamId在SPDY协议规定里只有31个bit，也就是说省去了符号位，Integer.MAX_VALUE的二进制形式
     * 刚好是31个1，减去2048是为了缓冲一下，如果在重置回1之前有别的线程调用了getAndAdd，
     * 就越界了
     * 
     */
    private static final int STREAM_ID_RESET_THRESHOLE = Integer.MAX_VALUE - 2048;
    
    private InetSocketAddress address = null;

    public SpdyFrameClient(InetSocketAddress inetSocketAddress) {
        this.address = inetSocketAddress;
    }

    private ReentrantLock lock = new ReentrantLock();

    public void init() {
    	if (lock.isLocked()) {
            return;
        }
    	lock.lock();
    	try {
			bootstrap = new ClientBootstrap(ClientChannelFactoryManager.getInstance()
					.getClientChannelFactory());
            // Configure the client.
            bootstrap.setOption("tcpNoDelay", true);
            bootstrap.setOption("keepAlive", true);
            
            // Set up the event pipeline factory.
            bootstrap.setPipelineFactory(new ClientPipelineFactory(this));
            bootstrap.connect(address).addListener(new ChannelFutureListener() {
    			@Override
    			public void operationComplete(ChannelFuture channelFuture) throws Exception {
    				channel = channelFuture.getChannel();
    				logChannelStatus("on init-connecting complete");
    			}
    		});
		} catch (Exception e) {
			throw new XoaConnectException(e);
		} finally {
			lock.unlock();
		}
    }

    public boolean isConnected() {
    	return channel != null && channel.isConnected();
    }
    
    /*public void setChannel(Channel channel) {
        this.channel = channel;
    }*/

    public void send(Frame frame, ChannelHandlerContext toClientCtx) {
        /*if (channel == null) {
            initClient();
        }*/
        /*try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}*/
    	if(!isConnected()) {
        	//logger.error("Not connected to node:" + address);
        	throw new XoaConnectException(
					"client not connected to remote host:" + address); 
        }
        
        int sid = streamId.getAndAdd(2);
        
        //StreamId在规定里只有31个bit，也就是说省去了符号位，Integer.MAX_VALUE的二进制形式
    	//刚好是31个1，减去2048是为了缓冲一下，如果在重置回1之前有别的线程调用了getAndAdd，
        //就越界了
        if (sid >= STREAM_ID_RESET_THRESHOLE) {	//DCL防重复reset
        	synchronized (streamIdResetLock) {
				if (streamId.get() >= STREAM_ID_RESET_THRESHOLE) {
					streamId.set(1);
				}
			}
        }

        
        final int originalStreamId = ((StreamIdentifiableFrame)frame).getStreamId();
        final ChannelHandlerContext originalContext = toClientCtx;
        
        ((StreamIdentifiableFrame)frame).setStreamId(sid);	//变更streamId
        
        obs.put(sid, new ResponseCallback() {
			
        	@Override
			public void frameReceived(Frame frame) {
				
        		if (logger.isDebugEnabled()) {
        			logger.debug("callback:" + frame);
        		}
        		if (frame instanceof StreamIdentifiableFrame) {
        			StreamIdentifiableFrame siFrame = (StreamIdentifiableFrame)frame;
        			siFrame.setStreamId(originalStreamId);
        			originalContext.getChannel().write(frame);
        		} else {
            		logger.warn("unexpected frame received:" + frame);
            	}
        		
        		/*if (frame instanceof SynReply) {
					
				} else if (frame instanceof DataFrame) {
					
				} else {
					logger.warn("unexpected frame received:" + frame);
				}*/
			}
		});
        if (channel != null) {
            channel.write(frame);
        } else {
            XoaConnectException ex = new XoaConnectException("Server Connection Initialing..");
            throw ex;
        }
    }

    public ChannelHandler getResponseChannelHandler() {
        return this;
    }

    @Override
    public void channelDisconnected(ChannelHandlerContext ctx, ChannelStateEvent e) {
        logger.warn("channel " + channel + " disconnected from " + address);
    }

    @Override
    public void channelConnected(ChannelHandlerContext ctx, ChannelStateEvent e) {
    	if (logger.isInfoEnabled()) {
    		logger.info("connected to: " + this.address + ", channel:" + ctx.getChannel());
    	}
    	reconnectCount = 0;	//链接成功后将reconnectCount重置为0
    	logChannelStatus("on channelConnected");
    }

    @Override
    public void channelClosed(ChannelHandlerContext ctx, ChannelStateEvent e) {
   		logger.warn("channel closed:" + ctx.getChannel());
   		delayedReconnect();
    }
    
    /**
     * 延时重连
     */
    private void delayedReconnect() {
    	int delay = calculateReconnectDelay(this.reconnectCount);
    	if (logger.isInfoEnabled()) {
    		logger.info("try reconnect after " + delay + " seconds");
    	}
    	timer.newTimeout(new TimerTask() {
            public void run(Timeout timeout) throws Exception {
            	reconnect();
            }
        }, delay, TimeUnit.SECONDS);
    }
    
    /**
     * 计算重连延迟，按fibonacci增长
     * 
     * @param retryCount
     * @return
     */
    private int calculateReconnectDelay(int retryCount) {
    	if (retryCount >= fibonacci.length) {
    		return fibonacci[fibonacci.length - 1];
    	} else {
    		return fibonacci[retryCount];
    	}
	}

	/**
     * 重连
     */
    private void reconnect() {
    	if (channel.isConnected()) {
			logger.warn("channel already connected:" + channel);
    		return;
    	}
    	reconnectCount ++;
    	if (logger.isInfoEnabled()) {
    		logger.info(reconnectCount + "-th reconnecting to:" + address);
    	}
    	bootstrap.connect(address).addListener(new ChannelFutureListener() {
			@Override
			public void operationComplete(ChannelFuture channelFuture) throws Exception {
				channel = channelFuture.getChannel();
				logChannelStatus("on reconnecting complete");
			}
		});
    }

    /**
     * log当前的连接状态信息
     * @param msg 说明信息
     */
    private void logChannelStatus(String msg) {
    	if (logger.isDebugEnabled()) {
    		if (channel == null) {
    			logger.debug(msg + ": channel null");
    		} else {
    			StringBuilder sb = new StringBuilder();
    			sb.append(msg + "-");
    			sb.append("channel:" + channel);
    			sb.append(", ");
        		sb.append("isBound:" + channel.isBound());
        		sb.append(", ");
        		sb.append("isOpen:" + channel.isOpen());
        		sb.append(", ");
        		sb.append("isConnected:" + channel.isConnected());
        		sb.append(", ");
        		sb.append("isReadable:" + channel.isReadable());
        		sb.append(", ");
        		sb.append("isWritable:" + channel.isWritable());
        		logger.debug(sb.toString());
    		}
    	}
    }
    
    @Override
    public void messageReceived(ChannelHandlerContext ctx, MessageEvent e) throws Exception {
        if (e.getMessage() instanceof Frame) {
            
        	Frame frame = (Frame)e.getMessage();
        	
        	if (logger.isDebugEnabled()) {
        		logger.debug("back frame received:" + frame);
        	}
        	if (frame instanceof StreamIdentifiableFrame) {
        		int streamId = ((StreamIdentifiableFrame)frame).getStreamId();
        		ResponseCallback callback = obs.get(streamId);
        		if (callback != null) {
        			callback.frameReceived(frame);
        		} else {
        			logger.error("no callback for frame with such streamId:" + streamId);
        		}
        	} else {
        		logger.warn("unexpected frame received:" + frame);
        	}
        } else {
        	//作为整个pipeline上的最后一个节点，代码不应该执行到这里
        	logger.warn(this.getClass().getName() + " received message " + e.getMessage().getClass().getName());
        	super.messageReceived(ctx, e);
        }
    }

    //有异常需要重新建立连接，每个client有自己的重建方法，这里简单地从容器里去掉
    @Override
    public void exceptionCaught(ChannelHandlerContext ctx, ExceptionEvent e) throws Exception {
    	logChannelStatus("on exceptionCaught");
    	Throwable cause = e.getCause();
        if (cause instanceof ConnectException) {
        	
        	/*
        	 * 这个逻辑是从例子里抄过来的，还没看出来会什么用。。。
        	 * 连接异常断掉的时候channel.close()会被ChannelFutureListener(line57)
        	 * 调用，也就是说，在exceptionCaught之前，channelClosed方法
        	 * 已经被调用过了，代码走到这里的时候channel已经被关掉了，
        	 * 这里重复关闭没有产生任何效果
        	 * 
        	 * */
        	ctx.getChannel().close();
        }
        
        /*
         * 这里不要加入重连代码，因为连接断掉的时候并不一定要抛出异常。
         * 而连接断掉的时候一定会进入channelClosed方法
         * 
         * */
        XoaConnectException ex = new XoaConnectException(cause);
        throw ex;
    }

	@Override
	public void proxy(Frame frame, ChannelHandlerContext toClientCtx) {
		if (logger.isDebugEnabled()) {
			logger.debug("frame received:" + frame);
		}
		send(frame, toClientCtx);
	}
}
