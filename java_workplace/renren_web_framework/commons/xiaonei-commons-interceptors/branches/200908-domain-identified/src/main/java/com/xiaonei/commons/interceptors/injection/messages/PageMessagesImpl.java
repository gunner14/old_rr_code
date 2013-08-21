package com.xiaonei.commons.interceptors.injection.messages;

import java.util.ArrayList;
import java.util.List;

public class PageMessagesImpl implements PageMessages {

	private List<PageMessage> messages = new ArrayList<PageMessage>();

	public PageMessagesImpl() {
	}

	public List<PageMessage> getMessages() {
		return messages;
	}

	@Override
	public void add(PageMessage pm) {
		this.messages.add(pm);
	}

	@Override
	public void addMessage(String name, String defaultMessage) {
		this.messages.add(new PageMessage("message", name, defaultMessage));
	}

	@Override
	public void addWarning(String name, String defaultMessage) {
		this.messages.add(new PageMessage("warning", name, defaultMessage));
	}

	@Override
	public void addError(String name, String defaultMessage) {
		this.messages.add(new PageMessage("error", name, defaultMessage));
	}
}
