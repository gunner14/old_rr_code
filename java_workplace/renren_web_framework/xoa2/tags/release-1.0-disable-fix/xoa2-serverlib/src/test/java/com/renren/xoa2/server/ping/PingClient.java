package com.renren.xoa2.server.ping;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TFramedTransport;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;
import org.apache.thrift.transport.TTransportException;

public class PingClient {
    private TTransport transport;
    PingServer.Client client;

	public void init(int port) {
		try {
			transport = new TFramedTransport(new TSocket("127.0.0.1", port));
			TProtocol protocol = new TBinaryProtocol(transport);
			client = new PingServer.Client(protocol);
			transport.open();
        } catch (TTransportException e) {
            e.printStackTrace();
        }
    }
	
	public int invoke() throws TException {
		int number = client.ping();
		System.out.println("Client: " + number + " ping!");
		return number;
	}

	
	public void close() {
		transport.close();
	}
}
