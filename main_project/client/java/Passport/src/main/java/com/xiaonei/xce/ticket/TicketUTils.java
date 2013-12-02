package com.xiaonei.xce.ticket;

import java.util.BitSet;

public final class TicketUTils {
	private final static int LONG_TICKET_LEN = 30;
	private final static int SHORT_TICKET_LEN = 20;
	private final static int RAW_TICKET_LEN = 15;
	private final static int CHAR_LEN = 128;

	// 字典表，用于保存票的编码字符
	private final static String _lDict = "0123456789abcdef";
	private final static String _sDict = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";

	// 辅助表，用于快速判断字符是否存在字典表中
	private final static BitSet _lMask;
	private final static BitSet _sMask;

	// 索引表，用于快速查找合法字符的下标，不合法的字符下标值为0，但实际不会被访问到
	private final static int _lIndex[];
	private final static int _sIndex[];

	static {
		_lMask = new BitSet(CHAR_LEN);
		_sMask = new BitSet(CHAR_LEN);
		_lIndex = new int[CHAR_LEN];
		_sIndex = new int[CHAR_LEN];

		for (int i = 0; i < _lDict.length(); i++) {
			_lMask.set(_lDict.charAt(i));
			_lIndex[_lDict.charAt(i)] = i;
		}

		for (int i = 0; i < _sDict.length(); i++) {
			_sMask.set(_sDict.charAt(i));
			_sIndex[_sDict.charAt(i)] = i;
		}
	}

	public final static class TicketUtil {
		public static String sTicket2LTicket(final String sTicket) {
			char raw[] = new char[RAW_TICKET_LEN];
			char buffer[] = new char[LONG_TICKET_LEN];

			// 从sDict字符转成RAW
			// 每个sDict字符用6b表示，4个lDict字符转成3个RAW字节
			for (int i = 0, j = 0; i < SHORT_TICKET_LEN; i += 4, j += 3) {
				char b0 = (char) (_sIndex[(int) sTicket.charAt(i)]);
				char b1 = (char) (_sIndex[(int) sTicket.charAt(i + 1)]);
				char b2 = (char) (_sIndex[(int) sTicket.charAt(i + 2)]);
				char b3 = (char) (_sIndex[(int) sTicket.charAt(i + 3)]);

				raw[j] = (char) ((b0 << 2) | (b1 >> 4));
				raw[j + 1] = (char) ((b1 << 4) | (b2 >> 2));
				raw[j + 2] = (char) ((b2 << 6) | b3);
			}

			// 从RAW转成lDict字符
			// 每个lDict字符用4b表示，每个RAW字节转成2个lDict字符
			int index;
			for (int i = 0, j = 0; i < LONG_TICKET_LEN; i += 2, j++) {
				// 前4位
				index = (raw[j] >> 4 & 0x0F);
				buffer[i] = _lDict.charAt(index);
				// 后4位
				index = (raw[j] & 0x0F);
				buffer[i + 1] = _lDict.charAt(index);
			}

			return new String(buffer);
		}

		public static String lTicket2STicket(final String lTicket) {
			StringBuffer result = new StringBuffer();

			return result.toString();
		}

		public static int getIdFromLTicket(final String lTicket) {
			int result = 0;
			for (int i = 0; i < 8; i++) {
				result |= ((_lIndex[lTicket.charAt(3 * (i + 1))] ^ i) & 0xf) << (4 * i);
			}
			return result;
		}

		public static int getTypeFromLTicket(final String lTicket) {
			int result = 0;
			result |= _lIndex[lTicket.charAt(0)] << 4;
			result |= _lIndex[lTicket.charAt(27)];
			return result;
		}

		public static int getId(final String sTicket) {
			String lTicket = sTicket2LTicket(sTicket);
			return getIdFromLTicket(lTicket);
		}

		public static int getType(final String sTicket) {
			String lTicket = sTicket2LTicket(sTicket);
			return getTypeFromLTicket(lTicket);
		}

		public static boolean isCheckSumValid(final String lTicket) {
			boolean result = false;

			String sumOfTicket = lTicket.substring(28, 30);
			char c[] = { 0, 0 };
			for (int i = 0; i < LONG_TICKET_LEN - 2; i += 2) {
				c[0] ^= _lIndex[lTicket.charAt(i)];
				c[1] ^= _lIndex[lTicket.charAt(i + 1)];
			}
			c[0] = _lDict.charAt(c[0]);
			c[1] = _lDict.charAt(c[1]);
			String sumOfCalc = String.valueOf(c);
			result = sumOfTicket.equals(sumOfCalc);

			return result;
		}

		public static boolean isLStringValid(final String lTicket) {
			boolean result = true;
			if (lTicket.length() == LONG_TICKET_LEN) {
				for (int i = 0; i < LONG_TICKET_LEN; i++) {
					result &= _lMask.get(lTicket.charAt(i));
				}
			} else {
				result = false;
			}
			return result;
		}
		
		public static boolean isSStringValid(final String sTicket) {
			boolean result = true;
			if (sTicket.length() == SHORT_TICKET_LEN) {
				for (int i = 0; i < SHORT_TICKET_LEN; i++) {
					result &= _sMask.get(sTicket.charAt(i));
				}
			} else {
				result = false;
			}
			return result;
		}

		public static boolean isLTicketValid(final String lTicket) {
			return (isLStringValid(lTicket) && isCheckSumValid(lTicket));
		}
		
		public static boolean isSTicketValid(final String sTicket) {
			return (isSStringValid(sTicket) && isCheckSumValid(sTicket2LTicket(sTicket)));
		}
	}

	public static void main(String[] args) {
		String lTicket = "12345abcde67890abcde12345abcde";
		String sTicket = "1234567890ABCDEhijkl";
		// String sTicket = "24Xf8cJqRaW86VFyUVnV";
		//
		// System.out.println("\nlTicket2STicket");
		// System.out.println(lTicket);
		// System.out.println(TicketUtil.lTicket2STicket(lTicket));

		System.out.println("\nsTicket2LTicket");
		System.out.println(sTicket);
		System.out.println(TicketUtil.sTicket2LTicket(sTicket));
		//
		// System.out.println("\nsTicket2sTicket");
		// System.out.println(sTicket);
		// System.out.println(TicketUtil.lTicket2STicket(TicketUtil
		// .sTicket2LTicket(sTicket)));
		//
		// System.out.println("\nlTicket2lTicket");
		// System.out.println(sTicket);
		// System.out.println(TicketUtil.sTicket2LTicket(TicketUtil
		// .lTicket2STicket(lTicket)));
		//
		sTicket = "0MWlBYIgQSXD5ZCfTnnj";
		System.out.println("\ngetType");
		System.out.println(sTicket);
		System.out.println(TicketUtil.sTicket2LTicket(sTicket));
		System.out.println(TicketUtil.getType(sTicket));

		sTicket = "0MWlBYIgQSXD5ZCfTnnj";
		System.out.println("\ngetId");
		System.out.println(sTicket);
		System.out.println(TicketUtil.sTicket2LTicket(sTicket));
		System.out.println(TicketUtil.getId(sTicket));
		// 54C5638
		//
		lTicket = TicketUtil.sTicket2LTicket(sTicket);
		System.out.println(lTicket);
		System.out.println(TicketUtil.isCheckSumValid(lTicket));
		System.out.println(TicketUtil.isLTicketValid(lTicket));

		System.exit(0);
	}
}