package com.xiaonei.reg.guide.flows.oldactionsutil;

import java.util.HashMap;
import java.util.Map;

/**
 * ActionMapping.java
 * @author inshion(xin.yin@opi-corp.com)
 * 
 * 2010-8-27 
 */
public class ActionMapping {

        private Map<String, ActionForward> mapping = new HashMap<String, ActionForward>();

        public ActionMapping(Map<String, ActionForward> mapping) {
            set(mapping);
        }

        public ActionMapping set(Map<String, ActionForward> mapping) {
            if (mapping == null) {
                return this;
            }
            this.mapping.putAll(mapping);
            return this;
        }

        public Map<String, ActionForward> getMap() {
            return mapping;
        }

        public ActionForward findForward(String name) {
            return mapping.get(name);
        }
    }