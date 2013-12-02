

import java.io.IOException;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.util.Bytes;

import com.cloudera.sqoop.hbase.PutTransformer;

/**
 * Class UserTimePutTransformer
 * 
 * By default, Sqoop use a ToStringTransformer, which insert all the data with STRING format. To
 * convert them into BYTES and be compatible with the known format, we use this class.
 * 
 * Additionally, we HASH the key here, using ELF_HASH. The key is compose of two parts: 
 *   [ELF_HASH, 4byte][host, 4 byte] 
 * To get the original host key, simply & 0x7FFFFFFF.
 * 
 * Usage: Compile the file with compile.sh and run with following args: 
 *   -libjars UserTimePutTransformer.jar 
 *   -D sqoop.hbase.insert.put.transformer.class = UserTimePutTransformer
 */

public class UserTimePutTransformer extends PutTransformer {

  public static final Log LOG = LogFactory.getLog(UserTimePutTransformer.class.getName());

  public UserTimePutTransformer() {

  }

  @Override
  /** {@inheritDoc} */
  public List<Put> getPutCommand(Map<String, Object> fields) throws IOException {

    String rowKeyCol = getRowKeyColumn();
    String colFamily = getColumnFamily();
    byte[] colFamilyBytes = Bytes.toBytes(colFamily);

    Integer rowKey = (Integer)fields.get("id");

    Integer rtime = 0;
    Object registerTime = fields.get("registertime");
    if (registerTime != null)
        rtime = (int)(((java.sql.Timestamp)registerTime).getTime() / 1000);

    Integer ltime = 0;
    Object loginTime = fields.get("lastlogintime");
    if (loginTime != null)
        ltime = (int)(((java.sql.Timestamp)loginTime).getTime() / 1000);
    
    if (rowKey == null || rtime == null || ltime == null) {
      // If the row-key column is null, we don't insert this row.
      LOG.warn("Could not insert row with null value for row-key column: " + rowKeyCol);
      return null;
    }
    
    Put put = new Put(Bytes.toBytes(String.valueOf(rowKey)));
    put.add(colFamilyBytes, Bytes.toBytes("r"), Bytes.toBytes(rtime));
    put.add(colFamilyBytes, Bytes.toBytes("l"), Bytes.toBytes(ltime));
    return Collections.singletonList(put);
  }

}
