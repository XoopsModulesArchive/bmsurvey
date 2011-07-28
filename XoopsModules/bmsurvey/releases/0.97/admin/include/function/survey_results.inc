<?php
# $Id: survey_results.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $
//
// Original v 1.29 2003/03/05 19:26:31 Written by James Flemer <jflemer@alum.rpi.edu>
//
require_once XOOPS_ROOT_PATH.'/header.php';
include_once XOOPS_ROOT_PATH."/include/xoopscodes.php";

if(empty($GLOBALS['ESPCONFIG']['DEBUG_RESULTS']))
	$GLOBALS['ESPCONFIG']['DEBUG_RESULTS'] = $GLOBALS['ESPCONFIG']['DEBUG'];

/* {{{ proto void survey_results_navbar(int survey_id, int response_id)
   Builds HTML for a response navigation bar to move between individual
   responses.
 */
function survey_results_navbar($sid, $curr_rid) {
	$sql = "SELECT R.id FROM ".TABLE_RESPONSE." R WHERE R.survey_id='${sid}' AND R.complete = 'Y' ORDER BY R.id";
	$result = mysql_query($sql);
	$total = mysql_num_rows($result);
	if ($total < 1) return;

	$rids = array();
	
	$i = 0;
	$curr_pos = -1;
	while ($row = mysql_fetch_row($result)) {
		array_push($rids, $row[0]);
		if ($row[0] == $curr_rid)
			$curr_pos = $i;
		$i++;
	}
	mysql_free_result($result);

	$prev_rid = ($curr_pos > 0) ? $rids[$curr_pos - 1] : null;
	$next_rid = ($curr_pos < $total - 1) ? $rids[$curr_pos + 1] : null;
	$rows_per_page = 1;
	$pages = ceil($total / $rows_per_page);

	$url = $GLOBALS['ESPCONFIG']['ME'] . "?where=results&sid=$sid";

	$mlink = create_function('$i,$r', 'return "<a href=\"'.$url.'&rid=$r\">$i</a>";');

	$linkarr = array();

	$display_pos = 1;
	if ($prev_rid != null)
		array_push($linkarr, "<a href=\"$url&rid=$prev_rid\">"._GT_Previous.'</a>');
	for ($i = 0; $i < $curr_pos; $i++) {
		array_push($linkarr, "<a href=\"$url&rid=".$rids[$i]."\">$display_pos</a>");
		$display_pos++;
	}
	array_push($linkarr, "<a href=\"$url&rid=$curr_rid\"><b>$display_pos</b></a>");
	for (++$i; $i < $total; $i++) {
		$display_pos++;
		array_push($linkarr, "<a href=\"$url&rid=".$rids[$i]."\">$display_pos</a>");
	}
	if ($next_rid != null)
		array_push($linkarr, "<a href=\"$url&rid=$next_rid\">"._GT_Next.'</a>');

	?>
		<center>
		<h2><?php echo _GT_Navigate_Individual_Respondent_Submissions ?></h2>
		</center>
		<br />
		<?php echo implode(' | ', $linkarr); ?>
		<p>
		<?php
}
/* }}} */

/* {{{ proto string survey_results(int survey_id, int precision, bool show_totals, int question_id, array choice_ids, int response_id)
	Builds HTML for the results for the survey. If a
	question id and choice id(s) are given, then the results
	are only calculated for respodants who chose from the
	choice ids for the given question id.
	Returns empty string on sucess, else returns an error
	string. */
function survey_results($sid, $precision = 1, $showTotals = 1, $qid = '', $cids = '', $rid = '', $guicross='',$deny='',$defset='') {
	global $xoopsModuleConfig;
	$bg = '';
	if(empty($precision))	$precision  = 1;
	if($showTotals === '')	$showTotals = 1;
		
	if(is_int($cids))
		$cids = array($cids);
	if(is_string($cids))
		$cids = split(" ",$cids); // turn space seperated list into array

	// set up things differently for cross analysis
	$cross = !empty($qid);
	if($cross) {
		if(is_array($cids) && count($cids)>0) {
			$cidstr = array_to_insql($cids);
		} else {
			$cidstr = '';
		}
	}
	$sql = "SELECT R.username FROM ".TABLE_RESPONSE." R WHERE R.survey_id='${sid}' and R.id='${rid}'";
	if($result = mysql_query($sql)) {
		list($curr_uname) = mysql_fetch_row($result);
		mysql_free_result($result);
	}
	// build associative array holding whether each question
	// type has answer choices or not and the table the answers are in
	$has_choices = array();
	$response_table = array();
	$sql = "SELECT id,has_choices,response_table FROM ".TABLE_QUESTION_TYPE." ORDER BY id";
	if(!($result = mysql_query($sql))) {
        $errmsg = sprintf('%s [ %s: question_type ]',
                _GT_Error_system_table_corrupt, _GT_Table);
		return($errmsg);
	}
	while($row = mysql_fetch_row($result)) {
		$has_choices[$row[0]]=$row[1];
		$response_table[$row[0]]=TABLE_.$row[2];
	}
	mysql_free_result($result);

	// load survey title (and other globals)
	$sql = "SELECT * FROM ".TABLE_SURVEY." WHERE id='${sid}'";
	if(!($result = mysql_query($sql))) {
		$errmsg = _GT_Error_opening_survey ." [ ID:${sid} R:" . mysql_num_rows($result) ."]";
		return($errmsg);
	}
	$survey = mysql_fetch_array($result, MYSQL_ASSOC);
	mysql_free_result($result);

	// load survey questions
	$sql = "SELECT * FROM ".TABLE_QUESTION." WHERE survey_id='${sid}' AND deleted='N' ORDER BY position,id";
	if(!($questions_result = mysql_query($sql))) {
		$errmsg = _GT_Error_opening_survey .' '. _GT_No_questions_found ." [ ID:${sid} ]";
		return($errmsg);
	}

	// find out more about the question we are cross analyzing on (if any)
	if($cross) {
		$sql = "SELECT type_id FROM ".TABLE_QUESTION." WHERE id=${qid}";
		$result = mysql_query($sql);
		$crossTable = $response_table[mysql_result($result,0,0)];
		$responseTable = TABLE_RESPONSE;
		mysql_free_result($result);
		if(!in_array($crossTable, array('response_single',TABLE_RESPONSE_BOOL,'response_multiple'))) {
			$errmsg = _GT_Error_cross_analyzing_Question_not_valid_type .' [ '. _GT_Table .": ${crossTable} ]";
			return($errmsg);
		}
	}

// find total number of survey responses
// and relevant response ID's
	if (!empty($rid)) {
		$ridstr = "= '${rid}'";
        $total = 1;
	} else {
	    $sql = "";
	    if($cross) {
		    if(!empty($cidstr))
			    $sql = "SELECT A.response_id
					      FROM ${crossTable} A,
						       ${responseTable} R
					     WHERE A.response_id=R.id AND
						       R.complete='Y' AND
						       A.question_id='${qid}' AND
				    	       A.choice_id ${cidstr}
					     ORDER BY A.response_id";
		    else
			    $sql = "SELECT A.response_id
					      FROM ${crossTable} A,
						       ${responseTable} R
					     WHERE A.response_id=R.id AND
						       R.complete='Y' AND
						       A.question_id='${qid}' AND
				    	       A.choice_id = 0
					     ORDER BY A.response_id";
	    } else {
		    $sql = "SELECT R.id
				      FROM ".TABLE_RESPONSE." R
				     WHERE R.survey_id='${sid}' AND
					       R.complete='Y'
				     ORDER BY R.id";
	    }
	    if(!($result = mysql_query($sql))) {
		    $errmsg = _GT_Error_opening_survey ." [ ID:${sid} ] [ ".mysql_error()."]";
		    return($errmsg);
	    }
	    $total = mysql_num_rows($result);
	    if($total < 1) {
		    $errmsg = _GT_Error_opening_survey .' '. _GT_No_responses_found ." [ ID:${sid} ]";
			    return($errmsg);
            if ($GLOBALS['ESPCONFIG']['DEBUG'])
                echo("<!-- \$errmsg = '$errmsg' -->\n");
	    }

		$rids = array();
		while($row = mysql_fetch_row($result)) {
			array_push($rids, $row[0]);
		}
		mysql_free_result($result);
		// create a string suitable for inclusion in a SQL statement
		// containing the desired Response IDs
		// ex: "('304','311','317','345','365','370','403','439')"
		$ridstr = "IN (" . ereg_replace("([^,]+)","'\\1'", join(",", $rids)) .")";
	}
    
    if (!empty($rid)) {
        // show response navigation bar
        survey_results_navbar($sid, $rid);
    }

?>
<h2><?php echo($survey["title"]); ?></h2>
<h3><?php echo($survey["subtitle"]); ?></h3>
<blockquote><?php echo($survey["info"]); ?></blockquote>

<?php
	echo _MD_RESPONDENT." : ".$curr_uname;
	if($cross) {
		echo("<blockquote>" ._GT_Cross_analysis_on_QID ." ${qid}</blockquote>\n");
	}
?>
<table border="0" cellspacing="0" cellpadding="0" width="100%">
<?php
	$i=0; // question number counter
	while($question = mysql_fetch_array($questions_result)) {
		// process each question
		$qid = $question['id'];
		$tid = $question['type_id'];
		$table = $response_table[$tid];
		$totals = $showTotals;

		if ($tid == 99) {
			echo("<tr><td><hr></td></tr>\n");
			continue;
		}
		if($bg != '#eeeeee')	$bg = '#eeeeee';
		else                	$bg = '#ffffff';
		if ($tid == 100) {
			echo("<tr><td bgcolor='".$bg."'><br><b>". $question['content'] ."</b><hr></td></tr>\n");
			continue;
		}
		echo "<tr xbgcolor=$bg><td align='left'>";
		if ($tid < 50) {
			if (!empty($guicross)){
				echo ('<input type="hidden" name="where" value="results" />');
				echo ('<input type="hidden" name="sid" value="'.$sid.'" />');
				echo ("\n<table width=\"90%\" border=\"0\">\n");
				echo ("<tbody>\n");
				echo ("   <tr>\n");
				echo ("      <td width=\"30%\" height=\"31\" bgcolor=\"khaki\">\n");
				if ($tid==1 || $tid==4 || $tid==5 || $tid==6){
					echo ("<div align=\"center\">\n");
					echo ("   <input type=\"radio\" name=\"qid\" value=\"".$qid."\" />\n");
					echo ("</div>\n");
				}
				echo ("</td>\n");
				echo ("<td align=\"left\" width=\"30%\" bgcolor=\"#CCCCCC\">\n");
			} //end if empty($guicross)
			echo ("<A NAME=\"Q".++$i."\"><b>".$i.".</b></A>\n");
			$patterns = "/\[TH](.*)\[\/TH\]/sU";
			$replacements = '';
			$content = preg_replace($patterns, $replacements, $question['content']);
			if (preg_match($patterns, $question['content'], $thstr))
				$th_title = explode( "," , $thstr[1] );
			else
				$th_title = array(1,2,3,4,5);
			echo ("<b>".$content."</b>\n");
			if (!empty($guicross)){
				echo ("</td>\n");
				echo ("<td width=\"30%\" bgcolor=\"#CC0000\">\n");
				if ($tid==1 || $tid==4 || $tid==5 || $tid==6){
					echo ("<div align=\"center\">\n");
					echo ("<input type=\"radio\" name=\"qidr\" value=\"".$qid."\" />\n");
					echo ("</div>\n");
				}
				echo ("</td>\n");
				echo ("<td width=\"30%\" bgcolor=\"#0099FF\">\n");
				if ($tid==1 || $tid==4 || $tid==5 || $tid==6){
					echo ("<div align=\"center\">\n");
					echo ("<input type=\"radio\" name=\"qidc\" value=\"".$qid."\" />\n");
					echo ("</div>\n");
				}
				echo ("</td>\n");
				echo ("</tr>\n");
				echo ("</tbody>\n");
				echo ("</table>\n");
			} //end if empty($guicross)
		} //end if ($tid < 50)

$counts = array();
switch ($table) {
// -------------------------------- response_bool --------------------------------
		case TABLE_RESPONSE_BOOL:
            $counts = array(_GT_Yes => 0, _GT_No => 0);
			$sql = "SELECT A.choice_id, COUNT(A.response_id) FROM ${table} A 
				WHERE A.question_id='${qid}' AND A.response_id ${ridstr} AND A.choice_id != '' GROUP BY A.choice_id";
			if($GLOBALS['ESPCONFIG']['DEBUG_RESULTS'])	echo("<!-- \$sql = '$sql' -->\n");
			if($result = mysql_query($sql)) {
				while(list($ccontent,$count) = mysql_fetch_row($result)) {
					if ($ccontent == 'Y') $ccontent = _GT_Yes;
					else $ccontent = _GT_No;
					$counts[$ccontent] = $count;
				}
//var_dump($counts);
				mysql_free_result($result);
			}
			if(empty($question["result_id"]))
				$question["result_id"] = 1;	// default to percentages for yes/no
			break;
// -------------------------------- response_single --------------------------------
// -------------------------------- response_multiple --------------------------------
		case TABLE_RESPONSE_MULTIPLE:
			$totals = 0;
		case TABLE_RESPONSE_SINGLE:
			$sql = "SELECT C.content, COUNT(A.response_id) AS num
					  FROM ".TABLE_QUESTION_CHOICE." C,
						   ${table} A
					 WHERE C.question_id='${qid}' AND
						   C.content NOT LIKE '!other%' AND
						   A.question_id=C.question_id AND
						   A.choice_id=C.id AND
						   A.response_id ${ridstr}
					 GROUP BY C.id";
			if($GLOBALS['ESPCONFIG']['DEBUG_RESULTS'])	echo("<!-- \$sql = '$sql' -->\n");
			if($result = mysql_query($sql)) {
				while(list($ccontent,$count) = mysql_fetch_row($result)) {
					$counts[$ccontent] = $count;
				}
				mysql_free_result($result);
			}

			// handle 'other...'
			$sql = "SELECT A.response, C.content
					  FROM ".TABLE_RESPONSE_OTHER."  A,".TABLE_QUESTION_CHOICE." C
					 WHERE A.question_id='${qid}' AND
						   A.choice_id=C.id AND
    					   A.response_id ${ridstr}
					 ORDER BY C.id, A.response";
			if($GLOBALS['ESPCONFIG']['DEBUG_RESULTS'])	echo("<!-- \$sql = '$sql' -->\n");
			$otherformat = $xoopsModuleConfig['CSV_OTHERF'] ? $xoopsModuleConfig['CSV_OTHERF'] : "Other: %s";
			if($result = mysql_query($sql)) {
				while(list($answer,$ccontent) = mysql_fetch_row($result)) {
					//$content = preg_replace(array('/^!other=/', '/^!other/'),array('', 'Other:'), $ccontent);
					$content = sprintf($otherformat, htmlspecialchars($answer) );
					$counts[$content] = (isset($counts[$content]) ? $counts[$content] : 0)+1;
				}
				mysql_free_result($result);
			}
			if(empty($question["result_id"]))
				$question["result_id"] = 1;	// default to percentages
			break;
// -------------------------------- response_text --------------------------------
		case TABLE_RESPONSE_TEXT:
			$sql = "SELECT A.response, COUNT(A.response_id) AS num
					  FROM ${table} A
					 WHERE A.question_id='${qid}' AND
    					   A.response_id ${ridstr}
					 GROUP BY A.response";
			if($GLOBALS['ESPCONFIG']['DEBUG_RESULTS'])	echo("<!-- \$sql = '$sql' -->\n");
			if($result = mysql_query($sql)) {
				while(list($text, $num) = mysql_fetch_row($result)) {
					if(!empty($text))
						//$counts[htmlspecialchars($text)] = $num;
						$counts[$text] = $num;	// modified by Y.Sakai
				}
				mysql_free_result($result);
			}
			$question["result_id"] = 4;	// force "list" type response for text fields
			break;
// -------------------------------- response_date --------------------------------
		case TABLE_RESPONSE_DATE:
			$sql = "SELECT A.response, COUNT(A.response_id) AS num
					  FROM ${table} A
					 WHERE A.question_id='${qid}' AND
    					   A.response_id ${ridstr}
					 GROUP BY A.response";
			if($GLOBALS['ESPCONFIG']['DEBUG_RESULTS'])	echo("<!-- \$sql = '$sql' -->\n");
			if($result = mysql_query($sql)) {
				while(list($text, $num) = mysql_fetch_row($result)) {
					if(!empty($text))
						$counts[htmlspecialchars($text)] = $num;
				}
				mysql_free_result($result);
			}
			$question["result_id"] = 4;	// force "list" type response for date fields
			break;
// -------------------------------- response_rank --------------------------------
		case TABLE_RESPONSE_RANK:
			if($tid == 8) { //Rank
/*                $sql = "SELECT C.content FROM ".TABLE_QUESTION_CHOICE." C
                        WHERE C.question_id='${qid}' AND C.content NOT LIKE '!other%'
                        ORDER BY C.id";
                if ($result = mysql_query($sql)) {
                    for ($j = 0; $j < mysql_num_rows($result); $j++)
                        $counts[mysql_result($result, $j, 0)] = null;
                    mysql_free_result($result);
                }
*/
				$avgorcnt = $xoopsModuleConfig['RESULT_RANK'] ? $xoopsModuleConfig['RESULT_RANK'] : 0;
                if ($avgorcnt==0){
					$sql = "SELECT C.content, AVG(A.rank+1) AS average
							FROM ".TABLE_QUESTION_CHOICE." C, ${table} A
							WHERE C.question_id='${qid}' AND A.question_id='${qid}' AND
								  A.choice_id=C.id AND A.rank>=0 AND A.response_id ${ridstr}
							GROUP BY C.id";                        
					if($GLOBALS['ESPCONFIG']['DEBUG_RESULTS'])	echo("<!-- \$sql = '$sql' -->\n");
					if($result = mysql_query($sql)) {
						while(list($ccontent,$avg) = mysql_fetch_row($result)) {
							$counts[$ccontent] = $avg;
						}
						mysql_free_result($result);
					}
					$question["result_id"] = 99;	// force to rank
				}else{
	                $sql = "SELECT COUNT(*) FROM ".TABLE_RESPONSE." WHERE survey_id='${sid}'";
	                if ($ridstr) $sql .= " AND id ${ridstr}";
	                list($fullcount) = mysql_fetch_row(mysql_query($sql));
					$sql = "SELECT C.id, C.content, A.rank, COUNT(A.rank) AS cnt
							FROM ".TABLE_QUESTION_CHOICE." C, ${table} A
							WHERE C.question_id='${qid}' AND A.question_id='${qid}' AND
								  A.choice_id=C.id AND A.rank>=0 AND A.response_id ${ridstr}
							GROUP BY C.id,A.rank";
					if($result = mysql_query($sql)) {
						while(list($cid,$ccontent,$rank,$cnt) = mysql_fetch_row($result)) {
							$counts[$ccontent][$rank] = $cnt;
						}
						mysql_free_result($result);
					}
					$question["result_id"] = 98;	// force to count
				}
			} else {
				$sql = "SELECT A.rank, COUNT(A.response_id) AS num
						  FROM ${table} A
						 WHERE A.question_id='${qid}' AND
    						   A.response_id ${ridstr}
						 GROUP BY A.rank";
				if($result = mysql_query($sql)) {
					while(list($rank, $num) = mysql_fetch_row($result)) {
						if($rank == -1) { $rank = "N/A"; }
						$counts[_($rank)] += $num;
					}
					mysql_free_result($result);
				}
				if(empty($question["result_id"]))
					$question["result_id"] = 2;	// default to rank
			}
			break;
		}
// ---------------------------------------------------------------------------
	if (!empty($guicross) && $question["result_id"] == 1){
		mkcrossformat($counts,$qid,$tid);
	} else {
		switch($question["result_id"]) {
			case "1":	// Percentages
				mkrespercent($counts,$total,$precision,$totals);
				break;
			case "2":	// Rank
				mkresrank($counts,$total,$precision,$totals);
				break;
			case "3":	// Count
				mkrescount($counts,$total,$precision,$totals);
				break;
			case "4":	// List
				mkreslist($counts,$total,$precision,$totals);
				break;
			case "98":	// Count for rate
				mkresratecount($counts,$total,$precision,$totals,$question['length'],$fullcount,$th_title);
				break;
			case "99":	// Average
				mkresavg($counts,$total,$precision,$totals,$question['length']);
				break;
		} // end switch
	} //end if
?>
		</td>
	</tr>
<?php } // end while ?>
</table>
<?php
	global $xoopsUser;
	$admin = null;
	if (!empty($xoopsUser)) $admin = $xoopsUser->isAdmin();
	if ($admin && $rid){
		//
		// Delete Response
		//
		if (!$deny){
			$url = $GLOBALS['ESPCONFIG']['ME'] . "?where=results&sid=$sid";
			echo "<HR><a href=\"".$url."&rid=".$rid."&deny=1\">"._MD_DENYRESULT."</a>";
		}elseif ($deny==1){
			$url = $GLOBALS['ESPCONFIG']['ME'] . "?where=results&sid=$sid";
			echo "<HR><a href=\"".$url."&rid=".$rid."&deny=2\">"._MD_DENYRESULTSURE."</a>";
		}elseif ($deny==2){
			$sql = "UPDATE ".TABLE_RESPONSE." SET complete = 'N' WHERE survey_id='${sid}' AND id ='${rid}'";
			$result = mysql_query($sql);
			echo _MD_DENYRESULTDONE;
		}
		//
		// Set Default Respones to survey table
		//
		if (!$defset){
			$url = $GLOBALS['ESPCONFIG']['ME'] . "?where=results&sid=$sid";
			echo "&nbsp;&nbsp;<a href=\"".$url."&rid=".$rid."&defset=1\">"._MD_DEFAULTRESULT."</a>";
		}elseif ($defset==1){
			$sql = "UPDATE ".TABLE_SURVEY." SET response_id = '${rid}' WHERE id='${sid}'";
			$result = mysql_query($sql);
			echo "&nbsp;&nbsp;"._MD_DEFAULTRESULTDONE;
		}
	}
	mysql_free_result($questions_result);
	return;
}
/* }}} */

?>