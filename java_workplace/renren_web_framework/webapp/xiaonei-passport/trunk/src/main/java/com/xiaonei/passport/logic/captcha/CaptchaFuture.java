package com.xiaonei.passport.logic.captcha;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class CaptchaFuture implements Future<Integer>{
	private Integer result=0;
	public CaptchaFuture(Integer result){
		this.result=result;
	}
	public CaptchaFuture(){
	}
	@Override
	public boolean cancel(boolean mayInterruptIfRunning) {
		return false;
	}

	@Override
	public Integer get() throws InterruptedException, ExecutionException {
		return result;
	}

	@Override
	public Integer get(long timeout, TimeUnit unit)
			throws InterruptedException, ExecutionException, TimeoutException {
		return result;
	}

	@Override
	public boolean isCancelled() {
		return true;
	}

	@Override
	public boolean isDone() {
		return true;
	}

}
