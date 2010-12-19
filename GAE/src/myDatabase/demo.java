package myDatabase;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;
 
@PersistenceCapable(identityType = IdentityType.APPLICATION)
public class demo {
	  
	@PrimaryKey
    @Persistent(valueStrategy = IdGeneratorStrategy.IDENTITY)
    private Long id;
	  
	@Persistent
    private Long field1;
	@Persistent
    private String field2;
	
	public Long   getId()            { return id;        }
	public Long   getField1()        { return field1;    }
	public String getField2()        { return field2;    }

	public void setField1(Long in)   { this.field1 = in; } 
	public void setField2(String in) { this.field2 = in; }

}
