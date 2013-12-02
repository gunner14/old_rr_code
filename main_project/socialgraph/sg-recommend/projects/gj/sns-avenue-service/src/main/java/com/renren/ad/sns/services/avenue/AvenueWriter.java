package com.renren.ad.sns.services.avenue;

import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 *
 * @author   xiong.hua
 */
public class AvenueWriter {
    private static final Log LOG = LogFactory.getLog(AvenueWriter.class);

    public void writeIndexToFile(String path, AvenueIndex index)
                    throws IOException {
            LOG.debug("serializing index ...");
            final int BUF_SIZE = 10 * 1024 * 1024;
            FileOutputStream fos = new FileOutputStream(path);
            DataOutputStream dos = new DataOutputStream(new BufferedOutputStream(
                            fos, BUF_SIZE));
            dos.writeUTF(index.getClass().getName());
            index.write(dos);
            dos.flush();
            dos.close();
            fos.close();
            LOG.debug("writing finished");
    }

    public AvenueIndex readIndexFromFile(String path) throws IOException,
                    InstantiationException, IllegalAccessException,
                    ClassNotFoundException {
            FileInputStream fis = new FileInputStream(path);
            DataInputStream in = new DataInputStream(fis);
            String clazzName = in.readUTF();
            AvenueIndex index = (AvenueIndex) Class.forName(clazzName)
                            .newInstance();
            index.readFields(in);
            in.close();
            fis.close();
            return index;
    }
}

