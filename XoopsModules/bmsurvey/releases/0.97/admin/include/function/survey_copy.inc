<?php

# $Id: survey_copy.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// <jflemer@alum.rpi.edu>

/* {{{ proto void add_squotes(string* value, string key)
   Adds single quotes to value. */
function add_squotes (&$val, $key) {
	$val = "'" . addcslashes($val,"\\'") . "'";
}
/* }}} */

/* {{{ proto bool survey_copy(int survey_id)
   Creates an editable copy of a survey. */
function survey_copy($sid) {
	$sql = "SELECT * FROM ".TABLE_SURVEY." WHERE id='${sid}'";
	$result = mysql_query($sql);
	if(mysql_num_rows($result) != 1)
		return(false);
	$survey = mysql_fetch_array($result,MYSQL_ASSOC);
	mysql_free_result($result);

	// clear the sid, clear the creation date, change the name, and clear the status
	$survey['id'] = '';
	$survey['changed'] = '';
	$survey['owner'] = $_SESSION['acl']['username'];
	$survey['name'] .= '_copy';
	$survey['status'] = 0;

	// check for 'name' conflict, and resolve
	$sql = "SELECT COUNT(*) FROM ".TABLE_SURVEY." WHERE name='". $survey['name'] ."'";
	$i=0;
	while(mysql_result(mysql_query($sql),0,0) > 0) {
		$sql = "SELECT COUNT(*) FROM ".TABLE_SURVEY." WHERE name='". $survey['name'] . ++$i ."'";
	}
	if($i)
		$survey['name'] .= $i;

	// create new survey
	array_walk($survey, 'add_squotes');
	$sql  = "INSERT INTO ".TABLE_SURVEY;
	$sql .= ' ('. join(',',array_keys($survey)) .') ';
	$sql .= 'VALUES ( '. join(',',array_values($survey)) .' )';
	$result = mysql_query($sql);
	if(!$result)
		return(false);
	$new_sid = mysql_insert_id();

	$has_choices = esp_type_has_choices();
	
	// make copies of all the questions
	$sql = "SELECT * FROM ".TABLE_QUESTION." WHERE survey_id='${sid}' AND deleted='N' ORDER by position,id";

	$result = mysql_query($sql);
	$pos=1;
	while($question = mysql_fetch_array($result,MYSQL_ASSOC)) {
		$tid = $question['type_id'];
		$qid = $question['id'];

		// fix some fields first
		$question['id'] = '';
		$question['survey_id'] = $new_sid;
		$question['position'] = $pos++;

		// copy question to new survey
		array_walk($question,'add_squotes');
		$sql = "INSERT INTO ".TABLE_QUESTION." ";
		$sql .= '('. join(',',array_keys($question)) .') ';
		$sql .= 'VALUES ( '. join(',',array_values($question)) .' )';
		if(!mysql_query($sql))
			return(false);
		$new_qid = mysql_insert_id();

		// make copies of all the question choices (if exist)
		if($has_choices[$tid]) {
			$sql = "SELECT * FROM ".TABLE_QUESTION_CHOICE."
				WHERE question_id='${qid}'
				ORDER BY id";
			$result2 = mysql_query($sql);
			while($choice = mysql_fetch_array($result2,MYSQL_ASSOC)) {
				$choice['id'] = '';
				$choice['question_id'] = $new_qid;

				array_walk($choice,'add_squotes');
				$sql = "INSERT INTO ".TABLE_QUESTION_CHOICE." ";
				$sql .= '('. join(',',array_keys($choice)) .') ';
				$sql .= 'VALUES ( '. join(',',array_values($choice)) .' )';
				if(!mysql_query($sql))
					return(false);
			}
			mysql_free_result($result2);
		}
	}
	mysql_free_result($result);

	return(true);
}
/* }}} */

?>
