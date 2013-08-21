package com.xiaonei.passport.ticket;

public class TicketServiceFactory {
	public static ITicketService getInstance(){
		return TicketServiceImpl.getInstance();
	}
}
