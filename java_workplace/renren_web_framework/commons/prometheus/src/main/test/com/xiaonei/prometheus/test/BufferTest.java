package com.xiaonei.prometheus.test;

import java.nio.BufferUnderflowException;

import org.junit.Assert;
import org.junit.Test;

import com.xiaonei.prometheus.nio.Buffer;


public class BufferTest {
	@Test
	public void test(){
		Buffer buf = new Buffer();
		buf.writeByte((byte)0);
		Assert.assertTrue(buf.size()==1);
		byte[] bs = new byte[]{(byte)1,(byte)2,(byte)3};
		buf.writeByte(bs);
		Assert.assertTrue(buf.size()==4);
		buf.writeInt(1000);
		Assert.assertTrue(buf.size()==8);
		buf.writeInt(2000);//need resize;
		Assert.assertTrue(buf.size()==12);
		buf.flip();
		Assert.assertTrue(buf.readByte()==0);
		Assert.assertTrue(buf.readByte()==1);
		buf.readByte();buf.readByte();
		Assert.assertTrue(buf.readInt()==1000);
		Assert.assertTrue(buf.readInt()==2000);
		
		try{
			buf.readInt();
			Assert.assertTrue(false);
		}catch(BufferUnderflowException ex){
			Assert.assertTrue(true);
		}
	}
}
