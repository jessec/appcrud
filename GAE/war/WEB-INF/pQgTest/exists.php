<?php
	
	$noDelete   = TRUE;
	$noBaseLine = TRUE;
	
	include "$testLocation/open.php";

    sql("truncate person");
	//sql("truncate personCheck");
    
	sql("insert into person ( firstName, lastName,     email,                salary )
					 values ( 'Piet',    'Pietersen',  'piet@gmail.com',     4500   ) ,
							( 'Klaas',   'Klaassens',  'klaas@gmail.com',    3500   ) ,
							( 'Hendrik', 'Hendriksen', 'hendrik@gmail.com',  5500   ) ,
							( 'Wim',     'Willemsen',  'wim@gmail.com',      3000   ) ,
							( 'Jan',     'Janssen',    'jan@gmail.com',      2400   ) ");

	//sql("INSERT into personCheck (firstName) values ('Klaas')");
	//sql("INSERT into personCheck (firstName) values ('Piet')");
	
	echo "<b>Table person</b>";		
	pQgDumpTable('person');
	echo "<b>Table personCheck</b>";		
	pQgDumpTable('personCheck');
	echo "<hr>";
			
	pQgTest("select firstName, lastName from person a
			 where not exists (select 1 from personCheck b where b.firstName = a.firstName) ");

	pQgTest("select firstName from personCheck a
			  where exists (select 1 from person b 
			                  where b.firstName = a.firstName 
			                    and salary between 3000.00 and 4000.00)"); 	
	
    include "$testLocation/close.php";
	
?>	