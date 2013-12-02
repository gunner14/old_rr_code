/**
 * Class RelationPutTransformer
 * 
 * By default, Sqoop use a ToStringTransformer, which insert all the data with STRING format. To
 * convert them into BYTES and be compatible with the known format, we use this class.
 * 
 * Usage: Compile the file with compile.sh and run with following args: 
 *   -libjars RelationPutTransformer.jar 
 *   -D sqoop.hbase.insert.put.transformer.class = RelationPutTransformer
 */


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
import com.cloudera.sqoop.lib.BlobRef;

public class FriendRankPutTransformer extends PutTransformer {

  public static final Log LOG = LogFactory.getLog(FriendRankPutTransformer.class.getName());

  public FriendRankPutTransformer() {

  }

  @Override
  /** {@inheritDoc} */
  public List<Put> getPutCommand(Map<String, Object> fields) throws IOException {

    String rowKeyCol = getRowKeyColumn();
    String colFamily = getColumnFamily();
    byte[] colFamilyBytes = Bytes.toBytes(colFamily);

    Long rowKey = (Long)fields.get("user_id");
    BlobRef rankBlob = (BlobRef)fields.get("rank");
    
    if (rowKey == null) {
      // If the row-key column is null, we don't insert this row.
      LOG.warn("Could not insert row with null value for row-key column: " + rowKeyCol);
      return null;
    }
    
    Put put = new Put(Bytes.toBytes(String.valueOf(rowKey)));
    put.add(colFamilyBytes, Bytes.toBytes("rank"), rankBlob.getData());
    return Collections.singletonList(put);
  }

}
