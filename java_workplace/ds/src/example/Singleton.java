package example;

public class Singleton {
    
    private static int a = 100;
   
    
    private static Singleton instance = new Singleton();
    
    private Singleton() {
        // init....
    }
    
    public static Singleton getInstance() {
//        if (instance == null) {
//            synchronized (Singleton.class) {
//                if (instance == null) {
//                    // TODO: try...catch
//                    instance = new Singleton();
//                }
//            }
//        }
        return instance;
    }

}

// Init Once
