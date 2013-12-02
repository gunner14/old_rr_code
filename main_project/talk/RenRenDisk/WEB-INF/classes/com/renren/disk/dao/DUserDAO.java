package com.renren.disk.dao;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

@DAO
public interface DUserDAO {

	/**
	 * 向user_info表中添加一条用户记录
	 * 
	 * @param userId
	 *            人人用户id
	 * @param accessToken
	 *            百度网盘access_token
	 * @param refreshToken
	 *            百度网盘refresh_token
	 */
	@SQL("insert into user_info (user_id,access_token,refresh_token) values (:1, :2, :3)")
	public void insert(int userId, String accessToken, String refreshToken);

	/**
	 * 通过user_id查找用户信息
	 * 
	 * @param userId
	 *            用户的人人账户id
	 * @return User对象
	 */
	@SQL("select * from user_info where user_id = :1")
	public DUser findById(int userId);

	/**
	 * 按属性查找用户
	 * 
	 * @param property
	 *            属性名
	 * @param value
	 *            属性值
	 * @return User对象
	 */
	@SQL("select * from user_info where ##(:1) = :2")
	public DUser findByProperty(String property, String value);

	/**
	 * 更新access_token的值
	 * 
	 * @param value
	 *            access_token的新值
	 * @param oldValue
	 *            access_token的原始值
	 */
	@SQL("update user_info set access_token = :1 where access_token = :2")
	public void updateToken(String value, String oldValue);

	/**
	 * 通过id和token新值，更新原有token值
	 * 
	 * @param value
	 *            access_token的新值
	 * @param userId
	 *            用户的id，对应表中user_id
	 */
	@SQL("update user_info set access_token = :1 where user_id = :2")
	public void updateTokenById(String value, int userId);

	/**
	 * 通过user_id查找property对应的值
	 * 
	 * @param propertry
	 *            属性名
	 * @param userId
	 *            user_id的值
	 * @return 属性值
	 */
	@SQL("select ##(:1) from user_info where user_id = :2")
	public String findProperty(String propertry, int userId);

}
