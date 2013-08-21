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
	private Selector writerSelector;
	//private int interestOps = 0;
	private SelectionKey selectionKey;//just writerSelector selectionKey;
	//private static final int StateNotValidated = 1;
	//private static final int StateActive = 2;
	private static final int StateHolding = 3;
	//private static final int StateClosing = 4;
	private static final int StateClosed = 5;
	private CountDownLatch registerLatch = new CountDownLatch(2);
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
		writerSelector.addConnection(this);
	}
	
	public void setWriterSelector(Selector s){
		this.writerSelector = s;
	}
	
	/**
	 * just call by WriterSelector
	 * @param key
	 */
	public void setWriterSeletionKey(SelectionKey key){
		this.selectionKey = key;
	}
	
	/**
	 * just call by WriterSelector,ReaderWriterSelector don't need fixupOps
	 */
	public int fixupOps(){
		if(this.selectionKey!=null&&this.selectionKey.isValid()){
			int ops = 0;
			if(writeBytes > 0 || inputQueue.size() > 0){
				ops |= SelectionKey.OP_WRITE;
			}
			this.selectionKey.interestOps(ops);
			return ops;
		}
		return 0;
	}
	
	public java.nio.channels.SocketChannel getSocket(){
		return socketChannel;
	}
	
	public void register(Selector selector,int interestOps){
		selector.register(this, interestOps);
	}
	public void waitRegisterComplete(){
		try{
			registerLatch.await();
		}catch(InterruptedException ex){
			ex.printStackTrace();
		}
	}
	public void registerCallback(){
		registerLatch.countDown();
	}
	
	public void socketWrite() throws IOException{
		//System.out.println("socketWrite");
		fillWriteBuffer();
		boolean canWriteMore =  writeBytes > 0;
		if(!canWriteMore){
			//no operation
			fixupOps();
			return;
		}
		
		while(canWriteMore){
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
		//System.out.println("read length = "+read);
		if(read < 0 )throw new IOException("Disconnected");
		while(read > 0){
			readBuffer.flip();
			while(readBuffer.remaining() > 0){
				if(message.read(readBuffer)){
					if(message.getMessageType() == Protocol.validateConnectionMsg){
						setState(StateHolding);
						System.out.println("connection validate");
					}else if(message.getMessageType() == Protocol.replyMsg || message.getMessageType() == Protocol.requestMsg){
						int requestId = message.getRequestId();
						//System.out.println("message requestId = "+requestId);
						Operation op = readMap.remove(requestId);
						op.callback(message.getData());
					}
					message = new Message();
				}
			}
			readBuffer.clear();
			read = socketChannel.read(readBuffer);
		}
	}
}
