package com.xiaonei.prometheus.server;

final public class Protocol {
	/**
	 * Size Of Protocol Header
	 *  magic number 3 bytes
	 *  opcode			 1 byte
	 *  message size 4 bytes
	 *  seq number 	 4 bytes
	 */
	public final static int headerSize = 12;
	public final static byte magic[] = {84 , 87 , 74};//T,W,J
	
	public final static byte[] header = {
		Protocol.magic[0],
		Protocol.magic[1],
		Protocol.magic[2],
		(byte)0,//opcode
		(byte)0,(byte)0,(byte)0,(byte)0,//message size
		(byte)0,(byte)0,(byte)0,(byte)0//seq number
	};
	
	public static byte OPCODE_GET = 1;
}
