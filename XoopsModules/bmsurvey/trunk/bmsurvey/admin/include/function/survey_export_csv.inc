<?php

# $Id: survey_export_csv.inc,v 1.2 2005/10/04 11:12:12 yoshis Exp $

// Original by James Flemer <jflemer@alum.rpi.edu>
// Modified by Yoshi Sakai <webmaster@bluemooninc.biz>

function cnv_mbstr($str, $encto) {
	if (extension_loaded('mbstring')){
		return  mb_convert_encoding($str,$encto,mb_internal_encoding());
	} else {
		return $str;
	}
}

/* {{{ proto array survey_generate_csv(int survey_id)
	Exports the results of a survey to an array.
	*/
function survey_generate_csv($sid,$csv_charset='ASCII') {
	global $xoopsModuleConfig;
	@set_time_limit(300);	

 	$output = array();
	$columns = array(
            'RESPONSE',
            'SUBMITTED',
            'USERNAME',
        );
	$types = array(
            0,
            0,
            1,
        );
    $numcols = 3;
    $makenumber = $xoopsModuleConfig['CSV_CHOICEOPT']==0 ? null : true;
    $otherformat = $xoopsModuleConfig['CSV_OTHERF'];
	$arr = array();
	if ($makenumber==null){
		// default
		$id_to_csv_map = array(
		'0',	// 0: unused
		'1',	// 1: bool -> boolean
		'1',	// 2: text -> string
		'1',	// 3: essay -> string
		'1',	// 4: radio -> string
		'1',	// 5: check -> string
		'1',	// 6: dropdn -> string
		'0',	// 7: rating -> number
		'0',	// 8: rate -> number
		'1',	// 9: date -> string
		'0', 	// 10: numeric -> number
		);
	} else {
		$id_to_csv_map = array(
		'0',	// 0: unused
		'1',	// 1: bool -> boolean
		'1',	// 2: text -> string
		'1',	// 3: essay -> string
		'2',	// 4: radio -> seq number
		'2',	// 5: check -> seq number
		'2',	// 6: dropdn -> seq number
		'0',	// 7: rating -> number
		'0',	// 8: rate -> number
		'1',	// 9: date -> string
		'0', 	// 10: numeric -> number
		);
	}
	$id_to_csv_map[40] = '1'; // For fileupload
	$sql = "SELECT q.id, q.name, q.type_id FROM ".TABLE_QUESTION.
		" q WHERE q.survey_id = $sid AND q.deleted = 'N' AND q.type_id < 50 ORDER BY position";
	$result = mysql_query($sql);
	$numcols += mysql_num_rows($result);
	$i = 1;
	while( list( $qid, $col, $type ) = mysql_fetch_row($result) ) {
		if (!$col){ $col = "Field Name Error :".$i; }
		if ($type == 8) {
			// rate
			$sql = "SELECT CONCAT(q.name, ' ', c.content) FROM ".TABLE_QUESTION.
				" q,".TABLE_QUESTION_CHOICE." c WHERE q.id = $qid AND q.id = c.question_id ORDER BY c.id";
			$result2 = mysql_query($sql);
			if ($result2){
				$numcols += mysql_num_rows($result2) - 1;
				while( list($col) = mysql_fetch_row($result2) ) {
					//echo $col;
					/*
					$str1 = $col;
					do {
						$str2 = $str1;
						$str1 = eregi_replace(
						"(^| )(what|which|why|how|who|where|how|is|are|were|the|a|it|of|do|you|your|please|enter)[ ?]",
						" ", $str2);
					} while ($str1 != $str2);
					$col = $str1;
					$col = trim(strtoupper(eregi_replace("[^A-Z0-9]+", " ", $col)));
					*/
					$col = ereg_replace(' +','_',$col);
					array_push($columns,cnv_mbstr( $col , $csv_charset ));
					array_push($types, $id_to_csv_map[$type]);
				}
				/* Test for number response
				$r = 1;
				while( list($col_r) = mysql_fetch_row($result2) ) {
					array_push($columns,$col."_".$r);
					array_push($types, $id_to_csv_map[$type]);
					$r++;
				}
				*/
				mysql_free_result($result2);
			}
		} else {	// For responce except upload file (type=40)
			array_push($columns,$col);
			array_push($types, $id_to_csv_map[$type]);
		}
		$i++;
	}
	mysql_free_result($result);
	$num = 0;
	array_push($output, $columns);

	// Add a number for column titile
	if ($csv_addnumber = bmsurveyUtils::getXoopsModuleConfig('CSV_ADDNUMBER')){
		for($i = 3; $i < $numcols; $i++) {
			$output[0][$i] = sprintf($csv_addnumber,$i-2).$output[0][$i];
		}
	}
	//$sql = "SELECT r.id,r.submitted,r.username FROM ".TABLE_RESPONSE.
	$sql = "SELECT r.id,DATE_FORMAT(r.submitted, '%Y%m%d%H%i%S'),r.username FROM ".TABLE_RESPONSE.
		" r WHERE r.survey_id='$sid' AND r.complete='Y' ORDER BY r.submitted";

	$result = mysql_query($sql);
	while($row = mysql_fetch_row($result)) {
		// get the response
		$response = response_select_name($sid, $row[0],null,$makenumber,$otherformat);
		$arr = array();
        array_push($arr, $row[0]);
        array_push($arr, date(_DATESTRING,convert_mysql_timestamp($row[1])));
        array_push($arr, $row[2]);

		// merge it
		for($i = 3; $i < $numcols; $i++) {
			if ( !isset($columns[$i])) continue;
			if ( isset($response[$columns[$i]]) )
				if ( is_array($response[$columns[$i]]) ) $response[$columns[$i]] = join(',', $response[$columns[$i]]);
			switch ($types[$i]) {
			case 3: // special
				break;
			case 2: // Sequencial Number
				if (isset($response[$columns[$i]]))
					array_push($arr, "\"" . cnv_mbstr($response[$columns[$i]],$csv_charset ) . "\"");
				else
					array_push($arr, '');
				break;
			case 1: // string
				if (isset($response[$columns[$i]])) {
                    /* Excel seems to allow "\n" inside a quoted string, but
                     * "\r\n" is used as a record separator and so "\r" may
                     * not occur within a cell. So if one would like to preserve
                     * new-lines in a response, remove the "\n" from the
                     * regex below.
                     */
					$response[$columns[$i]] = ereg_replace("[\r\n\t]", ' ', $response[$columns[$i]]);
					$response[$columns[$i]] = ereg_replace('"', '""', $response[$columns[$i]]);
					$response[$columns[$i]] = '"'.cnv_mbstr( $response[$columns[$i]],$csv_charset ).'"';
				}
				// fall through
			case 0: // number
				if (isset($response[$columns[$i]]))
					array_push($arr, $response[$columns[$i]]);
				else
					array_push($arr, '');
				break;
			}
		}
		array_push($output, $arr);
	}
	mysql_free_result($result);

	return $output;
}
/* }}} */

/* {{{ proto bool survey_export_csv(int survey_id, string filename)
	Exports the results of a survey to a CSV file.
	Returns true on success.
	*/
function survey_export_csv($sid, $filename,$csv_charset='ASCII') {
	$umask = umask(0077);
	$fh = fopen($filename, 'w');
	umask($umask);
	if(!$fh)
		return 0;

	$data = survey_generate_csv($sid,$csv_charset);

	foreach ($data as $row) {
		fputs($fh, join(',', $row ) . "\n");
	}

	fflush($fh);
	fclose($fh);

	return 1;
}
function convert_mysql_timestamp ($mysql_timestamp, $adjust="") {
  $timestring = substr($mysql_timestamp,0,8)." ".
  substr($mysql_timestamp,8,2).":".
  substr($mysql_timestamp,10,2).":".
  substr($mysql_timestamp,12,2);
  return strtotime($timestring." $adjust");
}
/* }}} */
?>