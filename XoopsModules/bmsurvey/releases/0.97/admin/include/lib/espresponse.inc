<?php
# $Id: espresponse.inc,v 0.96 2008/11/25 16:31:00 yoshis Exp $
//
// Modified by Y.Sakai @ bluemooninc.biz
// Since 2004/5/27 Maninly add for attach
//
// Original by James Flemer
// <jflemer@alum.rpi.edu>

/* {{{ proto string response_check_required(int survey_id, int section)
   Reads current form variables from _POST.
   Returns an empty string if all required fields are
   completed, else returns a message string indicating which
   questions need to be completed. */

function get_attach($uname,$qid) {
	global $_FILES,$ESPCONFIG;
	// Upload folder
	$file_charset = bmsurveyUtils::getXoopsModuleConfig('FILE_CHARSET');
	$uploads = XOOPS_ROOT_PATH.$ESPCONFIG['attach_path'];
	$upurl = XOOPS_URL.$ESPCONFIG['attach_path'];
	$upfile_url = "";
	// You must set at php.ini 'register_globals = on;'
	$upfile	  = $_FILES["Q".$qid];				//upload file object 
	$upfile_tmp  = $_FILES["Q".$qid]['tmp_name'];	//tmp file name 
    $upfile_name  = basename($_FILES["Q".$qid]['name']);	//Local File Name ( Use basename for security )
    $upfile_name  = (get_magic_quotes_gpc()) ? stripslashes($upfile_name) : $upfile_name;
	$upfile_size = $_FILES["Q".$qid]['size'];		//File Size
	$upfile_type = $_FILES["Q".$qid]['type'];		//File MIME Content-Type
	//Disp File Infomation for debug
	//print("Start get_attach: $uname,$qid<BR>\n");
	//print("File Infomation:<BR>\n");
	//print("File From : $upfile_tmp<BR>\n");
	//print("File To : $upfile_name<BR>\n");
	//print("File Size - $upfile_size<BR>\n");
	//print("File type - $upfile_type<BR>\n");
	if ($upfile_tmp != "" ){
		if (eregi($ESPCONFIG['subtype'], $upfile_type)){ $deny=0; } else { $deny=1; };
		$ext = strtolower(end(explode(".",$upfile_name))); 
		if (eregi($ESPCONFIG['viri'], $ext)) $deny = 1;
		if (eregi($ESPCONFIG['imgtype'], $upfile_type)){
			$size = getimagesize($upfile_tmp);
			if ( !$size || !strcmp($size['type'],$upfile_type) ) return "getimagesize error!";
			$upfile_localname = $uname."_".$qid."_".time()."_".cnv_mbstr($upfile_name, $file_charset);	// convert for mbstrings
			$upfile_url = "<img src='".$upurl.rawurlencode($upfile_localname)."'>".$upfile_name;	//rawurlencode
			move_uploaded_file( $upfile_tmp, $uploads.$upfile_localname );
		} elseif ($deny==0) {
			$upfile_localname= $qid.$uname."_".$upfile_name.".".time();
			$upfile_url = "<a href='".XOOPS_URL."/modules/bmsurvey/download.php?url=/".rawurlencode($upfile_localname)."' target='_blank'>".$upfile_name."</a>";
			move_uploaded_file($upfile_tmp,$uploads.$upfile_localname);
		} else {
			$upfile_url = "Upload Denied...".$upfile_type." ext ".$ext;
		}
	}
	return $upfile_url;
}

function response_check_required($sid, $section) {
	global $_POST,$_FILES;

	$sql = "SELECT id,type_id,name,content FROM ".TABLE_QUESTION." ".
		survey_select_section_sql($sid,$section)." AND required='Y' AND deleted='N' ORDER BY position";
	$result = mysql_query($sql);
	if(mysql_num_rows($result) < 1) {
		// no required fields! so no need to continue
		mysql_free_result($result);
		return('');
	}

	$missing = array();	// array of missing questions

	while(list($qid,$tid,$name,$content) = mysql_fetch_row($result)) {
		//echo	$qid."-".$tid."-".$content;
		// Check for type of question
		switch ($tid){
		case 2:		// single text line
			if ( eregi("email",$content) || eregi("mailfrom|mailto",$name) ){ 
				// email address check
				if ( !checkEmail( $_POST["Q".$qid] ) ) {
					$missing[$qid] =  $content._MD_BMSURVEY_FORMATERR;
				}
			}
			if ( empty($_POST["Q".$qid]) ) $missing[$qid] = $content;
			break;
		case 5:		// Multiple
			if ( empty($_POST["Q".$qid]) ) $missing[$qid] = $content;
			else{
				$sql = "SELECT precise FROM ".TABLE_QUESTION." WHERE id='$qid'";
				list($max_checkes) = mysql_fetch_row(mysql_query($sql));
				if ($max_checkes>0){
					if ( count($_POST["Q".$qid]) > $max_checkes )
						$missing[$qid] = sprintf($content._MD_BMSURVEY_MAXOVER,$max_checkes);
				}
			}
			break;
		case 8:	// Rank
			$sql = "SELECT id FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='$qid'";
			$cid_result = mysql_query($sql);
			$cid_count = 0;
			while(list($cid) = mysql_fetch_row($cid_result)) {
				if( isset($_POST["Q${qid}_${cid}"]) ) {
					$cid_count++;
					break;
				}
			}
			mysql_free_result($cid_result);
			if ( $cid_count==0 ){
				$patterns = "/\[TH](.*)\[\/TH\]/sU";
				$replacements = '';
				$missing[$qid] = preg_replace($patterns, $replacements, $content);
			}
			break;
		case 10:	// Numeric
			if ( !ctype_digit($_POST["Q".$qid]) ){
				$missing[$qid] =  $content._MD_BMSURVEY_DIGITERR;
			} elseif ( empty($_POST["Q".$qid]) && $_POST["Q".$qid]!="0" )
				$missing[$qid] = $content;
			break;
		case 40:	// Attach file
			if ( !isset($_FILES["Q".$qid]['tmp_name']) && empty($_POST["Q".$qid]) ) $missing[$qid] = $content;
			break;
		default:
			if ( empty($_POST["Q".$qid]) ) $missing[$qid] = $content;
			break;
		}
	}
	mysql_free_result($result);

	if(count($missing) > 0) {
		// missing required variables
		//$message = _GT_You_are_missing_the_following_required_questions ."<br>\n";
		$message = _GT_You_are_missing_the_following_required_questions ."<br>\n";
		while(list($qid,$content)=each($missing)) {
			if($GLOBALS['ESPCONFIG']['DEBUG'])
				$message .= "<!-- ${qid} -->";
			$message .= "${content}<br>\n";
		}
		return($message);
	}
	return('');
}
/* }}} */

/* {{{ proto void response_delete(int survey_id, int section, int response_id)
   Deletes values for the response. */
function response_delete($sid, $rid, $sec = null) {
	if (empty($rid))
		return;

	if ($sec != null) {
		if ($sec < 1)
			return;

		/* get question_id's in this section */
		$qids = survey_get_sections($sid);
		if (!isset($qids[$sec - 1]))
			return;
		$qids = 'AND question_id '. array_to_insql($qids[$sec - 1]);
	} else {
		/* delete all */
		$qids = '';
	}

	/* delete values */
	foreach (array('bool', 'single', 'multiple', 'rank', 'text', 'other', 'date') as $tbl) {
		$sql = "DELETE FROM ".TABLE_RESPONSE."_$tbl WHERE response_id = '$rid' $qids";
		$res = mysql_query($sql);
	}
	$sql = "UPDATE ".TABLE_RESPONSE." SET complete='N' WHERE id = '$rid'";
	mysql_query($sql);
}
/* }}} */

/* {{{ proto void response_delete_all(int survey_id)
   Deletes all responses from survey. */
function response_delete_all($sid) {
	$sec = survey_get_sections($sid);
	$qids = array();
	foreach ($sec as $s)
		$qids = array_merge($qids, $s);
	$qids = array_to_insql($qids);

	/* delete values */
	foreach (array('bool', 'single', 'multiple', 'rank', 'text', 'other', 'date') as $tbl) {
		$sql = "DELETE FROM ".TABLE_RESPONSE."_$tbl WHERE question_id $qids";
		$res = mysql_query($sql);
	}
		/* ensure responses from testing status are also deleted */
		$sql = "DELETE FROM ".TABLE_RESPONSE." WHERE survey_id=${sid}";
		mysql_query($sql);
}
/* }}} */

/* {{{ proto int response_insert(int survey_id, int section, int response_id)
   Reads current form variables from _POST.
   Returns the ID for the response. */
function response_insert($sid,$section,$rid) {
	global $_POST,$xoopsUser,$xoopsDB,$ESPCONFIG;

//	$userid = isset($GLOBALS['HTTP_SERVER_VARS']['PHP_AUTH_USER']) ? $GLOBALS['HTTP_SERVER_VARS']['PHP_AUTH_USER'] : '';
	if ($xoopsUser){
		$userid = $xoopsUser->uid();
		$userHander = new XoopsUserHandler($xoopsDB);
		$tUser = $userHander->get($userid);
		$uname = $tUser->uname();
	}else{
		if (empty($userid)) $userid = $_POST['userid'];
		if ($ESPCONFIG['anonymousname']=="0"){
			$uname = "Anonymous";
		}else{
			$uname = $userid;
		}
	}
	if(empty($rid)) {
		// create a uniqe id for this response
		$sql = "INSERT INTO ".TABLE_RESPONSE." (survey_id,username) VALUES ( '${sid}','${uname}' )";
		$result = mysql_query($sql);
		$rid = mysql_insert_id();
	}

	$sql  = "SELECT Q.id, Q.type_id, T.response_table FROM ".TABLE_QUESTION.
		" Q, ".TABLE_QUESTION_TYPE." T ". survey_select_section_sql($sid,$section,'Q') .
		" AND Q.survey_id='${sid}' AND Q.deleted='N' AND Q.type_id < 50 AND Q.type_id=T.id";
	$q_result = mysql_query($sql);
	while(list($qid, $tid, $table) = mysql_fetch_row($q_result)) {
		if (isset($_POST["Q".$qid])) {
			if (is_array($_POST["Q".$qid]))
				$bareval = array_map('_stripslashes', $_POST["Q".$qid]);
			else
				$bareval = _stripslashes($_POST["Q".$qid]);
		}
		else
			$bareval = '';
		if (is_string($bareval))
			$val = addslashes($bareval);
		else
			$val = implode("|", $bareval); //'';
		$session = array('sid'=>$sid, 'qid'=>$qid, 'val' => $val);
		switch($table ) {
			case 'response_bool':
				$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,choice_id ) VALUES ( '${rid}','${qid}','${val}' )";
						if (!empty($val))
						$result = mysql_query($sql);
				break;
			case 'response_text':
				// only insert if non-empty content
				if($tid == 10) { // numeric
					$bareval = ereg_replace("[^0-9.\-]*(-?[0-9]*\.?[0-9]*).*", '\1', $bareval);
				}
				if($tid == 40) { // attach by Y.Sakai
					if ($xoopsUser){
						$userHander = new XoopsUserHandler($xoopsDB);
						$tUser = $userHander->get($xoopsUser->uid());
						$uname = $tUser->uname();
					}else{
						$uname = "Anonymous";
					}
					$bareval = get_attach($uname,$qid);
				}
				if(ereg("[^ \t\n]",$bareval)) {
					$val = addslashes($bareval);
					$sql = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,response ) VALUES ( '${rid}','${qid}','${val}' )";
					$result = mysql_query($sql);
				}
				break;
			case 'response_date':
				// only insert if non-empty content
				$aSepList="-/ .";
				ereg("^([0-9]+)[$aSepList]([0-9]+)[$aSepList]([0-9]+)$", $bareval, $regs);
				if($regs) {
					date(_SHORTDATESTRING,strtotime($val));
					$dfmt = ereg_replace("[\/]|[\-]","",_SHORTDATESTRING);
					switch($dfmt){
						case "njY":
							$val=$regs[3]."-".$regs[1]."-".$regs[2];
							break;
						case "jnY":
							$val=$regs[3]."-".$regs[2]."-".$regs[1];
							break;
						case "Ynj":
							$val=$regs[1]."-".$regs[2]."-".$regs[3];
							break;
					}
					$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,response ) VALUES ( '${rid}','${qid}','${val}' )";
					$result = mysql_query($sql);
				}
				break;
			case 'response_single':
				if(empty($bareval)) {
					$sql = "SELECT id FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='${qid}' AND content LIKE '!other%' ORDER BY id";
					$c_result = mysql_query($sql);
					while(list($cid) = mysql_fetch_row($c_result)) {
									if (!isset($_POST["Q${qid}_${cid}"]))
											continue;
						$other = _addslashes($_POST["Q${qid}_${cid}"]);
						if(ereg("[^ \t\n]",$other)) {
							$sql = "INSERT INTO ".TABLE_RESPONSE_OTHER."  ( response_id,question_id,choice_id,response ) VALUES ( '${rid}','${qid}','${cid}','${other}' )";
							$result = mysql_query($sql);
							$val = $cid;
							break;
						}
					}
					mysql_free_result($c_result);
				}
				if(ereg("other_([0-9]+)", $bareval, $regs)) {
					$cid=$regs[1];
								if (!isset($_POST["Q${qid}_${cid}"]))
									break; // out of the case
					$other = _addslashes($_POST["Q${qid}_${cid}"]);
					if(ereg("[^ \t\n]",$other)) {
						$sql = "INSERT INTO ".TABLE_RESPONSE_OTHER."  ( response_id,question_id,choice_id,response ) VALUES ( '${rid}','${qid}','${cid}','${other}' )";
						$result = mysql_query($sql);
						$val = $cid;
					}
				}
				$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,choice_id ) VALUES ( '${rid}','${qid}','${val}' )";
				$result = mysql_query($sql);
				break;
			case 'response_multiple':
				$sql = "SELECT id FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='${qid}' AND content LIKE '!other%'";
				$c_result = mysql_query($sql);
				while(list($cid) = mysql_fetch_row($c_result)) {
					if (!isset($_POST["Q${qid}_${cid}"]) || empty($_POST["Q${qid}_${cid}"]))
						continue;
					if (!isset($_POST["Q".$qid]))
						$_POST["Q".$qid] = array($cid);
					else
						array_push($_POST["Q".$qid], $cid);
					$other = _addslashes($_POST["Q${qid}_${cid}"]);
					if(ereg("[^ \t\n]",$other)) {
						$sql = "INSERT INTO ".TABLE_RESPONSE_OTHER."  ( response_id,question_id,choice_id,response ) VALUES ( '${rid}','${qid}','${cid}','${other}' )";
						$result = mysql_query($sql);
					}
				}
				mysql_free_result($c_result);
				if(!isset($_POST["Q".$qid]) || count($_POST["Q".$qid]) < 1)
					break;
				foreach($_POST["Q".$qid] as $cid) {
					$cid = _addslashes($cid);
					if(ereg("other_[0-9]+", $cid))
						continue;
					$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,choice_id ) VALUES ( '${rid}','${qid}','${cid}' )";
					$result = mysql_query($sql);
				}
				break;
			case 'response_rank':
				if($tid == 8) { // Rank
					$sql = "SELECT id FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='${qid}'";
					$cid_result = mysql_query($sql);
					while(list($cid) = mysql_fetch_row($cid_result)) {
						if (!isset($_POST["Q${qid}_${cid}"])) continue;
						$val = _addslashes($_POST["Q${qid}_${cid}"]);
						if(strtolower($val) == "n/a")
							$rank = -1;
						else
							$rank = intval($val);
						$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,choice_id,rank ) ";
						$sql .= "VALUES ( '${rid}','${qid}','${cid}','${rank}' )";
						mysql_query($sql);
						$session['val'] .= $cid ."|";
					}
					mysql_free_result($cid_result);
					break;
				}
				if(strtolower($bareval) == "n/a")
					$rank = -1;
				else
					$rank = intval($bareval);
				$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,rank ) VALUES ( '${rid}','${qid}','${rank}' )";
				$result = mysql_query($sql);
				break;
		}
		$_SESSION['bmsurvey'][] = $session;
	}
	mysql_free_result($q_result);
	return($rid);
}
/* }}} */

/* {{{ proto bool response_commit(int response_id)
   Marks a response as completed.  Returns true on success.
 */
function response_commit($rid) {
	$sql = "UPDATE ".TABLE_RESPONSE." SET complete='Y', submitted = NULL WHERE id='${rid}'";
	if(mysql_query($sql)) return(true);
	return(false);
}
/* }}} */

/* {{{ proto int response_key_cmp(mixed left, mixed right)
   Key comparator for response keys (to handle numeric, and strings
   composed of QID_CID).
 */
function response_key_cmp($l, $r) {
	$lx = explode('_', $l);
	$rx = explode('_', $r);
	$lc = intval($lx[0]);
	$rc = intval($rx[0]);
	if ($lc == $rc) {
		if (count($lx) > 1 && count($rx) > 1) {
			$lc = intval($lx[1]);
			$rc = intval($rx[1]);
		} else if (count($lx) > 1) {
			$lc++;
		} else if (count($rx) > 1) {
			$rc++;
		}
	}
	if ($lc == $rc)
		return 0;
	return ($lc > $rc) ? 1 : -1;
}
/* }}} */

/* {{{ proto array response_select(int survey_id, int response_id, array columns, array question_ids)
   Returns the values from the specific response in a sorted
   (associative) array indexed by question_id.
   
   The key in the associative array is the Question ID, or in the case
   of "!other", or rank questions the Question ID concatenated with the
   Choice ID ("${qid}_${cid").  The value for all types is an array. 
   This array contains any columns requested by the parameter $col as
   the first elements.  The last two elements are the response content,
   and the content ID respectively.  (For multiple answer questions,
   the value is an array of these arrays.)
   
   For example, for a boolean type question (ID #42) and a check box
   type (ID #44), you might get something like:

	  $response_select = array(
		'42' => array(_('No'), 'N'),
		'44' => array(
				  array('Blue', 102),
				  array('Green', 104)
				)
		);

   When in doubt, consult var_dump() as to the format of the returned
   array.
 */
function response_select($sid, $rid, $col = null, $qids = null,$order = null, $makenumber=null) {
	$values = array();

	if ($col == null) {
		$col = '';
	}
	if (!is_array($col) && !empty($col)) {
		$col = explode(',', preg_replace("/\s/",'', $col));
	}
	if (is_array($col) && count($col) > 0) {
		$col = ',' . implode(',', array_map(create_function('$a','return "q.$a";'), $col));
	}

	if ($qids == null) {
		$qids = '';
	} elseif (is_array($qids)) {
		$qids = 'AND a.question_id ' . array_to_insql($qids);
	} elseif (intval($qids) > 0) {
		$qids = 'AND a.question_id = ' . intval($qids);
	} else {
		$qids = '';
	}
	if ($order == null) {
		$order = 'q.id';
	}
	// --------------------- response_bool ---------------------
	$sql = "SELECT $order $col,a.choice_id FROM ".TABLE_RESPONSE_BOOL." a, ".
		TABLE_QUESTION." q WHERE a.response_id='${rid}' AND a.question_id=q.id $qids";
	$result = mysql_query($sql);
	while($row = mysql_fetch_row($result)) {
		$qid = array_shift($row);
		$val = array_pop($row);
		$values[$qid] = $row;
		array_push($values["$qid"], ($val == 'Y') ? _GT_Yes : _GT_No, $val);
	}
	mysql_free_result($result);

	// --------------------- response_single ---------------------
	$sql = "SELECT $order $col,c.content,c.id FROM ".TABLE_RESPONSE_SINGLE." a, ".
		TABLE_QUESTION." q, ".TABLE_QUESTION_CHOICE." c WHERE a.response_id='${rid}' AND a.question_id=q.id AND a.choice_id=c.id $qids";
	$result = mysql_query($sql);
	while($row = mysql_fetch_row($result)) {
		$qid = array_shift($row);
		$c = count($row);
		$val = $row[$c - 2];
		settype($row[$c - 1], 'integer');
		// Make a Sequential number for CSV output
		if($makenumber!=null){
			$sql = "SELECT id FROM ".TABLE_QUESTION_CHOICE." WHERE question_id=$qid AND id<=".$row[$c - 1];
			$row[] = mysql_num_rows(mysql_query($sql));
		}
		if (ereg('/^!other/i', $val)) $row[$c - 1] = 'other_' . $row[$c - 1];
		$values[$qid] = $row;
	}
	mysql_free_result($result);

	// --------------------- response_multiple ---------------------
	$sql = "SELECT $order $col, c.content,c.id FROM ".
		TABLE_RESPONSE_MULTIPLE."  a, ".TABLE_QUESTION." q, ".TABLE_QUESTION_CHOICE.
		" c WHERE a.response_id='${rid}' AND a.question_id=q.id AND a.choice_id=c.id $qids ORDER BY a.question_id,c.id";
	$result = mysql_query($sql);
	$arr = array();
	$tmp = null;
	while($row = mysql_fetch_row($result)) {
		$qid = array_shift($row);
		$c = count($row);
		$cidp = $c - 1;
		$val = $row[$c - 2];
		settype($row[$c - 1], 'integer');
		if($tmp == $qid) {
			if($makenumber!=null) $row[] = array_search($row[$c - 1],$cids);
			if (ereg('/^!other/i', $val)) $row[$cidp] = 'other_' . $row[$cidp];
			$arr[] = $row;
			continue;
		}
		if($tmp != null)
			$values["$tmp"]=$arr;
		$tmp = $qid;
		// Make a Sequential number for CSV output
		if($makenumber!=null){
			$sql = "SELECT id FROM ".TABLE_QUESTION_CHOICE." WHERE question_id=$qid ORDER BY id";
			$res = mysql_query($sql);
			$cids = array();
			$i = 1;
			while(list($cid) = mysql_fetch_row($res)) {
				$cids[$i]=$cid;
				$i++;
			}
			$row[] = array_search($row[$c - 1],$cids);
			if (ereg('/^!other/i', $val)) $row[$cidp] = 'other_' . $row[$cidp];
		}
		$arr = array($row);
	}
	if($tmp != null)
		$values["$tmp"]=$arr;
	mysql_free_result($result);
	unset($arr);
	unset($tmp);
	unset($row);

	// --------------------- response_other ---------------------
	$sql = "SELECT $order, c.id $col, a.response FROM ".TABLE_RESPONSE_OTHER.
		" a, ".TABLE_QUESTION." q, ".TABLE_QUESTION_CHOICE.
		" c WHERE a.response_id='${rid}' AND a.question_id=q.id AND a.choice_id=c.id $qids ORDER BY a.question_id,c.id";
	$result = mysql_query($sql);
	while($row = mysql_fetch_row($result)) {
		$qid = array_shift($row);
		$cid = array_shift($row);
		array_push($row, $row[count($row) - 1]);
		$values["${qid}_${cid}"] = $row;
	}
	mysql_free_result($result);

	// --------------------- response_rank ---------------------
	$sql = "SELECT CONCAT($order,'_',c.id) AS id $col,c.content,a.rank FROM ".
		TABLE_RESPONSE_RANK."  a, ".TABLE_QUESTION." q, ".TABLE_QUESTION_CHOICE.
		" c WHERE a.response_id='${rid}' AND a.question_id=q.id AND a.choice_id=c.id $qids ORDER BY a.question_id,c.id";
	$result = mysql_query($sql);
	while($row = mysql_fetch_row($result)) {
		$qid = array_shift($row);
		settype($row[count($row) - 1], 'integer');
		$values[$qid] = $row;
	}
	mysql_free_result($result);

	// --------------------- response_text ---------------------
	$sql = "SELECT $order $col,a.response FROM ".TABLE_RESPONSE_TEXT." a, ".
		TABLE_QUESTION." q WHERE a.response_id='${rid}' AND a.question_id=q.id $qids";
	$result = mysql_query($sql);
	while($row = mysql_fetch_row($result)) {
		$qid = array_shift($row);
		$values["$qid"]=$row;
		$val = array_pop($values["$qid"]);
		array_push($values["$qid"], $val, $val);
	}
	mysql_free_result($result);

	// --------------------- response_date ---------------------
	$sql = "SELECT $order $col,a.response FROM ".TABLE_RESPONSE_DATE." a, ".
		TABLE_QUESTION." q WHERE a.response_id='${rid}' AND a.question_id=q.id $qids";
	$result = mysql_query($sql);
	while($row = mysql_fetch_row($result)) {
		$qid = array_shift($row);
		$values["$qid"]=$row;
		$val = array_pop($values["$qid"]);
		array_push($values["$qid"], $val, $val);
	}
	mysql_free_result($result);

	// --------------------- return ---------------------
	uksort($values, 'response_key_cmp');
	return($values);
}
/* }}} */

/* {{{ proto array response_select_human(int survey_id, int response_id, array question_ids)
   A wrapper around response_select(), that returns an array of
   key/value pairs more suitable for humans.
 */
function response_select_human($sid, $rid, $qids = null) {
	$res = response_select($sid, $rid, 'type_id,content', $qids,'position');	//
	//var_dump($res);
	$hmn = array();
	reset($res);
	$tmpk = null;
	$tmpv = array();
	while(list($qid, $arr) = each($res)) {
		$key = null;
		$val = null;
		if (strstr($qid, '_')) {
			// rank or other
			//   Those parameter will picked it up by non $sub array
			list($qid, $sub) = explode('_', $qid);
			if ($arr[0] != 8)
				continue;
			// rank
			$key = $arr[1];
			if ($arr[3] < 0)
				$arr[3] = 'n/a';
			else
				$arr[3]++;
			$val = $arr[2] . ' = ' . $arr[3];
			if ($tmpk != $key) {
				if (!empty($tmpk) || count($tmpv))
					array_push($hmn, array($tmpk, $tmpv));
				$tmpk = null;
				$tmpv = array();
			}
			$tmpk = $key;
			array_push($tmpv, $val);
			continue;
		}
		if (!empty($tmpk) || count($tmpv))
			array_push($hmn, array($tmpk, $tmpv));
		if (is_array($arr[0])) {
			// mutiple
			$key = $arr[0][1];
			$val = array();
			foreach ($arr as $subarr) {
				if (ereg("^!other", $subarr[2])) {
					$tmpv = preg_replace(array("/^!other=/","/^!other/"),
							array('', 'Other'), $subarr[2]);
					$tmp = $qid ."_". preg_replace("/^other/", $qid, $subarr[3]);
					if (isset($res[$tmp]))
						$tmpv .= ': '. $res[$tmp][2];
					array_push($val, $tmpv);
				} else {
					array_push($val, $subarr[2]);
				}
			}
		} else {
			$key = $arr[1];
			if (ereg("^!other", $arr[2])) {
				$val = preg_replace(array("/^!other=/","/^!other/"),
						array('', 'Other'), $arr[2]);
				$tmp = $qid ."_". preg_replace("/^other/", $qid, $arr[3]);
				if (isset($res[$tmp]))
					$val .= ': '. $res[$tmp][2];
			} else {
				$val = $arr[2];
			}
		}
		$tmpk = null;
		$tmpv = array();
		$hmn[] = array($key, $val);
	}
	if (!empty($tmpk) || count($tmpv))
		array_push($hmn, array($tmpk, $tmpv));
	return $hmn;
}
/* }}} */

/* {{{ proto array response_select_compact(int survey_id, int response_id, array question_ids)
   A wrapper around response_select(), that returns an array of
   key/value pairs more suitable for computer parsing.
 */
function response_select_compact($sid, $rid, $qids = null) {
	$res = response_select($sid, $rid, 'type_id', $qids);
	$cpq = array();
	reset($res);
	while(list($qid, $arr) = each($res)) {
		if (strstr($qid, '_')) {
			// rank or other
			if ($arr[0] == 8) {
				// rank
				$cpq[] = array($qid, $arr[2], array($arr[1]));
			} else {
				// other
				$cpq[] = array($qid, $arr[1]);
			}
		} elseif (is_array($arr[0])) {
			// multiple
			$cpq[] = array($qid,
					array_map(create_function('$a', 'return $a[2];'), $arr),
					array_map(create_function('$b', 'return $b[1];'), $arr));
		} else {
			if ($arr[0] == 4 || $arr[0] == 6)
				$cpq[] = array($qid, $arr[2], array($arr[1]));
			else
				$cpq[] = array($qid, $arr[2]);
		}
	}
	return $cpq;
}
function response_select_defval($sid, $rid, $qids = null) {
	$res = response_select($sid, $rid, 'type_id', $qids);
	$cpq = array();
	reset($res);
	while(list($qid, $arr) = each($res)) {
		$qname = "Q" . $qid;
		if (strstr($qid, '_')) {
			// rank or other
			if ($arr[0] == 8) {
				// rank
				$cpq[$qname] = array( $arr[2], array($arr[1]));
			} else {
				// other
				$cpq[$qname] = $arr[1];
			}
		} elseif (is_array($arr[0])) {
			// multiple
			$cpq[$qname] = array(
					array_map(create_function('$a', 'return $a[2];'), $arr),
					array_map(create_function('$b', 'return $b[1];'), $arr));
		} else {
			if ($arr[0] == 4 || $arr[0] == 6)
				$cpq[$qname] = array($arr[2], array($arr[1]));
			else
				$cpq[$qname] = $arr[2];
		}
	}
	return $cpq;
}
/* }}} */

/* {{{ proto array response_select_name(int survey_id, int response_id, array question_ids,int makenumber)
   A wrapper around response_select(), that returns an array of
   key/value pairs using the field name as the key.
 */
function response_select_name($sid, $rid, $qids=null, $makenumber=null, $otherformat='Other: %s') {
	$res = response_select($sid, $rid, 'type_id,name', $qids, null, $makenumber);
//echo var_dump($res);echo "<BR><br>";
	$nam = array();
	reset($res);
//	$r=1;
	while(list($qid, $arr) = each($res)) {
		$key = null;
		$val = null;
		if (strstr($qid, '_')) {
			// rank or other
			list($qid, $sub) = explode('_', $qid);
			if ($arr[0] != 8)
				continue; // other

			// rank
			$str1 = cnv_mbstr( $arr[2] , $GLOBALS['ESPCONFIG']['csv_charset'] );
/*
			do {
				$str2 = $str1;
				$str1 = eregi_replace(
					"(^| )(what|which|why|how|who|where|how|is|are|were|the|a|it|of|do|you|your|please|enter)[ ?]",
					" ", $str2);
			} while ($str1 != $str2);
			$str1 = trim(strtoupper(eregi_replace(
				"[^A-Z0-9]+", " ", $str1)));
*/
			$str1 = ereg_replace(' +','_',$str1);
			/* Change for number response
			$str1 = ereg_replace(' +','_',$r); $r++;
			*/
			$arr[1] .= "_$str1";
			$nam[$arr[1]] = $arr[3];
			continue;
		}
		if (is_array($arr[0])) {
			// mutiple
			$key = $arr[0][1];
			$val = array();
			foreach ($arr as $subarr) {
				if (ereg("^!other", $subarr[2])) {
					$tmp = $qid."_".$subarr[3];
					if (isset($res[$tmp]))
						$tmpv = sprintf($otherformat, $res[$tmp][2]);
					array_push($val, $tmpv);
				} else {
					if ($makenumber!=null)
						array_push($val, $subarr[4]);	// Set seq number
					else
						array_push($val, $subarr[2]);	// Set strings
				}
			}
		} else {
			$key = $arr[1];
			if (ereg("^!other", $arr[2])) {
				$tmp = $qid."_".$arr[3];
				if (isset($res[$tmp]))
					$val = sprintf($otherformat, $res[$tmp][2]);
			} else {
				if ($makenumber!=null and count($arr)>4){
					$val = $arr[4];		// Set number
				}else{
					$val = $arr[2];		// Set strings
				}
			}
		}
		$nam[$key] = $val;
	}
	return $nam;
}
/* }}} */
//
// Add function by Y.Sakai
//


function make_multipart($url,$fnm,$boundary,$mail_charset) {
	$fnamedotpos = strrpos($fnm,'.');
	$fext = strtolower(substr($fnm,$fnamedotpos+1));
	if ( function_exists('mb_encode_mimeheader') )
		$fnm = mb_encode_mimeheader( $fnm, $mail_charset, "B" );
	if( !empty( $url) ) {
		$fd = @fopen( $url, "r" );
		if( $fd ) {
			$attach_file = fread( $fd, filesize($url) );
			fclose( $fd );
			//$attach_mimetype = mime_content_type( $url );
		}
	}
	switch($fext) {
		case "pdf": $attach_mimetype = "application/pdf"; break;
		case "doc": $attach_mimetype = "applicaion/msword"; break;
		case "xls": $attach_mimetype = "application/excel"; break;
		case "ppt": $attach_mimetype = "application/powerpoint"; break;
		case "jpg": case "jpeg": $attach_mimetype = "image/jpeg"; break;
		case "gif": $attach_mimetype = "image/gif"; break;
		case "txt": $attach_mimetype = "text/plain"; break;
		default: $attach_mimetype = "application/octet-stream"; break;
	}
	$mimetype = "multipart/mixed;\n\tboundary=\"$boundary\"";
	$message = "--".$boundary."\n";
	$message .= "Content-Type: " . $attach_mimetype . ";\n\tname=\"".$fnm."\"\n";
	$message .= "Content-Transfer-Encoding: base64\n";
	$message .= "Content-Disposition: attachment;\n\tfilename=\"".$fnm."\"\n\n";
	$message .= chunk_split(base64_encode( $attach_file )) ."\n";
		//."--" .$boundary."\n";
	return $message;
}
function fnm2mime($fnm) {
	$fnamedotpos = strrpos($fnm,'.');
	$fext = strtolower(substr($fnm,$fnamedotpos+1));
	switch($fext) {
		case "pdf": $attach_mimetype = "application/pdf"; break;
		case "doc": $attach_mimetype = "applicaion/msword"; break;
		case "xls": $attach_mimetype = "application/excel"; break;
		case "ppt": $attach_mimetype = "application/powerpoint"; break;
		case "jpg": case "jpeg": $attach_mimetype = "image/jpeg"; break;
		case "gif": $attach_mimetype = "image/gif"; break;
		case "png": $attach_mimetype = "image/png"; break;
		case "txt": $attach_mimetype = "text/plain"; break;
		default: $attach_mimetype = "application/octet-stream"; break;
	}
	return $attach_mimetype;
}

function send_email_with_attach($from,$to,$subject,$message,$headers,$attach,$cc='',$bcc='') {
	global $xoopsConfig;

	if (!$to) return;
	$mail_charset = $GLOBALS['ESPCONFIG']['mail_charset'];
	if (!$from) $from = bmsurveyUtils::getXoopsModuleConfig('MAILADDR');
	if (!$from) $from = $xoopsConfig['adminmail'];
	$attach_flag = FALSE;
	$mpstr = '';
	/*
	$boundary = "----------". uniqid( rand() );
	foreach ($attach as $att) {
		$fnm=$att['fnm'];
		$url=$att['url'];
		$mpstr .= make_multipart($url,$fnm,$boundary,$mail_charset);
		$attach_flag = TRUE;
	}
	$mimetype = ($attach_flag ? "multipart/mixed;\n\tboundary=\"$boundary\"" : "text/plane;charset=\"".$mail_charset."\"" );
	$chgheader = 
		"From: ". $from ."\n" . 
		"Reply-To: ". $from . "\n" . 
		"X-Mailer: PHP/".phpversion()."\n" . 
		"Return-Path: <". $from . ">\n" . 
		"MIME-Version: 1.0\n" . 
		"Content-Type: " . $mimetype . "\n";
	$chgheader .= $cc ? "cc:" . $cc . "\n" : "";
	$chgheader .= $bcc ? "bcc:" . $bcc . "\n" : "";
	$chgheader .= $attach_flag ? "Status:   \n\nThis is multipart message.\n\n" : "";
	if( $attach_flag ) {
		$message = "--" .$boundary . "\n".
			"Content-Type: text/plain; charset = \"".$mail_charset."\"\n".
			"Content-Transfer-Encoding: 7bit\n\n".
			$message."\n\n".$mpstr;
	}
	*/
	if ( function_exists('mb_encode_mimeheader') )
		$subject = mb_encode_mimeheader( $subject, $mail_charset, "B" );
	$message = cnv_mbstr( $message, $mail_charset);
	$message = preg_replace("/\x0D\x0A|\x0D|\x0A/","\n",$message);
	//ini_set("sendmail_name", $from);
	//if (extension_loaded('mbstring'))
	//	return mb_send_mail($to, $subject, $message, $chgheader, "-f ".$from);
	//else
	//return mail($to, $subject, $message, $chgheader, "-f ".$from); //Will not work PHP safe mode
	//return mail($to, $subject, $message, $chgheader);
	$xoopsMailer =& getMailer();
	$xoopsMailer->useMail();
	$xoopsMailer->setToEmails($to);
	$xoopsMailer->setFromEmail($from);
	$xoopsMailer->setFromName($xoopsConfig['sitename']);
	$xoopsMailer->setSubject($subject);
	$xoopsMailer->setBody($message);
	for($i=0;$i<count($attach);$i++) {
		$res = $xoopsMailer->multimailer->AddAttachment($attach[$i]['url'],$attach[$i]['fnm'],"base64",fnm2mime($attach[$i]['fnm']));
		echo $attach[$i]['fnm'] . " Source Attached(".(($res)?ok:error).")<br />";
	}
	if ( $cc ) $xoopsMailer->AddCC($cc);
	if ( $bcc ) $xoopsMailer->AddBCC($bcc);
	if ( !$xoopsMailer->send() )
		echo "Send failed to " . $to;
	else
		echo "Sent to " . $to;
}
//
// Get mailto info from section text
//
function pickup_mail_info($referer) {
	if (preg_match("/\bhref\s*=\s*[\"']?(mailto:[^\s\"'>]*)/i",$referer,$reg)) $referer=$reg[1];
    $url = parse_url($referer);
	if (!eregi('mailto',$url['scheme'])) return false;
    $param = array();
    $param['to'] = $url['path'];
	if(isset($url['query'])){
	    $query = rawurldecode($url['query']);
    	parse_str($query,$ret);
    	$param['cc'] = $ret['cc'];
    	$param['bcc'] = $ret['bcc'];
    	$param['subject'] = $ret['subject'];
    	$param['body'] = $ret['body'];
    }
    return $param;
}
/* {{{ proto bool response_send_email(int survey_id, int response_id)
   Retrieves the response from the database and sends an email with the
   values.  Returns true if mail sent.
 */
function response_send_email($sid, $rid,$uname='',$rs_addr='') {
	global $xoopsUser, $xoopsDB, $xoopsConfig;
	global $_POST,$ESPCONFIG;

	$debug=0;
	if ($GLOBALS['ESPCONFIG']['allow_email']==0) {
		return true;
	}
	if ($debug) echo sprintf("%u %u %s %s<br />",$sid,$rid,$uname,$rs_addr);
	//
	// Load Survey Information
	//
	$sql = "SELECT name,title,info,email,from_option FROM ".TABLE_SURVEY." WHERE id='${sid}'";
	$result = mysql_query($sql);
	list($name, $title, $info, $qs_addr, $from_option) = mysql_fetch_row($result);
	mysql_free_result($result);
	if(empty($qs_addr)) return(false);	// $qs_addr = $xoopsConfig['adminmail'];
	if ($debug) echo $sql."<br />";
	//
	// load survey questions for mailto option
	//
	$mailto=array();
	$sql = "SELECT * FROM ".TABLE_QUESTION." WHERE survey_id='${sid}' AND deleted='N' ORDER BY position,id";
	if($questions_result = mysql_query($sql)) {
		while($question = mysql_fetch_array($questions_result)) {
			// process each question
			if ($question['type_id'] == 100) {
				if (eregi("mailto",$question['content'])){
					$mailtoparam = pickup_mail_info($question['content']);
				}
			}elseif ($question['type_id'] == 1) {
				// Yes/No
				if (eregi("mailfromset",$question['name'])){
					$mailoption['mailfromset']=$question['content'];
				}elseif (eregi("to_mailinglist",$question['name'])){
					$mailoption['to_mailinglist']=$question['content'];
				}
			}elseif ($question['type_id'] == 2) {
				if (eregi("mailto",$question['name'])){
					$mailoption['mailto']=$question['content'];
				}elseif (eregi("mailfrom",$question['name'])){
					$mailoption['mailfrom']=$question['content'];
				}elseif (eregi("mailcc",$question['name'])){
					$mailoption['mailcc']=$question['content'];
				}elseif (eregi("mailname",$question['name'])){
					$mailoption['mailname']=$question['content'];
				}elseif (eregi("mailsubject",$question['name'])){
					$mailoption['mailsubject']=$question['content'];
				}
			}elseif ($question['type_id'] == 3) {
				if (eregi("mailbody",$question['name'])){
					$mailoption['mailbody']=$question['content'];
				}
			}
		}
	}
	//lets check to see if user wants human readable email
	if ($GLOBALS['ESPCONFIG']['human_email']) {
		$answers = response_select_human($sid, $rid," ORDER BY q.position");
		$qsep = " : ";
		$isep = ",";
		$end = "\n";
	} else {
		$answers = response_select_compact($sid, $rid);
		$qsep = ' : ';
		$isep = ',';
		$end = "\n";
	}
	if( !$rs_addr && $xoopsUser ){
		$userHander = new XoopsUserHandler($xoopsDB);
		$tUser = $userHander->get($xoopsUser->uid());
		$uname = $tUser->uname();
		$rs_addr =  $tUser->email();
	}elseif(!$uname){
		if ($ESPCONFIG['anonymousname']=="0"){
			$uname = "Anonymous";
		}else{
			$uname = $_POST['userid'];
		}
		$rs_addr = $qs_addr;
	}
	if ($debug){
		echo "<br />Questioner:".$qs_addr;
		echo "<br />uname:".$uname;
		echo "<br />rs_addr: $rs_addr <br />";
	}
	$user = array('survey.id' => $sid);
	//'S'.$sid.'R'.$rid.':'. mb_(_MD_MAIL_TITLE) ." $name [$rid]";
	$subject = $title;
	if ($uname) $subject .= ','.$uname;	

	if (bmsurveyUtils::getXoopsModuleConfig('ADD_INFO'))
		$message = $info.$end.$end;
	else
		$message = '';
	reset($user);
	while (list($k, $v) = each($user))
//		$message .= $k . $qsep . $v . $end;
	$headers = "From: \"phpESP ".
		addslashes($GLOBALS['ESPCONFIG']['version']) .
		"\" <phpesp@". $GLOBALS['HTTP_SERVER_VARS']['SERVER_NAME'] .">\n";
	$headers .= "X-Sender: <phpesp@". $GLOBALS['HTTP_SERVER_VARS']['SERVER_NAME'] .">\n";
	$headers .= "X-Mailer: phpESP\n";
	$headers .= "Return-Path: <". $GLOBALS['HTTP_SERVER_VARS']['SERVER_ADMIN'] ."@".
		$GLOBALS['HTTP_SERVER_VARS']['SERVER_NAME'] . ">\n";

	reset($answers);

	$file_charset = bmsurveyUtils::getXoopsModuleConfig('FILE_CHARSET');
	$upurl = XOOPS_URL.$ESPCONFIG['attach_path'];
	$uploads = XOOPS_ROOT_PATH.$ESPCONFIG['attach_path'];
	$i=0;
	$attach=array();
	$blogid='';
	while($arr = array_shift($answers)) {
		unset($x);
		if (count($arr) > 2)
			list($k, $v, $x) = $arr;
		else
			list($k, $v) = $arr;
		if (is_array($v))
			$v = implode($isep, $v);
		if (isset($x)) {
			if (is_array($x))
				$v .= ' (' . implode($isep, $x) . ')';
			else
				$v .= ' = ' . $x;
		}
		//************************************
		// Mailto option controll
		//************************************
		if (isset($mailoption['mailto']  )){ if (eregi($mailoption['mailto'],$k)) $mailtoparam['to']=$v; }
		if (isset($mailoption['mailfrom'])){ if (eregi($mailoption['mailfrom'],$k)) $mailtoparam['from']=$v; }
		if (isset($mailoption['mailcc']))  { if (eregi($mailoption['mailcc'],$k)) $mailtoparam['cc']=$v; }
		if (isset($mailoption['mailname'])){ if (eregi($mailoption['mailname'],$k)) $mailtoparam['name']=$v; }
		if (isset($mailoption['mailsubject'])){ if (eregi($mailoption['mailsubject'],$k)) $mailtoparam['subject']=$v; }
		if (isset($mailoption['mailbody'])){ if (eregi($mailoption['mailbody'],$k)) $mailtoparam['body']=$v;}
		if (isset($mailoption['mailfromset'])){ if (eregi($mailoption['mailfromset'],$k)) $mailtoparam['mailfromset']=$v; }
		if (isset($mailoption['to_mailinglist']  )){ if (eregi($mailoption['to_mailinglist'],$k)) $mailtoparam['to_mailinglist']=$v; }
		//************************************
		// Special controll and Attached files
		//************************************
		if (!eregi("img src=|a href=",$v)){
			// Special Controll Strings
			if (eregi("blogid",$k)){
				$blogid = "b".$v.",";
			}elseif (eregi("email",$k)){
				$mailfrom = $v;
				$message .= $k . $qsep . $v . $end;
			}else{
				$message .= $k . $qsep . $v . $end;
			}
		} else {
			// Attached Files
			$u = split("[<,>]",$v);
			$url = ereg_replace("img src=|a href=","",$u[1]);
			$url = ereg_replace("target='_blank'","",$url);
			$url = ereg_replace("'","",$url);
			$url = $uploads.ereg_replace($upurl,"",$url);
			$url = cnv_mbstr(rawurldecode($url),$file_charset);
			$fnm = $u[2];
			$attach[$i]['url']=$url;
			$attach[$i]['fnm']=$fnm;
			$i++;
		}
	}
	//return(mail($qs_addr,$subject,$message,$headers));
	switch( $from_option ){
		case 0:	// SurveyAddress to questioner
			$mailfrom = bmsurveyUtils::getXoopsModuleConfig('MAILADDR');
			if (!$mailfrom) $mailfrom = $xoopsConfig['adminmail'];
			$mailto = $qs_addr; break;
			//$xoopsConfig['adminmail']
		case 1:	// Respondent to questioner
			$mailfrom = $rs_addr; $mailto = $qs_addr; break;
		case 2:	// From address in Questionnaire
			$mailto = $qs_addr; break;
		/* No longer option with allow_email
		case 2:
			$ret = send_email_with_attach($rs_addr,$qs_addr,$subject,$message,$headers,$attach); // Respondent to questioner
			$mailfrom = $qs_addr; $mailto = $rs_addr; break;
			break;			
		case 3:	// questioner to questioner
			$mailfrom = $qs_addr; $mailto = $qs_addr; break;
		case 4:	// questioner to Admin
			$mailfrom = $qs_addr; $mailto = $xoopsConfig['adminmail']; break;
		case 5:	// questioner to Special
			$mailfrom = $qs_addr; $mailto = $GLOBALS['ESPCONFIG']['special_addr']; 
			$subject = $GLOBALS['ESPCONFIG']['special_char'].$subject; break;
		*/
	}
	//
	// Send to mailto info in section text. If fill in $mailto.
	//
	//print_r($mailtoparam);
	if (isset($mailtoparam['to']) && isset($mailtoparam['subject']) && isset($mailtoparam['body'])){
		$from = $cc = $bcc = "";
		if ( isset($mailtoparam['mailfromset']) && isset($mailtoparam['from'] ) ){
			if(function_exists('mberegi')){
				if ( mberegi($mailtoparam['mailfromset'],_GT_Yes) ) $from=$mailtoparam['from'];
			}else{
				if ( preg_match($mailtoparam['mailfromset'],_GT_Yes) ) $from=$mailtoparam['from'];
			}
		}
		if (isset($mailtoparam['name'])) $mailtoparam['body'].="\n".$mailtoparam['name'];
		if (isset($mailtoparam['cc'])) $cc = $mailtoparam['cc'];
		if (isset($mailtoparam['bcc'])) $bcc = $mailtoparam['bcc'];
		//echo $from . $mailtoparam['to'] . $mailtoparam['subject']."<br>";
		send_email_with_attach($from,$mailtoparam['to'],$mailtoparam['subject'],$mailtoparam['body'],$headers,$attach,$cc,$bcc);
	}
	if (!isset($mailfrom) && $mailtoparam['from']) $mailfrom=$mailtoparam['from'];
	//
	// For Mailling List
	//
	if (isset($mailoption['to_mailinglist'])){ 
		$message = $mailfrom;	// Change Message to Mail address for mailing list
		// Deny to mail sending
		if(function_exists('mberegi')){
			if ( mberegi($mailtoparam['to_mailinglist'],_GT_No) ) $mailto = '';
		}else{
			if ( preg_match($mailtoparam['to_mailinglist'],_GT_No) )  $mailto = '';
		}
	}
	if ($debug) print(" from = $mailfrom,to = $mailto");

	//echo $mailfrom.$mailto.$blogid.$subject."<br>";
	return send_email_with_attach($mailfrom,$mailto,$blogid.$subject,$message,$headers,$attach);
}
/* }}} */

/* {{{ proto int response_select_max_pos(int survey_id, int response_id)
   Returns the position of the last answered question in a response.
 */
function response_select_max_pos($sid, $rid) {
	$max = 0;
	
	foreach (array('bool', 'single', 'multiple', 'rank', 'text', 'other', 'date') as $tbl) {
		$sql = "SELECT MAX(q.position) FROM ".TABLE_."response_$tbl a, question q
				WHERE a.response_id = '$rid' AND
					q.id = a.question_id AND
					q.survey_id = '$sid' AND
					q.deleted = 'N'";
		$res = mysql_query($sql);
		if (($num = mysql_result($res, 0, 0)) > $max)
			$max = $num;
		mysql_free_result($res);
	}
		return $max;
}
/* }}} */

/* {{{ proto int response_select_max_pos(int survey_id, int response_id)
   Returns the number of the section in which questions have been
   answered in a response.
 */
function response_select_max_sec($sid, $rid) {
	$pos = response_select_max_pos($sid, $rid);
	$sql = "SELECT COUNT(*)+1 FROM ".TABLE_QUESTION." q
			WHERE q.survey_id = '$sid' AND
				q.type_id = 99 AND
				q.position < '$pos' AND
				q.deleted = 'N'";
	$res = mysql_query($sql);
	$max = mysql_result($res, 0, 0);
	mysql_free_result($res);
	return $max;
}
/* }}} */

/* {{{ proto void response_import_sec(int survey_id, int response_id, int section, &array destination)
   Populates the destination array with the answers from a given
   section of a given response.
 */
function response_import_sec($sid, $rid, $sec, $varr = null) {
	if ($varr == null)
		$varr =& $GLOBALS['_POST'];
	
	$ids = survey_get_sections($sid);
	if ($sec < 1 || !isset($ids[$sec - 1]))
		return;
	$vals = response_select($sid, $rid, 'content', $ids[$sec - 1]);

	reset($vals);
	foreach ($vals as $id => $arr) {
		if (isset($arr[0]) && is_array($arr[0])) {
			// multiple
			$varr[$id] = array_map('array_pop', $arr);
		} else {
			$varr[$id] = array_pop($arr);
		}
	}	
}
/* }}} */
/******************************************************************************
Pickup data from answer mail
******************************************************************************/
function resmail_insert($uname,$sid,$l_stack,$section=1,$rid=null) {
	$debug = 0;

	if(is_null($rid)) {
		// create a uniqe id for this response
		$sql = "INSERT INTO ".TABLE_RESPONSE." (survey_id,username) VALUES ( '${sid}','${uname}' )";
		$result = mysql_query($sql);
		$rid = mysql_insert_id();
	}
	$sql  = "SELECT Q.position, Q.id, Q.type_id, T.response_table FROM ".TABLE_QUESTION.
		" Q, ".TABLE_QUESTION_TYPE." T ". survey_select_section_sql($sid,$section,'Q') .
		" AND Q.survey_id='${sid}' AND Q.deleted='N' AND Q.type_id < 50 AND Q.type_id=T.id Order By Q.position";
	$q_result = mysql_query($sql);
	while(list($position,$qid, $tid, $table) = mysql_fetch_row($q_result)) {
		$bareval = '';
		$qnum = $position + 1;
		if (is_array($l_stack[$qnum])){
			$bareval = array_map('_stripslashes', $l_stack[$qnum]);
		} elseif (isset($l_stack[$qnum])) {
			$bareval = _stripslashes($l_stack[$qnum]);
		}
		if (is_string($bareval)){
			$val = addslashes($bareval);
		} else {
			$val = $bareval;
		}
		if ($debug){
			echo "<br>${qnum},qid=${qid},bareval=${bareval},table=${table},val=${val}";
		}
		switch($table) {
			case 'response_bool':
				$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,choice_id ) VALUES ( '${rid}','${qid}','${val}' )";
				if (!empty($val))
					$result = mysql_query($sql);
				break;
			case 'response_text':
				// only insert if non-empty content
				if($tid == 10) { // numeric
					$bareval = ereg_replace("[^0-9.\-]*(-?[0-9]*\.?[0-9]*).*", '\1', $bareval);
				}
				if($tid == 40) { // attach by Y.Sakai
					$bareval = get_attach($uname,$qid);
				}
				if(ereg("[^ \t\n]",$bareval)) {
					$val = addslashes($bareval);
					$sql = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,response ) VALUES ( '${rid}','${qid}','${val}' )";
					$result = mysql_query($sql);
				}
				break;
			case 'response_date':
				// only insert if non-empty content
				$aSepList="-/ .";
				ereg("^([0-9]+)[$aSepList]([0-9]+)[$aSepList]([0-9]+)$", $bareval, $regs);
				if($regs) {
					date(_SHORTDATESTRING,strtotime($val));
					$dfmt = ereg_replace("[\/]|[\-]","",_SHORTDATESTRING);
					switch($dfmt){
						case "njY":
							$val=$regs[3]."-".$regs[1]."-".$regs[2];
							break;
						case "jnY":
							$val=$regs[3]."-".$regs[2]."-".$regs[1];
							break;
						case "Ynj":
							$val=$regs[1]."-".$regs[2]."-".$regs[3];
							break;
					}
					$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,response ) VALUES ( '${rid}','${qid}','${val}' )";
					$result = mysql_query($sql);
				}
				break;
			case 'response_single':
				if(empty($bareval)) {
					$sql = "SELECT id FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='${qid}'";// AND content LIKE '!other%' ORDER BY id";
					$c_result = mysql_query($sql);
					while(list($cid) = mysql_fetch_row($c_result)) {
						$keystr = $qnum . "_" . $cid;
						if (isset( $l_stack["o_".$cid])) {
							$other = _addslashes($l_stack["o_".$cid]);
							if (!$other) continue;
							$sql = "INSERT INTO ".TABLE_RESPONSE_OTHER."  ( response_id,question_id,choice_id,response ) VALUES ( '${rid}','${qid}','${cid}','${other}' )";
							$result = mysql_query($sql);
							$val = $cid;
							break;
						}
						if (!isset($l_stack["$keystr"])) continue;
					}
					mysql_free_result($c_result);
				}
/*
				if(ereg("o_([0-9]+)", $bareval, $regs)) {
					$cid=$regs[1];
					if (!isset($l_stack["$keystr"])) break; // out of the case
					if(ereg("[^ \t\n]",$other)) {
						$other = _addslashes($l_stack["$keystr"]);
						$sql = "INSERT INTO ".TABLE_RESPONSE_OTHER."  ( response_id,question_id,choice_id,response ) VALUES ( '${rid}','${qid}','${cid}','${other}' )";
						$result = mysql_query($sql);
						$val=$cid;
					}
				}
*/
				if (isset($val)){
					$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,choice_id ) VALUES ( '${rid}','${qid}','${val}' )";
					$result = mysql_query($sql);
				}
				break;
			case 'response_multiple':
				$sql = "SELECT id FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='${qid}'";	// AND content LIKE '!other%'";
				$c_result = mysql_query($sql);
				$c_val = array();
				while(list($cid) = mysql_fetch_row($c_result)) {
					$keystr = $qnum . "_" . $cid;
					if(isset( $l_stack["o_".$cid])) {
						$other = _addslashes($l_stack["o_".$cid]);
						if (!$other) continue;
						$sql = "INSERT INTO ".TABLE_RESPONSE_OTHER."  ( response_id,question_id,choice_id,response ) VALUES ( '${rid}','${qid}','${cid}','${other}' )";
						$result = mysql_query($sql);
						array_push($l_stack[$qnum], $cid);
					}
					if (!isset($l_stack["$keystr"]) || empty($l_stack["$keystr"])) continue;
					if (!isset($l_stack[$qnum]))
						$l_stack[$qnum] = array($cid);
					else
						array_push($l_stack[$qnum], $cid);
				}
				mysql_free_result($c_result);
				if(!isset($l_stack[$qnum]) || count($l_stack[$qnum]) < 1) break;
				foreach($l_stack[$qnum] as $cid) {
					$cid = _addslashes($cid);
					if(ereg("o_[0-9]+", $cid)) continue;
					$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,choice_id ) VALUES ( '${rid}','${qid}','${cid}' )";
					$result = mysql_query($sql);
				}
				break;
			case 'response_rank':
				if($tid == 8) { // Rank
					$sql = "SELECT id FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='${qid}'";
					$cid_result = mysql_query($sql);
					while(list($cid) = mysql_fetch_row($cid_result)) {
						if (!isset($l_stack["${qid}_${cid}"])) continue;
						$val = _addslashes($l_stack["${qid}_${cid}"]);
						if(strtolower($val) == "n/a")
							$rank = -1;
						else
							$rank = intval($val) - 1;
						$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,choice_id,rank ) ";
						$sql .= "VALUES ( '${rid}','${qid}','${cid}','${rank}' )";
						mysql_query($sql);
					}
					mysql_free_result($cid_result);
					break;
				}
				if(strtolower($bareval) == "n/a")
					$rank = -1;
				else
					$rank = intval($bareval);
				$sql  = "INSERT INTO ".TABLE_."${table} ( response_id,question_id,rank ) VALUES ( '${rid}','${qid}','${rank}' )";
				$result = mysql_query($sql);
				break;
		}
	}
	mysql_free_result($q_result);
	return($rid);
}

?>