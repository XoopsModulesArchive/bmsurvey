<?php
// $Id: bmsurveyUtils.php,v0.83 2008/01/08 18:38:03 yoshis Exp $
//  ------------------------------------------------------------------------ //
//                      BmSurvey - Bluemoon Multi-Survey                     //
//                   Copyright (c) 2005 - 2007 Bluemoon inc.                 //
//                       <http://www.bluemooninc.biz/>                       //
//              Original source by : phpESP V1.6.1 James Flemer              //
//  ------------------------------------------------------------------------ //
//  This program is free software; you can redistribute it and/or modify     //
//  it under the terms of the GNU General Public License as published by     //
//  the Free Software Foundation; either version 2 of the License, or        //
//  (at your option) any later version.                                      //
//                                                                           //
//  You may not change or alter any portion of this comment or credits       //
//  of supporting developers from this source code or any supporting         //
//  source code which is considered copyrighted (c) material of the          //
//  original comment or credit authors.                                      //
//                                                                           //
//  This program is distributed in the hope that it will be useful,          //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of           //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
//  GNU General Public License for more details.                             //
//                                                                           //
//  You should have received a copy of the GNU General Public License        //
//  along with this program; if not, write to the Free Software              //
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA //
//  ------------------------------------------------------------------------ //
if(
	!defined('XOOPS_ROOT_PATH') ||
	!defined('XOOPS_CACHE_PATH')
){
	exit();
}
include_once XOOPS_ROOT_PATH.'/modules/bmsurvey/conf.php';

class bmsurveyUtils {
	
	function log($str){
		//if( $ESPCONFIG['log_output'] == 1 ){
			$log = './log/survey.log';
			$fp = fopen($log, 'a');
			fwrite($fp, $str."\n");
			fclose($fp);
		//}
	}
	
	function getDateFromHttpParams(){
		
		$param = isset($_POST['param']) ? ($_POST['param']) : 0;
		if($param == 0){
			$param = isset($_GET['param']) ? ($_GET['param']) : 0;
		}
		/* It doesn't work with php4isapi.dll.
		if($param == 0){
			$tmp = explode('/', $_SERVER['REQUEST_URI']);
			$param = ($tmp[count($tmp)-1]);
		}*/
		$param = trim($param);
		
		if($param == 0){
			return false;
		}
		$result = array();
		$result['params'] = $param;
		//print("$param");
		if(preg_match("/^([0-9]+)-([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})-([a-zA-Z0-9]*)/", $param, $m)){
			$result['uid'] = bmsurveyUtils::checkUid($m[1]);
			$result['year'] = bmsurveyUtils::checkYear($m[2]);
			$result['month'] = bmsurveyUtils::checkMonth($m[3]);
			$result['date'] = bmsurveyUtils::checkDate($m[2], $m[3], $m[4]);
			$result['hours']=$m[5];
			$result['minutes']=$m[6];
			$result['seconds']=$m[7];
			$result['command'] = trim($m[8]);		// enc type for MT user
		}else if(preg_match("/^([0-9]+)-([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})/", $param, $m)){
			$result['uid'] = bmsurveyUtils::checkUid($m[1]);
			$result['year'] = bmsurveyUtils::checkYear($m[2]);
			$result['month'] = bmsurveyUtils::checkMonth($m[3]);
			$result['date'] = bmsurveyUtils::checkDate($m[2], $m[3], $m[4]);
			//print("$m[5]:$m[6]:$m[7]");
			$result['hours']=$m[5];
			$result['minutes']=$m[6];
			$result['seconds']=$m[7];
		}else if(preg_match("/^([0-9]+)-([0-9]{4})([0-9]{2})/", $param, $m)){
			$result['uid'] = bmsurveyUtils::checkUid($m[1]);
			$result['year'] = bmsurveyUtils::checkYear($m[2]);
			$result['month'] = bmsurveyUtils::checkMonth($m[3]);
		}else if(preg_match("/^([0-9]+)/", $param, $m)){
			$result['uid'] = bmsurveyUtils::checkUid($m[1]);
		}else{
			redirect_header(XOOPS_URL.'/',1,_MD_POPNUPBLOG_INVALID_DATE.'(INVALID PARAM)');
			exit();
		}
		return $result;
	}
	
	function getApplicationNum(){
		global $xoopsDB;
		if(!$dbResult = $xoopsDB->query('select count(*) num from '.POPNUPBLOG_TABLE_APPL)){
			return 0;
		}
		if(list($num) = $xoopsDB->fetchRow($dbResult)){
			return $num;
		}
		return 0;
	}
	
	function wesurveyUpdatesPing($rss, $url, $survey_name = null, $title = null, $excerpt = null){
		$ping = new bmsurveyPing2($rss, $url, $survey_name, $title, $excerpt);
		$ping->send();
		/* debug log
		ob_start();
		print_r($ping);
		$log = ob_get_contents();
		ob_end_clean();
		bmsurveyUtils::log($log);
		*/
	}
	
	function newApplication($in_title, $in_permission){
		global $xoopsUser, $xoopsDB;
		$title = "";
		$permission = -1;
		if(!empty($in_title)){
			$title = bmsurveyUtils::convert2sqlString($in_title);
		}
		if( ($in_permission == 0) || ($in_permission == 1) || ($in_permission == 2) || ($in_permission == 3)){
			$permission = intval($in_permission);
		}
		
		if($permission < 0){
			return _MD_POPNUPBLOG_ERR_INVALID_PERMISSION;
		}
		if(!$result = $xoopsDB->query('select uid from '.POPNUPBLOG_TABLE_APPL.' where uid = '.$xoopsUser->uid())){
			return "select error";
		}
		if(list($tmpUid) = $xoopsDB->fetchRow($result)){
			return _MD_POPNUPBLOG_ERR_APPLICATION_ALREADY_APPLIED;
		}
		if(!$result = $xoopsDB->query('select uid from '.POPNUPBLOG_TABLE_INFO.' where uid = '.$xoopsUser->uid())){
			return "select error";
		}
		if(list($tmpUid) = $xoopsDB->fetchRow($result)){
			return _MD_POPNUPBLOG_ERR_ALREADY_WRITABLE;
		}
		$sql = sprintf("insert into %s (uid, title, permission, create_date) values(%u, '%s', %u, CURRENT_TIMESTAMP())", 
			POPNUPBLOG_TABLE_APPL, $xoopsUser->uid(), $title, $permission);
		if(!$result = $xoopsDB->query($sql)){
			return "insert error";
		}
		
		return "";
	}
	
	function getXoopsModuleConfig($key){
		global $xoopsDB;
		$mid = -1;

		$sql = "SELECT mid FROM ".$xoopsDB->prefix('modules')." WHERE dirname = 'bmsurvey'";
		if (!$result = $xoopsDB->query($sql)) {
			return false;
		}
		$numrows = $xoopsDB->getRowsNum($result);
		if ($numrows == 1) {
			list($l_mid) = $xoopsDB->fetchRow($result);
			$mid = $l_mid;
		}else{
			return false;
		}
		$sql = "select conf_value from ".$xoopsDB->prefix('config')." where conf_modid = ".$mid." and conf_name = '".trim($key)."'";
		if (!$result = $xoopsDB->query($sql)) {
			return false;
		}
		$numrows = $xoopsDB->getRowsNum($result);
		if ($numrows == 1) {
			list($value) = $xoopsDB->fetchRow($result);
			//return intval($value);
			return $value;
		}else{
			return false;
		}
	}

	
	function get_survey_list($start = 0){
		global $xoopsUser, $xoopsDB;
		
		$block_list_num = bmsurveyUtils::getXoopsModuleConfig('BLOCKLIST');
		$dateFormat = '%m/%d %k:%i';

		$selectMax = $start + $block_list_num;
		$sql_select = sprintf(
			'select id,name,title,owner,UNIX_TIMESTAMP(changed),subtitle,public FROM %s WHERE status = %u ORDER BY changed desc limit %u',
			 TABLE_SURVEY, 1, $selectMax);
		if(!$result_select = $xoopsDB->query($sql_select)){
			return false;
		}
		$tmp = array();
		$i = 0;
		$userHander = new XoopsUserHandler($xoopsDB);
		$groups = $xoopsUser ? $xoopsUser->getGroups() : 0;
		while(list($sid,$name,$title,$owner,$changed,$subtitle,$public) = $xoopsDB->fetchRow($result_select)){
			if( $public == 'N' ){
				$arealm = false;
				$sql = "SELECT a.maxlogin, a.realm, a.resume, a.navigate FROM "
					.TABLE_ACCESS." a WHERE a.survey_id = '$sid'";
				$acs = $xoopsDB->query($sql);
				while(list($maxlogin, $arealm, $aresume, $anavigate) = $xoopsDB->fetchRow($acs)){
					if (in_array($arealm,$groups)){
						$arealm = true;
						break;
					}
				}
			}else{
				$arealm = true;
			}
			if($i >= $start && $arealm==true){
				$res = array();
				$submitted = 0;
				if($xoopsUser){
					$sql = sprintf('SELECT submitted FROM %s WHERE survey_id=%u and username="%s" ORDER BY submitted DESC;'
						,TABLE_RESPONSE, $sid,$xoopsUser->uname());
					list($submitted) = $xoopsDB->fetchRow($xoopsDB->query($sql));
				}
				$res['submitted'] = $submitted;
				$res['sid'] = $sid;
				$res['name'] = $name;
				$res['title'] = $title;
				$tUser = $userHander->get($owner);
				if ($tUser) $res['uname'] = $tUser->uname(); else $res['uname'] = "uid(" . $owner .")";
				$res['last_update'] = $changed;
				$res['last_update_s'] = formatTimestamp($changed, 's');
				$res['last_update_m'] = formatTimestamp($changed, 'm');
				$res['last_update_l'] = formatTimestamp($changed, 'l');
				$res['subtitle'] = $subtitle;
				$res['url'] = XOOPS_URL."/modules/bmsurvey/survey.php?name=".$name;
				$tmp[$i] = $res;
			}
			$i++;
		}
		//var_dump($tmp);
		return $tmp;
	}
	function get_Respondentinfo( $unm ){
		global $xoopsDB;
		$sql = "SELECT * FROM ".TABLE_RESPONDENT." WHERE username='".$unm."'";
		$result = mysql_query($sql);
		if (mysql_num_rows($result) != 1) return(false);
		$ret = mysql_fetch_array($result,MYSQL_ASSOC);
		mysql_free_result($result);
		$ret['sid'] = $ret['survey_id'];
		$ret['rid'] = $ret['response_id'];
		return $ret;
	}
	function delete_respondent( $username ){
		global $xoopsDB;
		$sql = "DELETE FROM ".TABLE_RESPONDENT." WHERE username='".$username."'";
		$result = mysql_query($sql);
		if(!mysql_query($sql)) {
			/* unsucessfull -- abort */
			echo 'Cannot delete account. '.$username.' ('. mysql_error() .')';
		}
	}
	function update_respondent( $respondent ){
		global $xoopsDB;
		$debug=0;
		
		if ($debug) print_r($respondent);
		$disabled = ($respondent['disabled']==1) ? 'Y' : 'N';
		$sql = "SELECT * FROM ".TABLE_RESPONDENT." WHERE username='".$respondent['username']."'";
		$result = mysql_query($sql);
		if (mysql_num_rows($result) != 1){
			$sql = sprintf("insert into %s 
				(username,password,fname,lname,email,disabled,survey_id,response_id,changed,expiration)
				values('%s','%s','%s','%s','%s','%s',%u,%u,CURRENT_TIMESTAMP(),'%s')", 
				TABLE_RESPONDENT,
				$respondent['username'],
				$respondent['password'],
				$respondent['fname'],
				$respondent['lname'],
				$respondent['email'],
				$disabled,
				$respondent['sid'],
				$respondent['rid'],
				$respondent['expiration']);
		}else{
			$sql = "UPDATE ".TABLE_RESPONDENT." SET "
			."password='".$respondent['password']."'"
			.",fname='".$respondent['fname']."'"
			.",lname='".$respondent['lname']."'"
			.",email='".$respondent['email']."'"
			.",disabled='". $disabled ."'"
			.",survey_id=".$respondent['sid']
			.",response_id=".$respondent['rid']
			.",changed='".$respondent['changed']."'"
			.",expiration='".$respondent['expiration']."'"
			." WHERE username='".$respondent['username']."'";
		}
		if ($debug) echo "<p>".$sql;
		$xoopsDB->queryF($sql);
	}
	
	function createRssURL($uid){
		if((empty($useRerite)) || ($useRerite == 0) ){
			return POPNUPBLOG_DIR.'rss.php'.POPNUPBLOG_REQUEST_URI_SEP.$uid;
		}else{
			return POPNUPBLOG_DIR.'rss/'.$uid.".xml";
		}
	}
	
	function createUrl($uid){
		return XOOPS_URL."/modules/bmsurvey/";
	}
	
	function createUrlNoPath($uid, $year = 0, $month = 0, $date = 0, $hours = 0, $minutes = 0, $seconds = 0, $command = null){
		$result = '';
		if((empty($useRerite)) || ($useRerite == 0) ){
			$result .= "index.php".POPNUPBLOG_REQUEST_URI_SEP.bmsurveyUtils::makeParams($uid, $year, $month, $date, $hours, $minutes, $seconds, $command);
		}else{
			$result .= "view/".bmsurveyUtils::makeParams($uid, $year, $month, $date, $hours, $minutes, $seconds, $command).".html";
		}
		return $result;
	}
	
	function mb_strcut($text, $start, $end){
		if(function_exists('mb_strcut')){
			// return mb_strcut($text, $start, $end);
			return mb_substr($text, $start, $end);
		}else{
			return substr($text, $start, $end);
			// return strcut($text, $start, $end);
		}
	}
	
	function toRssDate($time, $timezone = null){
		if(!empty($timezone)){
			$time = xoops_getUserTimestamp($time, $timezone);
		}
		$res =  date("Y-m-d\\TH:i:sO", $time);
		// mmmm
		$result = substr($res, 0, strlen($res) -2).":".substr($res, -2);
		return $result;
	}
	
	function checkUid($iuid){
		$uid = intval($iuid);
		if( $uid > 0){
			return $uid;
		}
	}

	function checkYear($iyear){
		$year = intval($iyear);
		if ( ($year > 1000) && ($year < 3000) ){
			return $iyear;
		}
		redirect_header(XOOPS_URL.'/',1,_MD_POPNUPBLOG_INVALID_DATE.'(YEAR)'.$iyear);
		exit();
	}
	
	function checkMonth($imonth){
		$month = intval($imonth);
		if ( ($month > 0) && ($month < 13) ){
			return $imonth;
		}
		redirect_header(XOOPS_URL.'/',1,_MD_POPNUPBLOG_INVALID_DATE.'(MONTH)');
		exit();
	}
	
	function checkDate($year, $month, $date){
		if(checkdate(intval($month), intval($date), intval($year))){
			return $date;
		}
		redirect_header(XOOPS_URL.'/',1,_MD_POPNUPBLOG_INVALID_DATE.'(ALL DATE) '.intval($year)."-".intval($month)."-". intval($date));
		exit();
	}
	
	function makeParams($uid, $year=0, $month=0, $date=0, $hours=0, $minutes=0, $seconds=0, $command = null){
		$result = '';
		$c = '';
		if(!empty($command)){
			$c = '-'.$command;
		}
		if($year == 0){
			$result = $uid;
		}else if($date == 0){
			$result = sprintf("%s-%04u%02u%s", "".$uid, $year, $month, $c);
		}else{
			$result = sprintf("%s-%04u%02u%02u%02u%02u%02u%s", "".$uid, $year, $month, $date, $hours, $minutes, $seconds, $c);
		}
		return $result;
	}
	
	function makeTrackBackURL($uid, $year = 0, $month = 0, $date = 0, $hours=0, $minutes=0, $seconds=0){
		return XOOPS_URL.'/modules/popnupsurvey/trackback.php'.POPNUPBLOG_REQUEST_URI_SEP.bmsurveyUtils::makeParams($uid, $year, $month, $date, $hours, $minutes, $seconds);
	}
	
	function isCompleteDate($d){
		if(!empty($d['year'])){
			if(checkdate(intval($d['month']), intval($d['date']), intval($d['year']))){
				return true;
			}
		}
		return false;
	}
	function complementDate($d){
		if(!checkdate(intval($d['month']), intval($d['date']), intval($d['year']))){
			$time = time();
			$d['year'] = date('Y',$time);
			$d['month'] = sprintf('%02u', date('m',$time));
			$d['date'] =  sprintf('%02u', date('d',$time));
			$d['hours'] =  sprintf('%02u', date('H',$time));
			$d['minutes'] =  sprintf('%02u', date('i',$time));
			$d['seconds'] =  sprintf('%02u', date('s',$time));
		}
		//print($d['hours'].$d['minutes'].$d['seconds']);
		return $d;
	}
	
	function convert_encoding(&$text, $from = 'auto', $to){
		if(function_exists('mb_convert_encoding')){
			return mb_convert_encoding($text, $to, $from); 
		} else if(function_exists('iconv')){
			return iconv($from, $to, $text);
		}else{
			return $text;
		}
	}
	
	function assign_message(&$tpl){
		$all_constants_ = get_defined_constants();
		foreach($all_constants_ as $key => $val){
			if(preg_match("/^_(MB|MD|AM|MI)_BMSURVEY_(.)*$/", $key) || preg_match("/^BMSURVEY_(.)*$/", $key)){
				if(is_array($tpl)){
					$tpl[$key] = $val;
				}else if(is_object($tpl)){
					$tpl->assign($key, $val);
				}
			}
		}
	}
	/*
	function get_recent_trackback($date){
		global $xoopsDB;
		$sql = 'select title, url from '.POPNUPBLOG_TABLE_TRACKBACK.' where uid = '.$date['uid'].' order by t_date desc';
		if(!$db_result = $this->xoopsDB->query($sql)){
			return false;
		}
		$i = 0;
		
		$result['html'] = '<div>';
		while(list($title, $url) = $this->xoopsDB->fetchRow($db_result)){
			$result[data][] = new array(){ 'title' => $title, 'url' => $url};
			$i++;
			$result['html'] .= '<a href="'.$url.'" target="_blank">'.$title.'</a><br />';
		}
		$result['html'] .= '</div>';
		
		return $result;
	}
	*/
	function send_trackback_ping($trackback_url, $url, $title, $survey_name, $excerpt = null) {
		bmsurveyPing2::send_trackback_ping($trackback_url, $url, $title, $survey_name, $excerpt) ;
	}
	
	
	function remove_html_tags($t){
		return preg_replace_callback(
			"/(<[a-zA-Z0-9\"\'\=\s\/\-\~\_;\:\.\n\r\t\?\&\+\%\&]*?>|\n|\r)/ms", 
			/* "/(<[*]*?>|\n|\r)/ms", */
			"popnupsurvey_remove_html_tags_callback", 
			$t);
	}
	
	
	function convert2sqlString($text){
		$ts =& MyTextSanitizer::getInstance();
		if(!is_object($ts)){
			exit();
		}
		$res = $ts->stripSlashesGPC($text);
		$res = $ts->censorString($res);
		$res = addslashes($res);
		return $res;
	}
	function mail_popimg(){
		global $log,$limit_min;
		if (filemtime($log) < time() - $limit_min * 60) {
			return "<div style=\"text-align:center;\"><img src=./pop.php?img=1&time=".time()."\" width=70 height=1 /></div>POPed";
		} else {
			return "snoozed";
		}
	}
	function get_mailcode(){
		switch (_LANGCODE){
		case "af": $code = "ISO-8859-1";break;	//Afrikaans
		case "ar": $code = "ISO-8859-6";break;	//Arabic
		case "be": $code = "ISO-8859-5";break;	//Byelorussian
		case "bg": $code = "ISO-8859-5";break;	//Bulgarian
		case "ca": $code = "ISO-8859-1";break;	//Catalan
		case "cs": $code = "ISO-8859-2";break;	//Czech
		case "da": $code = "ISO-8859-1";break;	//Danish
		case "de": $code = "ISO-8859-1";break;	//German
		case "el": $code = "ISO-8859-7";break;	//Greek
		case "en": $code = "us-ascii";	break;	//English
		case "eo": $code = "ISO-8859-3";break;	//Esperanto
		case "es": $code = "ISO-8859-1";break;	//Spanish
		case "eu": $code = "ISO-8859-1";break;	//Basque
		case "et": $code = "iso-8859-15";break;	//Estonian
		case "fi": $code = "ISO-8859-1";break;	//Finnish
		case "fo": $code = "ISO-8859-1";break;	//Faroese
		case "fr": $code = "ISO-8859-1";break;	//French
		case "ga": $code = "ISO-8859-1";break;	//Irish
		case "gd": $code = "ISO-8859-1";break;	//Scottish
		case "gl": $code = "ISO-8859-1";break;	//Galician
		case "hr": $code = "ISO-8859-2";break;	//Croatian
		case "hu": $code = "ISO-8859-2";break;	//Hungarian
		case "is": $code = "ISO-8859-1";break;	//Icelandic
		case "it": $code = "ISO-8859-1";break;	//Italian
		case "iw": $code = "ISO-8859-8";break;	//Hebrew
		case "ja": $code = "ISO-2022-JP";break;	//Japanese (Shift_JIS)
		case "ko": $code = "EUC_KR";	break;	//Korean	
		case "lt": $code = "ISO-8859-13";break;	//Lithuanian
		case "lv": $code = "ISO-8859-13";break;	//Latvian
		case "mk": $code = "ISO-8859-5";break;	//Macedonian
		case "mt": $code = "ISO-8859-5";break;	//Maltese
		case "nl": $code = "ISO-8859-1";break;	//Dutch
		case "no": $code = "ISO-8859-1";break;	//Norwegian
		case "pl": $code = "ISO-8859-2";break;	//Polish
		case "pt": $code = "ISO-8859-1";break;	//Portuguese
		case "ro": $code = "ISO-8859-2";break;	//Romanian
		case "ru": $code = "ISO-8859-5";break;	//Russian
		case "sh": $code = "ISO-8859-5";break;	//Serbo-Croatian
		case "sk": $code = "ISO-8859-2";break;	//Slovak
		case "sl": $code = "ISO-8859-2";break;	//Slovenian
		case "sq": $code = "ISO-8859-2";break;	//Albanian
		case "sr": $code = "ISO-8859-2";break;	//Serbian
		case "sv": $code = "ISO-8859-1";break;	//Swedish
		case "th": $code = "TIS620";	break;	//Thai
		case "tr": $code = "ISO-8859-9";break;	//Turkish
		case "uk": $code = "ISO-8859-5";break;	//Ukrainian
		case "zh": $code = "GB2312";	break;	//Chainese	
		default: $code = "UTF-8";break;
		}
		return $code;
	}
}
?>
