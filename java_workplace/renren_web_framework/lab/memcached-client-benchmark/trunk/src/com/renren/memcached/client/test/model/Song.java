package com.renren.memcached.client.test.model;


import java.util.Date;

import com.renren.memcached.client.test.model.MusicVO.MusicVOInt;


public class Song extends MusicVOInt{

	private static final long serialVersionUID = -6967227202240910642L;

	private String name;
	/**
	 * 专辑id.默认值0表示是单曲,不属于任何专辑
	 */
	private int albumId = 0;
	/**
	 * 歌手id.默认值0表示不知道谁唱的,佚名
	 */
	private int artistId = 0;
	/**
	 * 音乐文件名字
	 */
	private String src;
	/**
	 * 歌曲发布时间.默认值表示未知
	 */
	private Date publishTime = null;
	/**
	 * 歌曲在专辑中的顺序
	 */
	private int albumIndex = 0;
	/**
	 * 歌曲的入库时间
	 */
	private Date addtime;
	private Date uptime;
	/**
	 * 语言
	 * 其实是个tag的id
	 */
	private int languageId = 0;

	private transient int inhertedLangugaeId = 0;

	/**
	 * 歌曲的状态,0为正常
	 */
	private long status = 0;

	/**
	 * 歌曲的持续时间
	 */
	private int duration = 0;

	public String getName() {
		return name;
	}

	public void setName(final String name) {
		this.name = name;
	}

	public String getSrc() {
		return src;
	}

	public void setSrc(final String src) {
		this.src = src;
	}

	public Date getAddtime() {
		return addtime;
	}

	public void setAddtime(final Date addtime) {
		this.addtime = addtime;
	}

	public Date getPublishTime() {
		return publishTime;
	}

	public void setPublishTime(final Date publishTime) {
		this.publishTime = publishTime;
	}

	public int getAlbumId() {
		return albumId;
	}

	public void setAlbumId(final int albumId) {
		this.albumId = albumId;
	}

	public int getArtistId() {
		return artistId;
	}

	public void setArtistId(final int artistId) {
		this.artistId = artistId;
	}

	public int getAlbumIndex() {
		return albumIndex;
	}

	public void setAlbumIndex(final int albumIndex) {
		this.albumIndex = albumIndex;
	}

	/**
	 * 返回languageId，包括继承的
	 * 
	 * @return
	 */
	public int languageId() {
		if (languageId == 0) {
			return inhertedLangugaeId;
		} else {
			return languageId;
		}
	}

	public int getLanguageId() {
		return languageId;
	}

	public void setLanguageId(final int languageId) {
		this.languageId = languageId;
	}

	public void putInhertLanguageId(final int languageId) {
		this.inhertedLangugaeId = languageId;
	}

	public Date getUptime() {
		return uptime;
	}

	public void setUptime(final Date uptime) {
		this.uptime = uptime;
	}

	@Override
	public boolean equals(final Object obj) {
		if (obj == null) {
			return false;
		}

		if (getId() == null) {
			return false;
		}

		if (!(obj instanceof Song)) {
			return false;
		}
		final Integer oId = ((Song) obj).getId();
		return id.equals(oId);
	}

	@Override
	public int hashCode() {
		return id == null ? 0 : id;
	}

	public int getDuration() {
		return duration;
	}

	public void setDuration(final int duration) {
		this.duration = duration;
	}

	public long getStatus() {
		return status;
	}

	public void setStatus(final long status) {
		this.status = status;
	}

}
