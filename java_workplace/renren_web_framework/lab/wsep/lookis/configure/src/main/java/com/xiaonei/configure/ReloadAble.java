package com.xiaonei.configure;

/**
 * 
 * 这个接口描述了可能会被重新加载的对象
 * 
 * @author lookis (lookisliu@gmail.com)
 * 
 */
public interface ReloadAble {

    //加载全部配置
    public void reloadAll();

    //针对具体的Key去重新加载
    public void reload(ConfigKey k);
}
