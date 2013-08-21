package com.xiaonei.passport.ticket;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.xiaonei.passport.constants.TicketType;
import com.xiaonei.passport.util.TypeConvertUtil;
import com.xiaonei.xce.ticket.TicketInfo;
import com.xiaonei.xce.ticket.TicketInfo.Type;

public class TicketServiceImpl implements ITicketService {
	private ITicketAdapter ticketAdapter=TicketAdapter.getInstance();
	private static Log logger=LogFactory.getLog(TicketServiceImpl.class);
	private static ITicketService _instance=new TicketServiceImpl();
	private TicketServiceImpl(){}
	public static ITicketService getInstance(){
		return _instance;
	}

	@Override
	public String createTicket(long userId, long realId, String ip, int loginType, long expiredTime, TicketType ticketType) {
		try{
			if(userId==-1||realId==-1||StringUtils.isEmpty(ip)||loginType==-1){
				logger.error("createTicket method error,  some parametes can not be empty, the user id is "+ userId);
				return null;
			}
			
			if(loginType==-1){
				logger.error("createTicket method error, login type can not be empty, the user id is "+userId);
				return null;
			}
			TicketInfo ticketInfo=new TicketInfo();
			ticketInfo.set_id(userId);
			ticketInfo.set_realId(realId);
			ticketInfo.set_ip(ip);
			ticketInfo.set_type(loginType);
			String t=this.ticketAdapter.getTikcetAdapter(ticketType).createTicket(ticketInfo, expiredTime);
			return t;
		}catch(Exception e){
			logger.error("createTicket method error, the userId is "+userId, e);
			return null;
		}
	}

	@Override
	public boolean destroyTicket(String ticket, TicketType ticketType) {
		try{
			if(StringUtils.isEmpty(ticket)){
				logger.error("destroyTicket method error, ticket is empty");
				return false;
			}
			return this.ticketAdapter.getTikcetAdapter(ticketType).destroyTicket(ticket);
		}catch(Exception e){
			logger.error("destroyTicket method error", e);
			return false;
		}
		
	}

	@Override
	public boolean destroyTicketById(long id, int type, TicketType ticketType) {
		try{
			if(id==-1){
				logger.error("destroyTicketById method error, the id or loginType is null, the userId is "+id);
				return false;
			}
			if(type==-1){
				logger.error("destroyTicketById method error, login type can not be empty, the user id is "+id);
				return false;
			}	
			return ticketAdapter.getTikcetAdapter(ticketType).destroyTicketById(id, type);
		}catch(Exception e){
			logger.error("destroyTicketById method error", e);
			return false;
		}
		
	}

	@Override
	public TicketInfo getTicketInfo(String ticket, TicketType ticketType) {
		try{
			if(StringUtils.isEmpty(ticket)){
				logger.error("getTicketInfo method error, ticket is empty");
				return null;
			}
			return ticketAdapter.getTikcetAdapter(ticketType).getTicketInfo(ticket);
		}catch(Exception e){
			logger.error("getTicketInfo method error", e);
			return null;
		}
		
	}

	@Override
	public long verifyTicket(String ticket, int type, TicketType ticketType) {
		try{
			if(StringUtils.isEmpty(ticket)){
				logger.error("verifyTicket method error,  some parametes can not be empty, the ticket is "+ ticket);
				return -1;
			}
			if(type==-1){
				logger.error("verifyTicket method error, login type can not be empty");
				return -1;
			}	
			List<Integer> types=new ArrayList<Integer>();
			types.add(type);
			return this.ticketAdapter.getTikcetAdapter(ticketType).verifyTicket(ticket, types);
		}catch(Exception e){
			logger.error("verifyTicket method error", e);
			return -1;
		}
		
	}

	@Override
	public long verifyTicket(String ticket,
			List<Integer> types,
			TicketType ticketType) {
		return this.ticketAdapter.getTikcetAdapter(ticketType).verifyTicket(ticket, types);
	}

	
	


}
