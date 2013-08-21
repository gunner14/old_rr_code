package com.xiaonei.sns.platform.core.opt.ice;

import java.util.List;
import java.util.Map;


/**
 * 好友分组
 * @author Michael(liyong@opi-corp.com)
 *
 */
public interface IBuddyGroupAdapter extends IRegisterable {

    void addBuddyToGroup(int host, String groupName, int buddy);

    void addBuddiesToGroup(int host, String groupName, int[] buddies);

    void changeBuddyGroups(int host, int buddy, String[] nameSeq);

    void removeBuddyFromGroup(int host, String groupName, int buddy);

    void addGroup(int host, String groupName);

    void removeGroup(int host, String groupName);

    void renameGroup(int host, String oldName, String newName);

    void cleanGroup(int host, String groupName);

    List<String> getGroupList(int host);

    List<Integer> getBuddyList(int host, String groupName, int begin, int limit);

    int getCountByGroup(int host, String groupName);

    List<String> getBuddyGroups(int host, int buddy);

    Map<String, Integer> getAllGroupsBuddyCount(int host);

    Map<Integer, List<String>> getBuddiedGroupsMap(int host);

}
