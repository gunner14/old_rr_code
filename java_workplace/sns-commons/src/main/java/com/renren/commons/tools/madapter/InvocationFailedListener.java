package com.renren.commons.tools.madapter;

public interface InvocationFailedListener {

    public void invocationFailed(final Object targetObject, final String methodName, int failsThreshold);
}
