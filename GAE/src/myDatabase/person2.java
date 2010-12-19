package myDatabase;

import javax.jdo.annotations.IdGeneratorStrategy;
import javax.jdo.annotations.IdentityType;
import javax.jdo.annotations.PersistenceCapable;
import javax.jdo.annotations.Persistent;
import javax.jdo.annotations.PrimaryKey;

@PersistenceCapable(identityType = IdentityType.APPLICATION)

public class person2 { 

	@PrimaryKey @Persistent(valueStrategy = IdGeneratorStrategy.IDENTITY)
    
                    private		Long		id;
	@Persistent		private		String 		firstName;
	@Persistent		private		String 		lastName;
	@Persistent		private		String 		email;
	@Persistent		private 	Double 		salary;

	public   Long     getId         ()     			{ return id;           }
	public   String   getFirstName  ()    			{ return firstName;    }
	public   String   getLastName	()    			{ return lastName;     }
	public   String   getEmail      ()           	{ return email;        }
	public   Double   getSalary     ()              { return salary;       }
	
    public   void 	  setFirstName  ( String in )	{ this.firstName = in; } 
    public   void 	  setLastName   ( String in )	{ this.lastName  = in; } 
    public   void 	  setEmail      ( String in )	{ this.email     = in; } 
    public   void 	  setSalary     ( Double in )	{ this.salary    = in; } 
	
}