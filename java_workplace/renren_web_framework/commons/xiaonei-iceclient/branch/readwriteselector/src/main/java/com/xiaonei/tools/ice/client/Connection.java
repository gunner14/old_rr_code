package com.xiaonei.tools.ice.client;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.SocketChannel;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicInteger;

public class Connection {
	private ByteBuffer writeBuffer;
	private ByteBuffer readBuffer;
	private InetSocketAddress address;
	private SocketChannel socketChannel;
	private int writeBytes = 0;
	private Message message;
	private int state;
	private Selector selector;
	private int interestOps = 0;
	private SelectionKey selectionKey;
	//private static final int StateNotValidated = 1;
	//private static final int StateActive = 2;
	private static final int StateHolding = 3;
	//private static final int StateClosing = 4;
	private static final int StateClosed = 5;
	private CountDownLatch registerLatch = new CountDownLatch(1);
	private AtomicInteger nextRequestId = new AtomicInteger(0);
	private ConcurrentLinkedQueue<Operation> inputQueue = new ConcurrentLinkedQueue<Operation>(); 
	private ConcurrentHashMap<Integer, Operation> readMap = new ConcurrentHashMap<Integer,Operation>();
	
	public Connection(java.net.InetSocketAddress address,SocketChannel socketChannel,int bufSize){
		this.address = address;
		this.socketChannel = socketChannel;
		writeBuffer =  ByteBuffer.allocate(bufSize);
		readBuffer = ByteBuffer.allocate(bufSize);
		message = new Message();
	}
	
	private void fillWriteBuffer(){
		if(writeBytes==0){
			writeBuffer.clear();
			Operation op = inputQueue.peek();
			
			while(op!=null && writeBytes < writeBuffer.capacity() ){
				ByteBuffer buf = op.getBuffer();
				int copyByteSize = Math.min(buf.remaining(), writeBuffer.remaining());
				byte copyBytes[] = new byte[copyByteSize];
				buf.get(copyBytes);
				writeBuffer.put(copyBytes);
				
				if(!buf.hasRemaining()){
					op.writeComplete();
					int requestId = op.getRequestId();
					readMap.put(requestId, op);
					inputQueue.remove();
					op = inputQueue.peek();
				}
				writeBytes+=copyByteSize;
			}
			
			writeBuffer.flip();
		}
	}
	
	public void addOperation(Operation op){
		int requestId = nextRequestId.incrementAndGet();
		op.setRequestId(requestId);
		op.getBuffer().position(0);
		inputQueue.add(op);
		selector.addConnection(this);
		selector.wakeup();
	}
	
	public void setSelector(Selector s){
		this.selector = s;
	}
	
	public void setSelectionKey(SelectionKey key){
		this.selectionKey = key;
	}
	
	public int getInterestOps(){
		return this.interestOps;
	}
	
	public void setInterestOps(int ops){
		this.interestOps = ops;
	}
	public int getAvailInterestOps(){
		int ops = SelectionKey.OP_READ ;
		if(writeBytes > 0 || inputQueue.size() > 0){
			ops |= SelectionKey.OP_WRITE;
		}
		return ops;
	}
	
	public void fixupOps(){
		if(this.selectionKey!=null&&this.selectionKey.isValid()){
			int ops = getAvailInterestOps();
			this.selectionKey.interestOps(ops);
		}
	}
	
	public java.nio.channels.SocketChannel getSocket(){
		return socketChannel;
	}
	
	public void register(Selector selector,int interestOps){
		selector.register(this, interestOps);
		try {
			registerLatch.await();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	public void registerCallback(){
		registerLatch.countDown();
	}
	
	public void socketWrite() throws IOException{
		//System.out.println("socketWrite");
		fillWriteBuffer();
		boolean canWriteMore =  writeBytes > 0;
		while(canWriteMore){
			//System.out.println("fill "+writeBuffer.limit());
			int writed = socketChannel.write(writeBuffer);
			writeBytes -= writed;
			canWriteMore = writed > 0 && writeBytes > 0;
		}
	}
	
	public int getNeedWriteBytes(){
		return writeBytes;
	}
	
	public boolean isActive(){
		return state != StateClosed;
	}
	
	public void setState(int state){
		//System.out.println("set connection state = "+state);
		if(this.state == state)return;
		this.state = state;
	}
	
	public void exception(Exception ex){
		ex.printStackTrace();
		setState(StateClosed);
	}
	
	public void socketRead() throws IOException{
		//System.out.println("socketRead");
		int read = socketChannel.read(readBuffer);
		//System.out.println("read length="+read);
		if(read < 0 )throw new IOException("Disconnected");
		while(read > 0){
			readBuffer.flip();
			while(readBuffer.remaining() > 0){
				if(message.read(readBuffer)){
					if(message.getMessageType() == Protocol.validateConnectionMsg){
						setState(StateHolding);
						System.out.println("connection validate");
					}else if(message.getMessageType() == Protocol.replyMsg){
						int requestId = message.getRequestId();
						//System.out.println("message requestId = "+requestId);
						Operation op = readMap.remove(requestId);
						op.callback(message.getData());
					}
					//System.out.println("==============");
					message = new Message();
				}
			}
			readBuffer.clear();
			read = socketChannel.read(readBuffer);
		}
		fixupOps();
	}
}
