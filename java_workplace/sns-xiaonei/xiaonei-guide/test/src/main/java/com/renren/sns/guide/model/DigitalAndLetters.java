package com.renren.sns.guide.model;

public class DigitalAndLetters {
	
	private int id;

	private Integer digital;

	private Integer letters;
	
	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public Integer getDigital() {
		return digital;
	}

	public void setDigital(Integer digital) {
		this.digital = digital;
	}

	public Integer getLetters() {
		return letters;
	}

	public void setLetters(Integer letters) {
		this.letters = letters;
	}
	
	public DigitalAndLetters(Integer digital, Integer letters) {
        super();
        this.digital = digital;
        this.letters = letters;
    }
	
}
