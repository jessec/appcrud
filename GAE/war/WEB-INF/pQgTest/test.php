<?php
	
	$noDelete   = TRUE;
	$noBaseLine = TRUE;
	
	include "$testLocation/open.php";

    sql("truncate person");
	sql("truncate personExists");
    
	sql("insert into person ( firstName, lastName,     email,                salary )
					 values ( 'Piet',    'Pietersen',  'piet@gmail.com',     4500   ) ,
							( 'Klaas',   'Klaassens',  'klaas@gmail.com',    3500   ) ,
							( 'Hendrik', 'Hendriksen', 'hendrik@gmail.com',  5500   ) ,
							( 'Wim',     'Willemsen',  'wim@gmail.com',      3000   ) ,
							( 'Jan',     'Janssen',    'jan@gmail.com',      2400   ) ");

	sql("INSERT into personExists (firstName) values ('Piet')");
	
	pQgDumpTable('person');
	pQgDumpTable('personExists');

	unset($GLOBALS['pQgLog']);
			
	pQgTest("select id, firstName from person a where not exists 
			(select 1 from personExists b where b.firstName = a.firstName)");
	
    include "$testLocation/close.php";
	
?>	