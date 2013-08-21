package sns.renren.chong.beans;

public class LoginConfigBean {
	private int id = 0;
	private String configKey = "";
	private String configValue = "";
	
	public LoginConfigBean() {
		
	}
	public LoginConfigBean(String key, String value) {
		this.configKey = key;
		this.configValue = value;
	}
	public LoginConfigBean(LoginConfigBean bean) {
		this.configKey = bean.getConfigKey();
		this.configValue = bean.getConfigValue();
		this.id = bean.getId();
	}
	public LoginConfigBean(TableEditedBean bean) {
		this.configKey = bean.getConfigKey();
		this.configValue = bean.getNewValue();
	}
	
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getConfigKey() {
		return configKey;
	}
	public void setConfigKey(String configKey) {
		this.configKey = configKey;
	}
	public String getConfigValue() {
		return configValue;
	}
	public void setConfigValue(String configValue) {
		this.configValue = configValue;
	}
	
	
}
