package xce.tools;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;

import mop.hi.oce.adapter.impl.BuddyCoreAdapterImpl;
import mop.hi.oce.domain.buddy.CheckedBuddyRelation;
import com.xiaonei.xce.buddyrelationcache.BuddyRelationCacheAdapter;
import com.xiaonei.xce.xcealert.XceAlertAdapter;

class CorrectorQueueProducer extends Thread {

	public void run() {
		produce();
		BuddyRelationCorrecterNew.produceComplete = true;
	}

	private void produce() {
		List<BuddyRelationCorrecterNew.FromTo> result1 = fillAction(
				BuddyRelationCorrecterNew.needToCheck, 1);
		List<BuddyRelationCorrecterNew.FromTo> result2 = new ArrayList<BuddyRelationCorrecterNew.FromTo>();
		try {
			if (result1.size() > 0) {
				Thread.sleep(3 * 60 * 1000);
				result2 = fillAction(result1, 2);
			}
			if (result2.size() > 0) {
				Thread.sleep(6 * 60 * 1000);
				fillAction(result2, 3);
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	private List<BuddyRelationCorrecterNew.FromTo> fillAction(
			List<BuddyRelationCorrecterNew.FromTo> toBeCheck, int try_num) {
		BuddyCoreAdapterImpl core = new BuddyCoreAdapterImpl();
		List<BuddyRelationCorrecterNew.FromTo> result = new ArrayList<BuddyRelationCorrecterNew.FromTo>();
		for (BuddyRelationCorrecterNew.FromTo listEntry : toBeCheck) {
			String write = "";
			try {
				CheckedBuddyRelation corerelation = core
						.getBuddyCoreCheckedRelation(listEntry.from_,
								listEntry.to_);
				CheckedBuddyRelation cacherelation = BuddyRelationCacheAdapter
						.getInstance().getCheckedRelation(listEntry.from_,
								listEntry.to_);
				if (!corerelation.equals(cacherelation)
						|| !BuddyRelationCacheAdapter.getInstance()
								.checkRelation(listEntry.from_, listEntry.to_)) {
					if (corerelation.getSuccess() && cacherelation.getSuccess()) {
						write = "CoreRelation: " + corerelation.toString()
								+ " CacheRelation: " + cacherelation.toString()
								+ "\n";
						BuddyRelationCorrecterNew.fw.write(write);
						BuddyRelationCorrecterNew.correctQueue
								.put(corerelation);
					} else {
						if (try_num == 3) {
							String msg = "";
							if (!corerelation.getSuccess()) {
								msg = corerelation.toString();
							} else {
								msg = cacherelation.toString();
							}
							List<String> subjects = new ArrayList<String>();
							subjects.add("GetCheckedRelationWarning");
							XceAlertAdapter.getInstance().notifyAlertN(
									subjects, msg, 3);
						} else {
							result.add(new BuddyRelationCorrecterNew.FromTo(
									listEntry));
						}
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
				try {
					BuddyRelationCorrecterNew.fw.write("[ERROR]Exception id:"
							+ listEntry.from_ + " " + listEntry.to_);
				} catch (Exception e1) {
					e1.printStackTrace();
				}
				continue;
			}
		}
		return result;
	}
}

class CorrectorQueueConsumer extends Thread {
	public void run() {
		consume();
	}

	private void consume() {
		while (true) {
			if (BuddyRelationCorrecterNew.produceComplete == false) {
				CheckedBuddyRelation entry = null;
				try {
					entry = BuddyRelationCorrecterNew.correctQueue.poll(1,
							TimeUnit.SECONDS);
				} catch (InterruptedException e) {
					e.printStackTrace();
					continue;
				}
				if (entry == null) {
					continue;
				} else {
					try {
						BuddyRelationCacheAdapter.getInstance().setRelation(
								entry.getRelation());
					} catch (Exception e) {
						e.printStackTrace();
						try {
							BuddyRelationCorrecterNew.fw
									.write("[ERROR]Exception id:"
											+ entry.getRelation().fromId + " "
											+ entry.getRelation().toId);
						} catch (IOException e1) {
							e1.printStackTrace();
						}
					}
				}

			} else {
				while (BuddyRelationCorrecterNew.correctQueue.size() > 0) {
					CheckedBuddyRelation entry = BuddyRelationCorrecterNew.correctQueue
							.poll();
					if (entry == null) {
						continue;
					} else {
						try {
							BuddyRelationCacheAdapter.getInstance()
									.setRelation(entry.getRelation());
						} catch (Exception e) {
							e.printStackTrace();
							try {
								BuddyRelationCorrecterNew.fw
										.write("[ERROR]Exception id:"
												+ entry.getRelation().fromId
												+ " "
												+ entry.getRelation().toId);
							} catch (IOException e1) {
								e1.printStackTrace();
							}
						}
					}
				}
				break;
			}
		}
	}
}

public class BuddyRelationCorrecterNew {
	public static class FromTo {
		public int from_;
		public int to_;

		public FromTo(int from, int to) {
			from_ = from;
			to_ = to;
		}

		public FromTo(FromTo rhr) {
			from_ = rhr.from_;
			to_ = rhr.to_;
		}
	}

	public static void printUsage() {
		System.out
				.println("Usage: BuddyRelationCorreceterNew <input file> <outputfile>");
	}

	public static BlockingQueue<CheckedBuddyRelation> correctQueue = new ArrayBlockingQueue<CheckedBuddyRelation>(
			10000);
	public static List<FromTo> needToCheck = new ArrayList<FromTo>();
	public static boolean produceComplete = false;
	public static FileWriter fw = null;

	public static void main(String[] args) {
		if (args.length < 2) {
			printUsage();
			// System.exit(0);
		}
		try {
			BufferedReader br = new BufferedReader(new FileReader(args[0]));
			fw = new FileWriter(args[1]);
			int first = 0;
			int second = 0;
			String read = null;
			while (br.ready()) {
				read = br.readLine();
				String[] r = read.split(" ");
				try {
					first = Integer.parseInt(r[0]);
					second = Integer.parseInt(r[1]);
					needToCheck.add(new FromTo(first, second));
				} catch (Exception e) {
					e.printStackTrace();
					fw.write("[ERROR]File Read Exeption. \n");
					continue;
				}
			}
			List<Thread> threads = new ArrayList<Thread>();
			threads.add(new CorrectorQueueProducer());
			threads.add(new CorrectorQueueConsumer());
			for (Thread thread : threads) {
				thread.start();
			}
			for (Thread thread : threads) {
				try {
					thread.join();
				} catch (InterruptedException e) {
					fw.write("[ERROR]Thread.join is Encounter InterruptedException!! So it turn to the old Correcter!");
					if (produceComplete == false) {
						// 防止死锁
						produceComplete = true;
					}
					e.printStackTrace();
					List<BuddyRelationCorrecterN.FromTo> toBeCheck = new ArrayList<BuddyRelationCorrecterN.FromTo>();
					for (FromTo checkEntry : needToCheck) {
						toBeCheck.add(new BuddyRelationCorrecterN.FromTo(
								checkEntry.from_, checkEntry.to_));
					}
					List<BuddyRelationCorrecterN.FromTo> result = BuddyRelationCorrecterN
							.check(toBeCheck, fw, 1);
					List<BuddyRelationCorrecterN.FromTo> result1 = new ArrayList<BuddyRelationCorrecterN.FromTo>();
					if (result.size() > 0) {
						Thread.sleep(3 * 60 * 1000);
						result1 = BuddyRelationCorrecterN.check(result, fw, 2);
					}
					if (result1.size() > 0) {
						Thread.sleep(6 * 60 * 1000);
						BuddyRelationCorrecterN.check(result1, fw, 3);
					}
				}
			}
			br.close();
			fw.close();
			System.out.println("done!");
		} catch (Exception e) {
			e.printStackTrace();
		}
		System.exit(0);
	}
}
