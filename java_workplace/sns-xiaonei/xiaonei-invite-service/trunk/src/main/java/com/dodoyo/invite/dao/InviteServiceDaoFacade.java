package com.dodoyo.invite.dao;

import org.springframework.beans.factory.annotation.Autowired;

import com.dodoyo.invite.httpservice.dao.JadeClientIPDAO;
import com.dodoyo.invite.httpservice.dao.JadeTokenDAO;
import com.xiaonei.platform.core.opt.OpiConstants;


public class InviteServiceDaoFacade 
{
    private static final String RENREN = "renren.com";
    
    @Autowired
    private JadeKXEmailDAO kxEmailDao;
    
    @Autowired
    private JadeRREmailDAO rrEmailDao;
    
    @Autowired
    private JadeKXInviteDAO kxInviteDao;
    
    @Autowired
    private JadeRRInviteDAO rrInviteDao;
    
    @Autowired
    private JadeKXInviteUrlParasDAO kxInviteUrlParasDao;
    
    @Autowired
    private JadeRRInviteUrlParasDAO rrInviteUrlParasDao;
    
    @Autowired
    private JadeKXInviteMailStatDAO kxInviteMailStatDao;
    
    @Autowired
    private JadeRRInviteMailStatDAO rrInviteMailStatDao;
    
    @Autowired
    private JadeKXSequenceDAO kxSequenceDao;
    
    @Autowired
    private JadeRRSequenceDAO rrSequenceDao;
    
    @Autowired
    private JadeUserInviteCodeViewDAO userInviteCodeViewDao;
    
    @Autowired
    private JadeUserPersonalInfoDAO userPeronalInfodao;
    
    @Autowired
    private JadeClientIPDAO clientIPDao;
    
    @Autowired
    private JadeTokenDAO tokenDao;
    
    public JadeInviteDAO getInviteDAO()
    {
        if (OpiConstants.getCurrentDomain().toString().equals(RENREN)) {
            return rrInviteDao;
        }
        return kxInviteDao;
    }
    
    public JadeEmailDAO getEmailDAO()
    {
        if (OpiConstants.getCurrentDomain().toString().equals(RENREN)) {
            return rrEmailDao;
        }
        return kxEmailDao;
    }
    
    public JadeInviteUrlParasDAO getInviteUrlParasDAO()
    {
        if (OpiConstants.getCurrentDomain().toString().equals(RENREN)) {
            return rrInviteUrlParasDao;
        }
        return kxInviteUrlParasDao;
    }
    
    public JadeInviteMailStatDAO getInviteMailStatDAO()
    {
        if (OpiConstants.getCurrentDomain().toString().equals(RENREN)) {
            return rrInviteMailStatDao;
        }
        return kxInviteMailStatDao;
    }
    
    public JadeSequenceDAO getSequenceDAO()
    {
        if (OpiConstants.getCurrentDomain().toString().equals(RENREN)) {
            return rrSequenceDao;
        }
        return kxSequenceDao;
    }
    
    public JadeUserPersonalInfoDAO getUserPersonalInfoDAO()
    {
        return userPeronalInfodao;
    }
    
    public JadeUserInviteCodeViewDAO getUserInviteCodeViewDAO()
    {
        return userInviteCodeViewDao;
    }
    
    public JadeClientIPDAO getClientIPDAO()
    {
        return clientIPDao;
    }
    
    public JadeTokenDAO getTokenDAO()
    {
        return tokenDao;
    }
}
