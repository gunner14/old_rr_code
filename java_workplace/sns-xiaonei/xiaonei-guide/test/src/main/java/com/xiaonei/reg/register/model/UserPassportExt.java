package com.xiaonei.reg.register.model;

public class UserPassportExt {

	private int id;
	private String verify_code;
	private String new_email;
	private String email_verify;

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getVerify_code() {
		return verify_code;
	}

	public void setVerify_code(String verify_code) {
		this.verify_code = verify_code;
	}

	public String getNew_email() {
		return new_email;
	}

	public void setNew_email(String new_email) {
		this.new_email = new_email;
	}

	public String getEmail_verify() {
		return email_verify;
	}

	public void setEmail_verify(String email_verify) {
		this.email_verify = email_verify;
	}

}
