package com.xiaonei.platform.taglib.logic;

import java.util.List;

import com.xiaonei.platform.taglib.dao.TaglibInfoDAO;
import com.xiaonei.platform.taglib.dao.TaglibTempletDAO;
import com.xiaonei.platform.taglib.model.TaglibInfo;
import com.xiaonei.platform.taglib.model.TaglibTemplet;

/**
 * TaglibInfoLogic
 * 
 * @author lifs
 * 
 */
public class TaglibLogic {

	private static TaglibLogic instance = new TaglibLogic();

	public static TaglibLogic getInstance() {
		if (instance == null) {
			synchronized (TaglibLogic.class) {
				if (instance == null)
					instance = new TaglibLogic();
			}
		}
		return instance;
	}

	private TaglibLogic() {
	}

	public int saveTaglibInfo(TaglibInfo ab) {
		try {
			return TaglibInfoDAO.getInstance().save(ab);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}

	public int saveTaglibTemplet(TaglibTemplet tt) {
		try {
			return TaglibTempletDAO.getInstance().save(tt);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}
	public int updateTaglibInfo(TaglibInfo ab) {
		try {
			return TaglibInfoDAO.getInstance().updateInfo(ab);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;

	}
	public int updateTaglibTemplet(TaglibTemplet ab) {
		try {
			return TaglibTempletDAO.getInstance().updateTemplet(ab);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;

	}
	
	public TaglibInfo getTaglibInfo(int id) {
		try {
			return TaglibInfoDAO.getInstance().get(id);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public TaglibTemplet getTaglibTemplet(int id) {
		try {
			return TaglibTempletDAO.getInstance().get(id);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	/**
	 * 得到 taglib_info 列表
	 * @return
	 */
	public List<TaglibInfo> getTaglibInfos() {
		try {
			return TaglibInfoDAO.getInstance().gets();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;

	}
	/**
	 * 得到 taglib_templet 列表
	 * @return
	 */
	public List<TaglibTemplet> getTaglibTemplets(int tagId) {
		try {
			return TaglibTempletDAO.getInstance().gets(tagId);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;

	}
}
