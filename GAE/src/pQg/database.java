package pQg;

import javax.jdo.JDOHelper;
import javax.jdo.PersistenceManagerFactory;

public final class database {
    private static final PersistenceManagerFactory pmfInstance =
        JDOHelper.getPersistenceManagerFactory("transactions-optional");

    private database() {}

    public static PersistenceManagerFactory get() {
        return pmfInstance;
    }
}  
 