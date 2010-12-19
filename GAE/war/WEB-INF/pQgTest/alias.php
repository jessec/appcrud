<?php

	// Test inits

	include "$testLocation/open.php";

	pQgTest("select p.firstName, p.salary from person p where p.salary between 3000.00 and 4000.00");
	pQgTest("select * from floep.person where salary between 3000.00 and 4000.00");
	
	// Test exits
	
    include "$testLocation/close.php";
		
?>