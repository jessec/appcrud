<?php
include_once dirname(__FILE__).'/../lib/Validation.php';
include_once dirname(__FILE__).'/../validationRules.php';

class ActionController
{
	private $post;
	private $get;
	private $sess;
	private $validationRules;
	private $validator;
	private $messages = array();

	public function setParam ($post, $get, $sess)
	{
		$this->validator = new Validation;
		$this->validationRules = new ValidationRules;
		$this->post = $this->strp_tag ($post);
		$this->get = $this->strp_tag ($get);
		$this->sess = $sess;
		$this->validate();
	}

	public function getMessages ()
	{
		return $this->messages;
	}

	public function getResult ()
	{
		return $this->get;
	}

	private function strp_tag ($array)
	{
		if (empty($array) || !is_array($array)) return array();
		function my_filter (&$value, $key)
		{
			$value = strip_tags($value);
		}
		array_walk_recursive($array, 'my_filter');
		return $array;
	}

	private function validate ()
	{
		if (!empty($this->post))
		{
			$this->getErrors ('post', $this->validationRules->getPostRules());
		}
		if (!empty($this->get))
		{
			$this->getErrors ('get', $this->validationRules->getGetRules());
		}
	}

	private function getErrors ($req, $r)
	{
		$errors = $this->validator->validate ($this->$req, $r);
		if (!empty($errors))
		{
			$this->messages[$req] = array();
			foreach ($errors as $key => $error)
			{
				$k = explode(':', $error);
				array_push($this->messages[$req], 'key:'.$k[0].' message:'.$k[1]);
			}
		}
	}
}
