<?php

# $Id: survey_purge.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// <jflemer@alum.rpi.edu>

/* {{{ proto void survey_purge(array survey_ids)
   Purges all traces of survey(s) from the database.
   Returns void. */
function survey_purge($sids) {
	if(is_int($sids)) {
		$sidstr = '='.$sids;
	} else if (is_string($sids)) {
		$sids = split(" ",$sids);
	}
	if(is_array($sids)) {
		$sidstr = array_to_insql($sids);
	}

	// make a list of question IDs
	$sql = "SELECT id FROM ".TABLE_QUESTION." WHERE survey_id ${sidstr}";
	$result = mysql_query($sql);
	$qids = array();
	while(list($qid) = mysql_fetch_row($result)) {
		array_push($qids, $qid);
	}
	mysql_free_result($result);
	$qidstr = array_to_insql($qids);
	
	// work from the bottom up...
	// start with the survey results
	$tables = array('response_bool','response_date','response_multiple','response_other','response_rank','response_single','response_text');
	foreach($tables as $table) {
		$sql = "DELETE FROM ${table} WHERE question_id ${qidstr}";
		$result = mysql_query($sql);
	}

	// then responses
	$sql = "DELETE FROM ".TABLE_RESPONSE." WHERE survey_id ${sidstr}";
	mysql_query($sql);

	// then question choices
	$sql = "DELETE FROM ".TABLE_QUESTION_CHOICE." WHERE question_id ${qidstr}";
	mysql_query($sql);

	// then questions
	$sql = "DELETE FROM ".TABLE_QUESTION." WHERE survey_id ${sidstr}";
	mysql_query($sql);

	// and finally the survey(s)
	$sql = "DELETE FROM ".TABLE_SURVEY." WHERE id ${sidstr}";
	mysql_query($sql);

	return;
}
/* }}} */

?>