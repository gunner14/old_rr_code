package com.xiaonei.passport.event.after;

import org.springframework.stereotype.Service;

import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.event.ILogicAfterLogin;
import com.xiaonei.passport.model.LoginResult;
import com.xiaonei.passport.task.set.FlyerTaskSet;
import com.xiaonei.passport.task.set.LogTaskSet;
import com.xiaonei.passport.threadpool.RunnableTaskExecutor;

@Service
public class TaskAfterLogin implements ILogicAfterLogin {
	
	@Override
	public LoginStatusCode process(LoginResult result,
			ILoginParameter loginParameter) {
		ILoginContext context = loginParameter.getLogincontext();
		final String ip = context.getIp();
		int userId = result.getUserId();
		int loginRegistrationId = loginParameter.getLoginRegistrationId();

		// 后继任务
		RunnableTaskExecutor executors = RunnableTaskExecutor.getInstance();
		executors.runTaskForFM(new LogTaskSet(userId, ip, context.getRequestUrl(), loginRegistrationId));
		executors.runTaskForFM(new FlyerTaskSet(userId, ip, context.getRequestUrl(), loginRegistrationId));
		//executors.runTask(new LoginTimeTask(userId, result.getAccount()));
		return LoginStatusCode.LOGIN_OK;
	}

}
