package com.xiaonei.xce.ticket;

final public class TemporaryTicketImpl extends TicketImpl {

    private static TemporaryTicketImpl _instance = null;

    public TemporaryTicketImpl() {
        super("TTicket");
    }

    /**
     * 推荐使用
     * 
     * @return 单例
     */
    public TemporaryTicketImpl getInstance() {
        if (TemporaryTicketImpl._instance == null) {
            synchronized (TemporaryTicketImpl.class) {
                if (TemporaryTicketImpl._instance == null) {
                    TemporaryTicketImpl._instance = new TemporaryTicketImpl();
                }
            }
        }
        return TemporaryTicketImpl._instance;
    }
}
