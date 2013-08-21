package echo;

import Ice.Current;

public class EchoManagerI extends echo._EchoManagerDisp {

	@Override
	public int echo(int count, int delay, Current __current) {
		return count;
	}

}
