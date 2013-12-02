package xce.util.ng.channel.impl;

import xce.util.ng.channel.AbstractMultiCommunicatorChannel;

public class EdmChannelMC extends AbstractMultiCommunicatorChannel {

	public EdmChannelMC() {
		super(30);
	}

	@Override
	protected void initLocator() {
		configure("IceGrid.InstanceName", "Edm");
		configure(
				"Ice.Default.Locator",
				"Edm/Locator:default -h EdmRegistry -p 14810:default -h EdmRegistryReplica1 -p 14811:default -h EdmRegistryReplica2 -p 14812");
	}

}
