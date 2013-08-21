/**
 * 
 */
package com.xiaonei.platform.component.space.model;

import java.io.Serializable;

import com.xiaonei.platform.core.opt.permission.source.Status;

/**
 * PermissionBatchModel <br>
 * 
 * @author tai.wang@opi-corp.com Aug 4, 2010 - 3:05:31 PM
 */
public class PermissionBatchModel implements Serializable {

    private static final long serialVersionUID = -4125037877357825708L;

    /** 被访问者id */
    private int targetId;

    /** 被访问者 */
    private Status target;

    /** 被访问者 */
    private int basicConfig;

    private String targetProfilePrivacy;

    public void setTarget(int userId, int stage, int net, int auth, int status, int control) {
        if (null == target) {
            this.target = new Status(userId, userId, stage, net, auth, status, control);
        }
    }

    public int getTargetId() {
        return targetId;
    }

    public void setTargetId(int targetId) {
        this.targetId = targetId;
    }

    public int getBasicConfig() {
        return basicConfig;
    }

    public void setBasicConfig(int basicConfig) {
        this.basicConfig = basicConfig;
    }

    public String getTargetProfilePrivacy() {
        return targetProfilePrivacy;
    }

    public void setTargetProfilePrivacy(String targetProfilePrivacy) {
        this.targetProfilePrivacy = targetProfilePrivacy;
    }

    public Status getTarget() {
        return target;
    }

}
