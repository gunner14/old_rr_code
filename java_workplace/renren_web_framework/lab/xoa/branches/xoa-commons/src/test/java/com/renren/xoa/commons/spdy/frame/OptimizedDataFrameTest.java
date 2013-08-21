package com.renren.xoa.commons.spdy.frame;

import static org.junit.Assert.*;
import org.junit.Test;


import com.renren.xoa.commons.spdy.Utils;

public class OptimizedDataFrameTest {
	
	@Test
	public void testHeader() {

		OptimizedDataFrame frame = new OptimizedDataFrame(312412412, (byte)Frame.FLAG_FIN, new byte[]{1,2,3,4});
		
		//用老的方式来拼header
		byte[] headDifferentInByte = frame.getHeaderDifferentBlockByte();
        byte[] flagInByte = Utils.intToByte(frame.getFlags());
        byte[] fixSizeFlagInByte = Utils.trimBytes(flagInByte, 0, Utils
                .bitAlignToByte(DefaultDataFrame.FLAGS_LENGTH_IN_BIT));
        byte[] lengthInByte = Utils.intToByte(frame.getLength());
        byte[] fixSizeLengthInByte = Utils.trimBytes(lengthInByte, 0, Utils
                .bitAlignToByte(DefaultDataFrame.LENGTH_LENGTH_IN_BIT));
        byte[] headerOld = Utils.mergeBytes(headDifferentInByte, fixSizeFlagInByte, fixSizeLengthInByte);
		
        //用新的方式来获取header
        byte[] headerNew = frame.headerToBytes();
        assertArrayEquals(headerOld, headerNew);
	}
	
	@Test
	public void testAll1() {
		OptimizedDataFrame frame = new OptimizedDataFrame(312412412,
				(byte) ControlFrame.FLAG_FIN, new byte[] { 1, 2, 3, 4 });
		byte[] old = frame.toByte();

		byte[] neww = frame.toChannelBuffer().toByteBuffer().array();
		assertArrayEquals(old, neww);
	}

	@Test
	public void testAll2() {
		OptimizedDataFrame frame = new OptimizedDataFrame(312412412,
				(byte) ControlFrame.FLAG_UNIDIRECTIONAL, new byte[] { 1, 2, 3,
						4, 5 });
		byte[] old = frame.toByte();

		byte[] neww = frame.toChannelBuffer().toByteBuffer().array();
		assertArrayEquals(old, neww);
	}
	
	@Test
	public void testAll3() {
		OptimizedDataFrame frame = new OptimizedDataFrame(312412412,
				(byte)0, new byte[] { 1, 2, 3,
						4, 5 });
		byte[] old = frame.toByte();

		byte[] neww = frame.toChannelBuffer().toByteBuffer().array();
		assertArrayEquals(old, neww);
	}
	
}
