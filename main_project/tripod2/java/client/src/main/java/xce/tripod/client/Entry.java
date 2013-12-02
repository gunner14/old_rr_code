package xce.tripod.client;

/**
 * namespace和bussiness容器
 * 
 * @author michael
 * @email liyong@renren-inc.com
 * 
 */
public class Entry {

    private String namespaceId;

    private String businessId;

    Entry(String namespaceId, String businessId) {
        this.namespaceId = namespaceId;
        this.businessId = businessId;
        if (this.namespaceId == null) {
            this.namespaceId = "";
        }
        if (this.businessId == null) {
            this.businessId = "";
        }
    }

    public String getNamespaceId() {
        return namespaceId;
    }

    public void setNamespaceId(String namespaceId) {
        this.namespaceId = namespaceId;
    }

    public String getBusinessId() {
        return businessId;
    }

    public void setBusinessId(String businessId) {
        this.businessId = businessId;
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Entry)) {
            return false;
        }
        Entry e = (Entry) obj;
        return (namespaceId.equals(e.namespaceId) && businessId.equals(e.businessId));
    }

    @Override
    public int hashCode() {
        int result = 17;
        result += namespaceId.hashCode() * 37;
        result += businessId.hashCode() * 37;
        return result;
    }
}
