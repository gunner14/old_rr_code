package com.xiaonei.commons.interceptors.injection.messages;

import java.util.ArrayList;
import java.util.List;

import net.paoding.rose.web.Invocation;

public class PageMessagesImpl implements PageMessages {

    private Invocation invocation;

    private List<PageMessage> messages = new ArrayList<PageMessage>(4);

    public PageMessagesImpl(Invocation invocation) {
        this.invocation = invocation;
    }

    public List<PageMessage> getMessages() {
        return messages;
    }

    @Override
    public PageMessage addMessage(String code) {
        return this.addMessage(code, null);
    }

    @Override
    public PageMessage addMessage(String code, String defaultMessage) {
        PageMessage pm = new PageMessage(invocation, "message", code, defaultMessage);
        this.messages.add(pm);
        return pm;
    }

    @Override
    public PageMessage addWarning(String code) {
        return this.addWarning(code, null);
    }

    @Override
    public PageMessage addWarning(String code, String defaultMessage) {
        PageMessage pm = new PageMessage(invocation, "warning", code, defaultMessage);
        this.messages.add(pm);
        return pm;
    }

    @Override
    public PageMessage addError(String code) {
        return this.addError(code, null);
    }

    @Override
    public PageMessage addError(String code, String defaultMessage) {
        PageMessage pm = new PageMessage(invocation, "error", code, defaultMessage);
        this.messages.add(pm);
        return pm;
    }

}
