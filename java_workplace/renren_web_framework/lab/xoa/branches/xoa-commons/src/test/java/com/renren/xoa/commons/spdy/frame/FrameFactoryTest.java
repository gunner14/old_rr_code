package com.renren.xoa.commons.spdy.frame;

import static org.junit.Assert.*;

import java.util.Arrays;

import org.junit.Test;

import com.renren.xoa.commons.spdy.frame.ControlFrame;
import com.renren.xoa.commons.spdy.frame.DataFrame;
import com.renren.xoa.commons.spdy.frame.DefaultDataFrame;
import com.renren.xoa.commons.spdy.frame.Frame;
import com.renren.xoa.commons.spdy.frame.FrameFactory;

public class FrameFactoryTest {

	
	/**
	 * 测试构造DataFrame
	 */
	@Test
	public void testBuildDataFrame() {
		DefaultDataFrame origFrame = new DefaultDataFrame(312412412,
				(byte) ControlFrame.FLAG_FIN, new byte[] { 1, 2, 3, 4 });
		byte[] origBytes = origFrame.toByte();
		byte[] header = Arrays.copyOfRange(origBytes, 0, 8);
		byte[] data = Arrays.copyOfRange(origBytes, 8, origBytes.length);
		
		Frame dataFrame = FrameFactory.getInstance().buildFromByte(header, data, false);
		assertTrue(dataFrame instanceof DataFrame);
		byte[] bytes = dataFrame.toByte();
		assertArrayEquals(origBytes, bytes);
	}
	
}
