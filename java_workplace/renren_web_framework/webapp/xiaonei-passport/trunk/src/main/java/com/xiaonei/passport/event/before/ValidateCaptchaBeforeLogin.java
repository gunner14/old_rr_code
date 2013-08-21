package com.xiaonei.passport.event.before;

import java.util.ArrayList;
import java.util.List;

import org.springframework.stereotype.Service;

import com.renren.passport.service.LoginType;
import com.xiaonei.passport.ILoginContext;
import com.xiaonei.passport.ILoginParameter;
import com.xiaonei.passport.constants.LoginStatusCode;
import com.xiaonei.passport.constants.TicketType;
import com.xiaonei.passport.event.ILogicBeforeLogin;
import com.xiaonei.passport.logic.captcha.CaptchaAdapterFactory;
import com.xiaonei.passport.logic.captcha.ICaptchaAdapter;
import com.xiaonei.passport.ticket.ITicketService;
import com.xiaonei.passport.ticket.TicketServiceFactory;
import com.xiaonei.passport.util.TypeConvertUtil;
@Service
public class ValidateCaptchaBeforeLogin implements ILogicBeforeLogin{
	private ICaptchaAdapter captchaAdapter=CaptchaAdapterFactory.getCaptchaAdapterInstance();
	private ITicketService ticketService=TicketServiceFactory.getInstance();
	
	@Override
	public LoginStatusCode process(ILoginParameter loginParameter) {
		ILoginContext context=loginParameter.getLogincontext();
		String userName=loginParameter.getUserName();
		String icode=loginParameter.getCaptchaCode();
		String captchaType=loginParameter.getCaptchaType();
		String ick=loginParameter.getIck();
		String passportTicket = loginParameter.getLogincontext().getCooikes().get("passport");
		if(passportTicket != null) {
			long id = ticketService.verifyTicket(passportTicket, defineAllLoginTypes(), TicketType.PASSPORT);
			if(id != -1){
				ick = String.valueOf(id);
			}
		}
		
		//boolean result= captchaAdapter.validateCatcha(context, userName, icode, captchaType, ick, context.getIp());
		boolean result = true;
		return result?LoginStatusCode.LOGIN_OK:LoginStatusCode.FAILVALIDATELOGIN;
	}
	
	private List<Integer> defineAllLoginTypes() {
		List<Integer> types=new ArrayList<Integer>();
		Integer type=TypeConvertUtil.loginType2Type(LoginType.Web);
		if(type!=null){
			types.add(type);
		}
		return types;
	}

}
