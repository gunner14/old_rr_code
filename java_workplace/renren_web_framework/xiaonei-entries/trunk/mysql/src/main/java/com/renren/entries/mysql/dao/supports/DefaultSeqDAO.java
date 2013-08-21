package com.renren.entries.mysql.dao.supports;

import java.util.Random;

import com.renren.entries.mysql.dao.SeqDAO;

public class DefaultSeqDAO implements SeqDAO {

    int start = new Random().nextInt();

    @Override
    public int nextSequnenceSuffix() {
        return Math.abs(start++);
    }
}
