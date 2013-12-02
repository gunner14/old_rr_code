package xce.tripod;

import java.util.List;

public abstract class ZNodeListener {
    private String znode = "";

    public ZNodeListener(String znode) {
	if (znode != null) {
	    this.znode = znode;
	}
    }

    public String getZNode() {
	return znode;
    }

    public abstract boolean update(List<String> childrenNameList);
}
