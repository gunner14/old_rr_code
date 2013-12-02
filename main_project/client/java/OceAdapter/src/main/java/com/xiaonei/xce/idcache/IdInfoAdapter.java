package com.xiaonei.xce.idcache;

import idcache.IdRelationInfo;

public class IdInfoAdapter {
	
	private IdInfoReaderAdapter _readerAdapter = null;
	private IdInfoWriterAdapter _writerAdapter = null;
	private IdFilter _idFilter = null;
	private static IdInfoAdapter _instance = null;

	//注意：原有提供接口，开发者不可改为私有方法
	@Deprecated
	public IdInfoAdapter() {
		_idFilter = new IdFilter();
		_readerAdapter = new IdInfoReaderAdapter();
		_writerAdapter = new IdInfoWriterAdapter();
	}

    //调用者应使用getInstance接口作为入口
    public static IdInfoAdapter getInstance() {
        if (IdInfoAdapter._instance == null) {
            synchronized (IdInfoAdapter.class) {
                if (IdInfoAdapter._instance == null) {
                    IdInfoAdapter._instance = new IdInfoAdapter();
                }
            }
        }
        return IdInfoAdapter._instance;
    }

	/**
	 * 判断一个ID是否可能是虚拟ID
	 * 本地方法，过滤大多数非虚拟ID
	 * @param id
	 * @return 是否可能是虚拟ID
	 */
	public boolean isMaybeVirtualId(int id){
		return _idFilter.isMaybeVirtualId(id);
	}

	/**
	 * 判断一个ID是否虚拟ID
	 * 先调用本地方法过滤，如果可能是虚拟ID再RPC调用服务判定
	 * @param id
	 * @return 是否是虚拟ID
	 */
	public boolean isVirtualId(int id){
		if(!_idFilter.isMaybeVirtualId(id)){
			return false;
		}
		return _readerAdapter.isVirtualId(id);
	}

	/**
	 * 由虚拟ID得到管理员ID
	 * @param virtualId 虚拟ID
	 * @return 管理员ID
	 */
	public int getRelatedUserId(int virtualId){
		if(!_idFilter.isMaybeVirtualId(virtualId)){
			return virtualId;
		}
		return _readerAdapter.getRelatedUserId(virtualId);
	}
	
	/**
	 * 由虚拟ID获取公共主页ID
	 * @param virtualId 虚拟ID
	 * @return 公共主页ID
	 */
	public int getRelatedTransId(int virtualId){
		if(!_idFilter.isMaybeVirtualId(virtualId)){
			return virtualId;
		}
		return _readerAdapter.getRelatedTransId(virtualId);
	}

	/**
	 * 获取ID组合，对大多数普通ID会先本地过滤
	 * @param id 普通ID或者虚拟ID
	 * @return ID组合结构
	 */
	public VirtualIdInfoC getVirtualIdInfo(int id){
		return _readerAdapter.getVirtualIdInfo(id);
	}

    /**
     * 获取相关的虚拟ID（包括双帐号）
     * @param id 普通ID或者虚拟ID
     * @return 用户ID/双帐号ID与其管理的虚拟ID序列的map
     */
    public IdRelationInfo getIdRelationInfo(int id) {
        return _readerAdapter.getIdRelationInfo(id);
    }

	/**
	 * 创建公共主页管理员
	 * @param id
	 * @param transId
	 * @param virtualId
	 */
	public void createIdRelation(int id, int transId, int virtualId)
	{
		if(!_idFilter.isMaybeVirtualId(virtualId)){
			return;
		}
		_writerAdapter.createIdRelation(id, transId, virtualId);
	}
	
	/**
	 * 删除公共主页管理员
	 * @param id
	 * @param virtualId
	 */
	public void deleteIdRelation(int id, int virtualId)
	{
		if(!_idFilter.isMaybeVirtualId(virtualId)){
			return;
		}
		_writerAdapter.deleteIdRelation(id, virtualId);
	}
	
	public static void main(String[] args){
		IdInfoAdapter adapter = IdInfoAdapter.getInstance();
	    int vid = 2000000006; //正常虚拟id
//		int vid = 2000000004; 非法虚拟id
//		int vid = 35178; //普通id
//		int vid = 2000007472; //特殊普通id
		
		System.out.println(adapter.isVirtualId(vid));
		System.out.println(adapter.getRelatedTransId(vid));
		System.out.println(adapter.getRelatedUserId(vid));
		System.out.println(adapter.getVirtualIdInfo(vid));
		
		int id = 6813;
		int transId = id +1;
		int virtualId = 1 + 2000000000;
		
		if(adapter.isMaybeVirtualId(0)){
			System.out.println("maybe failed! 0");
		}
		
		if(adapter.isMaybeVirtualId(2000000000)){
			System.out.println("maybe failed! 2000000000");
		}
		
		if(!adapter.isMaybeVirtualId(2000000001)){
			System.out.println("maybe failed! 2000000001");
		}
		
		if(adapter.isMaybeVirtualId(2100000000)){
			System.out.println("maybe failed! 2100000000");
		}
		
		adapter.createIdRelation(id, transId, virtualId);
		
		if(adapter.getRelatedTransId(virtualId) != transId){
			System.out.println("create failed! transId");
		}
		
		if(adapter.getRelatedUserId(virtualId) != id){
			System.out.println("create failed! userId");
		}
		
		if(!adapter.isVirtualId(virtualId)){
			System.out.println("create failed! virtualId");
		}
	
		adapter.deleteIdRelation(id, virtualId);
		
		
		if(adapter.getRelatedTransId(virtualId) != -1){
			System.out.println("delete failed! transId");
		}
		
		if(adapter.getRelatedUserId(virtualId) != -1){
			System.out.println("delete failed! userId");
		}
		
		IdInfoReaderAdapter reader = new IdInfoReaderAdapter();
		IdRelationInfo data = reader.getIdRelationInfo(id);
		
		System.out.println(data.relatedIds.get(6813).length);
		System.out.println(data.relatedIds.get(6814).length);
		
		System.exit(0);
	}

}
