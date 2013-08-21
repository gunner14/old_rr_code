/**
 *
 */
package com.xiaonei.reg.guide.form;

import org.apache.commons.lang.builder.ToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;
import org.apache.struts.action.ActionForm;

import com.xiaonei.websecurity.adapter.SafeFilter;

/**
 * MobileStageFillinfoForm
 *
 * @author wang-tai(tai.wang@opi-corp.com) 2009-2-18 - 下午04:04:23
 */
public class MobileStageFillinfoForm extends ActionForm {

    private static final long serialVersionUID = -4825511161261237354L;

    private int stage;
    private String name;

    public int getStage() {
        return stage;
    }

    public void setStage(int stage) {
        this.stage = stage;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = SafeFilter.getInstance().getPureContent(name);
    }

    /**
     * @see java.lang.Object#toString()
     */
    public String toString() {
        return new ToStringBuilder(this, ToStringStyle.MULTI_LINE_STYLE).appendSuper(super.toString()).append(
                "multipartRequestHandler", this.multipartRequestHandler).append("name", this.name).append("stage",
                this.stage).append("servlet", this.servlet).toString();
    }

}
