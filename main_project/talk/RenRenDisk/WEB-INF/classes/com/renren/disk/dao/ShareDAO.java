package com.renren.disk.dao;

import java.util.List;

import net.paoding.rose.jade.annotation.DAO;
import net.paoding.rose.jade.annotation.SQL;

@DAO
public interface ShareDAO {

	/**
	 * 向表user_share插入一条记录
	 * 
	 * @param id
	 *            手动生成一个唯一的标识
	 * @param userId
	 *            用户的人人账号id
	 * @param url
	 *            分享文件的路径，这儿是一个相对路径
	 * @return 
	 */
	@SQL("insert into user_share (Id, user_id, url, size, time) values( :1, :2, :3, :4, :5)")
	public void insert(String id, int userId, String url, String size, String time);

	/**
	 * 根据userId以及url的值删除user_share中的记录
	 * 
	 * @param userId
	 *            表中user_id对应的值，这种作为筛选的一个条件
	 * @param url
	 *            表中url对应的值,也是作为筛选的一个条件
	 */
	@SQL("delete from user_share where user_id = :1 and url = :2")
	public void delete(int userId, String url);


	/**
	 * 根据分享的id查找对应的分享记录
	 * 
	 * @param id
	 *            分享记录的id
	 * @return 分享的记录
	 */
	@SQL("select * from user_share where Id = :1")
	public Share find(String id);

	/**
	 * 查找记录是否已经存在，若存在返回记录的id
	 * 
	 * @param userId
	 *            用户id
	 * @param url
	 *            用户分享的url
	 * @return 记录的id
	 */
	@SQL("select Id from user_share where user_id = :1 and url = :2")
	public String findId(int userId, String url);

	/**
	 * 根据Id查找user_share中的属性值
	 * 
	 * @param property
	 *            属性名
	 * @param id
	 *            share表中的id，即分享记录的id
	 * @return 属性值
	 */
	@SQL("select ##(:1) from user_share where Id = :2")
	public String findProperty(String property, int id);

	/**
	 * 查找用户所有分享内容
	 * 
	 * @param user_id
	 *            用户的id
	 * @return 返回分享内容的id
	 */
	@SQL("select * from user_share where user_id = :1")
	public List<Share> find(int user_id);

	/**
	 * 删除分享内容
	 * 
	 * @param userId
	 *            用户的id
	 * @param arr
	 *            用户分享内容的id，多条记录用数组存储
	 */
	@SQL("delete from user_share where user_id = :1 and url in (:2)")
	public void delete(int userId, String[] arr);

	/**
	 * 查找分享内容
	 * 
	 * @param userId
	 *            用户id
	 * @param arr
	 *            分享内容的路径
	 * @return 相应路径对应的id
	 */
	@SQL("select Id from user_share where user_id = :1 and url in (:2)")
	public String[] findId(int userId, String[] arr);

	/**
	 * 根据id删除记录
	 * 
	 * @param ids
	 *            id数组
	 */
	@SQL("delete from  user_share where Id in (:1)")
	public void delete(String[] ids);
	
	/**
	 * 根据id删除记录
	 * 
	 * @param ids
	 *            要删除id数组的id
	 */
	@SQL("delete from  user_share where Id = :1")
	public void delete(String id);
	
	/**
	 * 更新下载次数
	 * 
	 * @param id
	 *            用户id
	 * @param url
	 *            文件路径
	 * @return 
	 */
	@SQL("update user_share set downloads = downloads + 1 where user_id = :1 and url = :2")
	public int update(int userId, String url);
}
