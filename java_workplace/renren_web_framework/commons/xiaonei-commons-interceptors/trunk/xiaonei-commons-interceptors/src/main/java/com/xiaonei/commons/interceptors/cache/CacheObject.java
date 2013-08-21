package com.xiaonei.commons.interceptors.cache;

import java.io.Serializable;
import java.util.Map;

public class CacheObject implements Serializable {

    private static final long serialVersionUID = 5433878876318547882L;

    Object instruction;

    Map<String, Object> models;

    /**
     * @param instruction
     * @param models
     */
    public CacheObject(final Object instruction, final Map<String, Object> models) {
        super();
        this.instruction = instruction;
        this.models = models;
    }

    /**
     * @return the instruction
     */
    public Object getInstruction() {
        return this.instruction;
    }

    /**
     * @return the models
     */
    public Map<String, Object> getModels() {
        return this.models;
    }

    /**
     * @param instruction the instruction to set
     */
    public void setInstruction(final Object instruction) {
        this.instruction = instruction;
    }

    /**
     * @param models the models to set
     */
    public void setModels(final Map<String, Object> models) {
        this.models = models;
    }
}
