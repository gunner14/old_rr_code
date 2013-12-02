package com.renren.xcs;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class NodeData {

    private static Log logger = LogFactory.getLog(NodeData.class.getName());

    public final static String STATE_FIELD = "state";

    public final static String HEALTHY_FIELD = "healthy";

    public final static String DISABLED_FIELD = "disabled";

    private String state;

    private boolean disabled;

    private boolean healthy;

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }

    public boolean isDisabled() {
        return disabled;
    }

    public void setDisabled(boolean disabled) {
        this.disabled = disabled;
    }

    public boolean isHealthy() {
        return healthy;
    }

    public void setHealthy(boolean healthy) {
        this.healthy = healthy;
    }

    public NodeData(String state, boolean disabled, boolean healthy) {
        super();
        this.state = state;
        this.disabled = disabled;
        this.healthy = healthy;
    }

    @Override
    public String toString() {
        return "NodeData [state=" + state + ", disabled=" + disabled + ", healthy=" + healthy + "]";
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + (disabled ? 1231 : 1237);
        result = prime * result + (healthy ? 1231 : 1237);
        result = prime * result + ((state == null) ? 0 : state.hashCode());
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null) return false;
        if (getClass() != obj.getClass()) return false;
        NodeData other = (NodeData) obj;
        if (disabled != other.disabled) return false;
        if (healthy != other.healthy) return false;
        if (state == null) {
            if (other.state != null) return false;
        } else if (!state.equals(other.state)) return false;
        return true;
    }

    public byte[] toBytes() {
        return new StringBuffer().append(DISABLED_FIELD).append("=")
                .append(String.valueOf(this.disabled)).append("\n").append(HEALTHY_FIELD)
                .append("=").append(String.valueOf(this.healthy)).append("\n").append(STATE_FIELD)
                .append("=").append(String.valueOf(this.state)).append("\n").toString().getBytes();
    }

    public static NodeData valueOf(String data) {
        return valueOf(data.getBytes());
    }
    public static NodeData valueOf(byte[] data) {
        Properties prop = new Properties();
        try {
            prop.load(new ByteArrayInputStream(data));
        } catch (IOException e) {
            logger.warn("Failed to read NodeData: " + new String(data), e);
            throw new IllegalArgumentException("Invalid data");
        }

        String state = prop.getProperty(STATE_FIELD);
        String healthy = prop.getProperty(HEALTHY_FIELD);
        String disabled = prop.getProperty(DISABLED_FIELD);

        if (state == null || "".equals(state)) {
            throw new IllegalArgumentException("Invalid state field in data.");
        }

        if (disabled == null || (!"true".equals(disabled) && !"false".equals(disabled))) {
            throw new IllegalArgumentException("Invalid disabled field in data.");
        }

        if (healthy == null || (!"true".equals(healthy) && !"false".equals(healthy))) {
            throw new IllegalArgumentException("Invalid healthy field in data.");
        }

        return new NodeData(state, "true".equals(disabled) ? true : false,
                "true".equals(healthy) ? true : false);
    }

    public static void main(String[] args) {
        valueOf(new String(new NodeData("1", true, true).toBytes()));
    }
}
