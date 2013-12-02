package xce.util.channel;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class SearchChannel extends Channel {
	private static Log logger = LogFactory.getLog(SearchChannel.class);

	protected SearchChannel() {
		super("SearchChannel");
	}

	@Override
	protected void initLocator() {
		boolean isTestSearchChannel = Boolean.valueOf(System.getProperty(
				"isTestSearchChannel", "false"));
		boolean isABTestSearchChannel = Boolean.valueOf(System.getProperty(
				"isABTestSearchChannel", "false"));
		if (isTestSearchChannel) {
			logger.info("RenRenSearch: Using TestSearchChannel...");
			properties.setProperty("IceGrid.InstanceName", "TestXiaoNeiSearch");
			properties
					.setProperty(
							"Ice.Default.Locator",
							"TestXiaoNeiSearch/Locator:tcp -h TestXiaoNeiSearchRegistry -p 4094:"
									+ "tcp -h TestXiaoNeiSearchRegistryReplica1 -p 4094:"
									+ "tcp -h TestXiaoNeiSearchRegistryReplica2 -p 4094");
		} else if (isABTestSearchChannel) {
			logger.info("RenRenSearch: Using ABTestSearchChannel...");
			properties.setProperty("IceGrid.InstanceName",
					"ABTestXiaoNeiSearch");
			properties
					.setProperty(
							"Ice.Default.Locator",
							"ABTestXiaoNeiSearch/Locator:tcp -h ABTestXiaoNeiSearchRegistry -p 4095:"
									+ "tcp -h ABTestXiaoNeiSearchRegistryReplica1 -p 4095:"
									+ "tcp -h ABTestXiaoNeiSearchRegistryReplica2 -p 4095");
		} else {
			logger.info("RenRenSearch: Using  Online SearchChannel...");
			properties.setProperty("IceGrid.InstanceName", "XiaoNeiSearch");
			properties.setProperty("Ice.Default.Locator",
					"XiaoNeiSearch/Locator:tcp -h XiaoNeiSearchRegistry -p 4061:"
							+ "tcp -h XiaoNeiSearchRegistryReplica1 -p 4061:"
							+ "tcp -h XiaoNeiSearchRegistryReplica2 -p 4061");
		}

	}
}
