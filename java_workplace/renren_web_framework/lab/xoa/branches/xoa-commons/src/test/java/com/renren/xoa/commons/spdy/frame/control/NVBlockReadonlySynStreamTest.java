package com.renren.xoa.commons.spdy.frame.control;

import static org.junit.Assert.*;

import org.junit.Test;

import com.renren.xoa.commons.spdy.frame.Frame;

public class NVBlockReadonlySynStreamTest {

	
	@Test
	public void testDefaultSynStream() {
		
		byte flag = Frame.FLAG_FIN;
		
		DefaultSynStream sr1 = new DefaultSynStream(flag, 1234);
		sr1.putNV("haha", "hehe");
		byte[] bytes1 = sr1.toByte();

		DefaultSynStream sr2 = new DefaultSynStream(flag, sr1.getData());
		byte[] bytes2 = sr2.toByte();
		
		assertArrayEquals(bytes1, bytes2);
	}
	
	@Test
	public void testNVBlockReadonlySynStream1() {
		
		byte flag = Frame.FLAG_FIN;
		
		DefaultSynStream sr1 = new DefaultSynStream(flag, 1234);
		sr1.putNV("haha", "hehe");
		sr1.putNV("1111", "222222");
		byte[] bytes1 = sr1.toByte();
		
		NVBlockReadonlySynStream sr2 = new NVBlockReadonlySynStream(flag, sr1.getData());
		byte[] bytes2 = sr2.toByte();
		assertArrayEquals(bytes1, bytes2);
		
		bytes2 = sr2.toChannelBuffer().toByteBuffer().array();
		assertArrayEquals(bytes1, bytes2);
	}
	
	@Test
	public void testNVBlockReadonlySynStream2() {
		
		byte flag = Frame.FLAG_FIN;
		
		DefaultSynStream sr1 = new DefaultSynStream(flag, 1234);
		sr1.putNV("haha", "hehe");
		sr1.putNV("1111", "222222");
		
		NVBlockReadonlySynStream sr2 = new NVBlockReadonlySynStream(flag, sr1.getData());
		sr1.setStreamId(5678);
		sr2.setStreamId(5678);
		
		byte[] bytes2 = sr2.toChannelBuffer().toByteBuffer().array();
		assertArrayEquals(sr1.toByte(), bytes2);
	}
	
	@Test
	public void testStreamId() {
		
		byte flag = Frame.FLAG_FIN;
		int streamId = 1234;
		DefaultSynStream sr1 = new DefaultSynStream(flag, streamId);
		sr1.putNV("haha", "hehe");
		sr1.putNV("1111", "222222");
		
		NVBlockReadonlySynStream sr2 = new NVBlockReadonlySynStream(flag, sr1.getData());
		assertEquals(sr1.getStreamId(), streamId);
		assertEquals(sr1.getStreamId(), sr2.getStreamId());
		
		int streamId2 = 5678;
		sr2.setStreamId(streamId2);
		assertEquals(streamId2, sr2.getStreamId());
	}
}
