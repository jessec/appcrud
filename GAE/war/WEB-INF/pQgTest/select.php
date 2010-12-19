<?php

	// Test inits
	
	include "$testLocation/open.php";

	// Some select statements

	pQgTest("select * from person where firstName = 'Jan' 
									 or firstName = 'Piet' 
									 or firstName = 'Wim'");
	
	pQgTest("select  * 
			 from    person 
			 where   firstName in ('Jan','Piet','Wim')
			   and   salary <> 3000.00"); 
	
	pQgTest("select    firstName, salary 
			 from      person 
			 where     salary between 3000.00 and 4000.00
			 order by  salary desc");
	
	pQgTest("select substr(firstName, 0, 1) . '. ' .  strtoupper(lastName) as NAME         ,
					str_replace('@', ' at ', email)                        as SAVE_EMAIL   , 
					salary * 14 										   as YEAR_SALARY   
			 from   person");	

	// Test exits
	
    include "$testLocation/close.php";
			
?>