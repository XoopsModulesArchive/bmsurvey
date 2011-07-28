<?php

# $Id: survey_update.inc,v 1.2 2005/10/04 11:12:12 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

/* {{{ proto bool survey_update(int* survey_id, string* tab, string old_tab)
   Reads current form variables from _POST.
   Returns an true on sucess, else returns false and
   sets global $errstr with an error message. */
function survey_update(&$survey_id, &$tab, $old_tab, &$copy_question) {
	global $_POST, $_SERVER, $errstr;

	// do not need update
	if(empty($old_tab))
		return(1);

	$f_arr = array();
	$v_arr = array();

	// new survey
	if(empty($survey_id)) {
		if (isset($_POST['name'])) {
		$_POST['name'] = eregi_replace(
			"[^A-Z0-9]+", "_", trim($_POST['name']) );
		$_POST['name'] = ereg_replace('_$',"",$_POST['name']);
		}

		// need to fill out at least some info on 1st tab before proceeding
		if(empty($_POST['name']) || empty($_POST['title'])
				|| empty($_POST['realm'])) {
			$tab = "general";
			$errstr = mb_('Sorry, please fill out the name, group, and title before proceeding.');
			return(0);
		}

		// create a new survey in the database
		$fields = array('name','realm','title','subtitle','email','from_option','response_id','theme','thanks_page','thank_head','thank_body','info');
		foreach($fields as $f) {
			if(isset($_POST[$f])) {
				array_push($f_arr,$f);
				array_push($v_arr,"'"._addslashes($_POST[$f])."'");
			}
		}
		array_push($f_arr,'owner');
		array_push($v_arr,"'".$_SESSION['acl']['username']."'");
		$sql = "INSERT INTO ".TABLE_SURVEY." (" . join(',',$f_arr) . ") VALUES (" . join(',',$v_arr) . ")";

		$result = @mysql_query ($sql);
		if(!$result) {
			$tab = "general";
			$errstr = mb_('Sorry, name already in use. Pick a new name.') .' [ ' .mysql_errno().': '.mysql_error().' ]';
			return(0);
		}

		$sql = "SELECT id FROM ".TABLE_SURVEY." WHERE name='".  _addslashes($_POST['name']) ."'";
		$result = mysql_query($sql);
		$survey_id = mysql_result($result,0,0);
		mysql_free_result($result);
		return(1);
	}

	// survey already started
	switch($old_tab) {
		// coming from the general tab ...
		case "general":
			if (isset($_POST['name'])) {
			$_POST['name'] = eregi_replace(
				"[^A-Z0-9]+", "_", trim($_POST['name']) );
			$_POST['name'] = ereg_replace('_$',"",$_POST['name']);
			}

			if(empty($_POST['name']) || empty($_POST['title'])
					|| empty($_POST['realm'])) {
				$tab = "general";
				$errstr = mb_('Sorry, please fill out the name, group, and title before proceeding.');
				return(0);
			}
			$fields = array('name','realm','title','subtitle','email','from_option','response_id','theme','thanks_page','thank_head','thank_body','info');
			$sql = "SELECT name FROM ".TABLE_SURVEY." WHERE id='${survey_id}'";
			$result = mysql_query($sql);
			$name = mysql_result($result,0,0);
			mysql_free_result($result);

			// trying to change survey name
			if($name != $_POST['name']) {
				$sql = "SELECT COUNT(*) FROM ".TABLE_SURVEY." WHERE name='" . _addslashes($_POST['name']) ."'";
				$result = mysql_query($sql);
				$count = mysql_result($result,0,0);
				mysql_free_result($result);
				if($count != 0) {
					$tab = "general";
					$errstr = mb_('Sorry, that name is already in use.');
					return(0);
				}
			}

			// UPDATE the row in the DB with current values
			foreach($fields as $f) {
				array_push($f_arr,$f ."='" . _addslashes($_POST[$f]) . "'");
			}
			$sql = "UPDATE ".TABLE_SURVEY." SET " . join(', ',$f_arr) . " WHERE id='${survey_id}'";
			$result = mysql_query($sql);
			if(!$result) {
				$tab = "general";
				$errstr = mb_('Warning, error encountered.') .' [ '.mysql_errno().': '.mysql_error().' ]';
				return(0);
			}
			return(1);

		// coming from the questions tab ...
		case "questions":
			// insert for copy question : 2007/01/17 by yoshis
			if(!empty($_POST['copy_question']) && !empty($_POST['copy_qid']) ){
				$has_choices = esp_type_has_choices();
				$copy_qid = intval($_POST['copy_qid']);
				$sql = "SELECT * FROM ".TABLE_QUESTION." WHERE id='${copy_qid}' AND deleted='N' ORDER by position,id";
				$result = mysql_query($sql);
				if( $question = mysql_fetch_array($result,MYSQL_ASSOC)){
					$_POST['name'] = $question['name'];
					$_POST['type_id'] = $question['type_id'];
					$_POST['content'] = $question['content'];
					$_POST['length'] = $question['length'];
					$_POST['precise'] = $question['precise'];
					$_POST['required'] = $question['required'];
					if($has_choices[$question['type_id']]) {
						$sql = "SELECT * FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='${copy_qid}' ORDER BY id";
						$result2 = mysql_query($sql);
						$i = 1;
						while($choice = mysql_fetch_array($result2,MYSQL_ASSOC)) {
							$_POST["choice_content_${i}"]=$choice['content'];
							$i++;
						}
						$_POST['num_choices'] = $i;
					}
				}
				$copy_question = 1;
				return(1);
			}
			// if the question box is empty ... ignore everything else
			if(empty($_POST['content']) && empty($_POST['name']))
				return(1);

			if(empty($_POST['content'])) {
				$tab = 'questions';
				$dont_clear = 1;
				$errstr = mb_('Please enter text for this question.');
				return(0);
			}
			// constraint: fieldname must be not empty
			//   generate it from the content if empty
			//   validate/repair fieldname
			if(empty($_POST['name'])) {
				/*$str1 = $_POST['content'];
				do {
					$str2 = $str1;
					$str1 = eregi_replace(
					"(^| )(what|which|why|how|who|where|how|is|are|were|the|a|it|of|do|you|your|please|enter)[ ?]",
					" ", $str2);
				} while ($str1 != $str2);
				$_POST['name'] = $str1;*/
				$_POST['name'] = $_POST['id'];
			}
			$_POST['name'] = strtoupper(substr( eregi_replace(
				"[^A-Z0-9]+", "_", trim($_POST['name'])), 0, 30));
			$_POST['name'] = ereg_replace('_$',"",$_POST['name']);

			// constraint: question type required
			if(empty($_POST['type_id'])) {
				$tab = 'questions';
				$dont_clear = 1;
				$errstr= mb_('Sorry, you must select a type for this question.');
				return(0);
			}

			// constraint: qid must be int or empty
			if( strcmp($_POST['q'],mb_('New Field')) == 0 )
				$qid = '';
			$qid = intval($_POST['id']);	# curr_q_id
			// constraint: can not change between question w/ answer choices and one w/o
			$has_choices = esp_type_has_choices();
			if(!empty($qid)) {
				$sql =  "SELECT Q.type_id FROM ".TABLE_QUESTION." Q WHERE Q.survey_id='${survey_id}' AND Q.id='${qid}'";
				$result = mysql_query($sql);
				$old_type_id = mysql_result($result,0,0);
				mysql_free_result($result);
				if($has_choices[$_POST['type_id']] != $has_choices[$old_type_id]) { // trying to change between incompatible question types
					$tab = "questions";
					$_POST['type_id'] = $old_type_id;
					$dont_clear = 1;
					$errstr = mb_('Sorry, you cannot change between those types of question. Create a new question instead.');
					return(0);
				}
			}

			// constraint: length must be int
			$_POST['length']  = intval($_POST['length']) or 0;

			// constraint: precise must be int
			$_POST['precise'] = intval($_POST['precise']) or 0;

			// defaults for length field
			if(empty($_POST['length']) && $_POST['type_id'] < 50) {
				$arr = array(
					0,		// 0: unused
					0,		// 1: Yes/No
					20,		// 2: Text Box  (width)
					60,		// 3: Essay Box (width)
					0,		// 4: Radio Buttons
					0,		// 5: Check Boxes (minumum)
					0,		// 6: Dropdown Box (length)
					5,		// 7: Rating (# cols)
					5,		// 8: Rate (# cols)
					0,		// 9: Date
					10		// 10: Numeric (digits)
					);
				$_POST['length'] = $arr[$_POST['type_id']];
			}

			// defaults for precision field
			if(empty($_POST['precise']) && $_POST['type_id'] < 50) {
				$arr = array(
					0,		// 0: unused
					0,		// 1: Yes/No
					0,		// 2: Text Box
					5,		// 3: Essay Box (height)
					10,		// 4: Radio Buttons
					0,		// 5: Check Boxes (maximum)
					0,		// 6: Dropdown Box
					0,		// 7: Rating (use N/A)
					0,		// 8: Rate (use N/A)
					0,		// 9: Date
					0		// 10: Numeric (decimal)
					);
				$_POST['precise'] = $arr[$_POST['type_id']];
			}

			// UPDATE row in the DB for the current question
			if($qid != '') {
				$fields = array('name','type_id','length','precise','required','content');
				foreach($fields as $f) {
					if(isset($_POST[$f]))
						array_push($f_arr,$f."='"._addslashes($_POST[$f])."'");
				}
				$sql = "UPDATE ".TABLE_QUESTION." SET " . join(', ',$f_arr) . " WHERE id='${qid}'";

			// INSERT row in the DB for new question
			} else {
				// set the position to the end
				$sql = "SELECT MAX(position)+1 FROM ".TABLE_QUESTION." WHERE survey_id='${survey_id}'";
				$result = mysql_query($sql);
				$_POST['position'] = mysql_result($result,0,0);
				mysql_free_result($result);

				$_POST['survey_id'] = $survey_id;

				$fields = array('survey_id','name','type_id','length','precise','required','content','position');
				foreach($fields as $f) {
					if(isset($_POST[$f])) {
						array_push($f_arr, $f);
						array_push($v_arr, "'". _addslashes($_POST[$f]) ."'");
					}
				}
				$sql = "INSERT INTO ".TABLE_QUESTION." (" . join(', ',$f_arr) . ") VALUES (" . join(', ',$v_arr) .")";
			}
			$result = mysql_query($sql);
			if($qid == '')
				$qid = mysql_insert_id();
			$_POST['id'] = $qid;
			if(!$result) {
				$tab = 'questions';
				$dont_clear = 1;
				$errstr = mb_('Warning, error encountered.') .' [ '.mysql_errno().': '.mysql_error().' ]';
				return(0);
			}

			// UPDATE or INSERT rows for each of the question choices for this question
			if($has_choices[$_POST['type_id']]) {
				$cids = array();
				$sql = "SELECT c.id FROM ".TABLE_QUESTION." q, ".TABLE_QUESTION_CHOICE." c WHERE q.id=c.question_id AND q.survey_id=${survey_id}";
				$result = mysql_query($sql);
				for ($i = 0; $i < mysql_num_rows($result); ++$i) {
					array_push($cids, mysql_result($result, $i, 0));
				}
				mysql_free_result($result);
				$count = 0;
				for($i=1;$i<$_POST['num_choices']+1;$i++) {
					$sql='';
					$choice_id      = isset($_POST["choice_id_${i}"]) ? intval($_POST["choice_id_${i}"]) : '';
					$choice_content = _addslashes($_POST["choice_content_${i}"]);
					// each of the submitted choices
					if($choice_id=='' && $choice_content!='') {
						// insert (new)
						$sql = "INSERT INTO ".TABLE_QUESTION_CHOICE." (question_id,content) VALUES ('${qid}','${choice_content}')";
						++$count;
					} elseif($choice_id!='' && $choice_content=='') {
						// delete (old)
						$sql = "DELETE FROM ".TABLE_QUESTION_CHOICE." WHERE id='${choice_id}'";
					} elseif($choice_id!='' && $choice_content!='' && in_array($choice_id, $cids)) {
						// update (old)
						$sql = "UPDATE ".TABLE_QUESTION_CHOICE." SET content='${choice_content}' WHERE id='${choice_id}'";
						++$count;
					}
					if($sql != '') {
						$result = mysql_query($sql);
						if(!$result) {
							$tab = 'questions';
							$dont_clear = 1;
							$errstr = mb_('Warning, error encountered.') .' [ '.mysql_errno().': '.mysql_error().' ]';
							return(0);
						}
					}
				}
				if(!$count && !isset($_POST['extra_choices'])) {
					$tab = 'questions';
					$dont_clear = 1;
					$errstr = mb_('Sorry, you need at least one answer option for this question type.') .
						' [ '. mb_('ID') .': '. $_POST['type_id'] .' ]';
					return(0);
				}
			}
			return(1);

		case "preview":
			// can not change anything here yet, so no need to update DB.
			return(1);

		case "order":
			// it updates the DB itself
			return(1);
	}
	return(1);
}
/* }}} */

?>
