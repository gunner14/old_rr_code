package xce.util.channel;

public class ScoreNewChannel extends Channel {

	protected ScoreNewChannel() {
		super("ScoreNewChannel");
	}

	@Override
	protected void initLocator() {
		properties.setProperty("IceGrid.InstanceName", "ScoreNew");
		properties.setProperty("Ice.Default.Locator",
				"ScoreNew/Locator:default -h ScoreNewMaster -p 4070:"
						+ "default -h ScoreNewReplica1 -p 4070:"
						+ "default -h ScoreNewReplica2 -p 4070");
	}
}