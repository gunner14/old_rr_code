package com.renren.xoa.commons.spdy.frame.control;

import static org.junit.Assert.*;

import org.junit.Test;

import com.renren.xoa.commons.spdy.frame.Frame;

public class OptimizedSynReplyTest {

	
	@Test
	public void testDefaultSynReply() {
		
		byte flag = Frame.FLAG_FIN;
		
		DefaultSynReply sr1 = new DefaultSynReply(flag, 1234);
		sr1.putNV("haha", "hehe");
		byte[] bytes1 = sr1.toByte();

		DefaultSynReply sr2 = new DefaultSynReply(flag, sr1.getData());
		byte[] bytes2 = sr2.toByte();
		
		assertArrayEquals(bytes1, bytes2);
	}
	
	@Test
	public void testOptimizedSynReply1() {
		
		byte flag = Frame.FLAG_FIN;
		int streamId = 1234;
		DefaultSynReply sr1 = new DefaultSynReply(flag, streamId);
		sr1.putNV("haha", "hehe");
		sr1.putNV("1111", "222222");
		byte[] bytes1 = sr1.toByte();
		
		OptimizedSynReply sr2 = new OptimizedSynReply(flag, streamId);
		sr2.putNV("haha", "hehe");
		sr2.putNV("1111", "222222");
		byte[] bytes2 = sr2.toByte();
		assertArrayEquals(bytes1, bytes2);
		
		bytes2 = sr2.toChannelBuffer().toByteBuffer().array();
		assertArrayEquals(bytes1, bytes2);
	}
	
	@Test
	public void testToChannelBuffer() {
		
		byte flag = Frame.FLAG_FIN;
		int streamId = 413312;
		DefaultSynReply sr1 = new DefaultSynReply(flag, streamId);
		sr1.putNV("haha", "hehe");
		sr1.putNV("1111", "222222");
		
		OptimizedSynReply sr2 = new OptimizedSynReply(flag, streamId);
		sr2.putNV("haha", "hehe");
		sr2.putNV("1111", "222222");
		
		sr1.setStreamId(5678);
		sr2.setStreamId(5678);
		
		byte[] bytes2 = sr2.toChannelBuffer().toByteBuffer().array();
		assertArrayEquals(sr1.toByte(), bytes2);
	}
	
	@Test
	public void testGetLength1() {
		byte flag = Frame.FLAG_FIN;
		DefaultSynReply sr1 = new DefaultSynReply(flag, 1234);
		sr1.putNV("haha", "hehe");
		sr1.putNV("1111", "222222");
		
		OptimizedSynReply sr2 = new OptimizedSynReply(flag, sr1.getData());
		
		int length1 = sr1.getLength();
		int length2 = sr2.getLength();
		assertEquals(length1, length2);
	}
	
	@Test
	public void testGetLength2() {
		byte flag = 0;
		DefaultSynReply sr1 = new DefaultSynReply(flag, 1234);
		sr1.putNV("haha", "hehe");
		sr1.putNV("1111", "222222");
		
		OptimizedSynReply sr2 = new OptimizedSynReply(flag, 1234);
		sr2.putNV("1111", "222222");
		sr2.putNV("2222", "3333");
		
		int length1 = sr1.getLength();
		int length2 = sr2.getLength();
		assertEquals(length1, length2);
	}
	
	@Test
	public void testStreamId() {
		
		byte flag = Frame.FLAG_FIN;
		int streamId = 1234;
		DefaultSynReply sr1 = new DefaultSynReply(flag, streamId);
		sr1.putNV("haha", "hehe");
		sr1.putNV("1111", "222222");
		
		OptimizedSynReply sr2 = new OptimizedSynReply(flag, sr1.getData());
		assertEquals(sr1.getStreamId(), streamId);
		assertEquals(sr1.getStreamId(), sr2.getStreamId());
		
		int streamId2 = 5678;
		sr2.setStreamId(streamId2);
		assertEquals(streamId2, sr2.getStreamId());
	}
	
	@Test
	public void testContructor2() {
		
		byte flag = Frame.FLAG_FIN;
		
		DefaultSynReply sr1 = new DefaultSynReply(flag, 1234);
		sr1.putNV("haha", "hehe");
		sr1.putNV("1111", "222222");
		
		OptimizedSynReply sr2 = new OptimizedSynReply(flag, 1234);
		sr2.putNV("haha", "hehe");
		sr2.putNV("1111", "222222");
		
		byte[] bytes2 = sr2.toChannelBuffer().toByteBuffer().array();
		assertArrayEquals(sr1.toByte(), bytes2);
	}
}
