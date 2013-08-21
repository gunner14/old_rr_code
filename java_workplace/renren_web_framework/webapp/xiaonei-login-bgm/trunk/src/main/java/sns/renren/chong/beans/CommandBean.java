package sns.renren.chong.beans;

public class CommandBean {
	private boolean isRecover;
	private String opType = "";
	private String time = "";
	private String configKey = "";
	private String oldValue = "";
	private String newValue = "";
	private String editor="";
	private String comment="";
	
	public boolean isRecover() {
		return isRecover;
	}
	public void setRecover(boolean isRecover) {
		this.isRecover = isRecover;
	}
	public String getEditor() {
		return editor;
	}
	public void setEditor(String editor) {
		this.editor = editor;
	}
	public String getComment() {
		return comment;
	}
	public void setComment(String comment) {
		this.comment = comment;
	}
	
	public String getTime() {
		return time;
	}
	public void setTime(String time) {
		this.time = time;
	}
	public String getOpType() {
		return opType;
	}
	public void setOpType(String opType) {
		this.opType = opType;
	}
	public String getConfigKey() {
		return configKey;
	}
	public void setConfigKey(String configKey) {
		this.configKey = configKey;
	}
	public String getOldValue() {
		return oldValue;
	}
	public void setOldValue(String oldValue) {
		this.oldValue = oldValue;
	}
	public String getNewValue() {
		return newValue;
	}
	public void setNewValue(String newValue) {
		this.newValue = newValue;
	}	
}
