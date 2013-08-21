package com.xiaonei.tools.ice.client.ice;

public final class Identity implements java.lang.Cloneable,
		java.io.Serializable {
	public String name;

	public String category;

	public Identity() {
	}

	public Identity(String name, String category) {
		this.name = name;
		this.category = category;
	}

	public boolean equals(java.lang.Object rhs) {
		if (this == rhs) {
			return true;
		}
		Identity _r = null;
		try {
			_r = (Identity) rhs;
		} catch (ClassCastException ex) {
		}

		if (_r != null) {
			if (name != _r.name && name != null && !name.equals(_r.name)) {
				return false;
			}
			if (category != _r.category && category != null
					&& !category.equals(_r.category)) {
				return false;
			}

			return true;
		}

		return false;
	}

	public int hashCode() {
		int __h = 0;
		if (name != null) {
			__h = 5 * __h + name.hashCode();
		}
		if (category != null) {
			__h = 5 * __h + category.hashCode();
		}
		return __h;
	}

	public java.lang.Object clone() {
		java.lang.Object o = null;
		try {
			o = super.clone();
		} catch (CloneNotSupportedException ex) {
			assert false; // impossible
		}
		return o;
	}

	public void __write(BasicStream __os) {
		__os.writeString(name);
		__os.writeString(category);
	}

	public void __read(BasicStream __is) {
		name = __is.readString();
		category = __is.readString();
	}
}
