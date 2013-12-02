package xce.util.channel;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SearchControllerChannel extends Channel {
	private static Log logger = LogFactory
			.getLog(SearchControllerChannel.class);

	protected SearchControllerChannel() {
		super("SearchControllerChannel");
	}

	@Override
	protected void initLocator() {

		logger.info("XiaoNeiSearchController: Using  Online XiaoNeiSearchController...");
		properties.setProperty("IceGrid.InstanceName", "XiaoNeiSearchController");
		properties.setProperty("Ice.Default.Locator",
				"XiaoNeiSearchController/Locator:tcp -h XiaoNeiSearchControllerRegistry -p 4098:"
						+ "tcp -h XiaoNeiSearchControllerRegistryReplica1 -p 4098:"
						+ "tcp -h XiaoNeiSearchControllerRegistryReplica2 -p 4098");

	}
}
