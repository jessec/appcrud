<?php
class DataStore
{
	private $st = array();

	private function store ()
	{
		$this->st['array_json'] = 'users.members.active_hilight,format.json.pretty';
		$this->st['lower_upper'] = 'format_lower.lower,format_upper.upper';
		$this->st['upper_lower'] = 'format_upper.upper,format_lower.lower';
		$this->st['ttest'] = 'format_lower.format_lower.lower,test.test_folder.format_upper.format_upper.upper';
	}

	
	function getStoreItem ($item)
	{
		$this->store();
		return $this->st[$item];
	}
	
}
