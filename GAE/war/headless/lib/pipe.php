<?php
class Pipe
{
	/*
	 * $datamodel = array(
	 *	'name' => 'testmodel',
	 *	'action' => explode(',',$model),
	 *	'data' => 'this shaSDFuld be in upper case'
	 *	);
	 *
	 * 
	 * 
	 */
	private $data;
	private $path;
	private $result;
	
	public function setPath($path){
		$this->path = $path;
	}
	
	public function setData($data){
		$this->data = $data;
	}

	public function setModel($model){
		$this->model = $model;
	}
	
	private function run ()
	{
		foreach ($this->model as $action)
		{
			$a = explode('.', $action);
			$c = count($a);
			$dir = '';

			$class = $a[$c - 2];
			$file = $this->path.$class.'.php';
			$function = $a[$c - 1];

			if (count($a) === 3)
			{
				$file = $this->path.$a[$c - 3].'.php';
			}

			if (count($a) > 3)
			{
				$dir = array_slice($a, 0, -3);
				$dir = implode('/', $dir).'/';
				$file = $this->path.$dir.$a[$c - 3].'.php';
			}

			if (is_file($file))
			{
				include $file;
			}
			$result = call_user_func_array(array($class, "$function"), $this->data);
		}
		$this->result = $result;
	}
	
	public function result(){
		$this->run();
		return $this->result;
	}
}
