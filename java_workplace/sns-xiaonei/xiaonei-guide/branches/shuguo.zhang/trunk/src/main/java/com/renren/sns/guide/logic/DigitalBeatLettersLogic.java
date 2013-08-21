package com.renren.sns.guide.logic;

import java.sql.SQLException;

import org.json.JSONObject;

import com.renren.sns.guide.dao.DigitalBeatLettersDAO;
import com.renren.sns.guide.dao.DigitalLettersSumDAO;
import com.renren.sns.guide.model.DigitalAndLetters;

public class DigitalBeatLettersLogic {
	
	private static DigitalBeatLettersLogic instance = new DigitalBeatLettersLogic();

	public static DigitalBeatLettersLogic getInstance() {
		return instance;
	}

	public String getDigitalAndLetters(){
		JSONObject json = new JSONObject();
		try {
			DigitalAndLetters dal=DigitalLettersSumDAO.getInstance().getDigitalAndLetters();
			if(dal!=null){				
				Integer digital=dal.getDigital();
				Integer letters=dal.getLetters();					
                json.put("digital", digital);
                json.put("letters", letters);
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return json.toString();
	}
	
	public void insertOrUpdateDigital(int userId, int digital) {
		DigitalBeatLettersDAO.getInstance().insertOrUpdateDigital(userId, digital);
	}

	public void insertOrUpdateLetters(int userId, int letters) {
		DigitalBeatLettersDAO.getInstance().insertOrUpdateDigital(userId, letters);
	}
	
	public void updateDigitalSum(int digital){
		DigitalLettersSumDAO.getInstance().updateDigital(digital);
	}
	
	public void updateLettersSum(int letters){
		DigitalLettersSumDAO.getInstance().updateLetters(letters);
	}
	
}
