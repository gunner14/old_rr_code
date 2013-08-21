package com.renren.xoa2.server.processor;

import org.junit.Test;
import static org.junit.Assert.assertEquals;

import org.apache.thrift.TException;
import org.apache.thrift.transport.TMemoryBuffer;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TType;

public class XoaProcessorTest {
	// 单元测试中，如何访问被测试对象的私有函数？
	@Test
	public void testCloneProto() {
		TMemoryBuffer inTrans = new TMemoryBuffer(0);
		TBinaryProtocol inProto = new TBinaryProtocol(inTrans);
		
		TMemoryBuffer outTrans = new TMemoryBuffer(0);
		TBinaryProtocol outProto = new TBinaryProtocol(outTrans);
		try {
			boolean blv = true;
			inProto.writeBool(blv);
			
			XoaProcessor.cloneProto(inProto, outProto, TType.BOOL, 1);
			
			boolean blv2 = outProto.readBool();
			assertEquals(blv, blv2);			
			
			// I16
			short stv = 12345;
			inProto.writeI16(stv);
			
			XoaProcessor.cloneProto(inProto, outProto, TType.I16, 1);
			short stv2 = outProto.readI16();
			assertEquals(12345, stv2);			
		} catch (TException e) {
			e.printStackTrace();
		}

		try {						
			// BYTE
			byte v = 32;
			inProto.writeByte(v);
			
			XoaProcessor.cloneProto(inProto, outProto, TType.BYTE, 1);
			byte v2 = outProto.readByte();
			assertEquals(v, v2);
		} catch (TException e) {
			e.printStackTrace();
		}
		
		try {						
			// I32
			int v = 12345;
			inProto.writeI32(v);
			
			XoaProcessor.cloneProto(inProto, outProto, TType.I32, 1);
			int v2 = outProto.readI32();
			assertEquals(v, v2);
		} catch (TException e) {
			e.printStackTrace();
		}
		
		try {						
			// I64
			long v = 12345;
			inProto.writeI64(v);
			
			XoaProcessor.cloneProto(inProto, outProto, TType.I64, 1);
			long v2 = outProto.readI64();
			assertEquals(v, v2);
		} catch (TException e) {
			e.printStackTrace();
		}

		try {						
			// DOUBLE
			double v = 32.329;
			inProto.writeDouble(v);
			
			XoaProcessor.cloneProto(inProto, outProto, TType.DOUBLE, 1);
			double v2 = outProto.readDouble();
			assertEquals(v, v2, 1e-15); // double的 assert 要特殊处理
		} catch (TException e) {
			e.printStackTrace();
		}		

		try {						
			// DOUBLE
			String v = "32.329";
			inProto.writeString(v);
			
			XoaProcessor.cloneProto(inProto, outProto, TType.STRING, 1);
			String v2 = outProto.readString();
			assertEquals(v, v2);
		} catch (TException e) {
			e.printStackTrace();
		}
		
		// TODO : ENUM, STRUCT, LIST, SET, MAP ???
		// assertEquals(0, 1);
	}
	@Test
	public void benchmark() {
		
	}
}
