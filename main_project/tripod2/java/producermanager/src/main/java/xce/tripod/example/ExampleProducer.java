package xce.tripod.example;

import xce.tripod.Producer;
import xce.tripod.example.dao.ExampleDAO;
import xce.tripod.util.JadeDAOUtil;

public class ExampleProducer extends Producer {

    @Override
    public byte[] create(String key, boolean useMaster) {
        ExampleDAO dao = JadeDAOUtil.getJadeDAO(ExampleDAO.class);
        System.out.println(dao.toString());
        String value = dao.select(Integer.parseInt(key));
        return value.getBytes();
        // return new Date().toGMTString();
    }

}
