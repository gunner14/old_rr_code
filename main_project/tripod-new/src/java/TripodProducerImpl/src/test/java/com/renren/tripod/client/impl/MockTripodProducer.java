package com.renren.tripod.client.impl;

import java.util.HashMap;
import java.util.Map;

import com.renren.tripod.client.model.AddressBookProtos.Person;
import com.renren.tripod.impl.AbstractTripodProducer;


public class MockTripodProducer extends AbstractTripodProducer {

    @Override
    public String[] createKeys(String beginKey, int limit) {
        return null;
    }

    @Override
    public String[] createHotKeys(String beginKey, int limit) {
        return null;
    }

    @Override
    public Map<String, Object> createObjects(String[] keys, boolean isMaster) {
        Map<String, Object> ret = new HashMap<String, Object>();
        for (String key : keys) {
            ret.put(key, getPerson(key));
        }
        return ret;
    }
    
    private Person getPerson(String name) {
        
        Person john =
            Person.newBuilder()
              .setId(1234)
              .setName(name)
              .setEmail("jdoe@example.com")
              .addPhone(
                Person.PhoneNumber.newBuilder()
                  .setNumber("555-4321")
                  .setType(Person.PhoneType.HOME))
              .build();
        return john;
    }
    
}
