<?php

	define('PQG_JDO',   FALSE );
	define('PQG_LOG',   TRUE  );
	define('PQG_CHECK', TRUE  );
	
	$noDelete   = TRUE;
	$noBaseLine = TRUE;
	
	include "$testLocation/open.php";

	for ($i=1; $i<=10; $i++) {
	
		sql("select * from person where salary <> 3000.00"); 
		
		sql("select    * 
			 from      person 
		     where     salary between 500.00 and 10000.00
			 order by  salary desc");
		
	}

	pQgTest("select * from person");
	
	a();
	
	include "$testLocation/close.php";
	
?>