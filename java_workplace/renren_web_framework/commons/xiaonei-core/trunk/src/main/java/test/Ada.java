package test;

public class Ada {
	private Fact fac ;
	public Ada(Fact fac){
		this.fac = fac ;
	}
	public User get(){
		return fac.create() ;
	}

}
