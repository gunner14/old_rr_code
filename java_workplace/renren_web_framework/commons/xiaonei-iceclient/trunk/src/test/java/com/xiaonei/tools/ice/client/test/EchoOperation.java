package com.xiaonei.tools.ice.client.test;

import com.xiaonei.tools.ice.client.BaseOperation;
import com.xiaonei.tools.ice.client.ice.Identity;

public class EchoOperation extends BaseOperation{
	public EchoOperation(Identity id,String operation){
		super(id,operation);
		stream.writeInt(10);//count
		stream.writeInt(0);
		stream.endWriteEncaps();
		stream.pos(10);
		stream.writeInt(stream.size());
		stream.prepareWrite();
	}
}
