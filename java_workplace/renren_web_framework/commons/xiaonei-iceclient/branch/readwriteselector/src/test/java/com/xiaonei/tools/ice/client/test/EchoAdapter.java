package com.xiaonei.tools.ice.client.test;

import echo.EchoManagerI;

public class EchoAdapter {
	public static void main(String[] args) {
		int status = 0;
		Ice.Communicator ic = null;
		try {
			ic = Ice.Util.initialize(args);
			Ice.ObjectAdapter adapter = ic.createObjectAdapterWithEndpoints(
					"echo", "tcp -h 127.0.0.1 -p 10000");
			Ice.Object object = new EchoManagerI();
			adapter.add(object, Ice.Util.stringToIdentity("echo"));
			adapter.activate();
			ic.waitForShutdown();
		} catch (Ice.LocalException e) {
			e.printStackTrace();
			status = 1;
		} catch (Exception e) {
			System.out.println(e.getMessage());
			status = 1;
		}
		if (ic != null) {
			// Clean up
			//
			try {
				ic.destroy();
			} catch (Exception e) {
				System.out.println(e.getMessage());
				status = 1;
			}
		}
		System.exit(status);

	}
}
