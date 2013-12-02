package xce.util.channel;

public class FeedControllerChannel extends Channel {

	protected FeedControllerChannel() {
		super("FeedControllerChannel");
	}

	@Override
	protected void initLocator() {
		properties.setProperty("IceGrid.InstanceName", "XceFeedController");
		properties.setProperty("Ice.Default.Locator",
				"XceFeedController/Locator:default -h XceFeedControllerRegistry -p 15200:"
						+ "default -h XceFeedControllerRegistryReplica1 -p 15201:"
						+ "default -h XceFeedControllerRegistryReplica2 -p 15202");

	}
}
