package com.renren.renrengame.services;

import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class PlayerAccessRecord {
      
	private static PlayerAccessRecord par = new  PlayerAccessRecord();
       
    private static Set<Integer> playerId = new HashSet<Integer>();
    
    private ReentrantReadWriteLock rwLock = new ReentrantReadWriteLock();
    
    private Lock rLock = rwLock.readLock();
    
    private Lock wLock = rwLock.writeLock();
	
	private PlayerAccessRecord() {}
      
    public static PlayerAccessRecord getInstance() {
    	return par;
    }
    
    public boolean viseted(int id) {
    	rLock.lock();
    	try {
    	    if (playerId.contains(id)) {
    		    return true;
    	    }
    	} finally {
    		rLock.unlock();
    	}
    	add(id);
    	return false;
    }
    
    public void add(int id) {
    	wLock.lock();
    	try {
    	    playerId.add(id);
    	} finally {
    		wLock.unlock();
    	}
    }
    
    public void clear() {
    	wLock.lock();
    	try {
    	    playerId.clear();
    	} finally {
    		wLock.unlock();
    	}
    }
    
}
