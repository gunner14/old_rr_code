package com.xiaonei.xce.userstate;

import com.renren.tripod.proto.UserBaseProto.PbUserState;

import mop.hi.oce.domain.Updatable;
import xce.userbase.CStateCHARITYINVITED;
import xce.userbase.CStateCHARITY;
import xce.userbase.CStateEVERVIPMEMBER;
import xce.userbase.CStateGUIDEDONE;
import xce.userbase.CStateMOBILEBIND;
import xce.userbase.CStateSocialWiki;
import xce.userbase.CStateSocialWikiHome;
import xce.userbase.CStateTimeline;
import xce.userbase.CStateTimelineGuide;
import xce.userbase.CStateVEST;
import xce.userbase.CStateDisneyVIP;
import xce.userbase.CStateMSNIMME;
import xce.userbase.CStateCOUPLESSPACE;
import xce.userbase.CStatePAGEMANAGER;
import xce.userbase.CStateSELECTED;
import xce.userbase.CStateKEEPUSE;
import xce.userbase.CStateSOCIALGRAPHGOOD;
import xce.userbase.CStateVIPMEMBER;
import xce.userbase.CStateAPPLYSPAMMER;
import xce.userbase.CStateGAMEMANAGER;
import xce.userbase.CStateHIDEADS;
import xce.userbase.CStateDOUBLEACCOUNT;
import xce.userbase.CStateSAMEPASSWORD;
import xce.userbase.CStateEXPERIMENT;
import xce.userbase.CStateJUMPLOGIC;
import xce.userbase.CStateLUNARBIRTH;
import xce.userbase.CStateGIFTMEMBER;
import xce.userbase.CStateMOBILEMEMBER;
import xce.userbase.CStateINWHITELIST;
import xce.userbase.CStateZOMBIE;
import xce.userbase.CStateGROUPMEMBER;
import xce.userbase.CStateCHANNELSPOKER;
import xce.userbase.CStateV6;
import xce.userbase.CStateV6GUIDE;
import xce.userbase.CStateMPSMEMBER;
import xce.userbase.CUSID;
import xce.userbase.CUSLEVEL;
import xce.userbase.CUSSTAR;
import xce.userbase.UserStateDataN;

public class UserStateInfo extends Updatable {
	private static final long serialVersionUID = 4616046340069748706L;
	private int id;
	private long state;
	private int star;
	private int level;

	/**
	 * state升级，请用getStateLong替代getState
	 */
	@Deprecated
	public int getState() {
		return (int)state;
	}

    public long getStateLong() {
        return state;
    }

	public boolean isSelected() {
		return 0 != (state & (1L << CStateSELECTED.value));
	}

	public boolean isVipMember() {
		return 0 != (state & (1L << CStateVIPMEMBER.value));
	}

	public boolean isMSNIMME() {
		return 0 != (state & (1L << CStateMSNIMME.value));
	}

	public boolean isMobileBind() {
		return 0 != (state & (1L << CStateMOBILEBIND.value));
	}
	
	public boolean isEverVipMember() {
		return 0 != (state & (1L << CStateEVERVIPMEMBER.value));
	}

	public boolean isGuideDone() {
		return 0 != (state & (1L << CStateGUIDEDONE.value));
	}
	
	public boolean isSocialGraphGood() {
		return 0 != (state & (1L << CStateSOCIALGRAPHGOOD.value));
	}
	
	public boolean isKeepUse() {
		return 0 != (state & (1L << CStateKEEPUSE.value));
	}
	
	public boolean isCharity() {
		return 0 != (state & (1L << CStateCHARITY.value));
	}

	public boolean isCharityInvited() {
		return 0 != (state & (1L << CStateCHARITYINVITED.value));
	}
	
	public boolean isDoubleAccount() {
	    return 0 != (state & (1L << CStateDOUBLEACCOUNT.value));
	}

	public boolean isInWhiteList() {
	    return 0 != (state & (1L << CStateINWHITELIST.value));
	}
	
  public boolean isGroupMember() {
	    return 0 != (state & (1L << CStateGROUPMEMBER.value));
	}
  
  public boolean isChannelSpoker() {
	    return 0 != (state & (1L << CStateCHANNELSPOKER.value));
	}

	public boolean isHideAds() {
	    return 0 != (state & (1L << CStateHIDEADS.value));
	}

	public boolean isVest() {
	    return 0 != (state & (1L << CStateVEST.value));
	}
	
	public boolean isDisneyVIP() {
	    return 0 != (state & (1L << CStateDisneyVIP.value));
	}
  
	public boolean isGameManager() {
	    return 0 != (state & (1L << CStateGAMEMANAGER.value));
	}

	public boolean isSamePassword() {
	    return 0 != (state & (1L << CStateSAMEPASSWORD.value));
	}

	public boolean isCouplesSpace() {
	    return 0 != (state & (1L << CStateCOUPLESSPACE.value));
	}

	public boolean isPageManager() {
	    return 0 != (state & (1L << CStatePAGEMANAGER.value));
	}
	
	public boolean isExperiment() {
	    return 0 != (state & (1L << CStateEXPERIMENT.value));
	}

	public boolean isJumpLogic() {
	    return 0 != (state & (1L << CStateJUMPLOGIC.value));
	}
	
  public boolean isLunarBirth() {
	    return 0 != (state & (1L << CStateLUNARBIRTH.value));
	}

	public boolean isApplySpammer() {
	    return 0 != (state & (1L << CStateAPPLYSPAMMER.value));
	}
  
	public boolean isGiftMember() {
	    return 0 != (state & (1L << CStateGIFTMEMBER.value));
	}
	
	public boolean isMobileMember() {
	    return 0 != (state & (1L << CStateMOBILEMEMBER.value));
	}
  
	public boolean isZombie() {
	    return 0 != (state & (1L << CStateZOMBIE.value));
	}
	
  public boolean isV6() {
	    return 0 != (state & (1L << CStateV6.value));
	}
  
  public boolean isV6Guide() {
	    return 0 != (state & (1L << CStateV6GUIDE.value));
	}
  
    public boolean isMpsMember() {
      return 0 != (state & (1L << CStateMPSMEMBER.value));
    }
    
    public boolean isTimelineOn() {
        return 0 != (state & (1L << CStateTimeline.value));
      }
    
    public boolean isTimelineGuideOn() {
        return 0 != (state & (1L << CStateTimelineGuide.value));
      }

    public boolean isSocialWikiOn() {
        return 0 != (state & (1L << CStateSocialWiki.value));
    }
    
    public boolean isSocialWikiHomeOn() {
        return 0 != (state & (1L << CStateSocialWikiHome.value));
    }
	
	public int getStar() {
		return star;
	}

	public void setStar(int star) {
		this.star = star;
		getUpdates().put(CUSSTAR.value, String.valueOf(star));
	}

	public int getLevel() {
		return level;
	}

	public void setLevel(int level) {
		this.level = level;
		getUpdates().put(CUSLEVEL.value, String.valueOf(level));
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
		getUpdates().put(CUSID.value, String.valueOf(id));
	}

	public UserStateInfo parse(UserStateDataN source) {
		id = source.id;
		state = source.state;
		star = source.star;
		level = source.level;

		return this;
	}
	
	public UserStateInfo parse(PbUserState source) {
		id = (int)source.getId();
		state = source.getState();
		star = source.getStar();
		level = source.getLevel();
		return this;
	}

	@Override
	public String toString() {
		return "[UserState:id=" + id + ";state=" + state + ";star=" + star
				+ ";level=" + level + "]";
	}

}
