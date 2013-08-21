package com.renren.sns.wiki.utils.event.exception;

/**
 * 调用路劲过长会导致该异常发生
 */
public class LongEventPathException extends RuntimeException {

    private static final long serialVersionUID = 1652278845277775748L;

    private String invocationPath;

    private int maxAllowedLength;

    public LongEventPathException(String invocationPath, int maxAllowedLength) {
        super("Long event invocation path detected. allowed max path length:" + maxAllowedLength
                + " invocationPath: " + invocationPath);
        this.invocationPath = invocationPath;
        this.maxAllowedLength = maxAllowedLength;
    }

    public String getInvocationPath() {
        return invocationPath;
    }

    public void setInvocationPath(String invocationPath) {
        this.invocationPath = invocationPath;
    }

    public int getMaxAllowedLength() {
        return maxAllowedLength;
    }

    public void setMaxAllowedLength(int maxAllowedLength) {
        this.maxAllowedLength = maxAllowedLength;
    }

}
